// Jomikael Ruiz
// CPSC 3200
// PartitionStream.h
// Date Created: Oct. 6th, 2024
// Last Modified: Oct. 15th, 2024

#ifndef PARTITIONSTREAM_H
#define PARTITIONSTREAM_H

#include <string>
#include "MsgStream.h"
using namespace std;

/**
 * Class Invariants:
 * - DefaultSize will always be 5 MessageStreams
 * - Max PartitionStream size is 11 MessageStreams
 * - Partition Keys will be integers with a set of operations performed
 *   to map a partitionKey to its respective index
 * Immutables:
 * - Once set, PartitionStream size cannot be altered
 */

class PartitionStream
{
	private:
		const int MAX_MSGSTREAM_OBJECTS = 11;
		const int DEFAULT_MSGSTREAM_OBJECTS = 5;

		MsgStream ** partitionStream = nullptr;
		int * partitionKeyStream = nullptr;

		unsigned int partitionSize;
		bool isNotEmpty;

		int arrayIndexHash(int);

		void isValidKey(int);
		void copyPartition(const PartitionStream&);
		void setMessageStream(int, MsgStream *);
		void deepReset();

	public:
		struct ArrayReturn
		{
			string * messages;
			int streamSize;
		};

		PartitionStream();
		PartitionStream(int, MsgStream *);

		PartitionStream(const PartitionStream&);
		PartitionStream(PartitionStream&&);

		~PartitionStream();

		PartitionStream& operator=(const PartitionStream&);
		PartitionStream& operator=(PartitionStream&&);

		void appendMessage(int, string);
		void appendMessage(int, MsgStream *);
		void resetStream();

		MsgStream::ArrayReturn readMessages(int);

};

/*
 * Implementation Invariants:
 * - When collisions occur internally with the
 *   partitionKey and arrayIndex, an exception is thrown
 * - Reading messages based on a partitionKey returns all
 *   the messages associated with that specific partition
 * - Object supports both Copy Semantics & Move Semantics
 *   and deep copying
 * - To return messages, a structure is returned containing the pointer to
 *   the array of messages and the size for easy traversal
 */



#endif //PARTITIONSTREAM_H
