// Demonstrate the use of the Standard Template Library "vector" class.
// George F. Riley, ECE3090 Georgia Tech, Fall 2009

// A vector is a variable length array. It starts out as "zero" length
// and grows or shrinks as needed. Further, the vector is a array
// of any arbitrary type, using the C++ "templates" feature.

#include <iostream>
#include <vector>
//#include "GFRVec.h" // Uncomment to use the "buggy" one
#include "GFRVec1.h" // Comment out this one if using the above one
class A {
public:
  A(); // Default constructor
  A(int); // Non-Default Constructor
  A(const A&); // A copy constructor is used by the compile whenever
  A& operator=(const A&); // Assignment operator
  ~A(); // Destructor
public:
  int x; // Single data member
};


typedef std::vector<A> AVec_t; // Define a type that is vector of A’s
typedef std::vector<A*> APVec_t;// Define a type that is vector of A pointers
// Uncomment to use our own implementation (and comment out above)
//typedef GFRVec<A> AVec_t; // Define a type that is vector of A’s
//typedef GFRVec<A*> APVec_t;// Define a type that is vector of A pointers

A::A()
{
  std::cout << "Hello from A::A() Default constructor" << std::endl;
}

A::A(int i) : x(i)
{
  std::cout << "Hello from A::A(int) constructor" << std::endl;
}

A::A(const A& a) : x(a.x)
{
  std::cout << "Hello from A Copy constructor" << std::endl;
}

A& A::operator=(const A& rhs)
{
  std::cout << "Hello from A Assignment operator" << std::endl;
  x = rhs.x;
}

A::~A()
{
  std::cout << "Hello from A Destructor" << std::endl;
}

int main()
{
  std::cout << "Creating A Vector"; getchar();
  AVec_t av0;

  std::cout << "Adding an three elements to av0"; getchar();
  av0.push_back(A(2)); // Elements are appended using "push_back"
  std::cout << "After first push_back"; getchar();
  av0.push_back(A(10)); // Elements are appended using "push_back"
  std::cout << "After second push_back"; getchar();
  av0.push_back(A(100)); // Elements are appended using "push_back"
  // New array allocated, values copied to new objects, old objects destroyed
  // Number of elements in a vector can be queried with "size()"
  std::cout << "After third push_back, size av0 is "<<av0.size()<<std::endl;

  // Now reserve space for up to 10 elements, allowing for more
  // efficient push_back.
  std::cout << "Reserving 10 elements"; getchar();
  av0.reserve(10);
  // Copies values from old array
  // Push a few more to show better efficiency
  std::cout << "Pushing three more elements"; getchar();
  av0.push_back(A(101));
  av0.push_back(A(102));
  av0.push_back(A(103));

  // Individual elements can be accessed with the [] operator
  std::cout << "Accessing elements with the [] operator"; getchar();
  std::cout << "av0[0].x is " << av0[0].x << std::endl;
  std::cout << "av0[1].x is " << av0[1].x << std::endl;
  std::cout << "av0[2].x is " << av0[2].x << std::endl;

  // Front and back of list have special accessors
  std::cout << "Accessing elements with the front and back"; getchar();
  std::cout << "av0.front().x is " << av0.front().x << std::endl;
  std::cout << "av0.back().x is " << av0.back().x << std::endl;

  // Vectors can be copied with copy constructor or assignment operator
  std::cout << "Making a copy of av0"; getchar();
  AVec_t av1(av0);
  std::cout << "Size of av1 is " << av1.size() << std::endl;
  std::cout << "av1[0].x is " << av1[0].x << std::endl;

  // Vectors can be shrunk with "pop_back". Notice that pop_back
  // does NOT return the element being popped
  std::cout << "Shrinking av0 with pop_back"; getchar();
  av0.pop_back(); // Remove last element
  // Element not returned because it is destroyed
  std::cout << "Size of av0 is " << av0.size() << std::endl;
  av0.pop_back(); // Remove another element
  std::cout << "Size of av0 is " << av0.size() << std::endl;

  // Vectors can be initialized to "n" copies of a specified object
  std::cout << "Constructing AVec_t with 10 elements"; getchar();
  AVec_t av2(10, A(1)); // Makes 10 elements of A(1)
  std::cout << "Size of av2 is " << av2.size() << std::endl;
  std::cout << "av2[0].x is " << av2[0].x << std::endl;

  // All elements of a vector can be removed with "clear()"
  std::cout << "Clearing av2"; getchar();
  av2.clear();
  std::cout << "Size of av2 is " << av2.size() << std::endl;

  // Push another element to demonstrate that "clear" did not
  // free the memory.
  // Simple int construct, copy construct, destroy
  std::cout << "push another on av2"; getchar();
  av2.push_back(A(100));

  std::cout << "push another on av2"; getchar();
  av2.push_back(A(200));

  // Create and populate a vector of A poiners
  // Don't put pointers in containers, because constructor not called
  // Destructor not called when cleared
  std::cout << "Creating A Pointer Vector"; getchar();
  APVec_t apv0;

  std::cout << "Adding an three elements to apv0"; getchar();
  apv0.push_back(new A(2));
  apv0.push_back(new A(10));
  apv0.push_back(new A(100));
  // Number of elements in a vector can be queried with "size()"
  std::cout << "Size of apv0 is " << apv0.size() << std::endl;

  // Clear the apv0 vector. Note: ~A() NOT called. Why not?
  std::cout << "Clearing apv0"; getchar();
  apv0.clear();
  std::cout << "Size of apv0 is " << apv0.size() << std::endl;

  std::cout << "Main program exiting"; getchar();
  return 0;
}

