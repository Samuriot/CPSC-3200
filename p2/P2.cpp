// Jomikael Ruiz
// CPSC 3200
// P2.cpp
// Date Created: Oct. 10th, 2024
// Last Modified: Oct. 15th, 2024

#include <iostream>
#include <string>
#include "MsgStream.h"
#include "PartitionStream.h"
using namespace std;

const unsigned int DEFAULT_TEST_SIZE = 2;
const unsigned int TESTING_KEY = 5;
const unsigned int UNALLOCATED_TESTING_KEY = 4;
const unsigned int OCCUPIED_TESTING_KEY = 10;
const string TEST_STRING = "hello world!";
const string TEST_STRING_2 = "more testing!";

void testAssignmentOperator(PartitionStream * array);
void testCopyConstructor(PartitionStream * array);
void catchingReadException(PartitionStream * array);
void catchingAppendException(PartitionStream * array);

int main()
{
	PartitionStream partitionStream[DEFAULT_TEST_SIZE];
	testAssignmentOperator(partitionStream);
	testCopyConstructor(partitionStream);
	catchingReadException(partitionStream);
	catchingAppendException(partitionStream);
	return 0;
}

void testAssignmentOperator(PartitionStream * array)
{
	cout << "testing assignment operator" << endl;

	PartitionStream * testObject = new PartitionStream;

	testObject->appendMessage(TESTING_KEY, TEST_STRING);
	testObject->appendMessage(TESTING_KEY, TEST_STRING_2);

	array[0] = *testObject;
	delete testObject;
	testObject = nullptr;

	MsgStream::ArrayReturn messages = array[0].readMessages(TESTING_KEY);
	for(int i = 0; i < messages.streamSize; i++) {
		cout << messages.messages[i] << endl;
	}
	delete [] messages.messages;
}

void testCopyConstructor(PartitionStream * array)
{
	cout << endl << "testing copy constructor" << endl;

	PartitionStream testObject(array[0]);
	testObject.appendMessage(TESTING_KEY, TEST_STRING_2);
	testObject.appendMessage(TESTING_KEY, TEST_STRING);

	MsgStream::ArrayReturn messages = testObject.readMessages(TESTING_KEY);
	for(int i = 0; i < messages.streamSize; i++)
		cout << messages.messages[i] << endl;
	array[1] = testObject;
	delete [] messages.messages;
}

void catchingReadException(PartitionStream * array)
{
	try
	{
		array->readMessages(UNALLOCATED_TESTING_KEY);
	}
	catch(invalid_argument &e)
	{
		cout << endl << "caught reading exception" << endl;
	}
}

void catchingAppendException(PartitionStream * array)
{
	try
	{
		array->appendMessage(OCCUPIED_TESTING_KEY, TEST_STRING);
	}
	catch(invalid_argument &e)
	{
		cout << endl << "caught append exception" << endl;
	}
}
