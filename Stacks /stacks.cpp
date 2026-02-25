#include "../Vectors/Vector.cpp"
#include <stdexcept>

template <typename Object>
class Stack
{
private:
    Vector<Object> container;

public:
    // Default constructor implicitly calls Vector's constructor

    // Core Modifiers
    void push(const Object &x)
    {
        container.push_back(x);
    }
    void push(Object &&x)
    {
        container.push_back(std::move(x));
    }
    void pop()
    {
        if (empty())
        {
            throw std::underflow_error("Stack is empty"); //dont worry about this
        }
        container.pop_back();
    }

    // Accessors
    Object &top(){
        if (empty())
        {
            throw std::underflow_error("Stack is empty"); // dont worry about this
        }
        return container.back();
    }
    const Object &top() const{
        if (empty())
        {
            throw std::underflow_error("Stack is empty"); // dont worry about this
        }
        return container.back();
    }

    // State
    bool empty() const
    {
        return container.empty();
    }
    int size() const{
        return container.size();
    }
    void clear(){
        container.clear();
    }
};