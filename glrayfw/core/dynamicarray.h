
#pragma once

#include <cstdint>
#include <cstdlib>
#include <cassert>

template <typename T>
class DynamicArray
{

protected:

	static const size_t INITIAL_SIZE = 5;
	static const size_t REALLOC_STEP = 5;

	T* data;
	size_t size = 0;
	size_t capacity;

	void Realloc( size_t realloc_step = REALLOC_STEP )
	{
		capacity += realloc_step;
		void* p = realloc( data, capacity * sizeof(T) );
		if( p || !capacity )
		{
			data = ((T*)p);
		}
	}

public:

	DynamicArray(size_t initial_size = INITIAL_SIZE)
	{
		capacity = initial_size;
		size = 0;
		data = ((T*)malloc(initial_size * sizeof(T)));
	}

	DynamicArray& operator=(const DynamicArray& other)
	{
		CopyOther(other);
		return *this;
	}

	DynamicArray(const DynamicArray& other)
	{
		CopyOther(other);
	}

	virtual ~DynamicArray( )
	{
		if( data )
		{
			free(data);
		}
	}

	void Add( T item )
	{
		if( size+1 > capacity )
		{
			Realloc();
		}

		data[size] = item;
		size++;
	}

	T& Back()
	{
		assert(!IsEmpty());
		return data[size-1];
	}

	T& operator[](size_t index) const
	{
		// no error check
		return data[index];
	}

	T& operator[](size_t index)
	{
		// no error check
		return data[index];
	}

	void Clear()
	{
		size = 0;
	}

	void RemoveLast()
	{
		if( !IsEmpty() ) size--;
	}

	bool IsEmpty()
	{
		return size == 0;
	}

	size_t Size() const
	{
		return size;
	}

	T* GetRawData()
	{
		return data;
	}

private:
	void CopyOther(const DynamicArray& other)
	{
		capacity = other.capacity;
		size = other.size;
		data = ((T*)malloc(capacity * sizeof(T)));
		for (int i = 0; i < size; i++)
		{
			data[i] = other.data[i];
		}
	}


};