// Output from this program is:
//
// Creating A Vector
// Adding an three elements to av0
// Hello from A::A(int) constructor
// Hello from A Copy constructor
// Hello from A Destructor
// After first push_back
// Hello from A::A(int) constructor
// Hello from A Copy constructor
// Hello from A Copy constructor
// Hello from A Destructor
// Hello from A Destructor
// After second push_back
// Hello from A::A(int) constructor
// Hello from A Copy constructor
// Hello from A Copy constructor
// Hello from A Copy constructor
// Hello from A Destructor
// Hello from A Destructor
// Hello from A Destructor
// After third push_back, size av0 is 3
// Reserving 10 elements

// Hello from A Copy constructor
// Hello from A Copy constructor
// Hello from A Copy constructor
// Hello from A Destructor
// Hello from A Destructor
// Hello from A Destructor
// Pushing three more elements
// Hello from A::A(int) constructor
// Hello from A Copy constructor
// Hello from A Destructor
// Hello from A::A(int) constructor
// Hello from A Copy constructor
// Hello from A Destructor
// Hello from A::A(int) constructor
// Hello from A Copy constructor
// Hello from A Destructor
// Accessing elements with the [] operator
// av0[0].x is 2
// av0[1].x is 10
// av0[2].x is 100
// Accessing elements with the front and back
// av0.front().x is 2
// av0.back().x is 103
// Making a copy of av0
// Hello from A Copy constructor
// Hello from A Copy constructor
// Hello from A Copy constructor
// Hello from A Copy constructor
// Hello from A Copy constructor
// Hello from A Copy constructor
// Size of av1 is 6
// av1[0].x is 2
// Shrinking av0 with pop_back
// Hello from A Destructor
// Size of av0 is 5
// Hello from A Destructor
// Size of av0 is 4
// Constructing AVec_t with 10 elements
// Hello from A::A(int) constructor
// Hello from A Copy constructor
// Hello from A Copy constructor
// Hello from A Copy constructor
// Hello from A Copy constructor
// Hello from A Copy constructor
// Hello from A Copy constructor
// Hello from A Copy constructor
// Hello from A Copy constructor
// Hello from A Copy constructor
// Hello from A Copy constructor
// Hello from A Destructor
// Size of av2 is 10
// av2[0].x is 1
// Clearing av2
// Hello from A Destructor
// Hello from A Destructor
// Hello from A Destructor
// Hello from A Destructor
// Hello from A Destructor
// Hello from A Destructor
// Hello from A Destructor
// Hello from A Destructor
// Hello from A Destructor
// Hello from A Destructor
// Size of av2 is 0
// push another on av2
// Hello from A::A(int) constructor
// Hello from A Copy constructor
// Hello from A Destructor
// Creating A Pointer Vector
// Adding an three elements to apv0
// Hello from A::A(int) constructor
// Hello from A::A(int) constructor
// Hello from A::A(int) constructor
// Size of apv0 is 3
// Clearing apv0
// Size of apv0 is 0
// Main program exiting
// Hello from A Destructor
// Hello from A Destructor
// Hello from A Destructor
// Hello from A Destructor
// Hello from A Destructor
// Hello from A Destructor
// Hello from A Destructor
// Hello from A Destructor
// Hello from A Destructor
// Hello from A Destructor
// Hello from A Destructor


// An implementation of a simplified STL Vector
// George F. Riley, Georgia Tech, Fall 2009

