// Jomikael Ruiz
// CPSC 3200
// MsgStream.cpp
// Date Created: Oct. 4th, 2024
// Last Modified: Oct. 15th, 2024

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
 * pre-condition: inputIndex is a valid positive integer
 */

void MsgStream::isValidIndex(unsigned int inputIndex)
{
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
	this->messageStream = new string[this->maxSize];
	this->currentArrayIndex = INITIAL_INDEX;
	this->totalOperations = INITIAL_INDEX;
}

void MsgStream::copyStream(const MsgStream& src)
{
	this->maxSize = src.maxSize;
	this->maxOperations = src.maxOperations;
	this->totalOperations = src.totalOperations;
	this->currentArrayIndex = src.currentArrayIndex;

	this->messageStream = new string[this->maxSize];

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

MsgStream::MsgStream(const MsgStream& src)
{
	copyStream(src);
}

MsgStream::MsgStream(MsgStream&& src)
{
	this->maxSize = src.maxSize;
	this->maxOperations = src.maxOperations;
	this->totalOperations = src.totalOperations;
	this->currentArrayIndex = src.currentArrayIndex;

	this->messageStream = src.messageStream;

	src.messageStream = nullptr;
	src.maxSize = 0;
	src.maxOperations = 0;
	src.totalOperations = 0;
	src.currentArrayIndex = 0;
}

MsgStream::~MsgStream()
{
	delete [] messageStream;
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
 * post-condition: messageStream is cleared of all data and reset
 */

bool MsgStream::resetStream()
{
	if ( isEmpty() )
		throw(out_of_range("The MsgStream is empty."));

	delete [] messageStream;
	messageStream = new string[maxSize];
	return true;
}

/**
 * pre-condition: messageIndex is a valid positive integer within range,
 * 				  otherwise an exception is thrown
 */

string MsgStream::readMessage(unsigned int messageIndex)
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

MsgStream::ArrayReturn MsgStream::readMessages(unsigned int startingIndex, unsigned int endingIndex)
{
	isValidIndex(startingIndex);
	isValidIndex(endingIndex);

	if ( exceedsMaxOperations() )
		throw(range_error("The MsgStream has reached its max number of operations."));

	unsigned int newArraySize = endingIndex - startingIndex + 1;
	unsigned int newArrayIndex = INITIAL_INDEX;

	string * retrievedMessages = new string[newArraySize];

	for ( unsigned int i = startingIndex; i <= endingIndex; i++)
	{
		retrievedMessages[newArrayIndex] = readMessage(i);
		newArrayIndex++;
	}
	ArrayReturn messages(retrievedMessages, newArraySize);
	totalOperations++;
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

	string * retrievedMessages = new string[currentArrayIndex];

	for (unsigned int i = 0; i < currentArrayIndex; i++)
		retrievedMessages[i] = readMessage(i);
	ArrayReturn messages(retrievedMessages, currentArrayIndex);
	totalOperations++;
	return messages;
}