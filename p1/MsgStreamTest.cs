// Jomikael Ruiz
// CPSC 3200
// File Created: 9/28/24
// MsgStreamTest.cs
// Last Revision: 10/3/24

namespace MsgStream;
using Microsoft.VisualStudio.TestTools.UnitTesting;

[TestClass]
public class MsgStreamTest
{
    [TestMethod]
    public void ConstructorSuccessfullyCreatesMsgStream()
    {
        // Arrange, Act, & Assert
        Assert.IsInstanceOfType(new MsgStream(10), typeof(MsgStream));
    }
    
    [TestMethod]
    public void ConstructorFailsWhenSizeGreaterThanMaxSize()
    {
        // Arrange, Act, & Assert
        Assert.ThrowsException<IndexOutOfRangeException>(() => new MsgStream(300));
    }
    
    [TestMethod]
    public void DefaultConstructorSuccessfullyCreatesMsgStream()
    {
        // Arrange, Act, & Assert
        Assert.IsInstanceOfType(new MsgStream(), typeof(MsgStream));
    }
    
    
    [TestMethod]
    public void AppendMessageSuccessfullyAddsMessage()
    {
        // Arrange
        MsgStream testObject = new MsgStream(1);

        // Act
        bool testResult = testObject.AppendMessage("hello world");

        // Assert
        Assert.IsTrue(testResult);
    }
    
    [TestMethod]
    public void AppendMessageFailsIfStreamIsFull()
    {
        // Arrange
        MsgStream testObject = new MsgStream(0);

        // Act & Asset
        Assert.ThrowsException<IndexOutOfRangeException>(() => testObject.AppendMessage("hello world"));
    }
    
    [TestMethod]
    public void AppendMessageFailsIfMessageIsNull()
    {
        // Arrange
        MsgStream testObject = new MsgStream();

        // Act & Assert
        Assert.ThrowsException<ArgumentNullException>(() => testObject.AppendMessage(null));
    }
    
    [TestMethod]
    public void AppendMessageFailsIfMaxOperationsIsMet()
    {
        // Arrange
        MsgStream testObject = new MsgStream(2);
        testObject.AppendMessage("hello world");
        for (int i = 0; i < 6 ; i++)
        {
            testObject.ReadMessage(0);
        }

        // Act & Assert
        Assert.ThrowsException<IndexOutOfRangeException>(() => testObject.AppendMessage("hello earth"));
    }
    
    [TestMethod]
    public void ReadMessageSuccessfullyReadsStream()
    {
        // Arrange
        MsgStream testObject = new MsgStream(2);
        testObject.AppendMessage("first");

        // Act
        string result = testObject.ReadMessage(0);
        
        // Assert
        Assert.AreEqual("first", result);
    }
    
    [TestMethod]
    public void ReadMessageFailsIfIndexIsNotInRange()
    {
        // Arrange
        MsgStream testObject = new MsgStream(10);

        // Act & Assert
        Assert.ThrowsException<IndexOutOfRangeException>(() => testObject.ReadMessage(5));
    }
    
    [TestMethod]
    public void ReadMessageFailsIfIndexGreaterThanSize()
    {
        // Arrange
        MsgStream testObject = new MsgStream(1);
        testObject.AppendMessage("first");

        // Act & Assert
        Assert.ThrowsException<IndexOutOfRangeException>(() => testObject.ReadMessage(2));
    }
    
    [TestMethod]
    public void ReadMessageFailsIfMaxOperationsIsMet()
    {
        // Arrange
        MsgStream testObject = new MsgStream(2);
        testObject.AppendMessage("hello world");
        testObject.AppendMessage("hello earth");
        for (int i = 0; i < 5; i++)
        {
            testObject.ReadMessage(0);
        }

        // Act & Assert
        Assert.ThrowsException<IndexOutOfRangeException>(() => testObject.ReadMessage(0));
    }
    
    [TestMethod]
    public void ReadMessagesSuccessfullyReadsStream()
    {
        // Arrange
        MsgStream testObject = new MsgStream(4);
        testObject.AppendMessage("first");
        testObject.AppendMessage("second");

        // Act
        string[] result = testObject.ReadMessages(0, 1);
        
        // Assert
        Assert.AreEqual("first", result[0]);
        Assert.AreEqual("second", result[1]);
    }
    
    [TestMethod]
    public void ReadMessagesFailsIfStartingIndexIsNotInRange()
    {
        // Arrange
        MsgStream testObject = new MsgStream(10);

        // Act & Assert
        Assert.ThrowsException<IndexOutOfRangeException>(() => testObject.ReadMessages(1, 2));
    }
    
    [TestMethod]
    public void ReadMessagesFailsIfStartingIndexGreaterThanSize()
    {
        // Arrange
        MsgStream testObject = new MsgStream(1);
        testObject.AppendMessage("first");

        // Act & Assert
        Assert.ThrowsException<IndexOutOfRangeException>(() => testObject.ReadMessages(2, 3));
    }
    
    [TestMethod]
    public void ReadMessagesFailsIfEndingIndexIsNotInRange()
    {
        // Arrange
        MsgStream testObject = new MsgStream(10);

        // Act & Assert
        Assert.ThrowsException<IndexOutOfRangeException>(() => testObject.ReadMessages(0, 11));
    }
    
    [TestMethod]
    public void ReadMessagesFailsIfEndingIndexGreaterThanSize()
    {
        // Arrange
        MsgStream testObject = new MsgStream(1);
        testObject.AppendMessage("first");

        // Act & Assert
        Assert.ThrowsException<IndexOutOfRangeException>(() => testObject.ReadMessages(0, 3));
    }
    
    [TestMethod]
    public void ResetStreamFailsIfStreamIsEmpty()
    {
        // Arrange
        MsgStream testObject = new MsgStream(1);

        // Act & Assert
        Assert.ThrowsException<IndexOutOfRangeException>(() => testObject.ResetStream());
    }
    
    [TestMethod]
    public void ReadMessagesFailsIfMaxOperationsIsMet()
    {
        // Arrange
        MsgStream testObject = new MsgStream(2);
        testObject.AppendMessage("hello world");
        testObject.AppendMessage("hello earth");
        for (int i = 0; i < 5; i++)
        {
            testObject.ReadMessage(0);
        }

        // Act & Assert
        Assert.ThrowsException<IndexOutOfRangeException>(() => testObject.ReadMessages(0,1));
    }
    
    [TestMethod]
    public void ReadAllMessagesFailsIfStreamIsEmpty()
    {
        // Arrange
        MsgStream testObject = new MsgStream(2);

        // Act & Assert
        Assert.ThrowsException<IndexOutOfRangeException>(() => testObject.ReadAllMessages());
    }
    
    [TestMethod]
    public void ReadAllMessagesSuccessfullyReadsStream()
    {
        // Arrange
        MsgStream testObject = new MsgStream(2);
        testObject.AppendMessage("first");
        testObject.AppendMessage("second");
        
        // Act
        string[] streamReturn = testObject.ReadAllMessages();

        // Assert
        Assert.AreEqual("first", streamReturn[0]);
        Assert.AreEqual("second", streamReturn[1]);
        
    }
    
    [TestMethod]
    public void ResetStreamSuccessfullyResetsStream()
    {
        // Arrange
        MsgStream testObject = new MsgStream(1);
        testObject.AppendMessage("first");
        
        // Act
        bool testResult = testObject.ResetStream();
        
        // Assert
        Assert.IsTrue(testResult);
    }
}