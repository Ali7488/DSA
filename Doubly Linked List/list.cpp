#include <iostream>
#include <algorithm> // For std::swap and std::move

template <typename Object>
class List
{
private:
    // Node struct, We do not need to know how to implement this
    struct Node
    {
        Object data;
        Node *prev;
        Node *next;

        Node(const Object &d = Object{}, Node *p = nullptr, Node *n = nullptr)
            : data(d), prev(p), next(n) {}

        Node(Object &&d, Node *p = nullptr, Node *n = nullptr)
            : data(std::move(d)), prev(p), next(n) {}
    };

    int theSize;
    Node *head; // Sentinel Node
    Node *tail; // Sentinel Node

public:
    void init()
    {
        theSize = 0;
        head = new Node;
        tail = new Node;

        // Initializing an empty list, where the 2 sentinel nodes point to each other
        head->next = tail;
        tail->prev = head;
    }
    class const_iterator
    {
    protected:
        Node *current;

        Object &retrieve() const
        {
            return current->data;
        }

        const_iterator(Node *p) : current{p} {}

        friend class List<Object>;

    public:
        const_iterator() : current{nullptr} {}

        const Object &operator*() const
        {
            return retrieve();
        }

        const_iterator &operator++()
        {
            current = current->next;
            return *this;
        }

        const_iterator &operator--()
        {
            current = current->prev;
            return *this;
        }

        bool operator==(const const_iterator &rhs) const
        {
            return current == rhs.current;
        }
        bool operator!=(const const_iterator &rhs) const
        {
            return current != rhs.current;
        }
    };

    class iterator : public const_iterator
    {
    protected:
        Node *current;
        Object &retrieve() const
        {
            return current->data;
        }

    public:
        iterator(Node *node) : current(node) {}

        Object &operator*()
        {
            return retrieve();
        }

        iterator &operator++()
        {
            this->current = this->current->next;
            return *this;
        }

        iterator operator++(int)
        {
            iterator old = *this;
            ++(*this);
            return old;
        }

        iterator &operator--()
        {
            current = current->prev;
            return *this;
        }

        friend class List<Object>;
    };

    // Default constructor
    List()
    {
        init();
    }

    const_iterator begin() const
    {
        return {head->next};
    }

    iterator begin()
    {
        return {head->next};
    }

    const_iterator end() const
    {
        return {tail};
    }

    iterator end()
    {
        return {tail};
    }

    // Return size
    int size() const
    {
        return theSize;
    }

    // check if list is empty
    bool empty() const
    {
        return size() == 0;
    }

    // Empty the list
    void clear()
    {
        while (!empty())
        {
            pop_front();
        }
    }

    Object &front()
    {
        return *begin();
    }

    const Object &front() const
    {
        return *begin();
    }

    Object &back()
    {
        return *--end();
    }

    const Object &back() const
    {
        return *--end();
    }

    void push_front(const Object &x)
    {
        insert(begin(), x);
    }

    void push_back(const Object &x)
    {
        insert(end(), x);
    }

    void pop_front()
    {
        erase(begin());
    }

    void pop_back()
    {
        erase(--end());
    }

    iterator insert(iterator itr, const Object &x)
    {
        Node *p = itr.current;
        ++theSize;
        Node *newNode = new Node{x, p->prev, p};
        p->prev->next = newNode;
        p->prev = newNode;
        return iterator{newNode};
    }

    iterator erase(iterator itr)
    {
        Node *p = itr.current;
        iterator retVal{p->next};
        p->prev->next = p->next;
        p->next->prev = p->prev;
        delete p;
        --theSize;
        return retVal;
    }

    iterator erase(iterator from, iterator to)
    {
        for (iterator itr = from; itr != to;)
        {
            itr = erase(itr);
        }
        return to;
    }
};

int main()
{
    std::cout << "--- 1. Testing Setup & Modifiers ---" << std::endl;
    List<int> myList;
    std::cout << "Initial size: " << myList.size() << " (Expected 0)\n";
    std::cout << "Is empty? " << (myList.empty() ? "Yes" : "No") << " (Expected Yes)\n";

    // Adding data
    myList.push_back(10);
    myList.push_back(20);
    myList.push_front(5);
    myList.push_back(30);

    std::cout << "\n--- 2. Testing Accessors ---" << std::endl;
    std::cout << "Front element: " << myList.front() << " (Expected 5)\n";
    std::cout << "Back element: " << myList.back() << " (Expected 30)\n";
    std::cout << "Current size: " << myList.size() << " (Expected 4)\n";

    std::cout << "\n--- 3. Testing Iterators (Printing List) ---" << std::endl;
    std::cout << "List contents: ";
    for (List<int>::iterator it = myList.begin(); it != myList.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << "\n(Expected: 5 10 20 30)\n";

    std::cout << "\n--- 4. Testing Erase & Pop ---" << std::endl;
    myList.pop_front();
    myList.pop_back();
    std::cout << "After popping front and back, size: " << myList.size() << " (Expected 2)\n";
    std::cout << "New Front: " << myList.front() << " (Expected 10)\n";
    std::cout << "New Back: " << myList.back() << " (Expected 20)\n";

    std::cout << "\n--- 5. Testing Clear ---" << std::endl;
    myList.clear();
    std::cout << "After clear, size: " << myList.size() << " (Expected 0)\n";
    std::cout << "Is empty? " << (myList.empty() ? "Yes" : "No") << " (Expected Yes)\n";

    std::cout << "\nAll core systems functional. The List is alive." << std::endl;
    return 0;
}