#include <iostream>
#include <vector>
using namespace std;
struct Movie
{
    string id;
    int year;
    int duration;
    double rating;
    double match = 0;
    string title;
    string genre;
    string country;
    string language;
    vector<string> actors;

    Movie(int y, int d, double r, string i, string t, string g, string c, string l, vector<string> a)
    {

        year = y;
        duration = d;
        rating = r;
        id = i;
        title = t;
        genre = g;
        country = c;
        language = l;
        actors = a;
    }
};

//min PQ implementation
class PriorityQueue
{
private:
    vector<int> thePQ;
    int size = 0;

public:
    PriorityQueue() {}

    //Code referenced to GeeksforGeeks
    int readPQ()
    {
        string x;
        cin >> x;
        while (x.compare("-1") != 0)
        {
            //thePQ.push_back(stoi(x));
            size++;
            PQInsert(stoi(x));            
            cin >> x;
        }
        return size;
    }

    void PQPrint()
    {
        for (int i = 0; i < size; i++)
        {
            cout << thePQ[i] << " ";
        }
    }

    void PQify(int &size, int index)
    {
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < size && thePQ[left] < thePQ[index] && thePQ[left] < thePQ[right])
        {
            largest = left;
            swap(thePQ[index], thePQ[largest]);
            PQify(size, largest);
        }
        else if (right < size && thePQ[right] < thePQ[largest] && thePQ[right] < thePQ[left])
        {
            largest = right;
            swap(thePQ[index], thePQ[largest]);
            PQify(size, largest);
        }
    }

    // Recursive heapify-up algorithm
    void heapify_up(int i)
    {
        // check if the node at index `i` and its parent violate the heap property
        if (i && thePQ[((i - 1) / 2)] < thePQ[i])
        {
            // swap the two if heap property is violated
            swap(thePQ[i], thePQ[((i - 1) / 2)]);
 
            // call heapify-up on the parent
            heapify_up((i - 1) / 2);
        }
    }

    void PQRemove()
    {
        thePQ[0] = thePQ.back();
        thePQ.pop_back();
        size--;
        PQify(size, 0);
    }

    void PQInsert(int value)
    {
        thePQ.push_back(value);
        int index = size - 1;
        heapify_up(index);
    }
};

int main()
{
    PriorityQueue Q;
    Q.readPQ();
    Q.PQPrint();
}