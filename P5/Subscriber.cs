// Jomikael Ruiz
// CPSC 3200
// Subscriber.cs
// File Created: 11/25/24
// Last Modified: 12/2/24

namespace P5;

/**
 * Class Invariants:
 * - Subscriber simulates multiple inheritance via holding a MsgStream object
 *   and inheriting the ISubscriber interface
 * - Subscriber prioritizes ISubscriber in terms of inheritance, thus only gaining
 *   access to the public interface of MsgStream
 * - Subscriber only inherits ISubscriber as the implementation more suitably fits
 *   only using the public interface of MsgStream, rather than inheriting MsgStream
 *   directly, i.e., Subscriber is-a ISubscriber and holds-a MsgStream
 * - NewMessage() inserts a message into the internal object, acting as a notification for
 *   subscribers
 * - NewMessage() is implemented to successfully inherit the ISubscriber Interface
 * - Dependency injection is supported for constructor injection for object initialization
 *
 * Immutables:
 * - Once set, the size of the internal MsgStream object cannot be changed
 * - Resetting Subscriber is only possible via the use of ResetMessages()
 */
public class Subscriber : ISubscriber
{
    private MsgStream _messages;
    private const int DefaultMessages = 250;
    private const int MaxMessages = 300;

    public Subscriber()
    {
        _messages = new MsgStream(DefaultMessages);
    }

    /**
     * PreCondition: size must be a valid, positive integer from range 0-299
     */
    public Subscriber(int size)
    {
        if (size < 0 || size >= MaxMessages)
            throw new ArgumentOutOfRangeException(nameof(size));
        _messages = new MsgStream(size);
    }
    
    /**
     * Precondition: msgStream is a valid MsgStream object
     * Post-condition: msgStream is injected into internal member MsgStream data
     */
    public Subscriber(MsgStream msgStream)
    {
        _messages = msgStream;
    }
    
    /**
     * Precondition: message is a valid string
     * Post-condition: message is injected into the internal MsgStream object
     */
    public void NewMessage(string message)
    {
        _messages.AppendMessage(message);
    }

    /**
     * Precondition: _messages is not empty, otherwise an exception is thrown
     * Post-condition: message is injected into the internal MsgStream object
     */
    public string[] ReadSubscriptions()
    {
        string[] subscriptions = _messages.ReadAllMessages();
        return subscriptions;
    }

    /**
     * Precondition: _messages is not empty, otherwise an exception is thrown
     * Post-condition: all messages in _messages are erased
     */
    public void ResetMessages()
    {
        try
        {
            _messages.Reset();
        }
        catch (IndexOutOfRangeException e)
        {
            throw new IndexOutOfRangeException("Subscriber Inbox is Empty");
        }
    }
}
/*
 * Implementation Invariants:
 * - Errors/Exceptions thrown by Subscriber is based on the public interface/state of
 *   internal MsgStream object
 * - If internal MsgStream object is empty, ResetMessages() will throw an error
 * - Subscriber instances can read their inboxes via the use of ReadMessages()
 * - Base constructor for Subscriber will always have a capacity of 250 messages
 * - Dependency Injection is supported via Constructor Injection
 * - Property Injection is not supported due to the possibility of data loss
 */