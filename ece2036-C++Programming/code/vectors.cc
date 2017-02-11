// Vector class

int main() {
    // Typed vector standard template library
    Vector<int> v;

    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    // v.push_back(Employee e) // Compile time error

    // Size of vector
    int sz = v.size();

    // ALlocate space for 10 objects
    v.reserve(10);

    // Indexing operator
    int m = v[1]; // Get the second element
    int k = v[2]; // Get random data, no bounds checking
    int k = v.at(2); // Same as indexing operator
    v[0] = 20;    // Set element to a certain value

    // Remove last element, returns void because it destructs object
    v.pop_back();

    // Access last and first elements
    int b = v.back();
    int f = v.front()
}

/* Construct new T object at specified array, without allocating memory
new (address) T; // emplace new

*/



/* Vector class implementation concept
template <typename T> class Vector {
private:
    int numElements;
    T* p;
public:
    Vector() {
        numElements = 0;
        p = 0;
    }
    push_back(const T& t0) {
        T* newP = new T[numElements + 1];

        // Copy elements to newP

        newP[numElements] = t0;
        numElements++;
        delete [] p;
        p = newP;
    }
    reserve(int sz) {
        p = malloc(sizeof(T) * sz);
    }
}
*/
