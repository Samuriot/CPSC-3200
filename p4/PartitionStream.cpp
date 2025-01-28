// Jomikael Ruiz
// CPSC 3200
// PartitionStream.cpp
// Date Created: Oct. 6th, 2024
// Last Modified: Nov. 15th, 2024

#include "PartitionStream.h"

#include <memory>
#include "MsgStream.h"
using namespace std;

/**
 * pre-condition: partitionKey is a valid integer
 */
int PartitionStream::arrayIndexHash(int partitionKey)
{
	if(partitionKey < 0)
		throw(invalid_argument("partitionKey is negative"));
	int keyIndex = partitionKey % partitionSize;
	if(partitionKeyStream[keyIndex] == partitionKey)
		return keyIndex;
	for(int i = 0; i < partitionSize; i++) {
		if(partitionKeyStream[i] == partitionKey)
			return i;
		if(partitionKeyStream[i] == DEFAULT_KEY_VALUE) {
			return i;
		}
	}
	return DEFAULT_KEY_VALUE;
}

/**
 * pre-condition: partitionKey is a valid integer
 */
void PartitionStream::isValidKey(int partitionKey)
{
	int arrayIndex = arrayIndexHash(partitionKey);
	if( arrayIndex == DEFAULT_KEY_VALUE )
	{
		throw(invalid_argument("The partitionKey was not found in the list"));
	}
}

bool PartitionStream::isEmpty()
{
	return numberOfStreams == 0;
}

/**
 * pre-condition: messageStream is a valid MsgStream pointer,
 * 				  partitionKey is a valid integer
 * post-condition: messageStream is inserted into the internal array
 */
void PartitionStream::setMessageStream(int partitionKey, unique_ptr<MsgStream>& messageStream)
{
	if(!messageStream)
		throw(invalid_argument("MsgStream pointer is nullptr"));

	isValidKey(partitionKey);

	int arrayIndex = arrayIndexHash(partitionKey);

	this->partitionStream[arrayIndex] = *messageStream.release();
	this->partitionKeyStream[arrayIndex] = partitionKey;
	numberOfStreams++;
}

/**
 * pre-condition: src is a valid PartitionStream to deep copy from
 * post-condition: a deep copied version of src is created
 */
void PartitionStream::copyPartition(const PartitionStream& src)
{
	this->partitionSize = src.partitionSize;
	this->numberOfStreams = src.numberOfStreams;
	this->partitionStream = make_unique<MsgStream[]>(partitionSize);
	this->partitionKeyStream.reset(new int[partitionSize]);

	for( int i = 0; i < partitionSize; i++ )
	{
		if( partitionKeyStream[i] == DEFAULT_KEY_VALUE )
			continue;
		this->partitionStream[i] = src.partitionStream[i];
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
		try {
			partitionStream[i].resetStream();
		} catch(exception& out_of_range) {
			continue;
		}
	}
	partitionStream.reset();
	partitionKeyStream.reset();
}

/**
 * pre-condition: PartitionStream's paritionSize must not be at MAX_MSGSTREAM_OBJECTS
 * post-condition: partitionSize is incrememented by one and pointers are resized
 */
void PartitionStream::increaseSize()
{
	if(partitionSize >= MAX_MSGSTREAM_OBJECTS)
		throw(out_of_range("PartitionStream at Max Size"));
	this->partitionSize++;
	unique_ptr<MsgStream[]> tempStream = make_unique<MsgStream[]>(partitionSize);
	unique_ptr<int[]> tempKeyStream = make_unique<int[]>(partitionSize);
	for(int i = 0; i < partitionSize - 1; i++) {
		tempStream[i] = partitionStream[i];
		tempKeyStream[i] = partitionKeyStream[i];
	}
	tempKeyStream[partitionSize - 1] = DEFAULT_KEY_VALUE;
	partitionStream.swap(tempStream);
	partitionKeyStream.swap(tempKeyStream);
}

/**
 * pre-condition: PartitionStream must not have the last array index occupied
 * post-condition: partitionSize is decremented by one and pointers are resized
 */
void PartitionStream::decreaseSize()
{
	if(partitionKeyStream[partitionSize - 1] != DEFAULT_KEY_VALUE)
		throw (out_of_range("A partition is occupying the last index, cannot decrement size."));
	this->partitionSize--;
	unique_ptr<MsgStream[]> tempStream = make_unique<MsgStream[]>(partitionSize);
	unique_ptr<int[]> tempKeyStream = make_unique<int[]>(partitionSize);
	for(int i = 0; i < partitionSize; i++) {
		tempStream[i] = partitionStream[i];
		tempKeyStream[i] = partitionKeyStream[i];
	}
	partitionStream.swap(tempStream);
	partitionKeyStream.swap(tempKeyStream);
}

