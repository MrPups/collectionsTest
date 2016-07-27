#include "IntrusiveContainersManager.h"
#include <iostream>

bool IntrusiveContainersManager::AddItem(phonebook_entry& entry)
{
	PhonebookWrapper* item = pool.malloc();
	item->SetPnonebookEntry(entry);

	list.push_back(*item);
	set->insert(*item);

	return true;
}

void IntrusiveContainersManager::Rehash()
{
	//if (set->bucket_count() <= set->size())
	//{
		size_t newSize = set->size() * 2;

		buckets.resize(newSize);
		set->rehash(PhonebookWrapper::set_type::bucket_traits(buckets.data(), newSize));
	//}
}

bool IntrusiveContainersManager::Find(phonebook_entry& entry)
{
	PhonebookWrapper item(entry.a, entry.b);

	return set->find(item) != set->end();
}

int IntrusiveContainersManager::GetSize()
{
	return set->size();
}