template<class T> class GFRVec
{
public:
  GFRVec() : first(0), last(0), end(0) {}
  GFRVec(size_t n)
{ // Create a GFRVec with "n" copies of T, with default constructor
  first = new T[n];
  last = first + n;
  end = last;
}
GFRVec(size_t n, const T& t)
{ // Create a GFRVec with "n" copies of t
  first = new T[n];
  for (size_t i = 0; i < n; ++i) first[i] = t; // Populate the vector
  last = first + n;
  end = last;
}
GFRVec(const GFRVec& c)
{ // Copy Constructor
  first = new T[c.size()]; // Allocate memory
  for (size_t i = 0; i < c.size(); ++i) first[i] = c[i]; // Copy elements
  last = first + c.size();
  end = last;
}
~GFRVec()
{ // Destructor, remove all elements
  clear();
}
GFRVec& operator=(const GFRVec& rhs)
{ // Assignment operator
  if (this == &rhs) return *this; // Self assignment
  delete [] first; // Free old memory
  first = new T[rhs.size()]; // Allocate memory
  for (size_t i = 0; i < rhs.size(); ++i)
  first[i] = rhs[i]; // Copy the elements
  last = first + rhs.size();
  end = last;
}
T& operator[](size_t i) const
{ // Indexing operator
  return first[i];
}
T& back() const
{ // Return last element
  return first[size()-1];
}
T& front() const
{ // Return last element
  return first[0];
}
 void pop_back()
{ // Remove last element
  last--;
  first[size()].~T(); // Call destructor on just popped object
}
void push_back(const T& t)
{ // Add new element
  if (last != end)
  { // Room for new object without re-allocating
    frst[size()] = t;
    last++;
  }
  else
  { // Need to re-allocate
  T* tmp = new T[end-first+1];
  for (size_t i = 0; i < size(); ++i) tmp[i] = first[i];
  tmp[size()] = t; // Add new element
  last = tmp + (last - first) + 1;
  end = last;
  delete [] first; // Delete and destroy old objects
  first = tmp;
  }
}
size_t size() const
{ // Number of elements in the vector
return last - first;
}
void reserve(size_t n)
{ // Reserve space for "n" elements
  if (n <= (end-first)) return; // Less than already reserved
  T* tmp = new T[n]; // Allocate new memory
  for (size_t i = 0; i < size(); ++i) tmp[i] = first[i];
  last = tmp + last - first;
  delete [] first;
  first = tmp;
  end = first + n;
}
void clear()
{ // Erase all elements
  while(size()) pop_back();
}

private:
  T* first; // Initial element
  T* last; // Last element
  T* end; // End of allocated storage
};




// An implementation of a simplified STL Vector
// This one uses uninitialized alloc and placement new operator
// George F. Riley, Georgia Tech, Fall 2009

template<class T> class GFRVec
{
public:
  GFRVec() : first(0), last(0), end(0) {}
  GFRVec(size_t n)
{ // Create a GFRVec with "n" copies of T, with default constructor
  first = (T*)malloc(n * sizeof(T));
  for (size_t i = 0; i < n; ++i)
  {
  new (&first[i]) T();
  }
  last = first + n;
  end = last;
}
GFRVec(size_t n, const T& t)
{ // Create a GFRVec with "n" copies of t
  first = (T*)malloc(n * sizeof(T));
  for (size_t i = 0; i < n; ++i)
  { // Use copy constructor to populate
  new (&first[i]) T(t);
  }
  last = first + n;
  end = last;
  }
GFRVec(const GFRVec& c)
{ // Copy Constructor
  first = (T*)malloc(c.size() * sizeof(T));
  for (size_t i = 0; i < c.size(); ++i)
  {
  new (&first[i]) T(c[i]); // Copy elements
  }

  last = first + c.size();
  end = last;
}
~GFRVec()
{ // Destructor, remove all elements
  clear();
}
GFRVec& operator=(const GFRVec& rhs)
{ // Assignment operator
  if (this == &rhs) return *this; // Self assignment
  free(first);
  first = (T*)malloc(rhs.size() * sizeof(T));
  for (size_t i = 0; i < rhs.size(); ++i)
  {
    new (&first[i]) T(rhs[i]); // Copy the elements
  }
  last = first + rhs.size();
  end = last;
}
T& operator[](size_t i) const

// Program GFRVec1.h

{ // Indexing operator
  return first[i];
}
T& back() const
{ // Return last element
  return first[size()-1];
}
T& front() const
{ // Return last element
  return first[0];
}
void pop_back()
{ // Remove last element
  last--;
  first[size()].~T(); // Call destructor on just popped object
}
void push_back(const T& t)
{ // Add new element
  if (last != end)
  { // Room for new object without re-allocating
    new (&first[size()]) T(t);
    last++;
  }
  else
  { // Need to re-allocate
  T* tmp = (T*)malloc((size() + 1) * sizeof(T));
  for (size_t i = 0; i < size(); ++i)
  { // Copy old elements
    new (&tmp[i]) T(first[i]);
  }
  new (&tmp[size()]) T(t);
  for (size_t i = 0; i < size(); ++i)
  { // Destroy old elements
    first[i].~T();
  }
  last = tmp + (last - first) + 1;
  end = last;
  free(first);
  first = tmp;
  }
}
size_t size() const
{ // Number of elements in the vector
return last - first;
}
void reserve(size_t n)
{ // Reserve space for "n" elements
  if (n <= (end-first)) return; // Less than already reserved
  T* tmp = (T*)malloc(n * sizeof(T));
  for (size_t i = 0; i < size(); ++i)
  { // Copy elements to new space
    new (&tmp[i]) T(first[i]);
  }
  last = tmp + last - first;
  free(first);
  first = tmp;
  Program GFRVec1.h (continued)

  end = first + n;
}
void clear()
{ // Erase all elements
  while(size()) pop_back();
  free(first);
  first = 0;
  last = 0;
  end = 0;
}

private:
  T* first; // Initial element
  T* last; // Last element
  T* end; // End of allocated storage
};