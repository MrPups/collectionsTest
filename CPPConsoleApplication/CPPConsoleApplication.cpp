// CPPConsoleApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "MessageStructures.h"
#include "IntrusiveContainersManager.h"

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
			std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
			book.insert(phonebook_entry(i, j));
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
			std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
			book[i].insert(InternalUnorderedContainer::value_type(j, phonebook_entry(i, j)));
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

#pragma region intrusive collections

void InitializeContainer(IntrusiveContainersManager& manager, int instrumentCount = 1000, int ordersCount = 10)
{
	std::map<__int64, int> distribution;
	__int64 amount = 0, min = INT_MAX, max = -1;

	for (int i = 0; i < instrumentCount; i++)
	{
		for (int j = 0; j < ordersCount; j++)
		{
			std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
			manager.AddItem(phonebook_entry(i, j));
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

	std::cout << "--------------------Intrusive collections insert items--------------------------" << std::endl;
	std::cout << "Min insert time is: " << min << " nanoseconds" << std::endl;
	std::cout << "Max insert time is: " << max << " nanoseconds" << std::endl;
	std::cout << "Average insert time is: " << amount / (ordersCount*instrumentCount) << " nanoseconds" << std::endl << std::endl;
}

void TestRandomAccess(IntrusiveContainersManager& manager, int instrumentCount = 1000, int ordersCount = 10)
{
	srand(100);

	std::map<__int64, int> distribution;
	__int64 amount = 0, min = INT_MAX, max = -1;

	for (int i = 0; i < (instrumentCount*ordersCount); i++)
	{
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
		bool res = manager.Find(phonebook_entry(rand() % instrumentCount, rand() % ordersCount));
		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

		__int64 current = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

		if (!res)
			std::cout << "Item's not found and this is an error!!" << std::endl;

		distribution[current]++;

		if (current < min)
			min = current;

		if (current > max)
			max = current;

		amount += current;
	}

	std::cout << "--------------------Intrusive collections random access-------------------------" << std::endl;
	std::cout << "Min access time is: " << min << " nanoseconds" << std::endl;
	std::cout << "Max access time is: " << max << " nanoseconds" << std::endl;
	std::cout << "Average access time is: " << amount / (instrumentCount*ordersCount) << " nanoseconds" << std::endl << std::endl;
}

#pragma endregion

int main(int argc, char* argv[])
{
	/*if (argc != 3)
		return 1;*/

	// intrusive collections must test separately, because they can't allocate memory
	// if other variants are testing.

	phonebook book;
	UnorderedContainer unBook;
	IntrusiveContainersManager manager(20000000);

	/*int instruments = atoi(argv[1]);
	int orders = atoi(argv[2]);*/

	int instruments = 1000000;
	int orders = 20;


	std::cout << "instruments count: " << instruments << std::endl;
	std::cout << "orders count: " << orders << std::endl << std::endl;

	// multiindex container
	//InitializeContainer(book, instruments, orders);
	// unordered maps
	//InitializeContainer(unBook, instruments, orders);
	// intrusive collections
	InitializeContainer(manager, instruments, orders);


	// multiindex container
	//TestRandomAccess(book, instruments, orders);
	// unordered maps
	//TestRandomAccess(unBook, instruments, orders);
	// intrusive collections
	TestRandomAccess(manager, instruments, orders);

	system("PAUSE");

	return 0;
}
