#include "Movie.h"

using namespace std;

/*class Movie
{
public:
    string id;
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
    double avg_votes[3][5] = {0};
    double num_votes[3][5] = {0};

    float match;

    Movie()
    { //default constructor
        id = "";
        title = "";
        original_title = "";
        year = 0;
        date_published = "";
        duration = 0;
        production_company = "";
        description = "";
        avg_vote = -1;
        votes = -1;
        match = -1;
    }
};*/

/*struct CompareMoviesMax
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
};*/
/*class MovieNaviGATOR
{
public:
    //priority_queue<string, vector<string>, CompareMoviesMax> listMax;
    //priority_queue<string, vector<string>, CompareMoviesMin> listMin;
    unordered_map<string, Movie *> movies;

    unordered_map<string, int> genreMap;    //holds frequencies of genres
    unordered_map<string, int> languageMap; //holds frequencies of languages

    int intConv(string &input);
    float floatConv(string &input);

    unordered_set<string> setConv(string input);
    vector<string> vectorConv(string input);
    string testQuotes(string &input);
    void displayMap(unordered_map<string, int> myMap, int length);
    void insertMap(unordered_set<string> &newSet, unordered_map<string, int> &mainMap);

    void makeMatch(string id, int d, double r);
    void printSet(unordered_set<string> temp);
    void maxHeapCreation();
    void showpq();

//public:
    MovieNaviGATOR();
    ~MovieNaviGATOR();
    void displayGenres();
    void displayLangs();

    void minHeapCreation(int num, int d, double r);
    void printMovieInfo(string title);
};*/

int MovieNaviGATOR::intConv(string &input)
{ //constructor helper function; converts string to integer, if possible
    int result;
    try
    {
        result = stoi(input);
    }
    catch (exception &err)
    {
        result = 0;
    }
    return result;
}

float MovieNaviGATOR::floatConv(string &input)
{ //constructor helper function; converts string to float, if possible
    float result;
    try
    {
        result = stof(input);
    }
    catch (exception &err)
    {
        result = 0;
    }
    return result;
}

unordered_set<string> MovieNaviGATOR::setConv(string input)
{ //constructor helper function; breaks apart string by commas and places into an unordered set
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

vector<string> MovieNaviGATOR::vectorConv(string input)
{ //constructor helper function; breaks apart string by commas and places into a vector
    vector<string> result;
    string currString = "";
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] != ',')
        {
            currString += input[i];
        }
        else
        {
            result.push_back(currString);
            currString = "";
            i++;
        }
    }
    result.push_back(currString);
    return result;
}

string MovieNaviGATOR::testQuotes(string &input)
{ //constructor helper function; removes quotes from a string, if present
    int lastChar = input.size() - 1;
    string result;
    if ((input[0] == '\"') && (input[lastChar] == '\"'))
    {
        return input.substr(1, lastChar - 1);
    }
    return input;
}

void MovieNaviGATOR::insertMap(unordered_set<string> &newSet, unordered_map<string, int> &mainMap)
{ //constructor helper function; puts contents of set into map frequency table
    auto it = newSet.begin();
    while (it != newSet.end())
    {
        if (mainMap.find(*it) != mainMap.end())
        {
            mainMap[*it] = mainMap[*it] + 1;
        }
        else
        {
            mainMap[*it] = 1;
        }
        it++;
    }
}

MovieNaviGATOR::MovieNaviGATOR()
{
    string line;
    vector<string> myString;
    ifstream inFile;
    inFile.open("C:\\Users\\Pandu\\source\\repos\\NaviGATOR\\NaviGATOR\\imdb_movies.tsv"); //first file; contains main movie data, but no detailed rating data, about 86k lines
    if (inFile.is_open())
    {

        //remove heading
        getline(inFile, line);
        getline(inFile, line);

        string id; //holds movie ID, which the map is organized by

        while (getline(inFile, line))
        { //for each row in the data file
            Movie *currMovie = new Movie();
            istringstream ss(line);

            string data;
            getline(ss, data, '\t'); //gets the ID
            currMovie->id = data;
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
            unordered_set<string> genres = setConv(testQuotes(data));
            currMovie->genres = genres;
            insertMap(genres, genreMap);

            getline(ss, data, '\t'); //gets the duration
            currMovie->duration = intConv(data);
            getline(ss, data, '\t'); //gets the countries
            currMovie->countries = setConv(testQuotes(data));
            getline(ss, data, '\t'); //gets the languages
            unordered_set<string> languages = setConv(testQuotes(data));
            currMovie->languages = languages;
            insertMap(languages, languageMap);

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

            movies[id] = currMovie;
        }
    }
    inFile.close();
    /*inFile.open("C:\\Users\\Pandu\\source\\repos\\NaviGATOR\\NaviGATOR\\imdb_ratings.tsv"); //second file; contains detailed rating information for each movie (referenced by ID)

    if (inFile.is_open())
    {
        //remove heading
        getline(inFile, line);
        getline(inFile, line);

        string id;    //holds movie ID, which the map is organized by
        string data;  //holds various pieces of important data
        string trash; //holds lines that are ignored

        while (getline(inFile, line))
        {
            istringstream ss(line);

            getline(ss, id, '\t');    //collect the ID for the current movie
            getline(ss, trash, '\t'); //trash the following line (irrelevant info)
            getline(ss, data, '\t');  //collect number of votes across all ages and genders
            movies[id]->num_votes[0][0] = floatConv(data);
            getline(ss, data, '\t'); //collect total number of votes across all ages and genders
            movies[id]->avg_votes[0][0] = floatConv(data);

            for (int i = 0; i < 11; i++)
            { //ignore next 11 lines
                getline(ss, data, '\t');
            }

            //set data that includes both genders (not including what has already been set)
            for (int i = 1; i < 5; i++)
            {
                getline(ss, data, '\t');
                movies[id]->avg_votes[0][i] = floatConv(data);
                getline(ss, data, '\t');
                movies[id]->num_votes[0][i] = floatConv(data);
            }

            //insert male then female data
            for (int i = 1; i < 3; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    getline(ss, data, '\t');
                    movies[id]->avg_votes[i][j] = floatConv(data);
                    getline(ss, data, '\t');
                    movies[id]->num_votes[i][j] = floatConv(data);
                }
            }
        }
    }
    inFile.close();*/
}

