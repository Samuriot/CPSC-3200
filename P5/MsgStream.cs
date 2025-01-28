// Jomikael Ruiz
// CPSC 3200
// MsgStream.cs
// File Created: 9/25/24
// Last Revision: 12/2/24

namespace P5;
/**
 * Class Invariants:
 * - DefaultSize will always be 100 strings
 * - MaximumStreamSize will always be 300 strings
 * - MaxOperations will always be based on the size of MsgStream
 *   with operations involving the Operational Constant
 * - This class can be inherited, with virtual methods for
 *   DeepCopy(), AppendMessage(), and Reset() to allow for
 *   method overriding
 * - PreConditions & PostConditions can be different for child classes
 * - ShallowCopy is not supported by this class
 * - Class supports Dispose() due to inheriting the IDisposable interface
 * Immutables:
 * - Maximum Number of Operations Cannot Be Changed
 * - Size of MsgStream cannot be changed once set
 * - Appending to MsgStream will always append to the end of the array
 */
public class MsgStream : IDisposable
{
    protected const uint DefaultSize = 100;
    protected const uint MaximumStreamSize = 300;
    private const int InitialIndex = 0;
    private const uint OperationalConstant = 5;
    
    private uint _maxOperations;
    private uint _totalOperations;
    private bool _isDisposed;
    
    protected uint _maxSize;
    protected int _currentArrayIndex;
    protected string[] _messageStream;
    
    protected bool IsEmpty()
    {
        return _currentArrayIndex == 0;
    }
    
    private bool IsFull()
    {
        return _currentArrayIndex == _maxSize;
    }

    private bool ExceedsMaxOperations()
    {
        return _totalOperations >= _maxOperations;
    }

    /*
     * Precondition: inputIndex is a valid positive integer
     */
    private void IsValidIndex(int inputIndex)
    {
        if (inputIndex < 0)
        {
            throw new IndexOutOfRangeException("Index is out of range.");
        }
        if (inputIndex >= _currentArrayIndex || inputIndex >= _maxSize)
        {
            throw new IndexOutOfRangeException("Index is out of range.");
        }
    }
    
    /*
     * Precondition: src is a valid MsgStream Object
     * Postcondition: contents of src are deep-copied into this instance of MsgStream
     */
    public virtual void DeepCopy(MsgStream src)
    {
        _maxSize = src._maxSize;
        _maxOperations = src._maxOperations;
        _totalOperations = src._totalOperations;
        _currentArrayIndex = src._currentArrayIndex;
        _isDisposed = src._isDisposed;

        _messageStream = new string[_maxSize];

        for(int i = 0; i < _currentArrayIndex; i++)
            _messageStream[i] = src._messageStream[i];
    }

    /*
     * Precondition: inputSize is a valid positive integer within range 0 - 299
     */
    public MsgStream(int inputSize)
    {
        if (inputSize >= MaximumStreamSize || inputSize < 0)
        {
            throw new IndexOutOfRangeException("Size is out of range.");
        }

        _maxSize = (uint)inputSize;
        _maxOperations = _maxSize + OperationalConstant;
        _messageStream = new string[_maxSize];
        _currentArrayIndex = InitialIndex;
        _totalOperations = InitialIndex;
        _isDisposed = false;
    }
    
    /*
     * Precondition: src is a valid MsgStream Object
     * Postcondition: contents of src are deep-copied into this instance of MsgStream
     */
    public MsgStream(MsgStream src)
    {
        DeepCopy(src);
    }
    
    public MsgStream()
    {
        _maxSize = DefaultSize;
        _maxOperations = _maxSize + OperationalConstant;
        _messageStream = new string[_maxSize];
        _currentArrayIndex = InitialIndex;
        _totalOperations = InitialIndex;
        _isDisposed = false;
    }
    
    /*
     * Precondition: message is a valid string, otherwise an exception is thrown
     * Postcondition: message is successfully added to the _messageStream array
     */
    public virtual void AppendMessage(string message)
    {
        if(_isDisposed)
            throw new ObjectDisposedException("MsgStream");
        if (IsFull())
            throw new IndexOutOfRangeException("The Message Stream is Full.");

        if (ExceedsMaxOperations())
            throw new IndexOutOfRangeException("Maximum number of operations has been reached.");
        _messageStream[_currentArrayIndex] = message ?? throw new ArgumentNullException();
        _currentArrayIndex++;
        _totalOperations++;
    }
    
