import java.util.Iterator;
import java.util.Set;
import java.util.Collection;

/**
* This class implements the SimpleSet interface
*
* @param <T> Param Type
* @version 1.0
* @author Kairi Kozuma
*/

public class MySet<T> implements Set<T> {

    private static final int INITIAL_SIZE = 10;

    private T[] backingArray;
    private int numElements;

    /**
     * MySet constructor
     */
    public MySet() {
        backingArray = (T[]) new Object[INITIAL_SIZE];
        numElements = 0;
    }

    /**
     * Add element to MySet without duplicates
     * @param  element the element to be added to the set
     * @return         whether set was changed when element added
     */
    @Override
    public boolean add(T element) {
        // Check if element already exists, return if already in array
        if (element == null || contains(element)) {
            return false;
        }

        // If number of elements is greater than the array size, resize
        if (numElements >= backingArray.length) {
            T[] tempArray = (T[]) new Object[backingArray.length * 2];

            // Copy elements to new array
            for (int i = 0; i < backingArray.length; i++) {
                tempArray[i] = backingArray[i];
            }

            // Use resized array as backing array
            backingArray = tempArray;
        }

        // Add the element to the array
        backingArray[numElements] = element;

        // Increment element count
        numElements++;
        return true;
    }

    /**
     * Add elements in collection to MySet without duplicates
     * @param  c collection of elements to be added
     * @return   whether set was changed when all elements added
     */
    @Override
    public boolean addAll(Collection<? extends T> c) {
        boolean added = false;
        boolean changed = false;
        for (Object o : c) {
            T myElement = (T) o;
            added = add(myElement);
            if (added) {
                changed = true;
            }
        }
        return changed;
    }


    /**
     * Clear all elements from the MySet
     */
    @Override
    public void clear() {
        backingArray = (T[]) new Object[INITIAL_SIZE];
        numElements = 0;
    }

    /**
     * Whether MySet contains a specified object
     * @param  o the element whose presence is tested
     * @return   if MySet contains element
     */
    @Override
    public boolean contains(Object o) {
        if (o == null) {
            return false;
        }

        T myElement = (T) o;
        // Return true if element is within array
        for (int i = 0; i < numElements; i++) {
            T element = backingArray[i];
            if (myElement.equals(element)) {
                return true;
            }
        }

        // Return false otherwise
        return false;
    }

    /**
     * Whether MySet contains all elements in c
     * @param  c collection of elements whose presence is tested in MySet
     * @return   if MySet contains all elements of c
     */
    @Override
    public boolean containsAll(Collection<?> c) {

        for (Object o : c) {
            T element = (T) o;
            // If an element is not contained, return false
            if (!contains(element)) {
                return false;
            }
        }

        // Return true if all elements are of type T and is contained
        return true;
    }

    /**
     * Whether MySet is equivalent to a specified object
     * @param  o the object tested for equivalence
     * @return   if MySet is equal to specified object
     */
    @Override
    public boolean equals(Object o) {
        // Check for self identity
        if (this == o) {
            return true;
        }

        if (!(o instanceof MySet)) {
            return false;
        }

        MySet<T> mySet = (MySet<T>) o;
        return (numElements == mySet.size()) && (containsAll(mySet));
    }

    /**
     * Calculate the hashcode of MySet
     * @return integer value of the calculated hashcode
     */
    @Override
    public int hashCode() {
        int prime = 17;

        int hashCode = 0;

        for (int i = 0; i < numElements; i++) {
            hashCode += backingArray[i].hashCode();
        }

        return hashCode;
    }

    /**
     * Whether MySet has no elements
     * @return if MySet has no elements
     */
    @Override
    public boolean isEmpty() {
        return (numElements == 0);
    }

    @Override
    public boolean remove(Object o) {
        if (o == null) {
            return false;
        }

        T myElement = (T) o;
        if (contains(myElement)) {
            int i = 0;
            boolean found = false;
            while (!found && i < numElements) {
                // When element found, set location in array to null
                // Move elements to fill the gap
                if (myElement.equals(backingArray[i])) {
                    backingArray[i] = null;
                    for (int j = i; j < numElements - 1; j++) {
                        backingArray[j] = backingArray[j + 1];
                    }
                }
                i++;
            }

            // Decrement number of elements
            numElements--;
            return true;
        }

        // Return false otherwise
        return false;
    }

    /**
     * Remove all elements in c whose presence is also in this collection
     * @param  c collection of elements to be removed
     * @return   if collection was changed due to removal
     */
    @Override
    public boolean removeAll(Collection<?> c) {
        boolean removed = false;
        boolean changed = false;

        for (Object o : c) {
            T element = (T) o;
            removed = remove(element);
            if (removed) {
                changed = true;
            }
        }
        return changed;
    }

    /**
     * Retain all elements in c whose presence is also in this collection
     * @param  c collection of elements to be removed
     * @return   if collection was changed due to removal
     */
    @Override
    public boolean retainAll(Collection<?> c) {
        boolean changed = false;

        System.out.println("RETAIN ALL");

        int i = 0;
        while (i < numElements) {
            T myElement = backingArray[i];

            // If element is not within the collection, remove
            if (!c.contains(myElement)) {
                remove(myElement);
                changed = true;
            } else {
                i++;
            }
        }

        return changed;
    }

    /**
     * Get the size of the collection
     * @return integer size of the collection
     */
    @Override
    public int size() {
        return numElements;
    }

    /**
     * Returns the set in array form
     * @return the set as an array
     */
    @Override
    public Object[] toArray() {
        // Create new array with size equal to number of elements
        T[] myArray = (T[]) new Object[numElements];

        // Copy by value to new array
        // // TODO
        for (int i = 0; i < numElements; i++) {
            myArray[i] = backingArray[i];
        }

        return myArray;
    }

    /**
     * Returns the set as an array stored in a
     * Creates a new array if a is smaller than the set
     * @return the set as an array stored in a, new array if smaller
     */
    @Override
    public <T> T[] toArray(T[] a) {
        if (a.length >= numElements) {
            for (int i = 0; i < numElements; i++) {
                a[i] = (T) backingArray[i];
            }
            return a;
        } else {
            return (T[]) toArray();
        }
    }

    /*
    ----------------------------------------------
    These methods are provided to help you, DO NOT MODIFY!
    Refer to the slides on Iterators if you want to learn more
    http://cs1331.org/slides/iterators.pdf
    */

    /**
    * Returns an iterator if you wish to use it
    * @return An iterator for the SimpleSet
    */
    @Override
    public Iterator<T> iterator() {
        return new MySetIterator();
    }

    private class MySetIterator implements Iterator {

        private int index = 0;
        public boolean hasNext() {
            return index < numElements;
        }

        public T next() {
            return backingArray[index++];
        }

        public void remove() {
            MySet.this.remove(backingArray[index]);
            index--;
        }
    }

}
