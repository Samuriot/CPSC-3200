// Jomikael Ruiz
// CPSC 3200
// P3.cs
// Date Created: Oct. 25th, 2024
// Last Modified: Oct. 27th, 2024

namespace P3;
using System.IO;

public class P3
{
    static MsgStream GetObjAddress(int index)
    {
        if (index % 2 == 0)
            return new MsgStream();
        return new DurableStream();
    }

    static void ReadMessages(PartitionStream partitionStream, int key)
    {
        string[] messages = partitionStream.ReadMessages(key);
        for (int j = 0; j < messages.Length; j++)
            Console.WriteLine(messages[j]);
        Console.WriteLine();
    }

    static void TestDeepCopy(PartitionStream partitionStream, int[] keys)
    {
        PartitionStream copyStream = new PartitionStream();
        copyStream.DeepCopy(partitionStream);
        
        partitionStream.ResetStream();
        for (int i = 0; i < keys.Length; i++)
        {
            ReadMessages(copyStream, keys[i]);
        }
    }

    static void TestMsgStreamPartitionStream(PartitionStream partitionStream, int[] keys)
    {
        MsgStream initialStream = new MsgStream();
        initialStream.AppendMessage("initial message, Hello!");
        for (int i = 0; i < keys.Length; i++)
        {
            partitionStream.AppendMessage(keys[i], new MsgStream(initialStream));
            partitionStream.AppendMessage(keys[i], i.ToString());
            ReadMessages(partitionStream, keys[i]);
        }
        TestDeepCopy(partitionStream, keys);
    }
    
    static void TestDurableStreamPartitionStream(PartitionStream partitionStream, int[] keys)
    {
        MsgStream initialStream = new DurableStream();
        initialStream.AppendMessage("initial message, Hello!");
        for (int i = 0; i < keys.Length; i++)
        {
            DurableStream ds = new DurableStream();
            ds.DeepCopy(initialStream);
            partitionStream.AppendMessage(keys[i], ds);
            partitionStream.AppendMessage(keys[i], i.ToString());
            ReadMessages(partitionStream, keys[i]);
        }
        TestDeepCopy(partitionStream, keys);
    }

    static void TestHeteroGeneousStream(PartitionStream partitionStream, int[] keys)
    {
        for (int i = 0; i < keys.Length; i++)
        {
            MsgStream chosenStream = GetObjAddress(i);
            chosenStream.AppendMessage("initial message, Hello!");
            partitionStream.AppendMessage(keys[i], chosenStream);
            partitionStream.AppendMessage(keys[i], i.ToString());
            ReadMessages(partitionStream, keys[i]);
        }
        
        TestDeepCopy(partitionStream, keys);
    }

    static void TestDurableStreamDisposeAndReset()
    {
        DurableStream initialStream = new DurableStream();
        initialStream.AppendMessage("initial message, Hello!");
        initialStream.Dispose();
        try
        {
            initialStream.AppendMessage("should be an exception");
        }
        catch (IOException e)
        {
            Console.WriteLine("Caught IOException");
        }
        
        initialStream.Reset();
        initialStream.AppendMessage("successfully reopened file after reset");
    }
    
    public static void Main(string[] args)
    {
        int result = 1;
        // int[] partitionKeys = { 0, 1, 2, 3, 4 };
        // string[] baseFileContents = { "Hello from the Backing File!" };
        // string filePath = "../../../messages.txt";
        // File.AppendAllLines(filePath, baseFileContents);
        //
        // Console.WriteLine("Testing Homogeneous Partition Stream of MsgStream");
        // PartitionStream onlyMsgStream = new PartitionStream();
        // TestMsgStreamPartitionStream(onlyMsgStream, partitionKeys);
        // Console.WriteLine();
        //
        // Console.WriteLine("Testing Homogeneous Partition Stream of DurableStream");
        // PartitionStream onlyDurableStream = new PartitionStream();
        // TestDurableStreamPartitionStream(onlyDurableStream, partitionKeys);
        // Console.WriteLine();
        //
        // Console.WriteLine("Testing Heterogeneous Stream");
        // PartitionStream heterogeneousStream = new PartitionStream();
        // TestHeteroGeneousStream(heterogeneousStream, partitionKeys);
        // Console.WriteLine();
        //
        // Console.WriteLine("Testing DurableStream Dispose and Reset");
        // TestDurableStreamDisposeAndReset();
        //
        // File.Delete(filePath);
    }
    
}