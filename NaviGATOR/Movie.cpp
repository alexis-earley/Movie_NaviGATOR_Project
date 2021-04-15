#include <string>
#include <vector>
#include <iostream>
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
    int listCapacity = 0;

public:
    
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

    void showpq(int reccomendations)
    {
        priority_queue<Movie *, vector<Movie *>, CompareMovies> templist = list;
        while (!templist.empty() && reccomendations > 0)
        {
            cout << '\t' << templist.top()->title << " " << templist.top()->match << endl;
            templist.pop();
            reccomendations--;
        }
        cout << '\n';
    }

    void addMovie(Movie* temp)
    {
        if (listCapacity < INT_MAX)
            list.emplace(temp);
        else
            cout << "List is full!" << endl;
    }
};

int main()
{
    MovieNaviGATOR session;
    vector<string> actorlist = {"actorone", "actortwo", "actorthree"};
    Movie *foo = new Movie(1894, 120, 7.7, "idnumber", "movietitle1", "genrecategory", "countryorigin", "languagespoken", actorlist);
    Movie *fee = new Movie(1994, 150, 5.7, "idnumber", "movietitle2", "genrecategory", "countryorigin", "c", actorlist);
    session.makeMatch(foo, "languagespoken", "genrecategory", 135, 7.3);
    session.makeMatch(fee, "languagespoken", "genrecategory", 135, 7.3);
    session.addMovie(foo);
    session.addMovie(fee);

    cout << "Your top matches are : " << endl;
    session.showpq(3);
}