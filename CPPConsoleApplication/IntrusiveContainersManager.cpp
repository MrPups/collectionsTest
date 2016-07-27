#include "IntrusiveContainersManager.h"
#include <iostream>

bool IntrusiveContainersManager::addItem(phonebook_entry& entry)
{
	try {
		PhonebookWrapper* item = pool.malloc();
		item->SetPnonebookEntry(entry);

		list.push_back(*item);
		set->insert(*item);

		/*if (set->bucket_count() == set->size())
		{
			size_t newSize = set->bucket_count() * 2;
			//need to realocate bucket
			buckets.resize(newSize);
			buckets.clear();
			set->rehash(PhonebookWrapper::set_type::bucket_traits(buckets.data(), newSize));

			std::cout << "rehashed" << std::endl;
		}*/
	}
	catch(std::exception)
	{
		return false;
	}

	return true;
}

int IntrusiveContainersManager::getSize()
{
	return set->size();
}
