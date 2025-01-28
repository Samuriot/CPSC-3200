// Jomikael Ruiz
// CPSC 3200
// DurableStream.cpp
// Date Created: Nov. 4th, 2024
// Last Modified: Nov. 15th, 2024

#include "DurableStream.h"
#include <fstream>

/**
  * PostCondition: the internal variables are set up for DurableStream
  */
void DurableStream::initializeStream()
{
	appendCounter = 0;
	messageStream.reset(new string[this->maxSize]);
	ifstream readStream(FILE_PREFIX + fileName);
	if(readStream.fail())
	{
		ofstream creatingFile(FILE_PREFIX + fileName);
		originalMessages.reset(new string[NO_READ_MESSAGES]);
		creatingFile.close();
	}
	else
		loadMessagesFromFile(readStream);
	readStream.close();
}

/**
 * PreCondition: _fileName must be a valid file, which is checked
 *               in methods that call LoadMessagesFromFile()
 * PostCondition: contents of _fileName are stored in _originalMessages
 */
void DurableStream::loadMessagesFromFile(ifstream& readStream)
{
	string temporaryMessage;
	int arraySize = 0;
	while(getline(readStream, temporaryMessage))
		arraySize++;

	if(arraySize > MAXIMUM_STREAM_SIZE)
		throw(range_error("The file is too large"));

	readStream.clear();
	readStream.seekg(0);

	originalMessageIndex = 0;
	originalMessages.reset(new string[arraySize]);
	while(getline(readStream, temporaryMessage)) {
		originalMessages[originalMessageIndex] = temporaryMessage;
		originalMessageIndex++;
		MsgStream::appendMessage(temporaryMessage);
	}
	originalMessagesSize = originalMessageIndex;
}

/**
 * PreCondition: _appendCounter is >= WriteToFileConstant
 * PostCondition: most recent 3 strings are written to backing file of _fileName
 */
void DurableStream::writeMessagesToFile()
{
	ofstream writeStream(FILE_PREFIX + fileName, ios_base::app);
	for(int i = currentArrayIndex - appendCounter; i < currentArrayIndex; i++)
		writeStream << messageStream[i] << endl;
	writeStream.close();
}

/**
 * PreCondition: messages must be a valid string array and _fileName must be a
 *               valid file location
 * PostCondition: contents from array are written to the newly created backing file
 */
void DurableStream::writeMessagesToFile(unique_ptr<string[]>& messages, int size)
{
	ofstream writeStream(FILE_PREFIX + fileName);
	for(int i = 0; i < size; i++)
		writeStream << messages[i] << endl;
	writeStream.close();
}

DurableStream::DurableStream() : MsgStream()
{
	fileName = DEFAULT_FILE;
	initializeStream();
}

/**
 * PreCondition: size is a valid unsigned integer
 */
DurableStream::DurableStream(int size) : MsgStream(size)
{
	fileName = DEFAULT_FILE;
	initializeStream();
}

/**
 * PreCondition: fileName must be in a valid format of .txt or other text format
 */
DurableStream::DurableStream(string fileName) : MsgStream()
{
	this->fileName = fileName;
	initializeStream();
}

DurableStream::~DurableStream()
{
	originalMessages.reset();
	fileName = "";
	originalMessageIndex = NO_READ_MESSAGES;
	originalMessagesSize = NO_READ_MESSAGES;
	appendCounter = NO_READ_MESSAGES;
}

/**
 * PreCondition: message must be a valid string, DurableStream must not be disposed,
 *               otherwise an exception is thrown
 * PostCondition: message will be added to the internal array and be written to
 *                the backing file linked to DurableStream
 */
bool DurableStream::appendMessage(string message)
{
	MsgStream::appendMessage(message);
	appendCounter++;
	if (appendCounter >= FILE_IO_CONSTANT)
	{
		writeMessagesToFile();
		appendCounter = 0;
	}
	return true;
}

/**
 * PreCondition: DurableStream must not be empty, otherwise an exception is thrown
 * PostCondition: DurableStream is reinitialized to its base state at creation, restoring
 *                data from _originalMessages to _messageStream
 */
bool DurableStream::resetStream()
{
	if (isEmpty())
	{
		throw(range_error("The stream is empty."));
	}
	messageStream.reset(new string[maxSize]);

	for (int i = 0; i < originalMessagesSize; i++)
		messageStream[i] = originalMessages[i];

	appendCounter = 0;
	currentArrayIndex = originalMessageIndex;
	writeMessagesToFile(originalMessages, originalMessagesSize);
	return true;
}
