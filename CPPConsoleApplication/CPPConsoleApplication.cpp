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

	phonebook_entry()
	{}

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

void InitializeContainer(phonebook& book, int instrumentCount = 1000, int ordersCount = 10)
{
	std::map<__int64, int> distribution;
	__int64 amount = 0, min = INT_MAX, max = -1;

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
			
			std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
			book.insert(phonebook_entry(i, j, itemName));
			std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

			__int64 current = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

			distribution[current]++;

			if (current < min)
				min = current;

			if (current > max)
				max = current;

			amount += current;
		}
	}
	std::cout << "--------------------Multi index-----------------------------------" << std::endl;
	std::cout << "Min insert time is: " << min << " nanoseconds" << std::endl;
	std::cout << "Max insert time is: " << max << " nanoseconds" << std::endl;
	std::cout << "Average insert time is: " << amount / (ordersCount*instrumentCount) << " nanoseconds" << std::endl << std::endl;
}

void TestRandomAccess(phonebook& book, int instrumentCount = 1000, int ordersCount = 10)
{
	srand(100);

	std::map<__int64, int> distribution;
	__int64 amount = 0, min = INT_MAX, max = -1;

	for (int i = 0; i < (instrumentCount*ordersCount); i++)
	{
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
		auto it = book.find(std::make_tuple(rand() % instrumentCount, rand() % ordersCount));
		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

		__int64 current = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

		distribution[current]++;

		if (current < min)
			min = current;

		if (current > max)
			max = current;

		amount += current;
	}

	std::cout << "--------------------Multi index-----------------------------------" << std::endl;
	std::cout << "Min access time is: " << min << " nanoseconds" << std::endl;
	std::cout << "Max access time is: " << max << " nanoseconds" << std::endl;
	std::cout << "Average access time is: " << amount / (instrumentCount*ordersCount) << " nanoseconds" << std::endl << std::endl;
}

#pragma endregion

#pragma region unordered_map
typedef unordered_map<int, unordered_map<int, phonebook_entry>> UnorderedContainer;
typedef unordered_map<int, phonebook_entry> InternalUnorderedContainer;

void InitializeContainer(UnorderedContainer& book, int instrumentCount = 1000, int ordersCount = 10)
{
	std::map<__int64, int> distribution;
	__int64 amount = 0, min = INT_MAX, max = -1;

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

			std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
			book[i].insert(InternalUnorderedContainer::value_type(j, phonebook_entry(i, j, itemName)));
			std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

			__int64 current = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

			distribution[current]++;

			if (current < min)
				min = current;

			if (current > max)
				max = current;

			amount += current;
		}
	}

	std::cout << "--------------------Unordered map----------------------------------" << std::endl;
	std::cout << "Min insert time is: " << min << " nanoseconds" << std::endl;
	std::cout << "Max insert time is: " << max << " nanoseconds" << std::endl;
	std::cout << "Average insert time is: " << amount / (ordersCount*instrumentCount) << " nanoseconds" << std::endl << std::endl;
}

void TestRandomAccess(UnorderedContainer& book, int instrumentCount = 1000, int ordersCount = 10)
{
	srand(100);

	std::map<__int64, int> distribution;
	__int64 amount = 0, min = INT_MAX, max = -1;

	for (int i = 0; i < (instrumentCount*ordersCount); i++)
	{
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
		auto it = book[rand() % instrumentCount][rand() % ordersCount];
		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

		__int64 current = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

		distribution[current]++;

		if (current < min)
			min = current;

		if (current > max)
			max = current;

		amount += current;
	}

	std::cout << "--------------------Unordered map----------------------------------" << std::endl;
	std::cout << "Min access time is: " << min << " nanoseconds" << std::endl;
	std::cout << "Max access time is: " << max << " nanoseconds" << std::endl;
	std::cout << "Average access time is: " << amount / (instrumentCount*ordersCount) << " nanoseconds" << std::endl << std::endl;
}

void TestRemoveItem(UnorderedContainer& book, int instrumentCount = 1000, int ordersCount = 10)
{
	srand(100);

	std::map<__int64, int> distribution;
	__int64 amount = 0, min = INT_MAX, max = -1;

	for (int i = 0; i < (1000); i++)
	{
		

		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		auto it = book[rand() % instrumentCount][rand() % ordersCount];
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		__int64 current = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

		distribution[current]++;

		if (current < min)
			min = current;

		if (current > max)
			max = current;

		amount += current;
	}

	std::cout << "--------------------Unordered map----------------------------------" << std::endl;
	std::cout << "Min access time is: " << min << std::endl;
	std::cout << "Max access time is: " << max << std::endl;
	std::cout << "Average access time is: " << amount / (1000) << std::endl << std::endl;
}
#pragma endregion

int main(int argc, char* argv[])
{
	if (argc != 3)
		return 1;

	phonebook book;
	UnorderedContainer unBook;

	int instruments = atoi(argv[1]);
	int orders = atoi(argv[2]);

	std::cout << "instruments count: " << instruments << std::endl;
	std::cout << "orders count: " << instruments << std::endl << std::endl;

	// multiindex container
	InitializeContainer(book, instruments, orders);
	TestRandomAccess(book, instruments, orders);

	// unordered maps
	InitializeContainer(unBook, instruments, orders);
	TestRandomAccess(unBook, instruments, orders);

	system("PAUSE");

	return 0;
}
