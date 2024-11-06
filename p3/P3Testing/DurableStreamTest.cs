// Jomikael Ruiz
// CPSC 3200
// File Created: 10/21/24
// DurableStreamTest.cs
// Last Revision: 10/27/24

using P3;

namespace P3Testing;
using Microsoft.VisualStudio.TestTools.UnitTesting;

[TestClass]
public class DurableStreamTest
{
    [TestMethod]
    public void ConstructorSuccessfullyCreatesDurableStream()
    {
        // Arrange, Act, & Assert
        Assert.IsInstanceOfType(new DurableStream(), typeof(DurableStream));
        File.Delete("../../../messages.txt");
    }
    
    [TestMethod]
    public void ConstructorSuccessfullyCreatesDurableStreamWithNewFileName()
    {
        // Arrange, Act, & Assert
        Assert.IsInstanceOfType(new DurableStream("testingFile.txt"), typeof(DurableStream));
        File.Delete("../../../testingFile.txt");
    }
    
    [TestMethod]
    public void ConstructorSuccessfullyCreatesNewFile()
    {
        // Arrange, Act, & Assert
        Assert.IsInstanceOfType(new DurableStream("testingFile.txt"), typeof(DurableStream));
        File.Delete("../../../testingFile.txt");
    }
    
    [TestMethod]
    public void ConstructorSuccessfullyCreatesDurableStreamWithDifferentSize()
    {
        // Arrange, Act, & Assert
        Assert.IsInstanceOfType(new DurableStream(5), typeof(DurableStream));
        File.Delete("../../../messages.txt");
    }
    
    [TestMethod]
    public void ConstructorFailsIfFileIsTooBig()
    {
        // Arrange
        string filePath = "../../../overloadMessages.txt";
        string[] overloadedArray = new string[310];
        for (int i = 0; i < overloadedArray.Length; i++)
        {
            overloadedArray[i] = "overloaded";
        }
        File.AppendAllLines(filePath, overloadedArray);
        // Act, & Assert
        Assert.ThrowsException<IOException>(() => new DurableStream("overloadMessages.txt"));  
        File.Delete(filePath);
    }
    
    [TestMethod]
    public void DisposeSuccessfullyClosesFile()
    {
        // Act
        DurableStream durableStream = new DurableStream("testingFile.txt");
        durableStream.AppendMessage("first append!");
        durableStream.Dispose();
        
        // Arrange & Assert
        Assert.ThrowsException<IOException>(() => durableStream.AppendMessage("testing!"));
    }
    
    [TestMethod]
    public void DisposeThrowsExceptionWhenStreamIsEmpty()
    {
        // Act
        DurableStream durableStream = new DurableStream("testingFile.txt");
        
        // Arrange & Assert
        Assert.ThrowsException<IndexOutOfRangeException>(() => durableStream.Dispose());
    }
    
    [TestMethod]
    public void DisposeThrowsExceptionWhenStreamIsClosed()
    {
        // Act
        DurableStream durableStream = new DurableStream("testingFile.txt");
        durableStream.AppendMessage("first append!");
        durableStream.Dispose();
        
        // Arrange & Assert
        Assert.ThrowsException<IOException>(() => durableStream.Dispose());
    }
    
    [TestMethod]
    public void ResetThrowsExceptionWhenStreamIsEmpty()
    {
        // Act
        DurableStream durableStream = new DurableStream("testingFile.txt");
        
        // Arrange & Assert
        Assert.ThrowsException<IndexOutOfRangeException>(() => durableStream.Reset());
    }
    
    [TestMethod]
    public void ResetSuccessfullyResetsDurableStream()
    {
        // Act
        string[] testStrings = { "Hello World from the File!" };
        string filePath = "../../../testingFile.txt";
        File.AppendAllLines(filePath, testStrings);
        DurableStream durableStream = new DurableStream("testingFile.txt");
        durableStream.AppendMessage("testing!");
        
        // Arrange
        durableStream.Reset();
        
        // Assert
        string[] originalMessages = durableStream.ReadAllMessages();
        for (int i = 0; i < originalMessages.Length; i++)
        {
            Assert.AreEqual(testStrings[i], originalMessages[i]);
        }
        File.Delete(filePath);
    }
    
    [TestMethod]
    public void AppendMessageSuccessfullyAddsMessage()
    {
        // Arrange
        DurableStream testObject = new DurableStream("testingFile.txt");

        // Act
        bool testResult = testObject.AppendMessage("hello world");

        // Assert
        Assert.IsTrue(testResult);
    }
    
    [TestMethod]
    public void AppendMessageSuccessfullyWritesToFile()
    {
        // Arrange
        string filePath = "../../../testingFile.txt";
        File.AppendAllLines(filePath, new string[] { "hello world" });
        DurableStream testObject = new DurableStream("testingFile.txt");

        // Act
        testObject.AppendMessage("hello world");
        testObject.AppendMessage("hello world pt2");
        bool finalResult = testObject.AppendMessage("hello world pt2");

        // Assert
        Assert.IsTrue(finalResult);
        File.Delete(filePath);
    }
    
    [TestMethod]
    public void DeepCopySuccessfullyCopiesDurableStream()
    {
        // Arrange
        string[] testStrings = { "Hello World from the File!" };
        string filePath = "../../../testingFile.txt";
        string copyFilePath = "../../../copyOftestingFile.txt";
        string originalFilePath = "../../../messages.txt";
        File.AppendAllLines(filePath, testStrings);
        DurableStream testObject = new DurableStream("testingFile.txt");
        DurableStream copyObject = new DurableStream();

        // Act
        copyObject.DeepCopy(testObject);
        string[] copiedMessages = copyObject.ReadAllMessages();
        string[] originalMessages = testObject.ReadAllMessages();

        // Assert
        for (int i = 0; i < originalMessages.Length; i++)
        {
            Assert.AreEqual(copiedMessages[i], originalMessages[i]);
        }

        File.Delete(filePath);
        File.Delete(copyFilePath);
        File.Delete(originalFilePath);
    }
    
    [TestMethod]
    public void DeepCopySuccessfullyCopiesMultipleTimes()
    {
        // Arrange
        string[] testStrings = { "Hello World from the File!" };
        string filePath = "../../../testingFile.txt";
        string copyFilePath = "../../../copyOftestingFile.txt";
        string secondCopyFilePath = "../../../1copyOftestingFile.txt";
        string defaultFilePath = "../../../messages.txt";
        
        File.AppendAllLines(filePath, testStrings);
        
        DurableStream testObject = new DurableStream("testingFile.txt");
        DurableStream copyObject = new DurableStream();
        DurableStream secondCopyObject = new DurableStream();

        // Act
        copyObject.DeepCopy(testObject);
        secondCopyObject.DeepCopy(testObject);
        
        string[] copiedMessages = copyObject.ReadAllMessages();
        string[] secondCopiedMessages = secondCopyObject.ReadAllMessages();
        string[] originalMessages = testObject.ReadAllMessages();

        // Assert
        for (int i = 0; i < originalMessages.Length; i++)
        {
            Assert.AreEqual(copiedMessages[i], originalMessages[i]);
            Assert.AreEqual(secondCopiedMessages[i], originalMessages[i]);
        }

        File.Delete(filePath);
        File.Delete(copyFilePath);
        File.Delete(secondCopyFilePath);
        for (int i = 0; i < 2; i++)
        {
            File.Delete(defaultFilePath);
        }
    }
    
}