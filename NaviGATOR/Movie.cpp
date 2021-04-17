#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <queue>
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

struct CompareMovies
{
    bool operator()(const Movie *m1, const Movie *m2)
    {
        return m1->match < m2->match;
    }
};

class MovieNaviGATOR
{
private:
    priority_queue<Movie *, vector<Movie *>, CompareMovies> list;

public:
    int listCapacity = 0;

    void makeMatch(Movie *temp, string l, string g, int d, double r)
    {
        if (temp->language == l)
            temp->match++;
        if (temp->genre == g)
            temp->match++;
        if ((temp->duration < 1.10 * d) && (temp->duration > 0.90 * d))
            temp->match++;
        if ((temp->rating < 1.10 * r) && (temp->rating > 0.90 * r))
            temp->match++;

        temp->match = temp->match / 4;
    }

    void showpq(int recomendations)
    {
        priority_queue<Movie *, vector<Movie *>, CompareMovies> templist = list;
        if (templist.empty())
        {
            cout << "No recomendations made!" << endl;
            return;
        }
        int underflow = recomendations - templist.size();
        int n = 0;
        while (!templist.empty() && recomendations > 0)
        {
            cout << n << ". " << templist.top()->title << " " << templist.top()->match << endl;
            templist.pop();
            recomendations--;
            n++;
        }
        while (abs(underflow) > 0)
        {
            cout << n << ". Recommendation not made" << endl;
            underflow++;
            n++;
        }
    }

    void addMovie(Movie *temp, string l, string g, int d, double r)
    {
        makeMatch(temp, l, g, d, r);
        list.emplace(temp);
        listCapacity++;
    }
};

int main()
{
    MovieNaviGATOR session;
    //read movies in from csv
    //store movies in both unordered map and unordered set
    //ask user for preferences
    //

    cout << "Welcome to MovieNAVIGATOR!" << endl;

    int n = 1;
    string language;
    string genre;
    int duration;
    string actor;
    double rating;
    int number;

    while (n < 7 && n > 0)
    {
        cout << "Menu:" << endl;
        cout << "1. Select preferred language (e.g. English)" << endl;
        cout << "2. Select preferred  genre (e.g. Action)" << endl;
        cout << "3. Select preferred duration in minutes (e.g. 120)" << endl;
        cout << "4. Select preferred main actor/actress (e.g. George Clooney)" << endl;
        cout << "5. Select preferred ranking (e.g. 7.5)" << endl;
        cout << "6. Recommend movies" << endl;
        cout << "7. Exit" << endl;
        cout << "Please enter a menu option (e.g. 3)" << endl;
        cin >> n;
        switch (n)
        {
        case 1:
            cout << "Languages in database: English, French, Spanish, Japanese, Italian, Hindi, German, Turkish, Russian, Korean, Portuguese, Malayalam, Tamil, Mandarin, Telugu, Cantonese, Persian, Swedish, Polish, Greek, Arabic, Danish, Dutch, Bengali, Finnish, Czech, Hungarian, Norwegian, Romanian, Thai" << endl;
            cout << "Enter preferred language (e.g. English)" << endl;
            cin >> language;
            break;
        case 2:
            cout << "Genres in database: Action, Adventure, Biography, Comedy, Crime, Drama, Family, Fantasy, History, Horror, Music, Musical, Mystery, Romance, Sci-Fi, Thriller, War, Western" << endl;
            cout << "Enter preferred genre (e.g. Action)" << endl;
            cin >> genre;
            break;
        case 3:
            cout << "Range of durations (in minutes) in database: 40 - 800" << endl;
            cout << "Enter preferred duration in minutes (e.g. 120)" << endl;
            cin >> duration;
            break;
        case 4:
            cout << "Enter preferred main actor/actress (e.g. George Clooney)" << endl;
            cin >> actor;
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
            session.showpq(number);
            break;
        default:
            break;
        }
        cout << endl;
    }
}