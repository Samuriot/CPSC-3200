// Jomikael Ruiz
// CPSC 3200
// MsgStream.cpp
// Date Created: Oct. 4th, 2024
// Last Modified: Nov. 15th, 2024

#include "MsgStream.h"
using namespace std;

/**
 * pre-condition: messageStream has contents within it,
 * 				 otherwise exception is thrown
 * post-condition: messageStream is reset and all data is cleared
 */

bool MsgStream::isEmpty()
{
	return currentArrayIndex == 0;
}

bool MsgStream::isFull()
{
	return currentArrayIndex == maxSize;
}

bool MsgStream::exceedsMaxOperations()
{
	return totalOperations >= maxOperations;
}

/**
 * pre-condition: stream is a valid MsgStream object
 */
bool MsgStream::checkEquality(const MsgStream& stream)
{
	if(stream.currentArrayIndex != this->currentArrayIndex)
		return false;
	for(int i = 0; i < currentArrayIndex; i++) {
		if(stream.messageStream[i] != this->messageStream[i])
			return false;
	}
	return true;
}

/**
 * pre-condition: inputIndex is a valid positive integer
 */

void MsgStream::isValidIndex(int inputIndex)
{
	if(inputIndex < 0)
		throw(invalid_argument("Negative Index Provided"));
	if(inputIndex >= currentArrayIndex || inputIndex >= maxSize)
		throw(out_of_range("Index is out of range"));
}

/**
 * pre-condition: inputSize must be a valid positive integer from
 * 				  range [0-299]
 */
void MsgStream::initializeStream(unsigned int inputSize)
{
	this->maxSize = inputSize;
	this->maxOperations = this->maxSize + OPERATIONAL_CONSTANT;
	this->messageStream.reset(new string[this->maxSize]);
	this->currentArrayIndex = INITIAL_INDEX;
	this->totalOperations = INITIAL_INDEX;
}

/**
 * pre-condition: src is a valid MsgStream object
 * post-condition: the contents from src are Deep Copied to this MsgStream object
 */
void MsgStream::copyStream(const MsgStream& src)
{
	this->maxSize = src.maxSize;
	this->maxOperations = src.maxOperations;
	this->totalOperations = src.totalOperations;
	this->currentArrayIndex = src.currentArrayIndex;

	this->messageStream.reset(new string[this->maxSize]);

	for(int i = 0; i < this->currentArrayIndex; i++)
		this->messageStream[i] = src.messageStream[i];
}

MsgStream::MsgStream()
{
	initializeStream(DEFAULT_SIZE);
}

/**
 * pre-condition: inputSize must be a valid positive integer from
 * 				  range [0-299], otherwise exception is thrown
 */
MsgStream::MsgStream(unsigned int inputSize)
{
	if (inputSize >= MAXIMUM_STREAM_SIZE)
		throw(out_of_range("Size is out of range"));
	initializeStream(inputSize);
}

/**
 * pre-condition: src is a valid MsgStream object
 * post-condition: the contents from src are Deep Copied to this MsgStream object
 */
MsgStream::MsgStream(const MsgStream& src)
{
	copyStream(src);
}

/**
 * pre-condition: src is a valid MsgStream object
 * post-condition: the contents from src are moved to this MsgStream object
 */
MsgStream::MsgStream(MsgStream&& src)
{
	this->maxSize = src.maxSize;
	this->maxOperations = src.maxOperations;
	this->totalOperations = src.totalOperations;
	this->currentArrayIndex = src.currentArrayIndex;
	this->messageStream.reset(src.messageStream.release());

	src.maxSize = 0;
	src.maxOperations = 0;
	src.totalOperations = 0;
	src.currentArrayIndex = 0;
}

/**
 * pre-condition: src is a valid MsgStream object
 * post-condition: the contents from src are Deep Copied to this MsgStream object
 */
MsgStream& MsgStream::operator=(const MsgStream& src)
{
	if( this == &src )
		return *this;

	copyStream(src);

	return *this;
}

