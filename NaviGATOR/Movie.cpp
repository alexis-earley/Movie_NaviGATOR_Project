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
    vector<string> actors;
    string description;
    float avg_vote;
    int votes;
    //pair<double, double> ratings[3][5];
    float match;
    double avg_votes[3][5] = {0};
    double num_votes[3][5] = {0};

    Movie()
    { //default constructor
        title = "";
        original_title = "";
        year = 0;
        date_published = "";
        duration = 0;
        production_company = "";
        description = "";
        //avg_vote = -1.0;
        //votes = -1;
        // for (int i = 0; i < 3; i++)
        // {
        //     for (int j = 0; j < 5; j++)
        //     {
        //         avg_votes[i][j] = 0;
        //     }
        // }
        // for (int i = 0; i < 3; i++)
        // {
        //     for (int j = 0; j < 5; j++)
        //     {
        //         num_votes[i][j] = 0;
        //     }
        // }
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
    unordered_set<string> setConv(string input);
    vector<string> vectorConv(string input);

    void makeMatch(string id, int d, double r);
    void maxHeapCreation();
    void showpq();
public:
    MovieNaviGATOR();
    ~MovieNaviGATOR();

    void minHeapCreation(int num, int d, double r);
    void printMovieInfo(string title);
};

int MovieNaviGATOR::intConv(string& input) { //converts string to integer
    int result;
    try {
        result = stoi(input);
    }
    catch(exception &err) {
        result = 0;
    }
    return result;
}

float MovieNaviGATOR::floatConv(string& input) {
    float result;
    try {
        result = stof(input);
    }
    catch(exception &err) {
        result = 0;
    }
    return result;
}

template <typename T>
T myMax(T x, T y)
{
    return (x > y)? x: y;
}

unordered_set<string> MovieNaviGATOR::setConv(string input) { //converts set to integer
    unordered_set<string> result;
    string currString = "";
    for (int i = 0; i < input.size(); i++) {
        if (input[i] != ',') {
            currString += input[i];
        }
       else {
            result.insert(currString);
            currString = "";
            i++;
        }
    }
    result.insert(currString);
    return result;
}

vector<string> MovieNaviGATOR::vectorConv(string input) { //converts set to integer
    vector<string> result;
    string currString = "";
    for (int i = 0; i < input.size(); i++) {
        if (input[i] != ',') {
            currString += input[i];
        }
        else {
            result.push_back(currString);
            currString = "";
            i++;
        }
    }
    result.push_back(currString);
    return result;
}

string MovieNaviGATOR::testQuotes(string& input) { //removes quotes, if present
    int lastChar = input.size() - 1;
    string result;
    if ((input[0] == '\"') && (input[lastChar] == '\"')) {
        return input.substr(1, lastChar - 1);
    }
    return input;
}

MovieNaviGATOR::MovieNaviGATOR()
{
    //cout << "constructor" << endl;
    string line;
    vector<string> myString;
    ifstream inFile;
    inFile.open("C:\\Users\\Pandu\\source\\repos\\NaviGATOR\\NaviGATOR\\imdb_movies.tsv");
    if (inFile.is_open()) {
        //remove header
        //cout << "movies opened" << endl;
        getline(inFile, line);
        getline(inFile, line);

        string id; //holds movie ID, which the map is organized by

        while (getline(inFile, line)) {
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
            currMovie->actors = vectorConv(testQuotes(data));
            getline(ss, data, '\t'); //gets the description
            currMovie->description = testQuotes(data);
            //getline(ss, data, '\t'); //gets the average vote
            //currMovie->avg_vote = floatConv(data);
            //getline(ss, data, '\t'); //gets the number of votes
            //currMovie->votes = intConv(data);

            movies[id] = currMovie;

        }
    }

    inFile.close();
    inFile.open("C:\\Users\\Pandu\\source\\repos\\NaviGATOR\\NaviGATOR\\imdb_ratings.tsv");

    if (inFile.is_open()) {
        cout << "ratings open" << endl;
        getline(inFile, line);
        getline(inFile, line);

        string id; //holds movie ID, which the map is organized by

        while (getline(inFile, line)) {
            istringstream ss(line);
            string id;
            string data;
            string trash;

            getline(ss, id, '\t');
            getline(ss, trash, '\t');
            getline(ss, data, '\t');
            movies[id]->num_votes[0][0] = floatConv(data);
            getline(ss, data, '\t');
            movies[id]->avg_votes[0][0] = floatConv(data);

            for (int i = 0; i < 11; i++) { //ignore next 11 lines
                getline(ss, data, '\t');
            }

            //set either male/female data (not including what has already been set)
            for (int i = 1; i < 5; i++) {
                getline(ss, data, '\t');
                movies[id]->avg_votes[0][i] = floatConv(data);
                getline(ss, data, '\t');
                movies[id]->num_votes[0][i] = floatConv(data);
            }

            //insert male then female data
            for (int i = 1; i < 3; i++) {
                for (int j = 0; j < 5; j++) {
                    getline(ss, data, '\t');
                    movies[id]->avg_votes[i][j] = floatConv(data);
                    getline(ss, data, '\t');
                    movies[id]->num_votes[i][j] = floatConv(data);
                }
            }
        }
    }
    inFile.close();
}

