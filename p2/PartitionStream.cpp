// Jomikael Ruiz
// CPSC 3200
// PartitionStream.cpp
// Date Created: Oct. 6th, 2024
// Last Modified: Oct. 15th, 2024

#include "PartitionStream.h"
#include "MsgStream.h"
using namespace std;

/**
 * pre-condition: partitionKey is a valid integer
 */
int PartitionStream::arrayIndexHash(int partitionKey)
{
	return partitionKey % partitionSize;
}

/**
 * pre-condition: partitionKey is a valid integer
 */
void PartitionStream::isValidKey(int partitionKey)
{
	int arrayIndex = arrayIndexHash(partitionKey);



	if( partitionStream[arrayIndex] &&
		partitionKeyStream[arrayIndex] != partitionKey )
	{
		throw(invalid_argument("An Index is occupying the expected partition key"));
	}
}

/**
 * pre-condition: messageStream is a valid MsgStream pointer,
 * 				  partitionKey is a valid integer
 * post-condition: messageStream is inserted into the internal array
 */
void PartitionStream::setMessageStream(int partitionKey, MsgStream * messageStream)
{
	if(!messageStream)
		throw(invalid_argument("MsgStream pointer is nullptr"));

	isValidKey(partitionKey);

	int arrayIndex = arrayIndexHash(partitionKey);

	this->partitionStream[arrayIndex] = messageStream;
	this->partitionKeyStream[arrayIndex] = partitionKey;
	this->isNotEmpty = true;
}

/**
 * pre-condition: src is a valid PartitionStream to deep copy from
 * post-condition: a deep copied version of src is created
 */
void PartitionStream::copyPartition(const PartitionStream& src)
{
	this->partitionSize = src.partitionSize;
	this->isNotEmpty = src.isNotEmpty;
	this->partitionStream = new MsgStream * [partitionSize];
	this->partitionKeyStream = new int[partitionSize];

	for( int i = 0; i < partitionSize; i++ )
	{
		if( !src.partitionStream[i] )
			continue;
		this->partitionStream[i] = new MsgStream( *src.partitionStream[i] );
		this->partitionKeyStream[i] = src.partitionKeyStream[i];
	}
}

/**
 * post-condition: PartitionStream is completely reset to its original form
 */
void PartitionStream::deepReset()
{
	for( unsigned int i = 0; i < partitionSize; i++ )
	{
		delete partitionStream[i];
		partitionStream[i] = nullptr;
	}

	delete [] partitionStream;
	delete [] partitionKeyStream;

	partitionStream = nullptr;
	partitionKeyStream = nullptr;
	this->isNotEmpty = false;
}

PartitionStream::PartitionStream()
{
	partitionSize = DEFAULT_MSGSTREAM_OBJECTS;
	partitionStream = new MsgStream * [partitionSize];
	partitionKeyStream = new int[partitionSize];
	isNotEmpty = false;
}

/**
 * pre-condition: messageStream is a valid MsgStream * for dependency injection
 * 				  partitionKey is a valid integer
 */
PartitionStream::PartitionStream(int partitionKey, MsgStream * messageStream)
{
	partitionSize = DEFAULT_MSGSTREAM_OBJECTS;
	partitionStream = new MsgStream * [partitionSize];
	partitionKeyStream = new int[partitionSize];
	setMessageStream(partitionKey, messageStream);
}

PartitionStream::PartitionStream(const PartitionStream& src)
{
	copyPartition(src);
}

/**
 * pre-condition: src is a valid PartitionStream
 */
PartitionStream::PartitionStream(PartitionStream&& src)
{
	this->partitionSize = src.partitionSize;
	this->isNotEmpty = src.isNotEmpty;
	this->partitionStream = src.partitionStream;
	this->partitionKeyStream = src.partitionKeyStream;

	src.partitionStream = nullptr;
	src.partitionSize = 0;
}

/**
 * post-condition: PartitionStream is completely deallocated
 */
PartitionStream::~PartitionStream()
{
	deepReset();
}

/**
 * pre-condition: src is a valid PartitionStream object
 * post-condition: PartitionStream object is deep copied from src
 */
PartitionStream& PartitionStream::operator=(const PartitionStream& src)
{
	if( this == &src )
		return *this;

	deepReset();
	copyPartition(src);

	return *this;
}

/**
 * pre-condition: src is a valid PartitionStream object
 * post-condition: PartitionStream object is deep copied from src
 */
PartitionStream& PartitionStream::operator=(PartitionStream&& src)
{
	if( this == &src )
		return *this;

	swap(this->partitionSize, src.partitionSize);
	swap(this->isNotEmpty, src.isNotEmpty);
	swap(this->partitionStream, src.partitionStream);
	swap(this->partitionKeyStream, src.partitionKeyStream);

	return *this;
}

/**
 * pre-condition: partitionKey is a valid integer, message is a valid string
 * post-condition: string is added to the respective partition
 */
void PartitionStream::appendMessage(int partitionKey, string message)
{
	int arrayIndex = arrayIndexHash(partitionKey);

	if( !this->partitionStream[arrayIndex] )
	{
		partitionStream[arrayIndex] = new MsgStream();
		partitionKeyStream[arrayIndex] = partitionKey;
	}

	isValidKey(partitionKey);

	partitionStream[arrayIndex]->appendMessage(message);
	isNotEmpty = true;
}

/**
 * pre-condition: partitionKey is a valid integer, messages is a valid MsgStream ptr
 * post-condition: string is added to the respective partition
 */
void PartitionStream::appendMessage(int partitionKey, MsgStream * messages)
{
	if( !messages )
		throw( invalid_argument("MsgStream pointer is nullptr") );
	setMessageStream(partitionKey, messages);
}

/**
 * pre-condition: PartitionStream must not be empty to reset,
 * 				  otherwise an exception is thrown
 * post-condition: internal array is completely wiped and reset
 */
void PartitionStream::resetStream()
{
	if( !isNotEmpty )
		throw(invalid_argument("The Stream is already empty"));

	deepReset();
	this->partitionStream = new MsgStream*[partitionSize];
}

/**
 * pre-condition: the PartitionStream must not be empty,
 * 				  otherwise an exception is thrown,
 * 				  partitionKey must be a valid integer
 * post-condition: a duplicate array with all values in the respective partition
 * 				   is returned in the form of a structure,
 * 				   client is responsible for deallocation of string returned
 */
MsgStream::ArrayReturn PartitionStream::readMessages(int partitionKey)
{
	isValidKey(partitionKey);
	int arrayIndex = arrayIndexHash(partitionKey);

	if( !partitionStream[arrayIndex] )
		throw((invalid_argument("There is no partition allocated here")));

	return partitionStream[arrayIndex]->readAllMessage();
}

