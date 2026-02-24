#include <iostream>

template <typename Object>
class Vector
{
private:
    int theSize;
    int theCapacity;
    Object *objects;

public:
    //! The following are just the big 5 for the code to work, we do not need to know them for the exam
    // Default Constructor
    Vector() : theCapacity(0), theSize(0), objects(nullptr) {}

    // Default Destructor
    ~Vector()
    {
        delete[] objects;
    }

    // Lvalue copy constructor
    Vector(const Vector &rhs) : theSize(rhs.theSize), theCapacity(rhs.theCapacity)
    {
        objects = new Object[theCapacity];
        for (int i = 0; i < theSize; i++)
        {
            objects[i] = rhs.objects[i];
        }
    }

    // Rvalue copy constructor
    Vector(Vector &&rhs) noexcept : theSize(rhs.theSize), theCapacity(rhs.theCapacity), objects(rhs.objects)
    {
        rhs.objects = nullptr;
        rhs.theSize = 0;
        rhs.theCapacity = 0;
    }

    // Copy assignment operator (Lvalue copy)
    Vector &operator=(const Vector &rhs)
    {
        if (this != &rhs)
        {
            delete[] objects;
            theSize = rhs.theSize;
            theCapacity = rhs.theCapacity;
            objects = new Object[theCapacity];

            for (int i = 0; i < theSize; ++i)
            {
                objects[i] = rhs.objects[i];
            }
        }
        return *this;
    }

    // Move assignment operator (Rvalue move)
    Vector &operator=(Vector &&rhs) noexcept
    {
        if (this != &rhs)
        {
            std::swap(theSize, rhs.theSize);
            std::swap(theCapacity, rhs.theCapacity);
            std::swap(objects, rhs.objects);
        }
        return *this;
    }

    /*
    From this point onwards, all the following functions will be from the notes and are in the exam:
    1) size
    2) isEmpty
    3) clear
    4) reserve
    5) resize
    6) push_back (for l and r values)
    7) pop_back
    8) operator[] and at()
    9) iterator classes
    */

    // Returns current size
    int size() const
    {
        return theSize;
    }

    // clears the vector
    void clear()
    {
        theSize = 0;
        delete[] objects;
    }

    // Checks if the vector is empty
    bool empty() const
    {
        return size() == 0;
    }

    // Sets the new size
    void resize(int newSize)
    {
        if (newSize > theCapacity)
        {
            reserve(newSize * 2);
        }
        theSize = newSize;
    }

    // allcotes a larger memory block for the vector
    void reserve(int newCapacity)
    {
        if (newCapacity < theSize)
        {
            return;
        }

        Object *newArray = new Object[newCapacity];
        for (int i = 0; i < theSize; i++)
        {
            newArray[i] = std::move(objects[i]);
        }
        theCapacity = newCapacity;
        std::swap(objects, newArray);
        delete[] newArray;
    }

    // adds an object to the end of the vector
    void push_back(const Object &x)
    {
        if (theSize == theCapacity)
        {
            reserve(2 * theCapacity + 1);
        }

        objects[theSize++] = x;
    }

    // same as previous but for rvalues
    void push_back(Object &&x)
    {
        if (theSize == theCapacity)
        {
            reserve(2 * theCapacity + 1);
        }

        objects[theSize++] = std::move(x);
    }

    // deletes last item
    void pop_back()
    {
        --theSize;
    }

    // returns last object
    const Object &back() const
    {
        return objects[theSize - 1];
    }

    // defines what [] does
    Object &operator[](int idx)
    {
        return objects[idx];
    }

    // same as previous, but checks idx is safe (Slower but safer)
    Object &at(int idx)
    {
        if (idx >= 0 && idx <= theSize - 1)
        {
            return objects[idx];
        }
        throw std::out_of_range("You accessed memory out of bounds."); // for this part in the exam just put "return;" but here it wont compile without this
    }

    // iterator abstractions
    typedef Object *iterator;
    typedef const Object *const_iterator;

    iterator begin()
    {
        return &objects[0];
    }

