// Jomikael Ruiz
// CPSC 3200
// P5.cs
// Date Created: 11/25/24
// Last Modified: 12/2/24

namespace P5;
using System.IO;

public class P5
{
    const string TestMessage = "Hello World!";
    const string TestMessage2 = "Hello World Again!";
    private const int StreamSize = 5;
    
    static MsgStream GetObjAddress(int index)
    {
        if (index % 2 == 0)
            return new MsgStream();
        return new DurableStream();
    }

    static void AddSubscribers(SubscriberStream subscriberStream, Subscriber[] subscriberReferences)
    {
        for(int i = 0; i < subscriberReferences.Length; i++)
            subscriberStream.AddSubscriber(subscriberReferences[i]);
    }

    static void ResetSubscribers(Subscriber[] subscriberReferences)
    {
        for(int i = 0; i < subscriberReferences.Length; i++)
            subscriberReferences[i].ResetMessages();
    }
    static void ReadMessages(string[] messages)
    {
        for (int j = 0; j < messages.Length; j++)
            Console.WriteLine(messages[j]);
    }
    static void TestDeepCopy(PartitionStream partitionStream, int[] keys)
    {
        PartitionStream copyStream = new PartitionStream();
        copyStream.DeepCopy(partitionStream);
        partitionStream.ResetStream();
        for (int i = 0; i < keys.Length; i++)
        {
            string[] messages = copyStream.ReadMessages(keys[i]);
            ReadMessages(messages);
        }
    }
    static void TestMsgStreamPartitionStream(PartitionStream partitionStream, int[] keys)
    {
        MsgStream initialStream = new MsgStream();
        initialStream.AppendMessage("Hello MsgStream!");
        for (int i = 0; i < keys.Length; i++)
        {
            partitionStream.AppendMessage(keys[i], new MsgStream(initialStream));
            partitionStream.AppendMessage(keys[i], i.ToString());
            string[] messages = partitionStream.ReadMessages(keys[i]);
            ReadMessages(messages);
        }
        TestDeepCopy(partitionStream, keys);
    }
    static void TestDurableStreamPartitionStream(PartitionStream partitionStream, int[] keys)
    {
        MsgStream initialStream = new DurableStream();
        initialStream.AppendMessage("Hello DurableStream!");
        for (int i = 0; i < keys.Length; i++)
        {
            DurableStream ds = new DurableStream();
            ds.DeepCopy(initialStream);
            partitionStream.AppendMessage(keys[i], ds);
            partitionStream.AppendMessage(keys[i], i.ToString());
            string[] messages = partitionStream.ReadMessages(keys[i]);
            ReadMessages(messages);
        }
        TestDeepCopy(partitionStream, keys);
    }
    static void TestHeteroGeneousStream(PartitionStream partitionStream, int[] keys)
    {
        for (int i = 0; i < keys.Length; i++)
        {
            MsgStream chosenStream = GetObjAddress(i);
            chosenStream.AppendMessage("Hello Heterogeneous Stream!");
            partitionStream.AppendMessage(keys[i], chosenStream);
            partitionStream.AppendMessage(keys[i], i.ToString());
            string[] messages = partitionStream.ReadMessages(keys[i]);
            ReadMessages(messages);
        }
        TestDeepCopy(partitionStream, keys);
    }
    
    static void TestSubscriberStream(SubscriberStream subscriberStream, Subscriber[] subscriberReferences, int[] keys)
    {
        for (int i = 0; i < StreamSize; i++)
        {
            subscriberReferences[i] = new Subscriber();
            subscriberStream.AddSubscriber(subscriberReferences[i]);
        }
        for (int i = 0; i < StreamSize; ++i)
        {
            if(i % 2 == 0)
                subscriberStream.AppendMessage(keys[i], TestMessage);
            else
                subscriberStream.AppendMessage(keys[i], TestMessage2);
        }
        ReadSubscriberMessages(subscriberReferences);
        subscriberStream.ResetStream();
    }