MovieNaviGATOR::~MovieNaviGATOR()
{ //destructor
    auto it = movies.begin();
    while (it != movies.end())
    {
        delete it->second;
        it++;
    }
    movies.clear();
}

//prints out top contents of frequency map, from most to least frequent
void MovieNaviGATOR::displayMap(unordered_map<string, int> myMap, int length)
{
    unordered_map<string, int> mapPrint = myMap;
    for (int i = 0; i < length; i++)
    { //"length" number of contents should be printed
        auto it = mapPrint.begin();
        auto max = it;
        while (it != mapPrint.end())
        {
            if ((it->second) > (max->second))
            { //if it is the largest so far, replace the max
                max = it;
            }
            it++;
        }
        cout << max->first;
        mapPrint.erase(max);

        //formatting
        if (i < length - 1)
        {
            cout << ", ";
        }
        else
        {
            cout << endl;
        }
    }
}

void MovieNaviGATOR::displayGenres()
{ //displays all genres
    displayMap(genreMap, genreMap.size());
}

void MovieNaviGATOR::displayLangs()
{ //displays top 10 languages
    displayMap(languageMap, 10);
}

void MovieNaviGATOR::makeMatch(string id, int d, double r)
{
    //cout << "match" << endl;
    movies[id]->match = ((double)rand() / (RAND_MAX));
    //cout << movies[id]->match << endl;
}

/*void MovieNaviGATOR::minHeapCreation(int num, int d, double r)
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
}*/

/*void MovieNaviGATOR::maxHeapCreation()
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
}*/

/*void MovieNaviGATOR::printMovieInfo(string title)
{
    Movie *current;
    while (!listMax.empty())
    {
        if (title == movies[listMax.top()]->title)
        {
            current = movies[listMax.top()];
            break;
        }
        listMax.pop();
    }
    if (listMax.empty())
        cout << "Movie not found." << endl;
    else
    {
        cout << "Title: " << current->title << endl;
        cout << "Original Title: " << current->original_title << endl;
        cout << "Year: " << current->year << endl;
        cout << "Date Published: " << current->date_published << endl;
        cout << "Genres: ";
        printSet(current->genres);
        cout << endl;
        cout << "Duration: " << current->duration << endl;
        cout << "Countries: ";
        printSet(current->countries);
        cout << endl;
        cout << "Languages: ";
        printSet(current->languages);
        cout << endl;
        cout << "Directors: ";
        printSet(current->directors);
        cout << endl;
        cout << "Writers: ";
        printSet(current->writers);
        cout << endl;
        cout << "Production Company: " << current->production_company << endl;
        cout << "Actors: ";
        // Creating a iterator pointing to start of set
        vector<string>::iterator it = current->actors.begin();
        // Iterate till the end of set
        while (it != current->actors.end())
        {
            // Print the element
            cout << (*it) << ", ";
            //Increment the iterator
            it++;
        }
        cout << endl;
        cout << "Description: " << current->description << endl;
        cout << "Average Vote: " << current->avg_vote << endl;
        cout << "Votes: " << current->votes << endl;
    }
}*/

void MovieNaviGATOR::printSet(unordered_set<string> temp)
{
    // Creating a iterator pointing to start of set
    unordered_set<string>::iterator it = temp.begin();
    // Iterate till the end of set
    while (it != temp.end())
    {
        // Print the element
        cout << (*it) << ", ";
        //Increment the iterator
        it++;
    }
}

/*void MovieNaviGATOR::showpq()
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
};*/

/*int main()
{
    MovieNaviGATOR session;

    cout << "Welcome to MovieNAVIGATOR!" << endl;

    int n = 1;
    int duration;
    double rating;
    int number;
    int age;
    int gender;

    unordered_set<string> genres;
    vector<string> actors;
    unordered_set<string> languages;
    unordered_set<string> countries;
    unordered_set<string> directors;
    unordered_set<string> writers;

    while (n <= 9 && n >= 1)
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
            cout << "Top 10 Most Prevalent Languages in Database" << endl;
            session.displayLangs();
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
            cout << "All Genres in Database" << endl;
            session.displayGenres();
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
            cout << "Which age group do you fall into?" << endl;
            cout << "1. 0-18 years" << endl;
            cout << "2. 19-30 years" << endl;
            cout << "3. 31-45 years" << endl;
            cout << "4. 46+ years" << endl;
            cin >> age;
            cout << "What gender are you?" << endl;
            cout << "1. Female" << endl;
            cout << "2. Male" << endl;
            cin >> gender;
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
                cin.ignore();
                cout << "Which of the above movies would you like more information on?" << endl;
                getline(cin, data);
                cout << data << endl;
                session.printMovieInfo(data);
            }
        }
        cout << endl;
    }
}*/