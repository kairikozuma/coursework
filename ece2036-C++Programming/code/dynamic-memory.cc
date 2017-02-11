// Dynamic Memory

// Memory layout

// Stack
// Heap
// Global
// OS/Text

// Stack used to store:
//    local variables within functions
//    formal parameters
//    return address
// - Stack allows calls to subroutines

// Heap used to store data

// - c method for allocating to heap
// - Call for 1000 employees of type emp_t
// emp_t* e = malloc(sizeof(emp_t) * 1000);
// - returns pointer to some area in heap

// - c method for freeing heap memory
// free(e);
// - free heap memory from pointer to e
// - size of memory allocated stored before the e pointer
// - free automatically knows amount of memory allocated

// - c++ method for allocating to heap
// A* a = new A[100];
// - initializes array of size 100 in heap
// - calls default constructor for each A object in array
// - a stored in stack, array stored in heap
// - "new" keyword is responsible for putting memory in the heap

// - c++ method for freeing heap memory
// delete[] a
// - [] tells runtime that a is a pointer
// - if no [], only frees the 0th element of a
// - deallocate heap memory
// - calls destructor each element in array

#include <iostream>

using namespace std;

class A {
    public:
        A();            // Default constructor
        A(int i);       // Int constructor
        A(const A& a0); // Copy constructor
        ~A();           // Destructor
    public:
        int a;
};

// Implement the constructors
A::A() : a(0) {
    cout << "Hello from A Default Constructor, a is " << a << endl;
}

A::A(int a0) : a(a0) {
    cout << "Hello from A int Constructor, a is " << a << endl;
}

A::A(const A& a0) : a(a0.a) {
    cout << "Hello from A Copy Constructor, a is " << a << endl;
}

A::~A() {
    cout << "Hello from A destructor, a is " << a << endl;
}

// Define a global array of A objects.
// We already know that the compiler is responsible for finding
// memory, calling the constructor(s) prior to entering "main"
// and calling the destructors after exiting "main"
A globalArrayA[10]; // 10 constructors called

// A subroutine that allocates an array dynamically and does
// not delete it.
void Sub1(int numberObjects) {
    // Allocate an array of A objects using new
    cout << "In sub1, allocating \"numberObjects\" A objects with new" << endl;
    A* pA = new A[numberObjects];
    // Since we used the default constructor, all a’s should be
    // initialized to zero.
    cout << "In sub1, printing object values" << endl;

    for (int i = 0; i < numberObjects; ++i) {
        cout << "pA[" << i << "] is " << pA[i].a << endl;
    }
    // Now we exit without doing anything with pA (no delete).
    // Memory leak, because delete not called on pA
}

A* Sub2(int numberObjects) {
    // Allocate an array of A objects using new
    cout << "In sub2, allocating \"numberObjects\" A objects with new" << endl;
    A* pA = new A[numberObjects];
    // Initialize the pA.a members to non-default values
    for (int i = 0; i < numberObjects; ++i) {
        pA[i].a = i * 100;
    }
    // Now we exit by returning the pA pointer to the caller.
    // but not "deleting" it
    // Not a memory leak yeat (caller must delete)
    return pA;
}

int main() {
    // Illustrate the use of dynamic memory
    // First some simple local variables. In all cases, the
    // memory to hold the variable is automatically allocated on
    // the stack, and the constructor is called. When the function
    // exits (in this case "main" exiting, the destructor is called
    // then the memory is "deleted"
    cout << "Entering main" << endl;
    A a0(1); // Single A object on stack with "int" constructor
    cout << "Creating local aArray[20]" << endl;
    // Stored in stack
    A aArray[20]; // Array of 20 A’s, using default constructor
    // Unfortunately, there is no easy syntax for creating an array of
    // "k" A object with non-default constructor, although it can be
    // done. We will discuss array initialization later.
    //A aArray2[10](10); // Won’t compile
    // So far we create a global array of A objects "globalArrayA"
    // and a local array "aArray". The problem is that in both cases
    // we have to know AT COMPILE TIME the size of the array. What
    // we really want is a way to decide AT RUN TIME how big an array
    // should be. This can by done by using the HEAP.

    // For this simple example, we just use the constant 8, but let’s
    // assume that "arraySize" is somehow determined at run time and
    // can be any reasonable value.
    int arraySize = 8;

    cout << "Allocating pointerToArray" << endl;
    A* pointerToArray = new A[arraySize];
    // Note the use of the "new" operator. This does three separate and
    // distinct things:
    // 1) Find enough contiguous memory for "arraySize" objects of class A
    // 2) Call the default constructor on each of the new A objects
    // 3) Return a POINTER to the allocated memory
    // Let’s initialize the new array to some value
    for (int i = 0; i < arraySize; ++i) {
        pointerToArray[i].a = i;
    }
    // And print them out
    for (int i = 0; i < arraySize; ++i) {
        cout << "element " << i << " is " << pointerToArray[i].a << endl;
    }
    // Since we explicitly allocated memory for "pointerToArray" with new,
    // we must explicitly destroy the memory with "delete". And since
    // we allocated an array of objects, we need a special form of
    // delete as shown.
    cout << "Deleting pointerToA" << endl;
    delete [] pointerToArray;
    // Destructor called in reverse order of constructor

    // We can also use "new" to allocate a single object. In this case
    // we can specify a non-default constructor
    cout << "Allocating single A object with int constructor" << endl;
    A* pA = new A(200); // Int constructor
    cout << "pA.a is " << pA->a << endl;
    // And we should return the memory with "delete".
    cout << "Deleting pA" << endl;
    delete pA;
    // One call to destructor

    // Call a subroutine to illustrate a "memory leak".
    cout << "Calling sub1" << endl;
    Sub1(5);

    // Call a subroutine illustrating a function returning a pointer
    // to a dynamically allocated array
    cout << "Calling sub2 to allocate an array " << endl;
    A* pA2 = Sub2(10);
    // Print out the returned pA2 array
    cout << "Printing pA2 (return from Sub2)" << endl;
    for (int i = 0; i < 10; ++i) {
        cout << "pA2[" << i << "] = " << pA2[i].a << endl;
    }

    // We need to return the memory for pA2 when we are done with it.
    cout << "Deleting pA2" << endl;
    delete [] pA2;
    cout << "Exiting Main" << endl;

    // Going out of scope
    // Destructs any local variable
    // Pointer is also destructed, but anything that it points to is not
    // because pointer can be returned to caller (without destroying array)

    // Double delete bug
    // Cannot delete a pointer twice
}