    static void ReadSubscriberMessages(Subscriber[] subscriberReferences)
    {
        for (int i = 0; i < StreamSize; i++)
        {
            Console.WriteLine("On subscriber #" + i);
            string[] subscriptions = subscriberReferences[i].ReadSubscriptions();
            for (int j = 0; j < subscriptions.Length; ++j)
                Console.WriteLine(subscriptions[j]);
            Console.WriteLine();
        }
    }
    
    static void TestSubscriberStreamDisposeAndReset(SubscriberStream partitionStream, int[] keys)
    {
        for(int i = 0; i < keys.Length; ++i)
            partitionStream.AppendMessage(keys[i], "filling out partition stream");
        partitionStream.Dispose();
        partitionStream.ResetStream();
        Subscriber subscriber = new Subscriber();
        partitionStream.AddSubscriber(subscriber);
        partitionStream.AppendMessage(keys[0], "successfully reopened file after reset");
        string[] messages = partitionStream.ReadMessages(keys[0]);
        ReadMessages(messages);
    }

    static void TestSubscriberStreamFunctionality(SubscriberStream subscriberStream, 
                                                  Subscriber[] subscriberReferences, 
                                                  int[] keys)
    {
        Console.WriteLine("Testing SubscriberStream: ");
        TestSubscriberStream(subscriberStream, subscriberReferences, keys);
        Console.WriteLine("Subscribers Messages after testing SubscriberStream: ");
        ReadSubscriberMessages(subscriberReferences);
        ResetSubscribers(subscriberReferences);
        
        Console.WriteLine("Testing SubscriberStream as Homogeneous MsgStream Collection: ");
        AddSubscribers(subscriberStream, subscriberReferences);
        TestMsgStreamPartitionStream(subscriberStream, keys);
        Console.WriteLine("Subscribers Messages after testing MsgStream SubscriberStream: ");
        ReadSubscriberMessages(subscriberReferences);
        ResetSubscribers(subscriberReferences);
        
        Console.WriteLine("Testing SubscriberStream as Homogeneous DurableStream Collection: ");
        AddSubscribers(subscriberStream, subscriberReferences);
        TestDurableStreamPartitionStream(subscriberStream, keys);
        Console.WriteLine("Subscribers Messages after testing DurableStream SubscriberStream: ");
        ReadSubscriberMessages(subscriberReferences);
        ResetSubscribers(subscriberReferences);
        
        Console.WriteLine("Testing SubscriberStream as Heterogeneous Collection: ");
        AddSubscribers(subscriberStream, subscriberReferences);
        TestHeteroGeneousStream(subscriberStream, keys);
        Console.WriteLine("Subscribers Messages after testing Heterogeneous SubscriberStream: ");
        ReadSubscriberMessages(subscriberReferences);
        ResetSubscribers(subscriberReferences);
        
        Console.WriteLine("Testing PartitionStream, MsgStream, and DurableStream Dispose");
        AddSubscribers(subscriberStream, subscriberReferences);
        TestSubscriberStreamDisposeAndReset(subscriberStream, keys);
        ResetSubscribers(subscriberReferences);

    }
    
    public static void Main(string[] args)
    {
        string[] baseFileContents = { "Hello from the Backing File!" };
        string filePath = "../../../messages.txt";
        string copyFilePath = "../../../copyOfmessages.txt";
        File.AppendAllLines(filePath, baseFileContents);
        
        int[] partitionKeys = { 0, 1, 2, 3, 4 };
        SubscriberStream subscriberStream = new SubscriberStream();
        Subscriber[] subscriberReferences = new Subscriber[StreamSize];
        
        TestSubscriberStreamFunctionality(subscriberStream, subscriberReferences, partitionKeys);
        
        File.Delete(filePath);
        File.Delete(copyFilePath);
    }
}