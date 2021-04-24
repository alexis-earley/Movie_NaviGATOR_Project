//PQ implementation
#include <iostream>
#include <vector>
using namespace std;

// Data structure to store min min-heap node
struct PriorityQueue
{
private:
    // vector to store heap elements
    vector<int> min;
    vector<int> max;

    // Recursive heapify-down algorithm.
    // The node at index `i` and its two direct children
    // violates the heap property
    void heapify_down_max(int i)
    {
        // get left and right child of node at index `i`
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        int largest = i;

        // compare `max[i]` with its left and right child
        // and find the largest value
        if (left < max.size() && max[left] > max[i])
        {
            largest = left;
        }

        if (right < max.size() && max[right] > max[largest])
        {
            largest = right;
        }

        // swap with max child having greater value and
        // call heapify-down on the child
        if (largest != i)
        {
            swap(max[i], max[largest]);
            heapify_down_max(largest);
        }
    }

    // Recursive heapify-up algorithm
    void heapify_up_max(int i)
    {
        // check if the node at index `i` and its parent violate the heap property
        if (i && max[(i - 1) / 2] < max[i])
        {
            // swap the two if heap property is violated
            swap(max[i], max[(i - 1) / 2]);

            // call heapify-up on the parent
            heapify_up_max((i - 1) / 2);
        }
    }

    // Recursive heapify-down algorithm.
    // The node at index `i` and its two direct children
    // violates the heap property
    void heapify_down_min(int i)
    {
        // get left and right child of node at index `i`
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        int smallest = i;

        // compare `min[i]` with its left and right child
        // and find the smallest value
        if (left < min.size() && min[left] < min[i])
        {
            smallest = left;
        }

        if (right < min.size() && min[right] < min[smallest])
        {
            smallest = right;
        }

        // swap with min child having lesser value and
        // call heapify-down on the child
        if (smallest != i)
        {
            swap(min[i], min[smallest]);
            heapify_down_min(smallest);
        }
    }

    // Recursive heapify-up algorithm
    void heapify_up_min(int i)
    {
        // check if the node at index `i` and its parent violate the heap property
        if (i && min[(i - 1) / 2] > min[i])
        {
            // swap the two if heap property is violated
            swap(min[i], min[(i - 1) / 2]);

            // call heapify-up on the parent
            heapify_up_min((i - 1) / 2);
        }
    }

public:
    void buildMin(vector<int> input)
    {
        for (int i : input)
        {
            if (min.size() > 10 && i < top_min())
            {
                continue;
            }
            push_min(i);
            if (min.size() > 10)
            {
                pop_min();
            };
        }
        buildMax();
    }

    void buildMax()
    {
        vector<int> copy = min;
        for (int i: copy)
        {
            push_max(top_min());
            pop_min();
        }
    }

    // insert key into the heap
    void push_min(int key)
    {
        // insert min new element at the end of the vector
        min.push_back(key);

        // get element index and call heapify-up procedure
        int index = min.size() - 1;
        heapify_up_min(index);
    }

    // Function to remove an element with the lowest priority (present at the root)
    void pop_min()
    {
        // replace the root of the heap with the last element
        // of the vector
        min[0] = min.back();
        min.pop_back();

        // call heapify-down on the root node
        heapify_down_min(0);
    }

    // Function to return an element with the lowest priority (present at the root)
    int top_min()
    {
        // otherwise, return the top_min (first) element
        return min.at(0); // or return min[0];
    }

    void show_max()
    {
        while (max.size() > 0)
        {
            cout << top_max() << endl;
            pop_max();
        }
    }

    // insert key into the heap
    void push_max(int key)
    {
        // insert max new element at the end of the vector
        max.push_back(key);

        // get element index and call heapify-up procedure
        int index = max.size() - 1;
        heapify_up_max(index);
    }

    // Function to remove an element with the highest priority (present at the root)
    void pop_max()
    {
        // replace the root of the heap with the last element
        // of the vector
        max[0] = max.back();
        max.pop_back();

        // call heapify-down on the root node
        heapify_down_max(0);
    }

    // Function to return an element with the highest priority (present at the root)
    int top_max()
    {
        // otherwise, return the top (first) element
        return max.at(0); // or return max[0];
    }
    
};

// Min Heap implementation in C++
int main()
{
    PriorityQueue pq;
    vector<int> random;
    // Note: The element's value decides priority
    for (int i = 0; i < 100; i++)
    {
        int b = rand() % 100 + 1;
        random.push_back(b);
    }

    pq.buildMin(random);
    pq.show_max();

    return 0;
}