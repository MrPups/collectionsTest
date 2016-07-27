#pragma once

#include "MessageStructures.h"

#include <boost\pool\object_pool.hpp>
#include <vector>

class IntrusiveContainersManager
{
public:
	IntrusiveContainersManager(int size): initialSize(size)
	{
		buckets.resize(size);
		set = new PhonebookWrapper::set_type(PhonebookWrapper::set_type::bucket_traits(buckets.data(), buckets.size()));
	}

	~IntrusiveContainersManager()
	{
		if (set)
			delete set;
	}

private:
	int initialSize;
	boost::object_pool<PhonebookWrapper> pool{ initialSize, 0 };

	std::vector<PhonebookWrapper::set_type::bucket_type> buckets;

	PhonebookWrapper::list_type list;
	PhonebookWrapper::set_type* set;

public:
	bool addItem(phonebook_entry& entry);
	int getSize();
};