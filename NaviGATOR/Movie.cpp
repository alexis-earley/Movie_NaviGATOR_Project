#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <queue>
#include <set>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <sstream>

using namespace std;

class Movie
{
public:
    string title;
    string original_title;
    int year;
    string date_published;
    unordered_set<string> genres;
    int duration;
    unordered_set<string> countries;
    unordered_set<string> languages;
    unordered_set<string> directors;
    unordered_set<string> writers;
    string production_company;
    unordered_set<string> actors;
    string description;
    float avg_vote;
    int votes;
    float match;
    double avg_votes[3][5];
    double num_votes[3][5];

    Movie()
    { //default constructor
        title = "";
        original_title = "";
        year = -1;
        date_published = "";
        duration = -1;
        production_company = "";
        description = "";
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                avg_votes[i][j] = -1;
            }
        }
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                num_votes[i][j] = -1;
            }
        }
    }
};
unordered_map<string, Movie *> movies;

struct CompareMoviesMax
{
    bool operator()(const string m1, const string m2)
    {
        return movies[m1]->match < movies[m2]->match;
    }
};

struct CompareMoviesMin
{
    bool operator()(const string m1, const string m2)
    {
        return movies[m1]->match > movies[m2]->match;
    }
};
class MovieNaviGATOR
{
private:
    priority_queue<string, vector<string>, CompareMoviesMax> listMax;
    priority_queue<string, vector<string>, CompareMoviesMin> listMin;
    int intConv(string &input);
    float floatConv(string &input);

    string testQuotes(string &input);

    void makeMatch(string id, unordered_set<string> l, unordered_set<string> g, int d, double r);

    void maxHeapCreation();

    void showpq();

public:
    MovieNaviGATOR();
    ~MovieNaviGATOR();
    void minHeapCreation(int num, unordered_set<string> l, unordered_set<string> g, int d, double r);
    void printMovieInfo();
    unordered_set<string> setConv(string input);
};

int MovieNaviGATOR::intConv(string &input)
{ //converts string to integer
    int result;
    try
    {
        result = stoi(input);
    }
    catch (exception &err)
    {
        result = -1;
    }
    return result;
}

float MovieNaviGATOR::floatConv(string &input)
{
    float result;
    try
    {
        result = stof(input);
    }
    catch (exception &err)
    {
        result = -1;
    }
    return result;
}

unordered_set<string> MovieNaviGATOR::setConv(string input)
{ //converts string to set
    unordered_set<string> result;
    string currString = "";
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] != ',')
        {
            currString += input[i];
        }
        else
        {
            result.insert(currString);
            currString = "";
            i++;
        }
    }
    result.insert(currString);
    return result;
}

string MovieNaviGATOR::testQuotes(string &input)
{ //removes quotes, if present
    int lastChar = input.size() - 1;
    string result;
    if ((input[0] == '\"') && (input[lastChar] == '\"'))
    {
        return input.substr(1, lastChar - 1);
    }
    return input;
}

MovieNaviGATOR::MovieNaviGATOR()
{
    string line;
    vector<string> myString;
    ifstream inFile;
    string filename = "C:\\Users\\Pandu\\source\\repos\\NaviGATOR\\NaviGATOR\\imdb_movies.tsv";
    inFile.open(filename);
    int i = 0;
    if (inFile.is_open())
    {
        //remove header
        getline(inFile, line);
        getline(inFile, line);

        string id; //holds movie ID, which the map is organized by

        while (getline(inFile, line))
        {
            Movie *currMovie = new Movie();
            istringstream ss(line);

            string data;
            getline(ss, data, '\t'); //gets the ID
            id = data;
            getline(ss, data, '\t'); //gets the title
            currMovie->title = testQuotes(data);
            getline(ss, data, '\t'); //gets the original title
            currMovie->original_title = testQuotes(data);
            getline(ss, data, '\t'); //gets the year
            currMovie->year = intConv(data);
            getline(ss, data, '\t'); //gets the date published
            currMovie->date_published = data;
            getline(ss, data, '\t'); //gets the genres
            currMovie->genres = setConv(data);
            getline(ss, data, '\t'); //gets the duration
            currMovie->duration = intConv(data);
            getline(ss, data, '\t'); //gets the countries
            currMovie->countries = setConv(testQuotes(data));
            getline(ss, data, '\t'); //gets the languages
            currMovie->languages = setConv(testQuotes(data));
            getline(ss, data, '\t'); //gets the directors
            currMovie->directors = setConv(testQuotes(data));
            getline(ss, data, '\t'); //gets the writers
            currMovie->writers = setConv(testQuotes(data));
            getline(ss, data, '\t'); //gets the production company
            currMovie->production_company = data;
            getline(ss, data, '\t'); //gets the actors
            currMovie->actors = setConv(testQuotes(data));
            getline(ss, data, '\t'); //gets the description
            currMovie->description = testQuotes(data);
            getline(ss, data, '\t'); //gets the average vote
            float result;
            try
            {
                result = stof(data);
            }
            catch (exception &err)
            {
                result = -1;
            }
            currMovie->avg_vote = result;
            getline(ss, data, '\t'); //gets the number of votes
            currMovie->votes = intConv(data);

            movies[id] = currMovie;
        }
    }
    inFile.close();
}

MovieNaviGATOR::~MovieNaviGATOR()
{
    auto it = movies.begin();
    while (it != movies.end())
    {
        delete it->second;
        it++;
    }
    movies.clear();
}

