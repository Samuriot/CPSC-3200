// Jomikael Ruiz
// CPSC 3200
// File Created: 10/21/24
// DurableStream.cs
// Last Revision: 10/27/24

namespace P3;
using System.IO;

/**
 * Class Invariants:
 * - DurableStream is a descendent child of MsgStream
 * - Functionality of DurableStream is the same as MsgStream with the added
 *   feature of a backing file that is located in the base folder
 * - 3 Strings can be added to DurableStream before it writes back to its
 *   backing file
 * - Once a DurableStream is disposed, the associated file is closed
 *   and _isClosed is set to true
 * - _isClosed can only be reset by using the Reset() method
 * - _originalMessages contains the messages from the original file, which
 *   Reset() uses to restore original status of internal array
 * - DurableStream overrides DeepCopy(), AppendMessage(), and Reset() from
 *   MsgStream
 * - Just like MsgStream, this class does not support ShallowCopy
 * Immutables:
 * - Once set, _fileName cannot be changed
 * - Size of DurableStream is managed by MsgStream and thus, can be changed
 *   with a maximum size of 300 strings and default size of 100 strings
 * - Once added to the DurableStream, strings cannot be removed
 * - Default Backing File Name will be "messages.txt"
 * - Because of this, all DurableStreams that call the default constructor
 *   will immediately refer to and write to "messages.txt"
 */
public class DurableStream : MsgStream
{
    private string[] _originalMessages;
    private string _fileName;

    private bool _isClosed;
    
    private int _originalMessageIndex;
    private int _appendCounter;
    
    private const string DefaultFileName = "messages.txt";
    private const string DefaultFileCopyName = "copyOf";
    private const string FilePrefix = "../../../";
    private const int WriteToFileConstant = 3;
    private const int NoMessagesRead = 0;

    /**
     * PostCondition: the internal variables are set up for DurableStream
     */
    private void InitializeStream()
    {
        _isClosed = false;
        _appendCounter = 0;
        _messageStream = new string[DefaultSize];
        if (File.Exists(FilePrefix + _fileName))
            LoadMessagesFromFile();
        else
        {
            File.CreateText(FilePrefix + _fileName);
            _originalMessages = new string[NoMessagesRead];
        }
    }
    
    /**
     * PreCondition: _fileName must be a valid file, which is checked
     *               in methods that call LoadMessagesFromFile()
     * PostCondition: contents of _fileName are stored in _originalMessages
     */
    private void LoadMessagesFromFile()
    {
        string [] storedMessages = File.ReadAllLines(FilePrefix + _fileName);
        
        if (storedMessages.Length > MaximumStreamSize)
            throw new IOException($"The file {_fileName} is too large.");
        
        _originalMessages = storedMessages;
        _originalMessageIndex = _originalMessages.Length;
        for (int i = 0; i < _originalMessages.Length; i++)
            base.AppendMessage(_originalMessages[i]);
    }

    /**
     * PreCondition: _appendCounter is >= WriteToFileConstant
     * PostCondition: most recent 3 strings are written to backing file of _fileName
     */
    private void WriteMessagesToFile()
    {
        string [] messagesToBeStored = new string[WriteToFileConstant];
        int messagesIndex = 0;
        for (int i = _currentArrayIndex - _appendCounter; i < _currentArrayIndex; i++)
        {
            messagesToBeStored[messagesIndex] = _messageStream[i];
            messagesIndex++;
        }
        File.AppendAllLines(FilePrefix + _fileName, messagesToBeStored);
    }

    /**
     * PreCondition: array must be a valid string array and _fileName must be a
     *               valid file location
     * PostCondition: contents from array are written to the newly created backing file
     */
    private void WriteMessagesToFile(string[] array)
    {
        File.Delete(FilePrefix + _fileName);
        File.AppendAllLines(FilePrefix + _fileName, array);
    }

