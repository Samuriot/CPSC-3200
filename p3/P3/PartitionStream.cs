namespace P3;

// Jomikael Ruiz
// CPSC 3200
// PartitionStream.cs
// Date Created: 10/21/24
// Last Modified: 10/27/24

/**
 * Class Invariants:
 * - DefaultSize will always be 5 MessageStreams
 * - Max PartitionStream size is 11 MessageStreams
 * - PartitionStream size must be in range 0-11
 * - Client may place DurableStreams in a PartitionStream
 * - Partition Keys will be positive integers with a set of operations performed
 *   to map a partitionKey to its respective index
 * - Partition Keys cannot be negative, otherwise an exception is thrown
 * Immutables:
 * - Once set, PartitionStream size cannot be altered
 */

public class PartitionStream
{
    private const int MaxMsgstreamObjects = 11;
    private const int DefaultMsgstreamObjects = 5;
    private const int DefaultKeyValue = -1;

    private MsgStream[] _partitionStream;
    private int[] _partitionKeyStream;

    private int _partitionSize;
    private bool _isEmpty;

    /**
     * PreCondition: partitionKey is a valid positive integer or 0
     */
    private int ArrayIndexHash(int partitionKey)
    {
        return partitionKey % _partitionSize;
    }

    /**
     * PreCondition: partitionKey is a valid positive integer or 0
     */
    private void IsValidKey(int partitionKey)
    {
        if (partitionKey < 0)
        {
            throw new IndexOutOfRangeException("PartitionKey is a negative value");
        }
        
        int arrayIndex = ArrayIndexHash(partitionKey);
        
        if( _partitionKeyStream[arrayIndex] != -1 &&
            _partitionKeyStream[arrayIndex] != partitionKey )
        {
            throw new IndexOutOfRangeException("An index is already occupying the expected partition");
        }
    }

    /**
     * PreCondition: messageStream is a valid MsgStream or DurableStream,
     * 				  partitionKey is a valid integer
     * PostCondition: messageStream is inserted into the internal array
     */
    private void SetMessageStream(int partitionKey, MsgStream messageStream)
    {
        IsValidKey(partitionKey);

        int arrayIndex = ArrayIndexHash(partitionKey);

        _partitionStream[arrayIndex] = messageStream;
        _partitionKeyStream[arrayIndex] = partitionKey; 
        _isEmpty = false;
    }

    /**
     * PreCondition: PartitionStream must not be empty, otherwise an exception
     *               is thrown
     * PostCondition: PartitionStream is completely reset to its original state by calling
     *                reset() for internal MsgStreams & DurableStreams
     */
    private void Reset()
    {
        if (_isEmpty)
        {
            throw new IndexOutOfRangeException("The stream is empty.");
        }

        for (int i = 0; i < _partitionSize; i++)
        {
            if (_partitionKeyStream[i] == DefaultKeyValue)
                continue;
            _partitionStream[i].Reset();
            _partitionKeyStream[i] = DefaultKeyValue;
        }
        _isEmpty = true;
    }

    /**
     * PreCondition: initialSize is a valid positive integer
     * PostCondition: all internal data for PartitionStream is initialized
     */
    private void InitializePartitionStream(int initialSize)
    {
        _partitionSize = initialSize;
        _partitionStream = new MsgStream[_partitionSize];
        _partitionKeyStream = new int[_partitionSize];
        for (int i = 0; i < _partitionSize; i++)
        {
            _partitionKeyStream[i] = DefaultKeyValue;
        }
        _isEmpty = true;
    }
    
    /**
     * PreCondition: src must be a valid PartitionStream
     * PostCondition: all data from src is DeepCopied into this PartitionStream
     */
    public void DeepCopy(PartitionStream src) {
        _partitionSize = src._partitionSize;
        _isEmpty = src._isEmpty;
        _partitionStream = new MsgStream[_partitionSize];
        _partitionKeyStream = new int[_partitionSize];

        for( int i = 0; i < _partitionSize; i++ )
        {
            if (src._partitionKeyStream[i] == -1)
                continue;
            _partitionStream[i] = new MsgStream( src._partitionStream[i] );
            _partitionKeyStream[i] = src._partitionKeyStream[i];
        }
    }

    public PartitionStream()
    {
        InitializePartitionStream(DefaultMsgstreamObjects);
    }
    
    /**
     * PreCondition: src must be a valid PartitionStream
     */
    public PartitionStream(PartitionStream src)
    {
        DeepCopy(src);
    }
    
    /**
     * PreCondition: inputSize must be a valid positive integer
     */
    public PartitionStream(int inputSize)
    {
        if (inputSize < 0)
            throw new IndexOutOfRangeException("The input size is a negative value");

        if (inputSize > MaxMsgstreamObjects)
            throw new IndexOutOfRangeException("The input size is too large.");
        
        InitializePartitionStream(DefaultMsgstreamObjects);
    }

    /**
     * PreCondition: partitionKey is a valid positive integer, otherwise
     *               an exception is thrown. messageStream must be a valid
     *               MessageStream or DurableStream
     */
    public PartitionStream(int partitionKey, MsgStream messageStream)
    {
        InitializePartitionStream(DefaultMsgstreamObjects);
        SetMessageStream(partitionKey, messageStream);
    }

    /**
     * PreCondition: message is a valid string, partitionKey is a valid integer,
     *               otherwise an exception is thrown
     * PostCondition: messageStream is inserted into the internal array
     */
    public void AppendMessage(int partitionKey, string message)
    {
        int arrayIndex = ArrayIndexHash(partitionKey);

        IsValidKey(partitionKey);

        if (_partitionKeyStream[arrayIndex] == DefaultKeyValue)
        {
            _partitionStream[arrayIndex] = new MsgStream();
            _partitionKeyStream[arrayIndex] = partitionKey;
        }
        _partitionStream[arrayIndex].AppendMessage(message);
        _isEmpty = false;
    }

    /**
     * PreCondition: messageStream is a valid MsgStream or DurableStream,
     * 				  partitionKey is a valid integer
     * PostCondition: messageStream is inserted into the internal array
     */
    public void AppendMessage(int partitionKey, MsgStream messageStream)
    {
        SetMessageStream(partitionKey, messageStream);
    }

    /**
     * PreCondition: PartitionStream must not be empty, otherwise an exception
     *               is thrown
     * PostCondition: PartitionStream is completely reset to its original state
     */
    public void ResetStream()
    {
        Reset();
    }

    /**
     * PreCondition: partitionKey is a valid positive integer,
     *               otherwise an exception is thrown. PartitionStream
     *               must not be empty, otherwise an exception is thrown
     * PostCondition: a string array with the contents of the specific partition
     *                are returned
     */
    public string[] ReadMessages(int partitionKey)
    {
        if (_isEmpty)
            throw new IndexOutOfRangeException("The stream is empty.");
        IsValidKey(partitionKey);
        int arrayIndex = ArrayIndexHash(partitionKey);

        return _partitionStream[arrayIndex].ReadAllMessages();
    }

};

/*
 * Implementation Invariants:
 * - When collisions occur internally with the
 *   partitionKey and arrayIndex, an exception is thrown
 * - Reading messages based on a partitionKey returns all
 *   the messages associated with that specific partition
 * - Object supports Deep Copy via deepCopy() method
 * - ShallowCopy is not supported within this class
 * - To return messages, a structure is returned containing the pointer to
 *   the array of messages and the size for easy traversal
 * - The Key Array is instantly set to values of -1.
 * - PartitionStream supports storage of both MsgStream and DurableStream
 */