MovieNaviGATOR::~MovieNaviGATOR()
{
    auto it = movies.begin();
    while (it != movies.end()) {
        delete it->second;
        it++;
    }
    movies.clear();
}

void MovieNaviGATOR::makeMatch(string id, int d, double r)
{
    //cout << "match" << endl;
    movies[id]->match = ((double) rand() / (RAND_MAX));
    //cout << movies[id]->match << endl;
}

void MovieNaviGATOR::minHeapCreation(int num, int d, double r)
{
    //cout << "min heap" << endl;
    //cout << movies.size() << endl;
    for (auto it = movies.begin(); it != movies.end(); ++it)
    {
        //cout << movies.size() << endl;
        if (num > 0)
        {
            //cout << it->first << endl;
            makeMatch(it->first, d, r);
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
    //cout << listMin.size() << endl;
    while (!listMin.empty())
    {
        listMax.emplace(listMin.top());
        //cout << movies[listMax.top()]->title << endl;
        listMin.pop();
    }
    //cout << listMax.size() << endl;
    showpq();
}

void MovieNaviGATOR::printMovieInfo(string title)
{
    Movie* current;
    while (!listMax.empty()) 
    {
        if (title == movies[listMax.top()]->title)
            {
                current = movies[listMax.top()];
                break;
            }
        listMax.pop();
    }
    cout << "Title: " << current->title << endl;
    cout << "Original Title: " << current->original_title << endl;
    cout << "Year: " << current->year << endl;
    cout << "Date Published: " << current->date_published << endl;
    //cout << "Genres: " << current->genres << endl;
    cout << "Duration: "<< current->duration << endl;
    cout << "Production Company: " << current->production_company << endl;
    cout << "Description: " << current->description << endl;
    cout << "Average Vote: " << current->avg_vote << endl;
    cout << "Votes: " << current->votes << endl;
}

void MovieNaviGATOR::showpq()
{
    priority_queue<string, vector<string>, CompareMoviesMax> temp = listMax;
    if (temp.empty())
    {
        cout << "No recommendations made!" << endl;
        return;
    }
    int n = 1;
    while (!temp.empty())
    {
        cout << "Movie #" << n << ": " << movies[temp.top()]->title << " " << movies[temp.top()]->match << endl;
        temp.pop();
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
    vector<string> actors;
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
                if (language[0] == ' ')
                    language.erase(0, 1);
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
                if (genre[0] == ' ')
                    genre.erase(0, 1);
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
                if (actor[0] == ' ')
                    actor.erase(0, 1);
            actors.push_back(actor);
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
                if (country[0] == ' ')
                    country.erase(0, 1);
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
                if (director[0] == ' ')
                    director.erase(0, 1);
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
                if (writer[0] == ' ')
                    writer.erase(0, 1);
            writers.insert(writer);
        }
        else if (n == 9)
        {
            cout << "Enter number of recommendations" << endl;
            cin >> number;
            cout << "Your top " << number << " movies are:" << endl;
            session.minHeapCreation(number, duration, rating);
            cout << "Would you like more information about any of these movies? (Y/N)" << endl;
            cin >> data;
            if (data == "Y")
            {
                cout << "Which of the above movies would you like more information on?" << endl;
                cin >> data;
                session.printMovieInfo(data);
            }
        }
        cout << endl;
    }
}