    /**
     * PreCondition: src must be a valid DurableStream to copy from
     * PostCondition: the contents from src are DeepCopied into
     *                this instance of DurableStream
     */
    private void CopyData(DurableStream src)
    {
        _originalMessages = src._originalMessages;
        _isClosed = src._isClosed;
        _fileName = DefaultFileCopyName + src._fileName;
        _originalMessageIndex = src._originalMessageIndex;
        _appendCounter = src._appendCounter;

        int counter = 1;
        // if copied file already exists
        while (File.Exists(FilePrefix + _fileName))
        {
            _fileName = counter + DefaultFileCopyName + src._fileName;
            counter++;
        }
        
        base.DeepCopy(src);

        string[] messagesToBeWritten = new string[_currentArrayIndex];
        for (int i = 0; i < _currentArrayIndex; i++)
            messagesToBeWritten[i] = _messageStream[i];
        
        WriteMessagesToFile(messagesToBeWritten);
    }

    /**
     * PreCondition: src must be a valid DurableStream to copy from
     * PostCondition: the contents from src are DeepCopied into
     *                this instance of DurableStream
     */
    public void DeepCopy(DurableStream src)
    {
        CopyData(src);
    }
    
    public DurableStream() : base()
    {
        _fileName = DefaultFileName;
        InitializeStream();
    }

    /**
     * PreCondition: size is a valid unsigned integer
     */
    public DurableStream(int size) : base(size)
    {
        _fileName = DefaultFileName;
        InitializeStream();
    }

    /**
     * PreCondition: fileName must be in a valid format of .txt or other text format
     */
    public DurableStream(string fileName) : base()
    {
        _fileName = fileName;
        InitializeStream();
    }

    /**
     * PreCondition: message must be a valid string, DurableStream must not be disposed,
     *               otherwise an exception is thrown
     * PostCondition: message will be added to the internal array and be written to
     *                the backing file linked to DurableStream
     */
    public override bool AppendMessage(string message)
    {
        if (_isClosed)
        {
            throw new IOException("Stream is closed.");
        }
        base.AppendMessage(message);
        _appendCounter++;
        if (_appendCounter >= WriteToFileConstant)
        {
            WriteMessagesToFile();
            _appendCounter = 0;
        }
        return true;
    }

    /**
     * PreCondition: DurableStream must not be empty, otherwise an exception is thrown
     * PostCondition: DurableStream is reinitialized to its base state at creation, restoring
     *                data from _originalMessages to _messageStream
     */
    public override bool Reset()
    {
        if (IsEmpty())
        {
            throw new IndexOutOfRangeException("Stream is empty");
        }
        _messageStream = new string[_maxSize];
        
        for (int i = 0; i < _originalMessages.Length; i++)
            _messageStream[i] = _originalMessages[i];

        _appendCounter = 0;
        _currentArrayIndex = _originalMessageIndex;
        _isClosed = false;
        WriteMessagesToFile(_originalMessages);
        return true;
    }

    /**
     * PreCondition: DurableStream is not empty and is not closed, otherwise an
     *               exception is thrown
     * PostCondition: The associated file is closed and appends will no longer work.
     *                Functionality is restored only through Reset()
     */
    public void Dispose()
    {
        if (IsEmpty()) 
            throw new IndexOutOfRangeException("Stream is empty");
        if (_isClosed)
            throw new IOException("Durable Stream File is already closed.");
        _isClosed = true;
    }
}
/*
 * Implementation Invariants:
 * - Default size of DurableStream is 100 strings
 * - Due to the nature of Rider File I/O, a prefix of '../../../' is added
 *   to each call using File class to allow for easier navigation to base directory of project
 * - If a DurableStream is disposed of, the client can only read the contents, unless
 *   they call Reset() to reset the DurableStream to base settings
 * - Reset() will store the contents of the original backing file into the array and restore
 *   functionality back to its original state
 * - If no backing file is present, DurableStream is completely wiped and reset to its original
 *   state
 * - A max amount of operations is tracked with functionality from MsgStream, throwing an exception
 *   once this limit is reached
 * - File Writing occurs after 3 string appends to the internal array
 * - When the DurableStream is empty, the client cannot read, reset, or dispose
 *   of the stream, otherwise an exception is thrown
 * - When DeepCopy() is called, a copied file will be created with prefix "copyOf"
 * - In the case that multiple instances of DurableStream DeepCopy from a single base DurableStream,
 *   a number indicator, starting at 1, will be added before prefix "copyOf" to prevent other
 *   DurableStreams from working on the same file
 */