    const_iterator begin() const
    {
        return &objects[0];
    }

    iterator end()
    {
        return &objects[size()];
    }

    const_iterator end() const
    {
        return &objects[size()];
    }

    // erase item at the iterator and shift everything left, then return iterator to next item
    iterator erase(iterator itr)
    {
        for (iterator i = itr; i != end(); ++i)
        {
            *i = std::move(*(i + 1));
        }
        --theSize;
        return itr; // Needed so sequence doesnt break
    }

    // erase range of items up until but excluding the to iterator
    iterator erase(iterator from, iterator to)
    {
        for (iterator itr = from; itr != to;)
        {
            itr = erase(itr); //deletes current item and moves to next one
        }
        return to;
    }

    // function to insert something at a certain position
    iterator insert(iterator pos, const Object &x)
    {
        // find where the position is in the vector just incase reallocation is needed
        int offset = pos - begin();
        if (theSize == theCapacity)
        {
            reserve(2 * theCapacity + 1);
            pos = begin() + offset;
        }

        // shift everything to the right to insert x in its spot
        for (iterator i = end(); i != pos; --i)
        {
            *i = std::move(*(i - 1));
        }
        *pos = x;
        ++theSize;
        return pos;
    }
};

//! NOT NEEDED, THIS IS JUST SO YOU CAN SEE THE FUNCTIONS AT WORK
void printVectorState(const Vector<int> &v, const std::string &label)
{
    std::cout << "\n[" << label << "] Size: " << v.size() << " | Elements: ";
    if (v.empty())
    {
        std::cout << "(Empty)";
    }
    else
    {
        //  This implicitly tests your begin(), end(), and iterator typedefs!
        for (auto itr = v.begin(); itr != v.end(); ++itr)
        {
            std::cout << *itr << " ";
        }
    }
    std::cout << "\n--------------------------------------------------\n";
}

int main()
{
    // test main to be run, run ./run in terminal
    //  --- PHASE 1: Construction & Insertion ---
    Vector<int> v;

    std::cout << "\n>> Pushing back 5 elements (Testing reserve scaling)...";
    for (int i = 10; i <= 50; i += 10)
    {
        v.push_back(i); // Inserts 10, 20, 30, 40, 50
    }
    printVectorState(v, "After push_back");

    // --- PHASE 2: Access & Safety ---
    std::cout << "\n>> Testing Access Pipelines...";
    std::cout << "\nRaw Array Access (v[2]): " << v[2];  // Should be 30
    std::cout << "\nSafe Access (v.at(4)): " << v.at(4); // Should be 50
    std::cout << "\nBack Element (v.back()): " << v.back() << "\n";

    // --- PHASE 3: The Iterator Abstraction (Insertion) ---
    std::cout << "\n>> Testing Iterator-Based Insertion...";
    // Insert '99' at index 2 (between 20 and 30)
    Vector<int>::iterator pos = v.begin() + 2;
    v.insert(pos, 99);
    printVectorState(v, "After insert(99)");

    // --- PHASE 4: The Iterator Abstraction (Erasure) ---
    std::cout << "\n>> Testing Single Erasure...";
    // Erase the '20' (which is now at index 1)
    v.erase(v.begin() + 1);
    printVectorState(v, "After erase(index 1)");

    std::cout << "Cannot test range erasure because notes range erasure is incorrect for vectors but unneeded";

    // --- PHASE 5: The Big Five (Copy & Move) ---
    std::cout << "\n>> Testing Deep Copy Construction...";
    Vector<int> vCopy = v; // Triggers Copy Constructor
    vCopy.push_back(1000); // Modify copy to prove independence

    std::cout << "\nOriginal Vector:";
    printVectorState(v, "Original");
    std::cout << "Copied Vector (Modified):";
    printVectorState(vCopy, "Deep Copy");

    // --- PHASE 6: Annihilation ---
    std::cout << "\n>> Testing State Wiping...";
    v.clear();
    printVectorState(v, "After clear()");
    
    return 0;
}