#ifndef _TINY_VECTOR_H
#define _TINY_VECTOR_H

#include <Arduino.h>

// Minimal class to replace std::vector
template<typename Data>
class Vector {

    size_t d_size; // Stores no. of actually stored objects
    size_t d_capacity; // Stores allocated capacity
    Data *d_data; // Stores data this is this "heap" we need a function that returns a pointer to this value, to print it
public:
    Vector() : d_size(0), d_capacity(0), d_data(0) {}; // Default constructor

    Vector(Vector const &other) : d_size(other.d_size), d_capacity(other.d_capacity), d_data(0) //for when you set 1 vector = to another
    {
        d_data = (Data *)malloc(d_capacity*sizeof(Data));
        memcpy(d_data, other.d_data, d_size*sizeof(Data));
    }; // Copy constuctor

    ~Vector() //this gets called
    {
        free(d_data);
    }; // Destructor

    Vector &operator=(Vector const &other)
    {
        free(d_data);
        d_size = other.d_size;
        d_capacity = other.d_capacity;
        d_data = (Data *)malloc(d_capacity*sizeof(Data));
        memcpy(d_data, other.d_data, d_size*sizeof(Data));
        return *this;
    }; // Needed for memory management

    // резервируем память для нужного кол-ва записей
    void reserve(size_t count)
    {
      if(count <= d_capacity) // уже есть буфер нужного размера
        return;

        d_capacity = count;

        Data *newdata = (Data *)malloc(d_capacity*sizeof(Data)); //allocates new memory
        memcpy(newdata, d_data, d_size * sizeof(Data));  //copies all the old memory over
        free(d_data);                                          //free old
        d_data = newdata;        
    }

    void push_back(Data const &x)
    {
        if (d_capacity == d_size) //when he pushes data onto the heap, he checks to see if the storage is full
            resize();  //if full - resize

        d_data[d_size++] = x;
    }; // Adds new value. If needed, allocates more space

    void pop() // extract the last element by simple decrease the write pointer
    {
        if(d_size)
          --d_size;
    };

    void empty() // simple set size to 0 without memory free
    {
      d_size = 0;
    }

    void clear() //here
    {
        if(d_data)
          memset(d_data, 0, d_size);
        d_capacity = 0;
        d_size = 0;
        free(d_data);
        d_data = NULL;
    }

    size_t size() const { return d_size; }; // Size getter

    Data const &operator[](size_t idx) const { return d_data[idx]; }; // Const getter

    Data &operator[](size_t idx) { return d_data[idx]; }; // Changeable getter

    Data *pData() { return (Data*)d_data; }

private:
    void resize()
    {
        d_capacity = d_capacity ? d_capacity * 2 : 1;
        Data *newdata = (Data *)malloc(d_capacity*sizeof(Data)); //allocates new memory
        memcpy(newdata, d_data, d_size * sizeof(Data));  //copies all the old memory over
        free(d_data);                                          //free old
        d_data = newdata;
    };// Allocates double the old space
};

#endif