/**
 * pre-condition: stream must be a valid PartitionStream
 */
bool PartitionStream::checkEquality(const PartitionStream& stream) {
	if (stream.partitionSize != this->partitionSize)
		return false;
	for(int i = 0; i < partitionSize; i++) {
		if(stream.partitionKeyStream[i] == DEFAULT_KEY_VALUE
			&& this->partitionKeyStream[i] == DEFAULT_KEY_VALUE)
			continue;

		if(stream.partitionKeyStream[i] == DEFAULT_KEY_VALUE
		   || this->partitionKeyStream[i] == DEFAULT_KEY_VALUE)
			return false;

		bool equalityCheck = ((stream.partitionStream[i])==(this->partitionStream[i]));
		if(!equalityCheck) {
			return false;
		}
	}
	return true;
}

/**
 * pre-condition: size must be a valid positive integer
 * post-condition: PartitionStream's data members are initialized
 */
void PartitionStream::initializeStream(int size)
{
	if(size <= 0) {
		throw(invalid_argument("non-positive input size"));
	}
	partitionSize = size;
	partitionStream = make_unique<MsgStream[]>(partitionSize);
	partitionKeyStream = make_unique<int[]>(partitionSize);
	for (int i = 0; i < partitionSize; i++)
	{
		partitionKeyStream[i] = DEFAULT_KEY_VALUE;
	}
	numberOfStreams = INITIAL_VALUE;
}

PartitionStream::PartitionStream()
{
	initializeStream(DEFAULT_MSGSTREAM_OBJECTS);
}

PartitionStream::PartitionStream(int size)
{
	initializeStream(size);
}

/**
 * pre-condition: messageStream is a valid MsgStream * for dependency injection
 * 				  partitionKey is a valid integer
 */
PartitionStream::PartitionStream(int partitionKey, unique_ptr<MsgStream> messageStream)
{
	initializeStream(DEFAULT_MSGSTREAM_OBJECTS);
	setMessageStream(partitionKey, messageStream);
}

/**
 * pre-condition: src must be a valid PartitionStream
 */
PartitionStream::PartitionStream(const PartitionStream& src)
{
	copyPartition(src);
}

/**
 * pre-condition: src is a valid PartitionStream
 */
