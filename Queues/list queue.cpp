#include "../Doubly Linked List/list.cpp"
#include <stdexcept>
#include <algorithm> // For std::move

template <typename Object>
class Queue
{
private:
    List<Object> container;

public:
    // Default constructor is implicitly generated and calls List's init()

    // Core Modifiers
    void enqueue(const Object &x)
    {
        container.push_back(x);
    }
    void enqueue(Object &&x)
    {
        container.push_back(std::move(x));
    }
    void dequeue()
    {
        Object popped = std::move(container.front());
        container.pop_front();
        return popped;
    }

    // Accessors
    Object &front()
    {
        return container.front();
    }
    const Object &front() const
    {
        return container.front();
    }
    Object &back()
    {
        return container.back();
    }
    const Object &back() const
    {
        return container.back();
    }

    // State
    bool empty() const { return container.empty(); }
    int size() const { return container.size(); }
    void clear() { container.clear(); }
};