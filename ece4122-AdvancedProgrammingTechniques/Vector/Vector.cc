// Implementation of the templated Vector class
// ECE4893/8893 lab 3
// Kairi Kozuma

#include <iostream> // debugging
#include "Vector.h"

// Your implementation here
// Fill in all the necessary functions below
using namespace std;

// Initial reserved memory size for default constructor
const int INITIAL_SIZE = 1;

// Multiplier if space runs out when pushing to array
const int GROWTH_FACTOR = 2;

// How many more elements to reserve if space runs out
const int GROWTH_RATE = 1;

// Default constructor
template <typename T>
Vector<T>::Vector() : count(0), reserved(INITIAL_SIZE) {
  elements = (T*)malloc(sizeof(T) * INITIAL_SIZE);
}

// Copy constructor
template <typename T>
Vector<T>::Vector(const Vector& rhs) : count(rhs.Size()), reserved(rhs.Size())//TODO
{
  // Allocate memory for size of rhs
  this->elements = (T*)malloc(sizeof(T) * rhs.Size());
  // Copy over values to reserved memory
  for (int i = 0; i < rhs.Size(); i++) {
    new (&elements[i]) T(rhs[i]);
  }
}

// Assignment operator
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& rhs)
{
  // Free left hand side memory // TODO reuse memory if larger?
  free(elements);

  // Allocate memory for size of rhs
  this = new Vector(rhs);

  // Return reference
  return this;
}

#ifdef GRAD_STUDENT
// Other constructors
template <typename T>
Vector<T>::Vector(size_t nReserved) : count(0), reserved(nReserved)
{ // Initialize with reserved memory
  elements = (T*)malloc(sizeof(T) * nReserved);
}

template <typename T>
Vector<T>::Vector(size_t n, const T& t) : count(n), reserved(n)
{ // Initialize with "n" copies of "t"
  elements = (T*)malloc(sizeof(T) * reserved);
  for (int i = 0; i < n; i++) {
    new (&elements[i]) T(t);
  }
}

template <typename T>
void Vector<T>::Reserve(size_t n)
{ // Reserve extra memory

  // Allocate memory for new size
  T* newElements = (T*) malloc(sizeof(T) * (n));

  // Copy over values to new memory
  for (int i = 0; i < count; i++) {
    new (&newElements[i]) T(elements[i]);
    elements[i].~T(); // Call destructor for each object
  }

  // Deallocate old memory
  free(elements);

  // Set pointer to new memory
  elements = newElements;

  // Increase reserved size by n
  reserved = n;
}

#endif

// Destructor
template <typename T>
Vector<T>::~Vector()
{
  // Destruct existing objects
  for (int i = 0; i < count; i++) {
    elements[i].~T(); // Call destructor for each object
  }
  // Deallocate memory
  free(elements);
}

// Add and access front and back
template <typename T>
void Vector<T>::Push_Back(const T& rhs)
{

  // If no reserved memory left, resize
  if (count == reserved) {
    // Set new reserved memory size
    reserved = count + GROWTH_RATE;

    // Allocate larger contiguous memory
    T* newElements = (T*) malloc(sizeof(T) * reserved);

    // Copy over elements
    for (int i = 0; i < count; i++) {
      new (&newElements[i]) T(elements[i]);
      elements[i].~T(); // Destruct
    }

    // Delete old memory pointer
    free(elements);

    // Update reference
    elements = newElements;
  }

  // Update count
  count++;

  // Insert at end
  new (&elements[count - 1]) T(rhs);
}


template <typename T>
void Vector<T>::Push_Front(const T& rhs)
{
  // If no reserved memory left, resize
  if (count == reserved) {
    // Set new reserved memory size
    reserved = count + GROWTH_RATE;

    // Allocate larger contiguous memory
    T* newElements = (T*) malloc(sizeof(T) * reserved);

    // Copy over elements
    for (int i = 0; i < count; i++) {
      new (&newElements[i + 1]) T(elements[i]); // Shift over one
      elements[i].~T(); // Destruct
    }

    // Delete old memory pointer
    free(elements);

    // Update reference
    elements = newElements;
  }

  // Update count
  count++;

  // Insert at beginning
  new (&elements[0]) T(rhs);
}

