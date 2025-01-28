// Jomikael Ruiz
// CPSC 3200
// PartitionStream.h
// Date Created: Oct. 6th, 2024
// Last Modified: Nov. 15th, 2024

#ifndef PARTITIONSTREAM_H
#define PARTITIONSTREAM_H

#include <string>
#include "MsgStream.h"
using namespace std;

/**
 * Class Invariants:
 * - DefaultSize will always be 5 MessageStreams
 * - Max PartitionStream size is 11 MessageStreams
 * - Size can be changed on a PartitionStream basis via the constructor
 *   as well as the increment and decrement operators
 * - Pre- and Post- decrement are supported for resizing operations
 * - Operators supported: ==/!=, >/>=, </<=, +/+=, ++, --
 * - Partition Keys will be positive integers with a set of operations performed
 *   to map a partitionKey to its respective index
 *  - Partition Keys cannot be negative, otherwise an exception is thrown
 *  - PartitionStreams support the storage of both MsgStreams and DurableStreams
 * Immutables:
 * - On initialization, all key values are inately set to -1
 */

class PartitionStream
{
	private:
		const int MAX_MSGSTREAM_OBJECTS = 11;
		const int DEFAULT_MSGSTREAM_OBJECTS = 5;
		const int INITIAL_VALUE = 0;
		const int DEFAULT_KEY_VALUE = -1;

		unique_ptr<MsgStream[]> partitionStream;
		unique_ptr<int[]> partitionKeyStream;

		unsigned int partitionSize;
		unsigned int numberOfStreams;

		int arrayIndexHash(int);

		void isValidKey(int);
		void copyPartition(const PartitionStream&);
		void setMessageStream(int, unique_ptr<MsgStream>&);
		void deepReset();
		void initializeStream(int);
		void increaseSize();
		void decreaseSize();

		bool checkEquality(const PartitionStream&);
		bool isEmpty();

		PartitionStream(const PartitionStream&);
		PartitionStream(PartitionStream&&) noexcept ;

		PartitionStream& operator=(const PartitionStream&);
		PartitionStream& operator=(PartitionStream&&) noexcept;
	public:
		PartitionStream();
		PartitionStream(int);
		PartitionStream(int, unique_ptr<MsgStream>);

		MsgStream& operator[](int);

		bool operator==(const PartitionStream&);
		bool operator!=(const PartitionStream&);

		bool operator>=(const PartitionStream&) const;
		bool operator>(const PartitionStream&) const;

		bool operator<=(const PartitionStream&) const;
		bool operator<(const PartitionStream&) const;

		PartitionStream operator+(PartitionStream&);
		PartitionStream operator+=(PartitionStream&);

		PartitionStream operator++();
		PartitionStream operator++(int);

		PartitionStream operator--();
		PartitionStream operator--(int);

		~PartitionStream();

		void appendMessage(int, string);
		void appendMessage(int, unique_ptr<MsgStream>&);
		void resetStream();

		MsgStream::ArrayReturn readMessages(int);

};

/*
 * Implementation Invariants:
 * - When collisions occur internally, partitionStream will search
 *   through the entire array to search for a specific partitionKey
 * - This search will return -1 IF AND ONLY IF there are no available indices
 * - When the PartitionStream is full, no more MsgStreams can be injected via dependency
 *   injection UNLESS the increment operators are called to resize partitionStream
 * - Reading messages based on a partitionKey returns all
 *   the messages associated with that specific partition via MsgStream::ArrayReturn structure
 * - partitionStreams will keep track of the number of Streams internally
 * - Object does not support both Copy Semantics & Move Semantics as these
 * 	 are suppressed
 * - Index Operators return the MsgStream/DurableStream at the specific partitionKey
 * - To return messages, a structure is returned containing the pointer to
 *   the array of messages and the size for easy traversal
 */



#endif //PARTITIONSTREAM_H
