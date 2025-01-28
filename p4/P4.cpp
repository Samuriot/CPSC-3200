// Jomikael Ruiz
// CPSC 3200
// P4.cpp
// Date Created: Nov. 10th, 2024
// Last Modified: Nov. 15th, 2024

#include <iostream>
#include <memory>
#include <string>
#include "MsgStream.h"
#include "DurableStream.h"
#include "PartitionStream.h"
using namespace std;

const unsigned int FIRST_INDEX = 0;
const unsigned int SECOND_INDEX = 1;
const unsigned int THIRD_INDEX = 2;
const unsigned int FOURTH_INDEX = 3;
const unsigned int SIXTH_INDEX = 5;
const unsigned int SEVENTH_INDEX = 6;

const unsigned int TESTING_KEY = 5;
const unsigned int TESTING_KEY_TWO = 4;

const string TEST_STRING = "hello world!";
const string TEST_STRING_TWO = "more testing!";
const string TESTING_FILE = "../messages.txt";
const string FILE_STRING = "hello from the backing file!";

MsgStream operator+(string message, MsgStream& stream) {
	return stream + message;
}

void testIndexingOperator(PartitionStream& stream);
void testEqualityOperator(PartitionStream& stream);
void testGreaterThanOperator(PartitionStream& stream);
void testLessThanOperator(PartitionStream& stream);
void testIncrementOperator(PartitionStream& stream);
void testDecrementOperator(PartitionStream& stream);
void testPlusOperator(PartitionStream& stream);
void testPlusEqualsOperator(PartitionStream& stream);
void testPartitionStream(PartitionStream& stream);
void fillPartitionWithDurable(PartitionStream&);
void fillHeterogeneousCollection(PartitionStream&);
void fillBackingFile();

unique_ptr<MsgStream> getObjAddr(int);

int main()
{
	fillBackingFile();

	unique_ptr<PartitionStream[]> partitions = make_unique<PartitionStream[]>(FOURTH_INDEX);
	fillPartitionWithDurable(partitions[SECOND_INDEX]);
	fillHeterogeneousCollection(partitions[THIRD_INDEX]);

//	cout << "testing homogeneous MsgStream" << endl;
//	testPartitionStream(partitions[FIRST_INDEX]);

	cout << endl << "testing homogeneous DurableStream" << endl;
	testPartitionStream(partitions[SECOND_INDEX]);

//	cout << endl << "testing heterogeneous collection" << endl;
//	testPartitionStream(partitions[THIRD_INDEX]);
	return 0;
}

void testPartitionStream(PartitionStream& stream)
{
	testIndexingOperator(stream);
	testEqualityOperator(stream);
	testGreaterThanOperator(stream);
	testLessThanOperator(stream);
	testIncrementOperator(stream);
	testDecrementOperator(stream);
	testPlusOperator(stream);
	testPlusEqualsOperator(stream);
}

unique_ptr<MsgStream> getObjAddr(int i)
{
	if(i % 2 == 0) return make_unique<MsgStream>();
	return make_unique<DurableStream>();
}

void fillPartitionWithDurable(PartitionStream& stream)
{
	for(int i = 0; i < TESTING_KEY; i++) {
		unique_ptr<MsgStream> tempStream = make_unique<DurableStream>();
		stream.appendMessage(i, tempStream);
	}
}

void fillHeterogeneousCollection(PartitionStream& stream)
{
	for (int i = 0; i < TESTING_KEY; i++) {
		unique_ptr<MsgStream> tempStream = getObjAddr(i);
		stream.appendMessage(i, tempStream);
	}
}

void testIndexingOperator(PartitionStream& stream)
{
	cout << "Testing Indexing Operator" << endl;
	stream.appendMessage(FIRST_INDEX, TEST_STRING);
	stream.appendMessage(SECOND_INDEX, TEST_STRING_TWO);
	cout << stream[FIRST_INDEX][FIRST_INDEX] << " " << stream[FIRST_INDEX][SECOND_INDEX] << endl;
}

void testEqualityOperator(PartitionStream& stream)
{
	cout << endl << "Testing Equality Operator" << endl;
	unique_ptr<PartitionStream> stream2 = make_unique<PartitionStream>();
	(*stream2)[FIRST_INDEX].appendMessage(TEST_STRING);
	(*stream2)[FIRST_INDEX].appendMessage(TEST_STRING_TWO);
	cout << "result of MsgStream equality operator: " << (stream[FIRST_INDEX] == (*stream2)[FIRST_INDEX]) << endl;
	cout << "result of PartitionStream equality operator: " << (stream == *stream2) << endl;
	cout << "result of MsgStream inequality operator: " << (stream[FIRST_INDEX] != (*stream2)[FIRST_INDEX]) << endl;
	cout << "result of PartitionStream inequality operator: " << (stream != *stream2) << endl;
}

void testGreaterThanOperator(PartitionStream& stream)
{
	cout << endl << "Testing Greater Than Operator" << endl;
	unique_ptr<PartitionStream> stream2 = make_unique<PartitionStream>();
	(*stream2)[FIRST_INDEX].appendMessage(TEST_STRING);
	cout << "result of MsgStream > operator: " << (stream[FIRST_INDEX] > (*stream2)[FIRST_INDEX]) << endl;
	cout << "result of PartitionStream > operator: " << (stream > *stream2) << endl;
	cout << "result of MsgStream >= operator: " << (stream[FIRST_INDEX] >= (*stream2)[FIRST_INDEX]) << endl;
	cout << "result of PartitionStream >= operator: " << (stream >= *stream2) << endl;
}

