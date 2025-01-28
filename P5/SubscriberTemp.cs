namespace P5;

public class SubscriberTemp : MsgStream, ISubscriber
{
    private const int DefaultMessages = 250;

    public SubscriberTemp() : base(DefaultMessages)
    {
        
    }
    
    public SubscriberTemp(int size) : base(size)
    {
        if(size <= 0 || size > MaximumStreamSize)
            throw new ArgumentOutOfRangeException("size must be between 0 and " + MaximumStreamSize);
    }
    public void NewMessage(string message)
    {
        base.AppendMessage(message);
    }

    public override void AppendMessage(string message)
    {
        NewMessage(message);
    }
}