/**
 * pre-condition: src is a valid MsgStream object
 * post-condition: the contents from src are moved to this MsgStream object
 */
MsgStream& MsgStream::operator=(MsgStream&& src)
{
	if( this == &src )
		return *this;

	swap(this->maxSize, src.maxSize);
	swap(this->maxOperations, src.maxOperations);
	swap(this->totalOperations, src.totalOperations);
	swap(this->currentArrayIndex, src.currentArrayIndex);
	this->messageStream.swap(src.messageStream);

	return *this;
}

/**
 * pre-condition: index is a valid positive integer, otherwise exception is thrown
 * post-condition: string at index is returned
 */
string MsgStream::operator[](int index) {
	return readMessage(index);
}

/**
 * pre-condition: stream is a valid MsgStream object
 */
bool MsgStream::operator==(MsgStream& stream)
{
	totalOperations++;
	return checkEquality(stream);
}

/**
 * pre-condition: stream is a valid MsgStream object
 */
bool MsgStream::operator!=(MsgStream& stream)
{
	totalOperations++;
	return !checkEquality(stream);
}

/**
 * pre-condition: stream is a valid MsgStream object
 * post-condition: a temporary MsgStream object is returned with results of combing both messageStreams
 */
MsgStream MsgStream::operator+(const MsgStream& stream)
{
	MsgStream tempStream(*this);
	if(stream.currentArrayIndex + tempStream.currentArrayIndex >= tempStream.maxSize)
		throw(out_of_range("input MsgStream is too large"));
	if(exceedsMaxOperations())
		throw(range_error("The MsgStream has reached its max number of operations."));
	for(int i = 0; i < stream.currentArrayIndex; i++) {
		tempStream.appendMessage(stream.messageStream[i]);
	}
	return tempStream;
}

/**
 * pre-condition: stream is a valid MsgStream object
 * post-condition: *this MsgStream object is returned with results of combing both messageStreams
 */
MsgStream MsgStream::operator+=(const MsgStream& stream)
{
	*this = *this + stream;
	return *this;
}

/**
 * pre-condition: message is a valid string
 * post-condition: a temporary MsgStream object is returned, appending message into its contents
 */
MsgStream MsgStream::operator+(string message)
{
	MsgStream tempStream(*this);
	tempStream.appendMessage(message);
	return tempStream;
}

/**
 * pre-condition: message is a valid string
 * post-condition: *this MsgStream object is returned, appending message into its contents
 */
MsgStream MsgStream::operator+=(const string& message)
{
	this->appendMessage(message);
	return *this;
}

/**
 * pre-condition: stream is a valid MsgStream object
 */
bool MsgStream::operator>(const MsgStream& stream)
{
	totalOperations++;
	if(this->currentArrayIndex > stream.currentArrayIndex)
		return true;
	return false;
}

/**
 * pre-condition: stream is a valid MsgStream object
 */
bool MsgStream::operator>=(const MsgStream& stream)
{
	totalOperations++;
	if(this->currentArrayIndex >= stream.currentArrayIndex)
		return true;
	return false;
}

/**
 * pre-condition: stream is a valid MsgStream object
 */
bool MsgStream::operator<(const MsgStream& stream)
{
	totalOperations++;
	if(this->currentArrayIndex < stream.currentArrayIndex)
		return true;
	return false;
}

/**
 * pre-condition: stream is a valid MsgStream object
 */
bool MsgStream::operator<=(const MsgStream& stream)
{
	totalOperations++;
	if(this->currentArrayIndex <= stream.currentArrayIndex)
		return true;
	return false;
}

/**
 * pre-condition: MsgStream must not be at the maximum stream size, otherwise an exception is thrown
 * post-condition: maxSize is increased by 1, counting as a successful operation
 */
MsgStream MsgStream::operator++()
{
	if(this->maxSize >= MAXIMUM_STREAM_SIZE)
		throw(out_of_range("MsgStream already at max capacity"));
	this->maxSize++;
	this->totalOperations++;
	return *this;
}

