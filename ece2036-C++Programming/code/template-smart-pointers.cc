// Illustrate the smart pointer approach using Templates
// George F. Riley, Georgia Tech, Spring 2012

#include <iostream>
#include <vector>

using namespace std;

// The Ptr class contains two members, one a pointer to a generic
// template type "T", and the int pointer for the reference count

template <typename T>
class Ptr {
public:
  Ptr(T* t0);
  // We need a copy constructor, assignment operator, and destructor
  Ptr(const Ptr&);                // Copy constructor
  Ptr& operator=(const Ptr& rhs); // Assignment operator
  ~Ptr();                         // Destructor

  // And we need a de-referencing operator, which should return
  // the dereferenced "t" pointer;
  const T& operator*() const;     // De-referencing operator
  T& operator*();                 // Non-const version
  // And the "arrow" operator.  Need both const and non-const
  T* operator->();
  const T* operator->() const;
private:
  // Members are private
  T* t;
  int* refCount;
};

// Ptr Implementations
template <typename T>
Ptr<T>::Ptr(T* t0)
: t(t0)
{ // Constructor
  refCount = new int; // Create the refcount variable
  *refCount = 1;      // Initially only one reference (this one).
};

// Copy Constructor
template<typename T>
Ptr<T>::Ptr(const Ptr<T>& rhs)
: refCount(rhs.refCount), t(rhs.t)
{
  // Increment the refCount
  (*refCount)++;
}

// Assignment operator
template <typename T>
Ptr<T>& Ptr<T>::operator=(const Ptr<T>& rhs)
{
  // Protect against self assignment
  if (&rhs == this) return *this;
  // First reduce refcount in lhs and free if needed
  (*refCount)--;
  if (*refCount == 0)
    { // No remaining references
      delete t;
      delete refCount;
    }
  refCount = rhs.refCount;
  t = rhs.t;
  (*refCount)++; // Another reference 
  return *this;
}

// Destructor
template<typename T>
Ptr<T>::~Ptr<T>()
{
  (*refCount)--;
  if (*refCount == 0)
    { // No remaining references
      delete t;
      delete refCount;
    }
}

// Dereferencing operators
template <typename T>
const T& Ptr<T>::operator*() const
{
  return *t;
}

template <typename T>
T& Ptr<T>::operator*()
{
  return *t;
}

// Arrow operators
template <typename T>
const T* Ptr<T>::operator->() const
{
  return t;
}

template <typename T>
T* Ptr<T>::operator->()
{
  return t;
}
//NewPage
// Implement the create functions

template <typename T>
Ptr<T> Create(void); // Constructor with no arguments
template <typename T, typename T1>
Ptr<T> Create(T1); // With one argument
template <typename T, typename T1, typename T2>
Ptr<T> Create(T1, T2); // With two arguments
template <typename T, typename T1, typename T2, typename T3>
Ptr<T> Create(T1, T2, T3); // With three arguments
  // Add additional "Create" with more arguments as needed
  

template<typename T>
Ptr<T> Create(void)
{
  T* t0 = new T; // Constructor with no arguments
  return Ptr<T>(t0);
}

template<typename T, typename T1>
Ptr<T> Create(T1 a1)
{
  T* t0 = new T(a1); // Constructor with one argument
  return Ptr<T>(t0);
}

template<typename T, typename T1, typename T2>
Ptr<T> Create(T1 a1, T2 a2)
{
  T* t0 = new T(a1, a2); // Constructor with two arguments
  return Ptr<T>(t0);
}

template<typename T, typename T1, typename T2, typename T3>
Ptr<T> Create(T1 a1, T2 a2, T3 a3)
{
  T* t0 = new T(a1, a2, a3); // Constructor with three arguments
  return Ptr<T>(t0);
}

// Now create classes A and B that we will use to illustrate the smart
// pointers.

class A 
{
public:
  // constructors and destructors
  A();
  A(int);
  A(const A&);
  ~A();
  void Hello() const;
public:
  int a;
public:
  static int constructorCount; // Counts total number of constructors
  static int destructorCount;  // Counts total number of destructors
};

class B
{
public:
  B();
  B(int, int, int); // three args
  B(const B&);
  ~B();
  void Hello() const;
public:
  int b0;
  int b1;
  int b2;
public:
  static int constructorCount; // Counts total number of constructors
  static int destructorCount;  // Counts total number of destructors
};

