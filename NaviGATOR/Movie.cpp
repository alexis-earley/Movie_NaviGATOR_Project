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
{ //holds the data for each node in the tree
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

    Movie()
    { //default constructor
        title = "";
        original_title = "";
        year = -1;
        date_published = "";
        duration = -1;
        production_company = "";
        description = "";
        avg_vote = -1.0;
        votes = -1;
        match = -1.0;
    }
};

class MovieNaviGATOR
{
private:
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

    priority_queue<string, vector<string>, CompareMoviesMax> listMax;
    priority_queue<string, vector<string>, CompareMoviesMin> listMin;
    unordered_map<string, Movie *> movies;
    int intConv(string &input);
    unordered_set<string> setConv(string input);
    string testQuotes(string &input);

    void makeMatch(string id, vector<string> l, vector<string> g, int d, double r);

    void maxHeapCreation();

    void showpq();

public:
    MovieNaviGATOR();
    ~MovieNaviGATOR();
    void minHeapCreation(int num, vector<string> l, vector<string> g, int d, double r);
    void printMovieInfo();
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

unordered_set<string> MovieNaviGATOR::setConv(string input)
{ //converts set to integer
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
    inFile.open("movies.txt");
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
    cout << movies["tt0000574"]->duration << endl;
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

void MovieNaviGATOR::makeMatch(string id, vector<string> l, vector<string> g, int d, double r)
{
    movies[id]->match = rand();
}

void MovieNaviGATOR::minHeapCreation(int num, vector<string> l, vector<string> g, int d, double r)
{
    //while (it != movies.end() && num > 0)
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
        cout << "Movie #" << n << ": " << movies[listMax.top()]->title << endl;
        listMax.pop();
        n++;
    }
}

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