PartitionStream::PartitionStream(PartitionStream&& src) noexcept
{
	this->partitionSize = src.partitionSize;
	this->partitionStream.reset(src.partitionStream.release());
	this->partitionKeyStream.reset(src.partitionKeyStream.release());

	src.partitionSize = 0;
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
PartitionStream& PartitionStream::operator=(PartitionStream&& src) noexcept
{
	if( this == &src )
		return *this;

	swap(this->partitionSize, src.partitionSize);
	swap(this->partitionStream, src.partitionStream);
	swap(this->partitionKeyStream, src.partitionKeyStream);

	return *this;
}

/**
 * pre-condition: partitionKey is a valid positive integer
 * post-condition: MsgStream at partitionKey is returned
 */
MsgStream& PartitionStream::operator[](int partitionKey)
{
	isValidKey(partitionKey);
	int index = arrayIndexHash(partitionKey);
	if(partitionKeyStream[index] == DEFAULT_KEY_VALUE) {
		partitionKeyStream[index] = partitionKey;
		numberOfStreams++;
	}
	return partitionStream[index];
}

/**
 * pre-condition: stream is a valid PartitionStream
 */
bool PartitionStream::operator==(const PartitionStream& stream)
{
	return checkEquality(stream);
}

/**
 * pre-condition: stream is a valid PartitionStream
 */
bool PartitionStream::operator!=(const PartitionStream& stream)
{
	return !checkEquality(stream);
}

/**
 * pre-condition: stream is a valid PartitionStream
 */
bool PartitionStream::operator>=(const PartitionStream& stream) const
{
	if(this->numberOfStreams >= stream.numberOfStreams)
		return true;
	return false;
}

/**
 * pre-condition: stream is a valid PartitionStream
 */
bool PartitionStream::operator>(const PartitionStream& stream) const
{
	if(this->numberOfStreams > stream.numberOfStreams)
		return true;
	return false;
}

/**
 * pre-condition: stream is a valid PartitionStream
 */
bool PartitionStream::operator<=(const PartitionStream& stream) const
{
	if(this->numberOfStreams <= stream.numberOfStreams)
		return true;
	return false;
}

/**
 * pre-condition: stream is a valid PartitionStream
 */
bool PartitionStream::operator<(const PartitionStream& stream) const
{
	if(this->numberOfStreams < stream.numberOfStreams)
		return true;
	return false;
}

/**
 * pre-condition: stream is a valid PartitionStream,
 * 				  the sum of both numberOfStreams between objects must not be > MAX_MSG_OBJECTS
 * post-condition: a temporary PartitionStream with all partitions between two objects is returned
 */
PartitionStream PartitionStream::operator+(PartitionStream& stream)
{
	if(stream.numberOfStreams + this->numberOfStreams > MAX_MSGSTREAM_OBJECTS)
		throw(out_of_range("Size of PartitionStreams exceeds maxSize"));

	PartitionStream temp(*this);
	unsigned int newSize = stream.numberOfStreams + temp.numberOfStreams;
	while(temp.partitionSize <= newSize)
		temp++;
	for(int i = 0; i < stream.partitionSize; i++) {
		if(stream.partitionKeyStream[i] == DEFAULT_KEY_VALUE)
			continue;
		MsgStream::ArrayReturn messages = stream[stream.partitionKeyStream[i]].readAllMessage();
		for(int j = 0; j < messages.streamSize; j++) {
			temp.appendMessage(stream.partitionKeyStream[i], messages.messages[j]);
		}
	}
	return temp;
}

/**
 * pre-condition: stream is a valid PartitionStream,
 * 				  the sum of both numberOfStreams between objects must not be > MAX_MSG_OBJECTS
 * post-condition: a temporary PartitionStream with all partitions between two objects is returned
 */
PartitionStream PartitionStream::operator+=(PartitionStream& stream)
{
	*this = *this + stream;
	return *this;
}

/**
 * pre-condition: PartitionStream's paritionSize must not be at MAX_MSGSTREAM_OBJECTS
 * post-condition: partitionSize is incrememented by one and pointers are resized
 */
PartitionStream PartitionStream::operator++()
{
	increaseSize();
	return *this;
}

/**
 * pre-condition: PartitionStream's paritionSize must not be at MAX_MSGSTREAM_OBJECTS
 * post-condition: partitionSize is incrememented by one and pointers are resized
 */
PartitionStream PartitionStream::operator++(int input)
{
	PartitionStream oldState = *this;
	increaseSize();
	return oldState;
}

/**
 * pre-condition: PartitionStream must not have the last array index occupied
 * post-condition: partitionSize is decremented by one and pointers are resized
 */
PartitionStream PartitionStream::operator--()
{
	decreaseSize();
	return *this;
}

/**
 * pre-condition: PartitionStream must not have the last array index occupied
 * post-condition: partitionSize is decremented by one and pointers are resized
 */
PartitionStream PartitionStream::operator--(int input)
{
	PartitionStream oldState = *this;
	decreaseSize();
	return oldState;
}

/**
 * post-condition: PartitionStream is completely deallocated
 */
PartitionStream::~PartitionStream()
{
	deepReset();
}

/**
 * pre-condition: partitionKey is a valid integer, message is a valid string
 * post-condition: string is added to the respective partition
 */
void PartitionStream::appendMessage(int partitionKey, string message)
{
	isValidKey(partitionKey);
	int arrayIndex = arrayIndexHash(partitionKey);
//	if( !this->partitionStream[arrayIndex] )
//	{
//		partitionStream[arrayIndex] = make_unique<MsgStream>();
//		partitionKeyStream[arrayIndex] = partitionKey;
//	}
	if(this->partitionKeyStream[arrayIndex] == DEFAULT_KEY_VALUE) {
		partitionKeyStream[arrayIndex] = partitionKey;
		numberOfStreams++;
	}

	partitionStream[arrayIndex].appendMessage(message);
}

/**
 * pre-condition: partitionKey is a valid integer, messages is a valid MsgStream ptr
 * post-condition: string is added to the respective partition
 */
void PartitionStream::appendMessage(int partitionKey, unique_ptr<MsgStream>& messages)
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
	if( isEmpty() )
		throw(invalid_argument("The Stream is already empty"));

	deepReset();
	partitionStream = make_unique<MsgStream[]>(partitionSize);
	partitionKeyStream = make_unique<int[]>(partitionSize);
	for(int i = 0; i < partitionSize; i++)
		partitionKeyStream[i] = DEFAULT_KEY_VALUE;
	numberOfStreams = INITIAL_VALUE;
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
	return partitionStream[arrayIndex].readAllMessage();
}

