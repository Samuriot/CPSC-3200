// Jomikael Ruiz
// CPSC 3200
// MsgStream.h
// Date Created: Oct. 4th, 2024
// Last Modified: Nov. 15th, 2024

#ifndef MSGSTREAM_H
#define MSGSTREAM_H

#include <string>
using namespace std;

/**
 * Class Invariants:
 * - Class can be inherited by other class objects
 * - DefaultSize will always be 100 strings
 * - MaxOperations will always be based on the size of MsgStream
 * - with operations involving the Operational Constant
 * - Operators supported: ==, !=, +, +=, >/>=, </<=, ++, []
 * - Resizing MsgStream can only occur via use of ++ operator
 * - Index Operator returns the string at a specific index
 * - Equality Checks are based on if the strings of both MsgStreams are equal
 * - Relational Checks are based on the number of strings currently in MsgStream
 * - Plus Operators add a string to the MsgStream and returns a temp version
 * - Plus Operators also support MsgStream + MsgStream addition,
 * 	 adding the contents together in a temporary MsgStream object
 * - PlusEquals Operators add a string directly to this MsgStream
 * - PlusEquals Operators also support MsgStream + MsgStream addition,
 *   adding the contents together to this MsgStream
 * Immutables:
 * - MsgStream cannot be reset if stream is empty
 * - Appending to MsgStream will always append to the end of the array
 * - Once maxOperations has been reached, no more operations can be performed on MsgStream
 */

class MsgStream
{
    private:
        const unsigned int INITIAL_INDEX = 0;
        const unsigned int OPERATIONAL_CONSTANT = 5;

        unsigned int maxOperations;
        unsigned int totalOperations;

        bool isFull();
        bool exceedsMaxOperations();
		bool checkEquality(const MsgStream&);

        void isValidIndex(int);
		void initializeStream(unsigned int);

	protected:
		const unsigned int DEFAULT_SIZE = 100;
		const unsigned int MAXIMUM_STREAM_SIZE = 300;

		unsigned int maxSize;
		unsigned int currentArrayIndex;

		unique_ptr<string[]> messageStream;

		void copyStream(const MsgStream &);

		bool isEmpty();
	public:
		struct ArrayReturn
		{
			unique_ptr<string[]> messages;
			int streamSize;
			ArrayReturn(unique_ptr<string[]>& input, int size) {
				messages.swap(input);
				streamSize = size;
			}
		};

        MsgStream();
		MsgStream(unsigned int);

		MsgStream(const MsgStream &);
		MsgStream(MsgStream &&);

		MsgStream& operator=(const MsgStream&);
		MsgStream& operator=(MsgStream&&);

		string operator[](int);

		bool operator==(MsgStream&);
		bool operator!=(MsgStream&);

		MsgStream operator+(const MsgStream&);
		MsgStream operator+(string);

		MsgStream operator+=(const MsgStream&);
		MsgStream operator+=(const string&);

		bool operator>(const MsgStream&);
		bool operator>=(const MsgStream&);

		bool operator<(const MsgStream&);
		bool operator<=(const MsgStream&);

		MsgStream operator++();
		MsgStream operator++(int);

		~MsgStream();

	virtual bool appendMessage(string);

	virtual bool resetStream();

        string readMessage(int);
        ArrayReturn readMessages(int, int);
		ArrayReturn readAllMessage();

};

/*
 * Implementation Invariants:
 * - Default size of MsgStream is 100 strings
 * - Exceptions are thrown when incorrect data is received into methods
 * - To return messages, a structure is returned containing the pointer to
 *   the array of messages and the size for easy traversal
 * - The use of + operator does not affect the internals of this MsgStream object
 * - += acts as a mutator and affects this MsgStream object's data
 * - [] allows client to access specific string indices by directly calling readMessage on that index
 * - Size checks are performed for +, +=, and ++ operators to prevent oversizing of MsgStream
 * - Support is provided for both pre- and post-increment
 * - Using any of the operators counts as an operation performed, incrementing the total operations count
 */


#endif 
