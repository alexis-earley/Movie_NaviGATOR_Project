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
};



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

class MovieNaviGATOR
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
};