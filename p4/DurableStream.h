// Jomikael Ruiz
// CPSC 3200
// DurableStream.h
// Date Created: Nov. 4th, 2024
// Last Modified: Nov. 15th, 2024

#ifndef DURABLESTREAM_H
#define DURABLESTREAM_H

#include "MsgStream.h"
#include "string"
#include "fstream"


/**
 * Class Invariants:
 * - DurableStream is a descendent child of MsgStream
 * - Functionality of DurableStream is the same as MsgStream with the added
 *   feature of a backing file that is located in the base folder
 * - 3 Strings can be added to DurableStream before it writes back to its
 *   backing file
 * - DurableStream automatically handles file closing via use of fstream
 * - _originalMessages contains the messages from the original file, which
 *   resetStream() uses to restore original status of internal array
 * - This class does not support copying to protect the data of the backing files
 * Immutables:
 * - Once set, _fileName cannot be changed
 * - Size of DurableStream is managed by MsgStream and thus, can be changed
 *   with a maximum size of 300 strings and default size of 100 strings
 * - DurableStream inherits overloaded operators from MsgStream of ==, !=, +, +=, >/>=, </<=, ++, []
 * - Once added to the DurableStream, strings cannot be removed
 * - Default Backing File Name will be "messages.txt"
 * - Because of this, all DurableStreams that call the default constructor
 *   will immediately refer to and write to "messages.txt"
 */
class DurableStream : public MsgStream
{
	private:
		unique_ptr<string[]> originalMessages;
		string fileName;
		int originalMessagesSize;
		int originalMessageIndex;
		int appendCounter;

		const string DEFAULT_FILE = "messages.txt";
		const string DEFAULT_FILE_COPY = "copyOf";
		const string FILE_PREFIX = "../";
		const int FILE_IO_CONSTANT = 3;
		const int NO_READ_MESSAGES = 0;

		void initializeStream();
		void loadMessagesFromFile(ifstream&);
		void writeMessagesToFile();
		void writeMessagesToFile(unique_ptr<string[]>&, int);

		DurableStream(const DurableStream&);
		DurableStream(DurableStream&&) noexcept ;

		DurableStream& operator=(const DurableStream&);
		DurableStream& operator=(DurableStream&&);
	public:
		DurableStream();
		DurableStream(int);
		DurableStream(string);
		~DurableStream();
		bool appendMessage(string) override;
		bool resetStream() override;

};
/*
 * Implementation Invariants:
 * - Default size of DurableStream is 100 strings
 * - Due to the nature of CLion file IO, a prefix of '../' is added
 *   to each call using File class to allow for easier navigation to base directory of project
 * - resetStream() will store the contents of the original backing file into the array and restore
 *   functionality back to its original state
 * - If no backing file is present, DurableStream is completely wiped and reset to its original
 *   state
 * - A max amount of operations is tracked with functionality from MsgStream, throwing an exception
 *   once this limit is reached
 * - File Writing occurs after 3 string appends to the internal array
 * - When the DurableStream is empty, the client cannot read, reset, or dispose
 *   of the stream, otherwise an exception is thrown
 * - Copying is suppressed in DurableStream as several issues can occur when copying files over to another
 * - Copying is also suppressed to protect the data of backing files and prevent clients from creating
 * - too many copies of each backing file
 */


#endif //DURABLESTREAM_H
