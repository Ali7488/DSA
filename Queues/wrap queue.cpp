#include <stdexcept>
#include <algorithm> // For std::move, std::swap

template <typename Object>
class ArrayQueue
{
private:
    Object *objects;
    int theCapacity;
    int theSize;
    int front;
    int back;

    void reserve(int newCapacity)
    {
        if (theCapacity < theSize)
        {
            return;
        }

        Object *newArray = new Object[newCapacity];
        for (int k = 0; k < theSize; k++)
        {
            int actualIndex = (front+k)%theCapacity;
            newArray[k] = std::move(objects[actualIndex]);
        }
        theCapacity = newCapacity;
        std::swap(objects, newArray);
        delete[] newArray;
        newArray = nullptr;
    }

public:
    // Core Constructor
    explicit ArrayQueue(int initialCapacity = 10)
        : theCapacity(initialCapacity), theSize(0), front(0), back(0)
    {
        objects = new Object[theCapacity];
    }

    // Destructor
    ~ArrayQueue()
    {
        delete[] objects;
    }

    // Core Modifiers
    void enqueue(const Object &x)
    {
        if (full())
        {
            reserve(2 * theCapacity + 1);
        }
        objects[back] = x;
        back = (back + 1) % theCapacity;
        ++theSize;
    }
    void enqueue(Object &&x)
    {
        if (full())
        {
            reserve(2 * theCapacity + 1);
        }
        objects[back] = std::move(x);
        back = (back + 1) % theCapacity;
        ++theSize;
    }
    Object &dequeue()
    {
        if (empty())
        {
            throw std::underflow_error("empty");
        }
        Object popped = getFront();
        front = (front + 1) % theCapacity;
        --theSize;
        return popped;
    }

    // Accessors
    Object &getFront()
    {
        return objects[front];
    }
    const Object &getFront() const
    {
        return objects[front];
    }

    // State
    bool empty() const
    {
        return theSize == 0;
    }
    bool full() const
    {
        return theSize == theCapacity;
    }
    int size() const
    {
        return theSize;
    }
    void clear()
    {
        theSize = 0;
        front = 0;
        back = 0;
    }
};