void MovieNaviGATOR::makeMatch(string id, unordered_set<string> l, unordered_set<string> g, int d, double r)
{
    Movie *temp = movies[id];
    if (temp->duration == d)
        temp->match++;
    if (temp->avg_vote == r)
        temp->match++;
}

void MovieNaviGATOR::minHeapCreation(int num, unordered_set<string> l, unordered_set<string> g, int d, double r)
{
    for (auto it = movies.begin(); it != movies.end(); ++it)
    {
        if (num > 0)
        {
            makeMatch(it->first, l, g, d, r);
            if (listMin.size() > num && (it->second)->match < movies[listMin.top()]->match)
            {
                continue;
            }

            listMin.emplace(it->first);
            if (listMin.size() > num)
            {
                listMin.pop();
            }
            it++;
        }
    }
    maxHeapCreation();
}

void MovieNaviGATOR::maxHeapCreation()
{
    while (!listMin.empty())
    {
        listMax.emplace(listMin.top());
        listMin.pop();
    }
    showpq();
}

void MovieNaviGATOR::printMovieInfo()
{
}

void MovieNaviGATOR::showpq()
{
    if (listMax.empty())
    {
        cout << "No recommendations made!" << endl;
        return;
    }
    int n = 1;
    while (!listMax.empty())
    {
        cout << "Movie #" << n << ": " << movies[listMax.top()]->title << " " << movies[listMax.top()]->match << endl;
        listMax.pop();
        n++;
    }
}

int main()
{
    MovieNaviGATOR session;

    cout << "Welcome to MovieNAVIGATOR!" << endl;

    int n = 1;
    int duration;
    double rating;
    int number;

    unordered_set<string> genres;
    unordered_set<string> actors;
    unordered_set<string> languages;
    unordered_set<string> countries;
    unordered_set<string> directors;
    unordered_set<string> writers;  

    while (n < 10 && n > 0)
    {
        string data;
        cout << "Menu:" << endl;
        cout << "1. Select preferred languages (e.g. English)" << endl;
        cout << "2. Select preferred genres (e.g. Action)" << endl;
        cout << "3. Select preferred duration in minutes (e.g. 120)" << endl;
        cout << "4. Select preferred actors/actresses (e.g. George Clooney)" << endl;
        cout << "5. Select preferred ranking (e.g. 7.5)" << endl;
        cout << "6. Select preferred countries (e.g. India)" << endl;
        cout << "7. Select preferred directors (e.g. Steven Spielberg)" << endl;
        cout << "8. Select preferred writers (e.g. Quentin Tarantino)" << endl;
        cout << "9. Recommend movies" << endl;
        cout << "10. Exit" << endl;
        cout << "Please enter a menu option (e.g. 3)" << endl;
        cin >> n;
        if (n == 1)
        {
            cin.ignore();
            cout << "Enter preferred language (e.g. English)" << endl;
            getline(cin, data);
            istringstream ss(data);
            string language;
            while (getline(ss, language, ','))
                {
                    if(language[0] == ' ')
                        language.erase(0,1);
                    languages.insert(language);
                }
        }
        else if (n == 2)
        {
            cin.ignore();
            cout << "Enter preferred genre (e.g. Action)" << endl;
            getline(cin, data);
            istringstream ss(data);
            string genre;
            while (getline(ss, genre, ','))
                {
                    if(genre[0] == ' ')
                        genre.erase(0,1);
                    genres.insert(genre);
                }
        }
        else if (n == 3)
        {
            cout << "Range of durations (in minutes) in database: 40 - 800" << endl;
            cout << "Enter preferred duration in minutes (e.g. 120)" << endl;
            cin >> duration;
        }
        else if (n == 4)
        {
            cin.ignore();
            cout << "Enter preferred main actor/actress (e.g. George Clooney)" << endl;
            getline(cin, data);
            istringstream ss(data);
            string actor;
            while (getline(ss, actor, ','))
                if(actor[0] == ' ')
                        actor.erase(0,1);
                    actors.insert(actor);
        }
        else if (n == 5)
        {
            cout << "Range of rankings in database: 1 - 10" << endl;
            cout << "Enter preferred ranking (e.g. 7.5)" << endl;
            cin >> rating;
        }
        else if (n == 6)
        {
            cin.ignore();
            cout << "Enter preferred countries (e.g. India)" << endl;
            getline(cin, data);
            istringstream ss(data);
            string country;
            while (getline(ss, country, ','))
                if(country[0] == ' ')
                        country.erase(0,1);
                    countries.insert(country);
        }
        else if (n == 7)
        {
            cin.ignore();
            cout << "Enter preferred directors (e.g. Steven Spielberg)" << endl;
            getline(cin, data);
            istringstream ss(data);
            string director;
            while (getline(ss, director, ','))
                if(director[0] == ' ')
                        director.erase(0,1);
                    countries.insert(director);
        }
        else if (n == 8)
        {
            cin.ignore();
            cout << "Enter preferred writers (e.g. Quentin Tarantino)" << endl;
            getline(cin, data);
            istringstream ss(data);
            string writer;
            while (getline(ss, writer, ','))
                if(writer[0] == ' ')
                        writer.erase(0,1);
                    writers.insert(writer);
        }
        else if (n == 9)
        {
            cout << "Enter number of recommendations" << endl;
            cin >> number;
            cout << "Your top " << number << " movies are:" << endl;
            session.minHeapCreation(number, languages, genres, duration, rating);
            cout << "Would you like more information about any of these movies? (Y/N)" << endl;
            cin >> data;
            if (data == "Y")
            {
                cout << "Which of the above movies would you like more information on?" << endl;
                cin >> data;
                session.printMovieInfo();
            }
        }
        cout << endl;
    }
}