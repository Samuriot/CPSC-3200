// Jomikael Ruiz
// CPSC 3200
// File Created: 9/25/24
// MsgStream.cs
// Last Revision: 10/3/24

namespace MsgStream
{
    /**
     * Class Invariants:
     * - DefaultSize will always be 100 strings
     * - MaximumStreamSize will always be 300 strings
     * - MaxOperations will always be based on the size of MsgStreamTest
     *   with operations involving the Operational Constant
     * Immutables:
     * - Maximum Number of Operations Cannot Be Changed
     * - Size of MsgStream cannot be changed once set
     * - Appending to MsgStream will always append to the end of the array
     */
    public class MsgStream
    {
        private const uint DefaultSize = 100;
        private const uint MaximumStreamSize = 300;
        private const uint InitialIndex = 0;
        private const uint OperationalConstant = 5;
        
        private uint _maxSize;
        private uint _currentArrayIndex;
        private uint _maxOperations;
        private uint _totalOperations;
        private string[] _messageStream;
        
        private bool IsEmpty()
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
        private void IsValidIndex(uint inputIndex)
        {
            if (inputIndex >= _currentArrayIndex || inputIndex >= _maxSize)
            {
                throw new IndexOutOfRangeException("Index is out of range.");
            }
        }

        /*
         * Precondition: inputSize is a valid positive integer within range 0 - 299
         */
        public MsgStream(uint inputSize)
        {
            if (inputSize >= MaximumStreamSize)
            {
                throw new IndexOutOfRangeException("Size is out of range.");
            }

            _maxSize = inputSize;
            _maxOperations = _maxSize + OperationalConstant;
            _messageStream = new string[_maxSize];
            _currentArrayIndex = InitialIndex;
            _totalOperations = InitialIndex;
        }
        
        public MsgStream()
        {
            _maxSize = DefaultSize;
            _maxOperations = _maxSize + OperationalConstant;
            _messageStream = new string[_maxSize];
            _currentArrayIndex = InitialIndex;
            _totalOperations = InitialIndex;
        }
        
        /*
         * Precondition: message is a valid string, otherwise an exception is thrown
         * Postcondition: message is successfully added to the _messageStream array
         */
        public bool AppendMessage(string message)
        {
            if (IsFull())
            {
                throw new IndexOutOfRangeException("The Message Stream is Full.");
            }

            if (ExceedsMaxOperations())
            {
                throw new IndexOutOfRangeException("Maximum number of operations has been reached.");
            }
            _messageStream[_currentArrayIndex] = message ?? throw new ArgumentNullException();
            _currentArrayIndex++;
            _totalOperations++;
            return true;
        }
        
       /*
        * Precondition: messageIndex is a valid positive integer within range, otherwise an exception is thrown
        */
        public string ReadMessage(uint messageIndex)
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
        public string[] ReadMessages(uint startingIndex, uint endingIndex)
        {
            IsValidIndex(startingIndex);
            IsValidIndex(endingIndex);
            if (ExceedsMaxOperations())
            {
                throw new IndexOutOfRangeException("Maximum number of operations has been reached.");
            }
            uint newArraySize = endingIndex - startingIndex + 1;
            uint newArrayIndex = 0;
            string[] retrievedMessages = new string[newArraySize];
            for (uint i = startingIndex; i <= endingIndex; i++)
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
            for (uint i = 0; i < _currentArrayIndex; i++)
            {
                retrievedMessages[i] = ReadMessage(i);
            }
            return retrievedMessages;
        }
        
        /*
         * Precondition: _messageStream has contents within it, otherwise an exception is thrown
         * Postcondition: _messageStream is cleared of all data and reset
         */
        public bool ResetStream()
        {
            if (IsEmpty())
            {
                throw new IndexOutOfRangeException("The stream is empty.");
            }
            _messageStream = new string[_maxSize];
            return true;
        }
    }
    /*
     * Implementation Invariants:
     * - Default size of MsgStreamTest is 100 strings
     * - Exceptions are thrown when incorrect data is received into methods
     */
}