/**
 * pre-condition: MsgStream must not be at the maximum stream size, otherwise an exception is thrown
 * post-condition: returns the previous state of Msgstream before
 * 				   maxSize is increased by 1, counting as a successful operation
 */
MsgStream MsgStream::operator++(int)
{
	if(this->maxSize >= MAXIMUM_STREAM_SIZE)
		throw(out_of_range("MsgStream already at max capacity"));
	MsgStream oldState = *this;
	this->maxSize++;
	this->totalOperations++;
	return oldState;
}

MsgStream::~MsgStream()
{
	messageStream.reset();
	maxSize = 0;
	totalOperations = 0;
	maxOperations = 0;
	currentArrayIndex = 0;
}

/**
 * pre-condition: message is a valid string, otherwise exception is thrown
 * post-condition: message is successfully added to messageStream array
 */

bool MsgStream::appendMessage(string message)
{
	if ( isFull() )
		throw(range_error("The MsgStream is full."));

	if ( exceedsMaxOperations() )
		throw(range_error("The MsgStream has reached its max number of operations."));

	messageStream[currentArrayIndex] = message;
	currentArrayIndex++;
	totalOperations++;
	return true;
}

/**
 * pre-condition: messageStream has contents within it,
 * 				  otherwise an exception is thrown
 * post-condition: messageStream is cleared of all data and reset, including all member data
 */

bool MsgStream::resetStream()
{
	if ( isEmpty() )
		throw(out_of_range("The MsgStream is empty."));
	messageStream.reset(new string[this->maxSize]);
	currentArrayIndex = 0;
	totalOperations = 0;
	return true;
}

/**
 * pre-condition: messageIndex is a valid positive integer within range,
 * 				  otherwise an exception is thrown
 */

string MsgStream::readMessage(int messageIndex)
{
	isValidIndex(messageIndex);

	if( exceedsMaxOperations() )
		throw(range_error("The MsgStream has reached its max number of operations."));

	totalOperations++;
	return messageStream[messageIndex];
}

/**
 * pre-condition: startingIndex & endingIndex are postitive integers
 * 				  within range, otherwise an exception is thrown
 * post-condition: a duplicate array with values ranging to and
 * 				   from the specified ranges is returned in the form of a structure,
 * 				   client is responsible for deallocation of string returned
 */

MsgStream::ArrayReturn MsgStream::readMessages(int startingIndex, int endingIndex)
{
	isValidIndex(startingIndex);
	isValidIndex(endingIndex);

	if ( exceedsMaxOperations() )
		throw(range_error("The MsgStream has reached its max number of operations."));

	unsigned int newArraySize = endingIndex - startingIndex + 1;
	unsigned int newArrayIndex = INITIAL_INDEX;

	unique_ptr<string[]> retrievedMessages(new string[newArraySize]);

	for ( unsigned int i = startingIndex; i <= endingIndex; i++)
	{
		retrievedMessages[newArrayIndex] = readMessage(i);
		newArrayIndex++;
	}
	totalOperations++;
	ArrayReturn messages(retrievedMessages, newArraySize);
	return messages;
}

/**
 * pre-condition: the MsgStream must not be empty,
 * 				  otherwise an exception is thrown
 * post-condition: a duplicate array with values ranging to and from
 * 				   currentArrayIndex is returned in the form of a structure,
 * 				   client is responsible for deallocation of string returned
 */

MsgStream::ArrayReturn MsgStream::readAllMessage()
{
	if ( isEmpty() )
		throw(out_of_range("The MsgStream is empty."));

	unique_ptr<string[]> retrievedMessages(new string[currentArrayIndex]);

	for (int i = 0; i < currentArrayIndex; i++) {
		retrievedMessages[i] = readMessage(i);
	}
	totalOperations++;
	ArrayReturn messages(retrievedMessages, currentArrayIndex);
	return messages;
}