#include <iostream>
#include <fstream>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <map>
#include <vector>
#include <sstream>
#include <string>
#include <queue>

using namespace std;

class MovieRec {
public:
    class Movie { //holds the data for each node in the tree
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
        double avg_votes[3][5]; //holds ratings across different age groups and genders
        double num_votes[3][5]; //same as above, but with total numbers of votes instead of average numbers of votes

        Movie() { //default constructor
            title = "";
            original_title = "";
            year = 0;
            date_published = "";
            duration = 0;
            production_company = "";
            description = "";
            for (int i = 0; i < 3; i++) { //fill 2D array with 0's
                for (int j = 0; j < 5; j++) {
                    avg_votes[i][j] = 0;
                }
            }
            for (int i = 0; i < 3; i++) { //again fill 2D array with 0's
                for (int j = 0; j < 5; j++) {
                    num_votes[i][j] = 0;
                }
            }
        }
    };

    MovieRec();
    ~MovieRec();
    void displayGenres();
    void displayLangs();

private:
    unordered_map<string, Movie*> movies; //holds each movie, with the key being the movie ID
    unordered_map<string, int> genreMap; //holds frequencies of genres
    unordered_map<string, int> languageMap; //holds frequencies of languages

    int intConv(string& input);
    float floatConv(string& input);
    unordered_set<string> setConv(string input);
    vector<string> vectorConv(string input);
    string testQuotes(string& input);
    void displayMap(unordered_map<string, int> myMap, int length);
    void insertMap(unordered_set<string> &newSet, unordered_map<string, int> &mainMap);
};

int MovieRec::intConv(string& input) { //constructor helper function; converts string to integer, if possible
    int result;
    try {
        result = stoi(input);
    }
    catch(exception &err) {
        result = 0;
    }
    return result;
}

float MovieRec::floatConv(string& input) { //constructor helper function; converts string to float, if possible
    float result;
    try {
        result = stof(input);
    }
    catch(exception &err) {
        result = 0;
    }
    return result;
}

unordered_set<string> MovieRec::setConv(string input) { //constructor helper function; breaks apart string by commas and places into an unordered set
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

vector<string> MovieRec::vectorConv(string input) { //constructor helper function; breaks apart string by commas and places into a vector
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

string MovieRec::testQuotes(string& input) { //constructor helper function; removes quotes from a string, if present
    int lastChar = input.size() - 1;
    string result;
    if ((input[0] == '\"') && (input[lastChar] == '\"')) {
        return input.substr(1, lastChar - 1);
    }
    return input;
}

void MovieRec::insertMap(unordered_set<string> &newSet, unordered_map<string, int> &mainMap) { //constructor helper function; puts contents of set into map frequency table
    auto it = newSet.begin();
    while (it != newSet.end()) {
        if (mainMap.find(*it) != mainMap.end()) {
            mainMap[*it] = mainMap[*it] + 1;
        }
        else {
            mainMap[*it] = 1;
        }
        it++;
    }
}

MovieRec::MovieRec() { //constructor; reads through files and sets up movie unordered_map
    string line;
    vector<string> myString;
    ifstream inFile;
    inFile.open("imdb_movies.tsv"); //first file; contains main movie data, but no detailed rating data, about 86k lines
    if (inFile.is_open()) {

        //remove heading
        getline(inFile, line);
        getline(inFile, line);

        string id; //holds movie ID, which the map is organized by

        while (getline(inFile, line)) { //for each row in the data file
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
    inFile.open("imdb_ratings.tsv"); //second file; contains detailed rating information for each movie (referenced by ID)

    if (inFile.is_open()) {
        //remove heading
        getline(inFile, line);
        getline(inFile, line);

        string id; //holds movie ID, which the map is organized by
        string data; //holds various pieces of important data
        string trash; //holds lines that are ignored

        while (getline(inFile, line)) {
            istringstream ss(line);

            getline(ss, id, '\t'); //collect the ID for the current movie
            getline(ss, trash, '\t'); //trash the following line (irrelevant info)
            getline(ss, data, '\t'); //collect number of votes across all ages and genders
            movies[id]->num_votes[0][0] = floatConv(data);
            getline(ss, data, '\t'); //collect total number of votes across all ages and genders
            movies[id]->avg_votes[0][0] = floatConv(data);

            for (int i = 0; i < 11; i++) { //ignore next 11 lines
                getline(ss, data, '\t');
            }

            //set data that includes both genders (not including what has already been set)
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

MovieRec::~MovieRec() { //destructor
    auto it = movies.begin();
    while (it != movies.end()) {
        delete it->second;
        it++;
    }
    movies.clear();
}

//prints out top contents of frequency map, from most to least frequent
void MovieRec::displayMap(unordered_map<string, int> myMap, int length) {
    unordered_map<string, int> mapPrint = myMap;
    for (int i = 0; i < length; i++) { //"length" number of contents should be printed
        auto it = mapPrint.begin();
        auto max = it;
        while (it != mapPrint.end()) {
            if ((it->second) > (max->second)) { //if it is the largest so far, replace the max
                max = it;
            }
            it++;
        }
        cout << max->first;
        mapPrint.erase(max);

        //formatting
        if (i < length - 1) {
            cout << ", ";
        }
        else {
            cout << endl;
        }
    }
}

void MovieRec::displayGenres() { //displays all genres
    displayMap(genreMap, genreMap.size());
}

void MovieRec::displayLangs() { //displays top 10 languages
    displayMap(languageMap, 10);
}

int main() {
    MovieRec test;
}