// Implement A and B
A::A()
  : a(0)
{
  constructorCount++;
};

A::A(int a0)
  : a(a0)
{
  constructorCount++;
};

A::A(const A& rhs)
  : a(rhs.a)
{
  constructorCount++;
};

A::~A()
{ // destructor
  destructorCount++;
};

void A::Hello() const
{
  cout << "Hello from A, a is " << a << endl;
}


B::B()
  : b0(0), b1(0), b2(0)
{
  constructorCount++;
};

B::B(int b00, int b01, int b02)
  : b0(b00), b1(b01), b2(b02)
{
  constructorCount++;
};

B::B(const B& rhs)
  : b0(rhs.b0), b1(rhs.b1), b2(rhs.b2)
{
  constructorCount++;
};

B::~B()
{ // destructor
  destructorCount++;
};

void B::Hello() const
{
  cout << "Hello from B, b0 is " << b0 << endl;
}

// Helper functions
void PrintCounts()
{
  cout << "A constructor " << A::constructorCount
       << " A destructor " << A::destructorCount << endl;
  cout << "B constructor " << B::constructorCount
       << " B destructor " << B::destructorCount << endl;
};

// Define the A and B static variables
int A::constructorCount = 0;
int A::destructorCount = 0;
int B::constructorCount = 0;
int B::destructorCount = 0;

void Test1()
{
  // Just create a single A and B (with Create) and do not delete
  Ptr<A> pA = Create<A>(10);
  Ptr<B> pB = Create<B>(10, 20, 30);
  // just exit; smart pointer semantics call destructors automatically
}

void Test2()
{
  // Just create a single A and B (with Create), then use Ptr copy constructor
  // to create two more; no deletes
  Ptr<A> pA = Create<A>(10);
  Ptr<A> pACopy(pA);
  Ptr<B> pB = Create<B>(10, 20, 30);
  Ptr<B> pBCopy(pB);
  // just exit; smart pointer semantics call destructors automatically
}

void Test3Helper(Ptr<A> aByValue,
                 Ptr<B> bByValue)
{
  // Also illustrates the dereferencing operator
  cout << "Hello from Test3Helper a is " << (*aByValue).a
       << " b0 is " << (*bByValue).b0 << endl;
  // Again, using arrow operator
  cout << "Hello agin from Test3Helper a is " << aByValue->a
       << " b0 is " << bByValue->b0 << endl;

}

void Test3()
{ 
  // Create an A and B, pass by value to a function
  Ptr<A> pA = Create<A>(10);
  Ptr<B> pB = Create<B>(10, 20, 30);
  Test3Helper(pA, pB);
}

void Test4Helper(const Ptr<A>& a, // by const reference
                 const Ptr<B>& b)
{
  // Illustrate calling A and B member functions using
  // arrow operator
  a->Hello();
  b->Hello();
}

void Test4()
{ 
  // Create an A and B, pass by const reference to a function
  Ptr<A> pA = Create<A>(10);
  Ptr<B> pB = Create<B>(10, 20, 30);
  Test4Helper(pA, pB);
}

void Test5()
{ 
  // Create an two A and two B Ptr objects, then use assignment
  // operator.
  Ptr<A> pA = Create<A>(10);
  Ptr<B> pB = Create<B>(10, 20, 30);
  Ptr<A> pA1 = Create<A>(100);
  Ptr<B> pB1 = Create<B>(100, 200, 300);
  // Use assignment operator
  pA = pA1;
  pB = pB1;
  pA->Hello();
  pB->Hello();
}

void Test6()
{
  // Create a vector of 100 Ptr<A> objects
  vector<Ptr<A> > v;
  for (size_t i = 0; i < 100; ++i)
    {
      v.push_back(Create<A>(i));
    }
  // and just exit, no delete or clean up
}

// Main program
int main(int argc, char** argv)
{
  if (argc < 2)
    {
      cout << "Usage: ./template-smart-pointers (testNum)" << endl;
      exit(1);
    }
  int testNum = atol(argv[1]);
  switch (testNum)
    {
    case 1:
      Test1();
      PrintCounts();
      break;
    case 2:
      Test2();
      PrintCounts();
      break;
    case 3:
      Test3();
      PrintCounts();
      break;
    case 4:
      Test4();
      PrintCounts();
      break;
    case 5:
      Test5();
      PrintCounts();
      break;
    case 6:
      Test6();
      PrintCounts();
      break;
    }
}
