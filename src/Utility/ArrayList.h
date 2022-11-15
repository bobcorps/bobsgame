#pragma once
#include "oktypes.h"
class Logger;
#include <vector>
#include <algorithm>
#include "Logger.h"



template <typename E> class Vector
{
public:
	static void removeAt(vector<E>&v,int index)
	{
		auto it = v.begin();
		advance(it, index);
		v.erase(it);
	}
};

class Integer
{
public:
	int i;
	Integer(int i) { this->i = i; }
	int value() { return i; }
//	static int parseInt(string s)
//	{
//		return stoi(s);
//	}

};
//
//bool stob(string s)
//{
//	return 0 != stoi(s);
//}
class OKBoolean
{
public:
	//Parses the string argument as a boolean. The boolean returned represents the value true if the string argument is not null and is equal, ignoring case, to the string "true". 
	//Example: Boolean.parseBoolean("True") returns true.
	//Example: Boolean.parseBoolean("yes") returns false.
	static bool parseBoolean(string s)
	{
		std::transform(s.begin(), s.end(), s.begin(), ::tolower);
		if (s == "true")return true;
		return false;
	}
};
//
//class Float
//{
//	static float parseFloat(string s)
//	{
//		return stof(s);
//	}
//
//};



template <typename E> class BobArray
{
public:
	static Logger log;

	BobArray<E>();
	BobArray<E>(int size);
	~BobArray<E>();



	void add(E e);
	void insert(int index, E e);
	void removeAt(int index);
	void remove(E e);
	bool contains(E);
	bool containsValue(E);
	void clear();
	bool isEmpty();
	int size();
	E get(int index);
	int indexOf(E);
	void deleteAll();
	sp<vector<E>>v;

	bool operator==(const BobArray<E>& rhs) const;
	bool operator!=(const BobArray<E>& rhs) const;


	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(v);
	}


};

//BOOST_CLASS_VERSION(template <typename E> BobArray<E>, 1)
//BOOST_CLASS_TRACKING(BobArray, boost::serialization::track_never)

/*

boolean 	add(E e)
Appends the specified element to the end of this list.

void 	add(int index, E element)
Inserts the specified element at the specified position in this list.

boolean 	addAll(Collection<? extends E> c)
Appends all of the elements in the specified collection to the end of this list, in the order that they are returned by the specified collection's Iterator.

boolean 	addAll(int index, Collection<? extends E> c)
Inserts all of the elements in the specified collection into this list, starting at the specified position.

void 	clear()
Removes all of the elements from this list.

Object 	clone()
Returns a shallow copy of this BobArray instance.

boolean 	contains(Object o)
Returns true if this list contains the specified element.

void 	ensureCapacity(int minCapacity)
Increases the capacity of this BobArray instance, if necessary, to ensure that it can hold at least the number of elements specified by the minimum capacity argument.

E 	get(int index)
Returns the element at the specified position in this list.

int 	indexOf(Object o)
Returns the index of the first occurrence of the specified element in this list, or -1 if this list does not contain the element.

boolean 	isEmpty()
Returns true if this list contains no elements.

Iterator<E> 	iterator()
Returns an iterator over the elements in this list in proper sequence.

int 	lastIndexOf(Object o)
Returns the index of the last occurrence of the specified element in this list, or -1 if this list does not contain the element.

ListIterator<E> 	listIterator()
Returns a list iterator over the elements in this list (in proper sequence).

ListIterator<E> 	listIterator(int index)
Returns a list iterator over the elements in this list (in proper sequence), starting at the specified position in the list.

E 	remove(int index)
Removes the element at the specified position in this list.

boolean 	remove(Object o)
Removes the first occurrence of the specified element from this list, if it is present.

boolean 	removeAll(Collection<?> c)
Removes from this list all of its elements that are contained in the specified collection.

protected void 	removeRange(int fromIndex, int toIndex)
Removes from this list all of the elements whose index is between fromIndex, inclusive, and toIndex, exclusive.

boolean 	retainAll(Collection<?> c)
Retains only the elements in this list that are contained in the specified collection.

E 	set(int index, E element)
Replaces the element at the specified position in this list with the specified element.

int 	size()
Returns the number of elements in this list.

List<E> 	subList(int fromIndex, int toIndex)
Returns a view of the portion of this list between the specified fromIndex, inclusive, and toIndex, exclusive.

Object[] 	toArray()
Returns an array containing all of the elements in this list in proper sequence (from first to last element).

<T> T[] 	toArray(T[] a)
Returns an array containing all of the elements in this list in proper sequence (from first to last element); the runtime type of the returned array is that of the specified array.

void 	trimToSize()
Trims the capacity of this BobArray instance to be the list's current size.

*/


template <typename E>
Logger BobArray<E>::log = Logger("BobArray");

template <typename E>
bool BobArray<E>::operator==(const BobArray<E>& rhs) const
{
	return
		this->v == rhs.v
		;
}

template <typename E>
bool BobArray<E>::operator!=(const BobArray<E>& rhs) const
{
	return
		(*this == rhs) == false
		;
}


template <typename E>
BobArray<E>::BobArray()
{
}

template <typename E>
BobArray<E>::BobArray(int size)
{
	v.resize(size);
}

template<typename E>
BobArray<E>::~BobArray()
{
	//delete v;
}

template <typename E>
void BobArray<E>::add(E e)
{
	v->push_back(e);
}

template <typename E>
void BobArray<E>::deleteAll()
{
	for(int i=0;i<size();i++)
	{
		delete get(i);
	}
	clear();
}

template <typename E>
void BobArray<E>::insert(int index, E e)
{
	v.insert(v.begin()+index, e);
}

template<typename E>
void BobArray<E>::removeAt(int index)
{
	if (index < 0 || index >= size())
	{
		log.error("Index out of bounds!");
		return;
	}

	//E temp = get(index);

	//typename BobArray<E>::iterator
		auto it = v.begin();
	advance(it, index);
	v.erase(it);

	//return temp;
}

template <typename E>
void BobArray<E>::remove(E e)
{
	//if (contains(e) == false)return nullptr;
	int index = indexOf(e);

	if (index < 0 || index >= size())
	{
		log.error("Index out of bounds!");
		return;
	}

	//E temp = get(index);

	v.erase(std::remove(v.begin(), v.end(), e), v.end());

	//return temp;
}

template <typename E>
bool BobArray<E>::contains(E e)
{
	return find(v.begin(), v.end(), e) != v.end();
}

template <typename E>
bool BobArray<E>::containsValue(E e)
{
	return contains(e);
}

template<typename E>
void BobArray<E>::clear()
{
	v->clear();
}

template<typename E>
bool BobArray<E>::isEmpty()
{
	if (size() > 0)return false;
	return true;
}

template<typename E>
int BobArray<E>::size()
{
	return (int)v->size();
}

template<typename E>
E BobArray<E>::get(int index)
{
	if (index < 0 || index >= size())
	{
		log.error("Index out of bounds!");
	}

	E temp = v->at(index);
	return temp;
}

template <typename E>
int BobArray<E>::indexOf(E e)
{
	//typename BobArray<E>::iterator 
		auto it = v.begin(); 
		
		find(v.begin(), v.end(), e);

	if (it == v.end())return -1;

	int index = (int)(it - v.begin());
	if (index < 0 || index >= size())return -1;

	return index;
}

