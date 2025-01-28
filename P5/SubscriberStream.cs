// Jomikael Ruiz
// CPSC 3200
// SubscriberStream.cs
// File Created: 11/25/24
// Last Modified: 12/2/24

namespace P5;

/**
 * Class Invariants:
 * - SubscriberStream is a descendent child of PartitionStream and ISubscriber to
 *   simulate Multiple Inheritance in C#
 * - SubscriberStream is-a PartitionStream and is-a ISubscriber and
 *   has-a collection of ISubscribers that it sends its messages to via NewMessage()
 * - SubscriberStream implements NewMessage() from ISubscriber by going through
 *   its internal collection and sending the message to each ISubscriber descendant
 * - Functionality of SubscriberStream is the same as PartitionStream with the added
 *   effect of housing and holding an ISubscriber collection
 * - SubscriberStream can be resized according to input, as long as input is within range [1,10]
 * - Whenever SubscriberStream appends a message to its partitions, a message is also sent to each
 *   subscriber within its internal collection
 * - The client must fill the subscriber collection themselves via dependency injection, allowing their
 *   reference to subscriber to also receive the same messages as SubscriberStream
 * - SubscriberStream overrides both versions of AppendMessage() to successfully send messages to its subscribers
 * - When a MsgStream object is inserted via dependency injection, its message contents are sent to subscribers
 * - SubscriberStream inherits Dispose() from PartitionStream, and thus can be disposed of 
 * Immutables:
 * - Once set, the size of SubscriberStream cannot be changed
 * - SubscriberStream will share the same number of partitions as subscribers
 * - Once SubscriberStream is disposed, the client can no longer append messages to its partitions/subscribers
 */
public class SubscriberStream : PartitionStream, ISubscriber
{
    private ISubscriber[] _subscribers;
    private int _subscriberCount;
    private int _maxSubscribers;

    private bool _isDisposed;

    /**
     * PreCondition: size must be a positive integer within the MaxMsgStreamObjects range
     * Post-Condition: all member data is initialized
     */
    private void InitializeStream(int size)
    {
        if(size <= 0 || size > MaxMsgstreamObjects)
            throw new ArgumentOutOfRangeException(nameof(size));
        _maxSubscribers = size;
        _subscribers = new ISubscriber[_maxSubscribers];
        _subscriberCount = 0;
    }

    public SubscriberStream() : base()
    {
        InitializeStream(DefaultMsgstreamObjects);
    }
    
    /**
     * PreCondition: size must be a positive integer within MaxMsgObjects range
     * Post-Condition: all member data is initialized
     */
    public SubscriberStream(int size) : base(size)
    {
        InitializeStream(size);
    }
    
    /**
     * PreCondition: subscriber must be a valid descendant of the ISubscriber Interface
     * Post-Condition: all member data is initialized and subscriber is added to ISubscriber collection
     */
    public SubscriberStream(ISubscriber subscriber) : base()
    {
        InitializeStream(DefaultMsgstreamObjects);
        AddSubscriber(subscriber);
    }

    /**
     * PreCondition: message is a valid string
     * PostCondition: message is sent to all subscribers in internal collection
     */
    public void NewMessage(string message)
    {
        for(int i = 0; i < _subscriberCount; i++)
            _subscribers[i].NewMessage(message);
    }
    
    /**
     * PreCondition: message is a valid string, partitionKey is a valid integer,
     *               otherwise an exception is thrown
     * Post-Condition: message is inserted into the internal array, and message is sent
     *                to all internal subscribers in the collection
     */
    public override void AppendMessage(int partitionKey, string message)
    {
        base.AppendMessage(partitionKey, message);
        NewMessage(message);
    }

    /**
     * PreCondition: messageStream is a valid MsgStream object, partitionKey is a valid integer,
     *               otherwise an exception is thrown
     * Post-Condition: messageStream is inserted into the internal array, and contents
     *                from messageStream are sent to all internal subscribers in the collection
     */
    public override void AppendMessage(int partitionKey, MsgStream messageStream)
    {
        base.AppendMessage(partitionKey, messageStream);
        string[] streamContent = messageStream.ReadAllMessages();
        for(int i = 0; i < streamContent.Length; i++)
            NewMessage(streamContent[i]);
    }
    
    /**
     * PreCondition: subscriber is a valid descendant of ISubscriber interface
     * Post-Condition: subscriber is successfully injected into internal ISubscriber collection
     */
    public void AddSubscriber(ISubscriber subscriber)
    {
        if(_subscriberCount >= _maxSubscribers)
            throw new InvalidOperationException("SubscriberStream is full");
        _subscribers[_subscriberCount] = subscriber;
        _subscriberCount++;
    }

    /**
     * PreCondition: just as PartitionStream, the stream cannot be empty, otherwise
     *               an exception is thrown
     * Post-Condition: SubscriberStream is reinitialized to its base state
     */
    public override void ResetStream()
    {
        base.ResetStream();
        _subscriberCount = 0;
        _subscribers = new ISubscriber[_maxSubscribers];
        _isDisposed = false;
    }

    /**
     * PreCondition: SubscriberStream is not disposed of, otherwise method will throw an exception
     * Post-Condition: All SubscriberStream internal data is disposed of
     */
    public override void Dispose(bool disposing)
    {
        if (_isDisposed)
            throw new ObjectDisposedException("SubscriberStream is already disposed.");
        _isDisposed = true;
    }

    /**
     * Post-Condition: SubscriberStream is cleaned up by garbage collection
     */
    ~SubscriberStream()
    {
        Dispose(disposing: false);
    }
}
/*
 * Implementation Invariants:
 * - Dependency Injection is used to insert ISubscriber Objects into the internal array
 * - Every time a message is inserting into SubscriberStream, each ISubscriber will receive
 *   the same message via calling NewMessage()
 * - ISubscriber Collection is not automatically filled, and thus the client is responsible
 *   for manually inserting and filling this collection
 * - Dependency Injection is supported via Property Injection and Constructor Injection
 *   to insert ISubscriber objects
 * - Because SubscriberStream inherits PartitionStream, it gains access to all of PartitionStream's
 *   public and protected methods, enhancing functionality
 * - When invalid data is passed through parameters, or if SubscriberStream reaches capacity, exceptions
 *   are thrown for the client to handle
 * - NewMessage() is implemented to iterate through internal ISubscriber collection to send the inputted
 *   message to each Subscriber object
 * - Dispose() cannot be undone unless, ResetStream() is called, resetting the state of SubscriberStream
 */
