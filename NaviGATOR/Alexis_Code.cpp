#include <iostream>
#include <fstream>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <string>
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

    class UnorderedMap { //holds movie id and movie data
    public:
        UnorderedMap();
        UnorderedMap(int setCapacity);
        ~UnorderedMap();
        void insert(string id, Movie* newMovie);
        Movie* find(string id);
        void remove(string id);
        vector<vector<pair<string, Movie*>>> table;
    private:
        int capacity;
        int size;
        int genHash(string id);
        void rehashMap(int size);
    };

    MovieRec();
    ~MovieRec();
    void displayGenres();
    void displayLangs();

private:
    UnorderedMap movies = UnorderedMap(110000); //holds each movie, with the key being the movie ID
    //initialize it with size 110000 as we know 86,000 values will be added, this will save the time of resizing
    //(although this functionality is available)
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

MovieRec::UnorderedMap::UnorderedMap() { //default constructor, makes room for 100 values
    size = 0;
    capacity = 100;
    for (int i = 0; i < capacity; i++) {
        vector<pair<string, Movie*>> addVector;
        table.push_back(addVector);
    }
}

MovieRec::UnorderedMap::UnorderedMap(int setCapacity) { //constructor that sets capacity to user-chosen value
    size = 0;
    capacity = setCapacity;
    for (int i = 0; i < capacity; i++) {
        vector<pair<string, Movie*>> addVector;
        table.push_back(addVector);
    }
}

MovieRec::UnorderedMap::~UnorderedMap() { //destructor
    for (int i = 0; i < table.size(); i++) { //deletes all node pointers in the map
        for (int j = 0; j < table[i].size(); j++) {
            delete table[i][j].second;
        }
    }
    table.clear();
    size = 0;
    capacity = 0;
}

int MovieRec::UnorderedMap::genHash(string id) { //generates hash code
    //because the movie ids in the table are all integers with "tt" at the start, to convert to a hash code, we take off these letters and convert to an int
    //we return the remainder of the division by the table size
    string stringID = id.substr(2, id.size());
    return stoi(stringID) % capacity;
}

void MovieRec::UnorderedMap::insert(string id, Movie* newMovie) {
    int bucketIndex = genHash(id); //get hash code
    string currID;
    for (int i = 0; i < table[bucketIndex].size(); i++) { //see if the element is already in the map
        currID = table[bucketIndex][i].first;
        if (currID == id) {
            table[bucketIndex][i] = make_pair(id, newMovie); //reassign this value and return
            return;
        }
    }
    table[bucketIndex].push_back(make_pair(id, newMovie));
    size++;
    if (((double)size / (double) capacity) > 0.8) { //if load factor is above 0.8
        rehashMap(size);
    }
}

MovieRec::Movie* MovieRec::UnorderedMap::find(string id) {
    int bucketIndex = genHash(id);
    string currID;
    for (int i = 0; i < table[bucketIndex].size(); i++) { //iterate through the smaller vector where the value should be
        currID = table[bucketIndex][i].first;
        if (currID == id) {
            return table[bucketIndex][i].second;
        }
    }
    return nullptr;
}

void MovieRec::UnorderedMap::remove(string id) {
    int bucketIndex = genHash(id);
    string currID;
    int i;
    for (i = 0; i < table[bucketIndex].size(); i++) { //search to see if the element is in the map
        currID = table[bucketIndex][i].first;
        if (currID == id) {
            break;
        }
    }

    if (i < table[bucketIndex].size()) { //if the element is in the map, delete it
        delete table[bucketIndex][i].second;
        table[bucketIndex].erase(table[bucketIndex].begin() + i);
        size--;
    }
}

void MovieRec::UnorderedMap::rehashMap(int newCapacity) { //called if the load factor is exceeded
    capacity = newCapacity;
    vector<vector<pair<string, Movie*>>> oldTable = table; //make the table the old table
    table.clear(); //clear the table
    for (int i = 0; i < newCapacity; i++) { //add capacity number of empty vectors
        vector<pair<string, Movie*>> addVector;
        table.push_back(addVector);
    }
    pair<string, Movie*> currPair;
    for (int i = 0; i < oldTable.size(); i++) {
        for (int j = 0; j < oldTable[i].size(); j++) {
            currPair = table[i][j];
            insert(currPair.first, currPair.second); //insert each old value into the new table
        }
    }
}

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

            movies.insert(id, currMovie);

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
            movies.find(id)->num_votes[0][0] = floatConv(data);
            getline(ss, data, '\t'); //collect total number of votes across all ages and genders
            movies.find(id)->avg_votes[0][0] = floatConv(data);

            for (int i = 0; i < 11; i++) { //ignore next 11 lines
                getline(ss, data, '\t');
            }

            //set data that includes both genders (not including what has already been set)
            for (int i = 1; i < 5; i++) {
                getline(ss, data, '\t');
                movies.find(id)->avg_votes[0][i] = floatConv(data);
                getline(ss, data, '\t');
                movies.find(id)->num_votes[0][i] = floatConv(data);
            }

            //insert male then female data
            for (int i = 1; i < 3; i++) {
                for (int j = 0; j < 5; j++) {
                    getline(ss, data, '\t');
                    movies.find(id)->avg_votes[i][j] = floatConv(data);
                    getline(ss, data, '\t');
                    movies.find(id)->num_votes[i][j] = floatConv(data);
                }
            }
        }
    }
    inFile.close();
}

MovieRec::~MovieRec() { //destructor
    //can remain empty because all memory management is taken care of in the unordered map
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
