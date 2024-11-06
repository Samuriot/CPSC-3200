// Jomikael Ruiz
// CPSC 3200
// File Created: 10/21/24
// PartitionStreamTest.cs
// Last Revision: 10/27/24

namespace P3Testing;
using P3;
using Microsoft.VisualStudio.TestTools.UnitTesting;

[TestClass]
public class PartitionStreamTest
{
    [TestMethod]
    public void ConstructorSuccessfullyCreatesPartitionStream()
    {
        // Arrange, Act, & Assert
        Assert.IsInstanceOfType(new PartitionStream(), typeof(PartitionStream));
    }

    [TestMethod]
    public void ConstructorSuccessfullyInjectsMsgStream()
    {
        // Arrange
        MsgStream msgStream = new MsgStream();
        msgStream.AppendMessage("hello world!");

        // Act
        PartitionStream partitionStream = new PartitionStream(5, msgStream);

        // Assert
        Assert.AreEqual(partitionStream.ReadMessages(5)[0], msgStream.ReadMessage(0));
    }
    
    [TestMethod]
    public void ConstructorSuccessfullyInjectsDurableStream()
    {
        // Arrange
        DurableStream durableStream = new DurableStream();
        durableStream.AppendMessage("hello world!");

        // Act
        PartitionStream partitionStream = new PartitionStream(5, durableStream);

        // Assert
        Assert.AreEqual(partitionStream.ReadMessages(5)[0], durableStream.ReadMessage(0));
    }
    
    [TestMethod]
    public void ConstructorSuccessfullyCopiesPartition()
    {
        // Arrange
        PartitionStream partitionStream = new PartitionStream();
        partitionStream.AppendMessage(5, "hello world!");

        // Act
        PartitionStream copiedStream = new PartitionStream(partitionStream);
        partitionStream.ResetStream();

        // Assert
        Assert.AreEqual("hello world!", copiedStream.ReadMessages(5)[0]);
    }

    [TestMethod]
    public void ConstructorSuccessfullyCreatesWithDifferentSize()
    {
        // Act, Arrange, & Assert
        Assert.IsInstanceOfType(new PartitionStream(6), typeof(PartitionStream));

    }

    [TestMethod]
    public void ConstructorFailsIfInputIsNegative()
    {
        // Arrange, Act & Assert
        Assert.ThrowsException<IndexOutOfRangeException>(() => new PartitionStream(-1));
    }

    [TestMethod]
    public void ConstructorFailsIfInputExceedsBounds()
    {
        // Arrange, Act & Assert
        Assert.ThrowsException<IndexOutOfRangeException>(() => new PartitionStream(12));
    }
    
    [TestMethod]
    public void AppendMessagesSuccessfullyInjectsMsgStream()
    {
        // Arrange
        PartitionStream partitionStream = new PartitionStream();
        MsgStream msgStream = new MsgStream();
        msgStream.AppendMessage("hello world!");

        // Act
        partitionStream.AppendMessage(5, msgStream);

        // Assert
        Assert.AreEqual(partitionStream.ReadMessages(5)[0], msgStream.ReadMessage(0));
    }
    
    [TestMethod]
    public void ResetFailsIfStreamIsEmpty()
    {
        // Arrange
        PartitionStream partitionStream = new PartitionStream();

        // Act & Assert
        Assert.ThrowsException<IndexOutOfRangeException>(() => partitionStream.ResetStream());
    }

    [TestMethod]
    public void ResetSuccessfullyResetsStream()
    {
        // Arrange
        PartitionStream partitionStream = new PartitionStream();
        partitionStream.AppendMessage(5, "hello world!");

        // Act
        partitionStream.ResetStream();

        // Assert
        Assert.ThrowsException<IndexOutOfRangeException>(() => partitionStream.ReadMessages(5));
    }

    [TestMethod]
    public void ReadMessageFailsIfStreamIsEmpty()
    {
        // Arrange
        PartitionStream partitionStream = new PartitionStream();

        // Assert
        Assert.ThrowsException<IndexOutOfRangeException>(() => partitionStream.ReadMessages(5));
    }

    [TestMethod]
    public void ReadMessageFailsIfInputTakesUpAnIndexAlready()
    {
        // Arrange
        PartitionStream partitionStream = new PartitionStream();
        partitionStream.AppendMessage(5, "hello world!");

        // Act & Assert
        Assert.ThrowsException<IndexOutOfRangeException>(() => partitionStream.ReadMessages(0));
    }
    
    [TestMethod]
    public void ReadMessageFailsIfInputIsNegative()
    {
        // Arrange
        PartitionStream partitionStream = new PartitionStream();
        partitionStream.AppendMessage(5, "hello world!");

        // Act & Assert
        Assert.ThrowsException<IndexOutOfRangeException>(() => partitionStream.ReadMessages(-10));
    }
}