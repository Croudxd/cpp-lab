#pragma once
#include <iostream>
#include <cassert>
#include <sstream>
#include <vector>
#include <stdexcept>
#include "linkedlist.h"

class Test
{

public:
	Test()
	{

	}
	void TestIntList()
	{

        ll::LinkedList<int> IntegerList;
		IntegerList.push_back(1);
		IntegerList.push_back(2);
		IntegerList.push_back(3);
		IntegerList.push_back(4);
		IntegerList.push_back(5);
		sizeTest(IntegerList, 5);
		std::vector<int> expOut = { 1, 2, 3, 4, 5 }; 
		listValidityTest(IntegerList, expOut);
	}
	void TestDoubleList()
	{
		ll::LinkedList<double> DoubleList;
		DoubleList.push_back(1.1);
		DoubleList.push_back(2.3);
		DoubleList.push_back(3.1);
		DoubleList.push_back(4.123);
		DoubleList.push_back(5.12123);
		sizeTest(DoubleList, 5);
		std::vector<double> expOut = { 1.1, 2.3, 3.1, 4.123, 5.12123 };
		listValidityTest(DoubleList, expOut);

	}
	void TestStringList()
	{
		ll::LinkedList<std::string> StringList;
		StringList.push_back("Hello");
		StringList.push_back("world");

		sizeTest(StringList, 2);
		std::vector<std::string> expOut = {"Hello", "world"};
		listValidityTest(StringList, expOut);
	}

	void TestCharList()
	{
        ll::LinkedList<char> CharList;
		CharList.push_back('a');
		CharList.push_back('b');
		CharList.push_back('c');
		CharList.push_back('d');

		sizeTest(CharList, 4);
		std::vector<char> expOut = {'a', 'b', 'c', 'd'};
		listValidityTest(CharList, expOut);
	}

    void TestMergeSort()
	{
        ll::LinkedList<int> IntList;
		IntList.push_back(4);
		IntList.push_back(1);
		IntList.push_back(4);
		IntList.push_back(9);
		IntList.push_back(6);
		IntList.push_back(2);

		sizeTest(IntList, 6);
		std::vector<int> expOut = {1, 2, 4, 4, 6, 9};
        Test::listValidityTest(IntList, expOut);
	}

	void run()
	{
		const std::string GREEN = "\033[32m";
		const std::string RESET = "\033[0m";

			TestIntList();
			TestDoubleList();
			TestCharList();
			TestStringList();

		std::cout << GREEN << "[PASSED TESTS]" << RESET << std::endl;
	}



private:
	template<typename T>
	void sizeTest(ll::LinkedList<T>& list, int expectedOutput)
	{
		int size = list.size();
		assert(size == expectedOutput);
	}

	template<typename T>
	void listValidityTest(ll::LinkedList<T>& list, std::vector<T> expectedOutput)
	{
        ll::internal::Node<T>* head = list.getHead();

		std::vector<T> listData;

		if (head == nullptr)
		{
			return;
		}
		else
		{
			ll::internal::Node<T>* current = head;
			while (current != nullptr)
			{
				listData.push_back(current->data);
				current = current->next;
			}
		}
		
		assert(listData == expectedOutput);
	}
};
