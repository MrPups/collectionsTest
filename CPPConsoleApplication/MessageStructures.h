#pragma once
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/unordered_set.hpp>

using namespace boost::intrusive;

struct phonebook_entry
{
	int a;
	int b;

	phonebook_entry()
	{}

	phonebook_entry(
		int _a,
		int _b) :
		a(_a), b(_b)
	{}

	friend bool operator== (const phonebook_entry &left, const phonebook_entry &right)
	{
		return left.a == right.a && left.b == right.b;
	}
};

class PhonebookWrapper
{
public:
	PhonebookWrapper(){}

	PhonebookWrapper(int _a, int _b) : pEntry(_a, _b) {}

private:
	list_member_hook<> list_hook;
	unordered_set_member_hook<> set_hook;

	phonebook_entry pEntry;

public:
	using list_type = boost::intrusive::list<PhonebookWrapper, member_hook<PhonebookWrapper, list_member_hook<>, &PhonebookWrapper::list_hook>>;
	using set_type = boost::intrusive::unordered_set<PhonebookWrapper, member_hook<PhonebookWrapper, unordered_set_member_hook<>, &PhonebookWrapper::set_hook>>;

	void SetPnonebookEntry(phonebook_entry& entry)
	{
		pEntry.a = entry.a;
		pEntry.b = entry.b;
	}

	friend bool operator== (const PhonebookWrapper &left, const PhonebookWrapper &right)
	{
		return left.pEntry == right.pEntry;
	}

	friend std::size_t hash_value(const PhonebookWrapper &value)
	{
		return std::size_t(value.pEntry.a*10 + value.pEntry.b + 1);
	}
};