template <typename T>
void Vector<T>::Pop_Back()
{ // Remove last element
  // If count is 0, nothing to remove
  if (count == 0) {
    return;
  }

  // Destroy last object
  elements[count - 1].~T();
  // Decrement count
  count--;
}

template <typename T>
void Vector<T>::Pop_Front()
{ // Remove first element
  // If count is 0, nothing to remove
  if (count == 0) {
    return;
  }

  // Decrement count
  count--;

  // Shift elements down
  for (int i = 0; i < count; i++){
    elements[i].~T(); // Destruct
    new (&elements[i]) T(elements[i + 1]); // Copy construct to lower index
  }

  // Destruct last object
  elements[count].~T();
}

// Element Access
template <typename T>
T& Vector<T>::Front() const
{
  return elements[0];
}

// Element Access
template <typename T>
T& Vector<T>::Back() const
{
  return elements[count - 1];
}

template <typename T>
const T& Vector<T>::operator[](size_t i) const
{ // const element access
  return elements[i];
}

template <typename T>
T& Vector<T>::operator[](size_t i)
{//nonconst element access
  return elements[i];
}

template <typename T>
size_t Vector<T>::Size() const
{
  return count;
}

template <typename T>
bool Vector<T>::Empty() const
{
  return count == 0;
}

// Implement clear
template <typename T>
void Vector<T>::Clear()
{
  // Destroy objects in place
  for (int i = 0; i < count; i++) {
    elements[i].~T();
  }

  // Update count
  count = 0;
}

// Iterator access functions
template <typename T>
VectorIterator<T> Vector<T>::Begin() const
{
  return VectorIterator<T>(elements);
}

template <typename T>
VectorIterator<T> Vector<T>::End() const
{
  return VectorIterator<T>(&(elements[count]));
}

#ifdef GRAD_STUDENT
// Erase and insert
template <typename T>
void Vector<T>::Erase(const VectorIterator<T>& it)
{
  // Decrement
  count--;

  VectorIterator<T> iter = it;
  while(iter != this->End())
  {
    (*iter).~T();
    new (&(*iter)) T(*++iter);
  }
  (*iter).~T(); // Destruct last element
}

template <typename T>
void Vector<T>::Insert(const T& rhs, const VectorIterator<T>& it)
{

  // Set new reserved memory size
  reserved = count + GROWTH_RATE;

  // Allocate larger contiguous memory
  T* newElements = (T*) malloc(sizeof(T) * reserved);

  VectorIterator<T> iter = this->Begin();
  int i = 0;
  bool inserted = false;
  while(iter != this->End())
  {
    if (!inserted && iter == it) { // If iterators are equal, insert element
      new (&newElements[i]) T(rhs); // Insert element
      inserted = true;
    } else {
      new (&newElements[i]) T(*iter); // Copy over to new array
      (*iter).~T(); // Destruct
      iter++; // Increment iterator
    }
    i++; // Increment index
  }

  // Delete old memory pointer
  free(elements);

  // Update reference
  elements = newElements;

  // Increment
  count++;
}
#endif

// Implement the iterators

// Constructors
template <typename T>
VectorIterator<T>::VectorIterator()
{
}

template <typename T>
VectorIterator<T>::VectorIterator(T* c) : current(c)
{
}

// Copy constructor
template <typename T>
VectorIterator<T>::VectorIterator(const VectorIterator<T>& rhs) : current(&(*rhs))
{
}

// Iterator defeferencing operator
template <typename T>
T& VectorIterator<T>::operator*() const
{
  return *current;
}

// Prefix increment
template <typename T>
VectorIterator<T>  VectorIterator<T>::operator++()
{
  current++;
  return *this;
}

// Postfix increment
template <typename T>
VectorIterator<T> VectorIterator<T>::operator++(int)
{
  T * prev = current;
  current++;
  return VectorIterator(prev);
}

// Comparison operators
template <typename T>
bool VectorIterator<T>::operator !=(const VectorIterator<T>& rhs) const
{
  return this->current != &(*rhs);
}

template <typename T>
bool VectorIterator<T>::operator ==(const VectorIterator<T>& rhs) const
{
  return this->current == &(*rhs);
}
