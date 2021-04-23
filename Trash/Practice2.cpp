#include <iostream>
#include <queue>
#include <stdlib.h>

using namespace std;

void showpq(priority_queue<int> gq)
{
    priority_queue<int> g = gq;
    while (!g.empty())
    {
        cout << '\t' << g.top();
        g.pop();
    }
    cout << '\n';
}

int main()
{
    //srand(time(NULL));

    priority_queue<int, vector<int>, greater<int>> gquiz;
    for (int i = 0; i < 100; i++)
    {
        int n = rand() % 100 + 1;
        cout << n << endl;
        if (gquiz.size() > 10 && n < gquiz.top())
        {
            continue;
        }
        gquiz.push(n);
        if (gquiz.size() > 10)
        {
            gquiz.pop();
        };
    }
    priority_queue<int> g;
    while (!gquiz.empty())
    {
        g.push(gquiz.top());
        gquiz.pop();
    }
    cout << "The priority queue gquiz is : ";
    showpq(g);

    cout << "\ng.size() : " << g.size();
    cout << "\ng.top() : " << g.top();

    cout << "\ngquiz.pop() : ";
    gquiz.pop();
    showpq(g);

    return 0;
}