void testLessThanOperator(PartitionStream& stream)
{
	cout << endl << "Testing Less Than Operator" << endl;
	unique_ptr<PartitionStream> stream2 = make_unique<PartitionStream>();
	stream[FIRST_INDEX].appendMessage(TEST_STRING);
	(*stream2)[FIRST_INDEX].appendMessage(TEST_STRING);
	(*stream2)[FIRST_INDEX].appendMessage(TEST_STRING);
	(*stream2)[SECOND_INDEX].appendMessage(TEST_STRING);
	cout << "result of MsgStream < operator: " << (stream[FIRST_INDEX] < (*stream2)[FIRST_INDEX]) << endl;
	cout << "result of PartitionStream < operator: " << (stream < *stream2) << endl;
	cout << "result of MsgStream <= operator: " << (stream[FIRST_INDEX] <= (*stream2)[FIRST_INDEX]) << endl;
	cout << "result of PartitionStream <= operator: " << (stream <= *stream2) << endl;
}

void testIncrementOperator(PartitionStream& stream)
{
	cout << endl << "Testing Increment Operator" << endl;
	for(int i = 0; i < SIXTH_INDEX; i++) {
		stream[i].appendMessage(TEST_STRING);
	}
	stream++;
	++stream;
	stream[SIXTH_INDEX].appendMessage(TEST_STRING);
	stream[SEVENTH_INDEX].appendMessage(TEST_STRING);
	for(int i = 0; i < SEVENTH_INDEX; i++) {
		MsgStream::ArrayReturn messages = stream.readMessages(i);
		cout << i <<" iteration: " << messages.messages[0] << endl;
	}
}

void testDecrementOperator(PartitionStream& stream)
{
	stream.resetStream();
	cout << endl << "Testing Decrement Operator" << endl;
	for(int i = 0; i < FOURTH_INDEX; i++) {
		cout << "at index " << i << endl;
		stream[i].appendMessage(TEST_STRING);
	}
	stream--;
	--stream;
	for(int i = 0; i < FOURTH_INDEX; i++) {
		MsgStream::ArrayReturn messages = stream.readMessages(i);
		cout << i <<" iteration: " << messages.messages[0] << endl;
	}
}
void testPlusOperator(PartitionStream& stream)
{
	cout << endl << "Testing Plus Operator" << endl;
	unique_ptr<PartitionStream> stream2 = make_unique<PartitionStream>();
	stream.appendMessage(TESTING_KEY, TEST_STRING);
	stream2->appendMessage(TESTING_KEY_TWO, TEST_STRING_TWO);
	PartitionStream addedStreams = stream + *stream2;
	MsgStream::ArrayReturn keyOneMessages = addedStreams[TESTING_KEY].readAllMessage();
	MsgStream::ArrayReturn keyTwoMessages = addedStreams[TESTING_KEY_TWO].readAllMessage();
	cout << "result after + operator: " << keyOneMessages.messages[0] << endl;
	cout << "result after + operator: " << keyTwoMessages.messages[0] << endl;

	cout << endl << "testing MsgStream + string addition" << endl;
	MsgStream tempStream = stream[TESTING_KEY] + TEST_STRING;
	MsgStream::ArrayReturn messages = tempStream.readAllMessage();
	for(int i = 0; i < messages.streamSize; i++)
		cout << i << ": " << messages.messages[i] << endl;

	cout << endl << "testing string + MsgStream addition" << endl;
	MsgStream stringAddition = TEST_STRING + stream[TESTING_KEY];
	MsgStream::ArrayReturn additionResults = tempStream.readAllMessage();
	for(int i = 0; i < additionResults.streamSize; i++)
		cout << i << ": " << additionResults.messages[i] << endl;

	cout << endl << "testing MsgStream + MsgStream addition" << endl;
	MsgStream tempSameTypeStream = stream[TESTING_KEY] + stream[TESTING_KEY];
	MsgStream::ArrayReturn sameTypeMessages = tempStream.readAllMessage();
	cout << "result after + string operator: " << endl;
	for(int i = 0; i < sameTypeMessages.streamSize; i++)
		cout << sameTypeMessages.messages[i] << endl;
}
void testPlusEqualsOperator(PartitionStream& stream)
{
	cout << endl << "Testing PlusEquals Operator" << endl;
	unique_ptr<PartitionStream> stream2 = make_unique<PartitionStream>();
	stream.appendMessage(TESTING_KEY, TEST_STRING);
	stream2->appendMessage(TESTING_KEY_TWO, TEST_STRING_TWO);
	stream += *stream2;
	MsgStream::ArrayReturn keyOneMessages = stream[TESTING_KEY].readAllMessage();
	MsgStream::ArrayReturn keyTwoMessages = stream[TESTING_KEY_TWO].readAllMessage();
	for(int i = 0; i < keyOneMessages.streamSize; i++)
		cout << keyOneMessages.messages[i] << endl;
	for(int i = 0; i < keyTwoMessages.streamSize; i++)
		cout << keyTwoMessages.messages[i] << endl;

	cout << endl << "result of MsgStream + string:" << endl;
	stream[TESTING_KEY]+= TEST_STRING;
	MsgStream::ArrayReturn messages = stream[TESTING_KEY].readAllMessage();
	for(int i = 0; i < messages.streamSize; i++)
		cout << messages.messages[i] << endl;

	cout << endl << "result of MsgStream + MsgStream:" << endl;
	stream[TESTING_KEY] += stream[TESTING_KEY];
	MsgStream::ArrayReturn sameTypeMessages = stream[TESTING_KEY].readAllMessage();
	for(int i = 0; i < sameTypeMessages.streamSize; i++)
		cout << sameTypeMessages.messages[i] << endl;
}

void fillBackingFile()
{
	fstream inFile(TESTING_FILE, fstream::out);
	inFile << TEST_STRING << endl;
	inFile << FILE_STRING << endl;
	inFile.close();
}