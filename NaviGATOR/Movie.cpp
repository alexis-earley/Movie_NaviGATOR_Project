#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <queue>
#include <set>
using namespace std;

set<string, greater<int>> movies;

struct CompareMoviesMax
{
    bool operator()(const string m1, const string m2)
    {
        return movies[m1] < movies[m2];
    }
};

struct CompareMoviesMin
{
    bool operator()(const string m1, const string m2)
    {
        return movies[m1] > movies[m2];
    }
};

class MovieNaviGATOR
{
private:
    priority_queue<string, vector<string>, CompareMoviesMax> listMax;
    priority_queue<string, vector<string>, CompareMoviesMin> listMin;

public:
    void minHeapCreation(int num, vector<string> l, vector<string> g, int d, double r)
    {
        set<string>::iterator it = movies.begin();
        while (it != movies.end() && num > 0)
        {
            if (listMin.size() > num && (*it) < listMin.top())
            {
                continue;
            }
            makeMatch(*it, l, g, d, r);
            listMin.emplace(*it);
            if (listMin.size() > num)
            {
                listMin.pop();
            }
            it++;
        }
        maxHeapCreation();
    }

    void makeMatch(string id, vector<string> l, vector<string> g, int d, double r)
    {
        movies[id]->match = rand();
    }

    void maxHeapCreation()
    {
        while (!listMin.empty())
        {
            listMax.emplace(listMin.top());
            listMin.pop();
        }
        showpq();
    }

    void printMovieInfo()
    {
    }

    void showpq()
    {
        if (listMax.empty())
        {
            cout << "No recommendations made!" << endl;
            return;
        }
        int n = 1;
        while (!listMax.empty())
        {
            cout << "Movie #" << n << ": " << movies[listMax.top()]->title << endl;
            listMax.pop();
            n++;
        }
    }
};

int main()
{
    MovieNaviGATOR session;

    cout << "Welcome to MovieNAVIGATOR!" << endl;

    int n = 1;
    string rawInput;
    int duration;
    double rating;
    int number;
    
    vector<string> actors;
    vector<string> genres;
    vector<string> languages;

    while (n < 7 && n > 0)
    {
        cout << "Menu:" << endl;
        cout << "1. Select preferred languages (e.g. English)" << endl;
        cout << "2. Select preferred genres (e.g. Action)" << endl;
        cout << "3. Select preferred duration in minutes (e.g. 120)" << endl;
        cout << "4. Select preferred actors/actresses (e.g. George Clooney)" << endl;
        cout << "5. Select preferred ranking (e.g. 7.5)" << endl;
        cout << "6. Recommend movies" << endl;
        cout << "7. Exit" << endl;
        cout << "Please enter a menu option (e.g. 3)" << endl;
        cin >> n;
        switch (n)
        {
        case 1:
            cout << "Enter preferred language (e.g. English)" << endl;
            while (getline(cin, rawInput, ','))
            {
                languages.push_back(rawInput);
            }
            break;
        case 2:
            cout << "Enter preferred genre (e.g. Action)" << endl;
            while (getline(cin, rawInput, ','))
            {
                genres.push_back(rawInput);
            }
            break;
        case 3:
            cout << "Range of durations (in minutes) in database: 40 - 800" << endl;
            cout << "Enter preferred duration in minutes (e.g. 120)" << endl;
            cin >> duration;
            break;
        case 4:
            cout << "Enter preferred main actor/actress (e.g. George Clooney)" << endl;
            while (getline(cin, rawInput, ','))
            {
                actors.push_back(rawInput);
            }
            break;
        case 5:
            cout << "Range of rankings in database: 1 - 10" << endl;
            cout << "Enter preferred ranking (e.g. 7.5)" << endl;
            cin >> rating;
            break;
        case 6:
            cout << "Enter number of recommendations" << endl;
            cin >> number;
            cout << "Your top " << number << " movies are:" << endl;
            session.minHeapCreation(number, languages, genres, duration, rating);
            cout << "Would you like more information about any of these movies? (Y/N)" << endl;
            cin >> rawInput;
            if (rawInput == "Y")
            {
                cout << "Which of the above movies would you like more information on?" << endl;
                cin >> rawInput;
                session.printMovieInfo();
            }
            break;
        default:
            break;
        }
        cout << endl;
    }
}