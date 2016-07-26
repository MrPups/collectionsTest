// CPPConsoleApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <chrono>
#include <map>
#include <unordered_map>
#include <deque>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/composite_key.hpp>

using namespace boost::multi_index;
using namespace std;

struct phonebook_entry
{
	int a;
	int b;
	std::string item_name;

	phonebook_entry(
		int _a,
		int _b,
		std::string _item_name) :
		a(_a), b(_b), item_name(_item_name)
	{}
};

#pragma region Multiindex container

// define a multi_index_container with a composite key on
// (family_name,given_name)
typedef multi_index_container<
	phonebook_entry,
	indexed_by<
		ordered_non_unique<
			composite_key<
				phonebook_entry,
				member<phonebook_entry, int, &phonebook_entry::a>,
				member<phonebook_entry, int, &phonebook_entry::b>
			>
		>,
		ordered_unique< // unique as numbers belong to only one subscriber
			member<phonebook_entry, std::string, &phonebook_entry::item_name>
		>
	>
> phonebook;

void InitializeMultiIndexContainer(phonebook& book)
{
	std::map<__int64, int> distribution;
	__int64 amount = 0, min = INT_MAX, max = -1;

	int instrumentCount = 1000;
	int ordersCount = 10;

	for (int i = 0; i < instrumentCount; i++)
	{
		for (int j = 0; j < ordersCount; j++)
		{
			char buf[5] = {0};
			std::string itemName;
			itemName += "item ";
			itemName += itoa(i, buf, 10);
			itemName += ' ';
			itemName += itoa(j, buf, 10);

			std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
			book.insert(phonebook_entry(i, j, itemName));
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

			__int64 current = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

			distribution[current]++;

			if (current < min)
				min = current;

			if (current > max)
				max = current;

			amount += current;
		}
	}

	std::cout << "Min insert time is: " << min << std::endl;
	std::cout << "Max insert time is: " << max << std::endl;
	std::cout << "Average insert time is: " << amount / (ordersCount*instrumentCount) << std::endl;
}

void TestMultiIndexContainerRandomAccess(phonebook& book)
{
	srand(100);

	std::map<__int64, int> distribution;
	__int64 amount = 0, min = INT_MAX, max = -1;

	for (int i = 0; i < 1000; i++)
	{
		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		auto it = book.find(std::make_tuple(rand() % 10, rand() % 10));
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		__int64 current = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

		distribution[current]++;

		if (current < min)
			min = current;

		if (current > max)
			max = current;

		amount += current;
	}

	std::cout << "Min access time is: " << min << std::endl;
	std::cout << "Max access time is: " << max << std::endl;
	std::cout << "Average access time is: " << amount / 1000 << std::endl;
}

#pragma endregion

#pragma region unordered_map
typedef unordered_map<int, unordered_map<int, phonebook_entry>> UnorderedContainer;
typedef unordered_map<int, phonebook_entry> InternalUnorderedContainer;

void InitializeUnorderedContainer(UnorderedContainer& book)
{
	std::map<__int64, int> distribution;
	__int64 amount = 0, min = INT_MAX, max = -1;

	int instrumentCount = 1000;
	int ordersCount = 10;

	for (int i = 0; i < instrumentCount; i++)
	{
		for (int j = 0; j < ordersCount; j++)
		{
			char buf[5] = { 0 };
			std::string itemName;
			itemName += "item ";
			itemName += itoa(i, buf, 10);
			itemName += ' ';
			itemName += itoa(j, buf, 10);

			std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
			book.insert(std::make_pair(i, std::make_pair(j, phonebook_entry(i, j, itemName))));
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

			__int64 current = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

			distribution[current]++;

			if (current < min)
				min = current;

			if (current > max)
				max = current;

			amount += current;
		}
	}

	std::cout << "Min insert time is: " << min << std::endl;
	std::cout << "Max insert time is: " << max << std::endl;
	std::cout << "Average insert time is: " << amount / (ordersCount*instrumentCount) << std::endl;
}
#pragma endregion

int main()
{
	phonebook book;	

	// multiindex container
	InitializeMultiIndexContainer(book);
	TestMultiIndexContainerRandomAccess(book);

	return 0;
}
