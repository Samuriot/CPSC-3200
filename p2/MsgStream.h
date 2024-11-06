// Jomikael Ruiz
// CPSC 3200
// MsgStream.h
// Date Created: Oct. 4th, 2024
// Last Modified: Oct. 15th, 2024

#ifndef MSGSTREAM_H
#define MSGSTREAM_H

#include <string>
using namespace std;

/**
 * Class Invariants:
 * - DefaultSize will always be 100 strings
 * - MaxOperations will always be based on the size of MsgStream
 * - with operations involving the Operational Constant
 * Immutables:
 * - Maximum Number of Operations cannot be changed
 * - Size of MsgStream cannot be changed once set
 * - Appending to MsgStream will always append to the end of the array
 */

class MsgStream
{
    private:
        const unsigned int DEFAULT_SIZE = 100;
        const unsigned int MAXIMUM_STREAM_SIZE = 300;
        const unsigned int INITIAL_INDEX = 0;
        const unsigned int OPERATIONAL_CONSTANT = 5;

        unsigned int maxSize;
        unsigned int currentArrayIndex;
        unsigned int maxOperations;
        unsigned int totalOperations;
        string * messageStream = nullptr;

        bool isEmpty();
        bool isFull();
        bool exceedsMaxOperations();

        void isValidIndex(unsigned int);
		void initializeStream(unsigned int);
		void copyStream(const MsgStream &);

	public:
		struct ArrayReturn
		{
			string * messages;
			int streamSize;
			ArrayReturn(string * messages, int size) {
				this->messages = messages;
				this->streamSize = size;
			}
		};

        MsgStream();
		MsgStream(unsigned int);

		MsgStream(const MsgStream &);
		MsgStream(MsgStream &&);

		~MsgStream();

        bool appendMessage(string);
        bool resetStream();

        string readMessage(unsigned int);
        ArrayReturn readMessages(unsigned int, unsigned int);
        ArrayReturn readAllMessage();

};

/*
 * Implementation Invariants:
 * - Default size of MsgStream is 100 strings
 * - Exceptions are thrown when incorrect data is received into methods
 * - To return messages, a structure is returned containing the pointer to
 *   the array of messages and the size for easy traversal
 */


#endif 