   /*
    * Precondition: messageIndex is a valid positive integer within range, otherwise an exception is thrown
    */
    public string ReadMessage(int messageIndex)
    {
        IsValidIndex(messageIndex);
        if (ExceedsMaxOperations())
        {
            throw new IndexOutOfRangeException("Maximum number of operations has been reached.");
        }
        _totalOperations++;
        return _messageStream[messageIndex];
    }
    
    /*
     * Precondition: startingIndex & endingIndex are valid positive integers within range,
     *               otherwise an exception is thrown
     * Postcondition: a duplicate array with values ranging to and from the specified ranges is returned
     */ 
    public string[] ReadMessages(int startingIndex, int endingIndex)
    {
        IsValidIndex(startingIndex);
        IsValidIndex(endingIndex);
        if (ExceedsMaxOperations())
        {
            throw new IndexOutOfRangeException("Maximum number of operations has been reached.");
        }
        int newArraySize = endingIndex - startingIndex + 1;
        int newArrayIndex = 0;
        string[] retrievedMessages = new string[newArraySize];
        for (int i = startingIndex; i <= endingIndex; i++)
        {
            retrievedMessages[newArrayIndex] = ReadMessage(i);
            newArrayIndex++;
        }
        _totalOperations++;
        return retrievedMessages;
    }

    /*
     * Precondition: the MsgStream must not be empty, otherwise an exception is thrown
     * Postcondition: a duplicate array with values ranging to and from _currentArrayIndex is returned
     */ 
    public string[] ReadAllMessages()
    {
        if (IsEmpty())
        {
            throw new IndexOutOfRangeException("The Message Stream is Empty.");
        }
        string[] retrievedMessages = new string[_currentArrayIndex];
        for (int i = 0; i < _currentArrayIndex; i++)
        {
            retrievedMessages[i] = ReadMessage(i);
        }
        return retrievedMessages;
    }
    
    /*
     * Precondition: _messageStream has contents within it, otherwise an exception is thrown
     * Postcondition: _messageStream is cleared of all data and reset
     */
    public virtual bool Reset()
    {
        if (IsEmpty())
        {
            throw new IndexOutOfRangeException("The stream is empty.");
        }
        _messageStream = new string[_maxSize];
        _totalOperations = InitialIndex;
        _currentArrayIndex = InitialIndex;
        _isDisposed = false;
        return true;
    }

    /**
     * PreCondition: _isDisposed must be false, otherwise an exception is thrown
     * Post-Condition: MsgStream is properly disposed of
     */
    public void Dispose()
    {
        Dispose(disposing: true);
        GC.SuppressFinalize(this);
    }

    /**
     * PreCondition: _isDisposed is false, otherwise an exception is thrown
     * Post-Condition: MsgStream is properly disposed of
     */
    public virtual void Dispose(bool disposing)
    {
        if (_isDisposed)
            throw new ObjectDisposedException("MsgStream is already closed.");
        _isDisposed = true;
    }

    /**
 * Post-Condition: DurableStream is cleaned up by garbage collection
 */
    ~MsgStream()
    {
        Dispose(disposing: false);
    }
}
/*
 * Implementation Invariants:
 * - Default size of MsgStream is 100 strings
 * - All index parameters are taken in as a uInt, preventing the use of negative numbers
 * - When incorrect data is given in constructor, read, and append methods,
 *   an exception is thrown for the client to handle
 * - Resetting a MsgStream will revert the object to its initial state, completely
 *   deleting all the original data
 * - A max amount of operations is tracked with totalOperations, where a call to either
 *   append or read will increment totalOperations
 * - Once totalOperations reaches its maximum amount of operations, an exception is thrown for
 *   the client to handle
 * - When the MsgStream is empty, the client cannot read or reset the stream, otherwise an exception
 *   is thrown
 * - Dispose() cannot be undone unless, Reset() is called, resetting the state of MsgStream
 */
