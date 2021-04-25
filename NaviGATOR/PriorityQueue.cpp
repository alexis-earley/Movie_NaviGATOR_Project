#include <iostream>
#include <fstream>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <string>
using namespace std;

class Movie
{ //holds the data for each node in the tree
public:
    double match;
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
    double num_votes;       //same as above, but with total numbers of votes instead of average numbers of votes

    Movie()
    { //default constructor
        title = "";
        original_title = "";
        year = 0;
        date_published = "";
        duration = 0;
        production_company = "";
        description = "";
        num_votes = 0;
        for (int i = 0; i < 3; i++)
        { //fill 2D array with 0's
            for (int j = 0; j < 5; j++)
            {
                avg_votes[i][j] = 0;
            }
        }
    }
};

class UnorderedMap
{ //holds movie id and movie data
public:
    vector<vector<pair<string, Movie *>>> table;
    UnorderedMap()
    { //default constructor, makes room for 100 values
        size = 0;
        capacity = 100;
        for (int i = 0; i < capacity; i++)
        {
            vector<pair<string, Movie *>> addVector;
            table.push_back(addVector);
        }
    }

    UnorderedMap(int setCapacity)
    { //constructor that sets capacity to user-chosen value
        size = 0;
        capacity = setCapacity;
        for (int i = 0; i < capacity; i++)
        {
            vector<pair<string, Movie *>> addVector;
            table.push_back(addVector);
        }
    }

    ~UnorderedMap()
    { //destructor
        for (int i = 0; i < table.size(); i++)
        { //deletes all node pointers in the map
            for (int j = 0; j < table[i].size(); j++)
            {
                delete table[i][j].second;
            }
        }
        table.clear();
        size = 0;
        capacity = 0;
    }

    void insert(string id, Movie *newMovie)
    {
        int bucketIndex = genHash(id); //get hash code
        string currID;
        for (int i = 0; i < table[bucketIndex].size(); i++)
        { //see if the element is already in the map
            currID = table[bucketIndex][i].first;
            if (currID == id)
            {
                table[bucketIndex][i] = make_pair(id, newMovie); //reassign this value and return
                return;
            }
        }
        table[bucketIndex].push_back(make_pair(id, newMovie));
        size++;
        if (((double)size / (double)capacity) > 0.8)
        { //if load factor is above 0.8
            rehashMap(size);
        }
    }

    Movie *find(string id)
    {
        int bucketIndex = genHash(id);
        string currID;
        for (int i = 0; i < table[bucketIndex].size(); i++)
        { //iterate through the smaller vector where the value should be
            currID = table[bucketIndex][i].first;
            if (currID == id)
            {
                return table[bucketIndex][i].second;
            }
        }
        return nullptr;
    }

    void pop(string id)
    {
        int bucketIndex = genHash(id);
        string currID;
        int i;
        for (i = 0; i < table[bucketIndex].size(); i++)
        { //search to see if the element is in the map
            currID = table[bucketIndex][i].first;
            if (currID == id)
            {
                break;
            }
        }

        if (i < table[bucketIndex].size())
        { //if the element is in the map, delete it
            delete table[bucketIndex][i].second;
            table[bucketIndex].erase(table[bucketIndex].begin() + i);
            size--;
        }
    }

private:
    int capacity;
    int size;
    int genHash(string id)
    { //generates hash code
        //because the movie ids in the table are all integers with "tt" at the start, to convert to a hash code, we take off these letters and convert to an int
        //we return the remainder of the division by the table size
        string stringID = id.substr(2, id.size());
        return stoi(stringID) % capacity;
    }

    void rehashMap(int newCapacity)
    { //called if the load factor is exceeded
        capacity = newCapacity;
        vector<vector<pair<string, Movie *>>> oldTable = table; //make the table the old table
        table.clear();                                          //clear the table
        for (int i = 0; i < newCapacity; i++)
        { //add capacity number of empty vectors
            vector<pair<string, Movie *>> addVector;
            table.push_back(addVector);
        }
        pair<string, Movie *> currPair;
        for (int i = 0; i < oldTable.size(); i++)
        {
            for (int j = 0; j < oldTable[i].size(); j++)
            {
                currPair = table[i][j];
                insert(currPair.first, currPair.second); //insert each old value into the new table
            }
        }
    }
};

struct PriorityQueue
{
private:
    // vector to store heap elements
    vector<Movie *> min;
    vector<Movie *> max;
    vector<Movie *> copy;
    //int size;
    int capacity;

    int duration;
    double rating;
    int number;
    int age;
    int gender;

    unordered_set<string> genres;
    unordered_set<string> languages;
    unordered_set<string> countries;
    unordered_set<string> directors;
    unordered_set<string> writers;
    vector<string> actors;

    int current = 0;

    string testQuotes(string &input)
    { //constructor helper function; pops quotes from a string, if present
        int lastChar = input.size() - 1;
        string result;
        if ((input[0] == '\"') && (input[lastChar] == '\"'))
        {
            return input.substr(1, lastChar - 1);
        }
        return input;
    }

    int intConv(string &input)
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

    unordered_set<string> setConv(string input)
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

    vector<string> vectorConv(string input)
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

    // Recursive heapify-down algorithm.
    // The node at index `i` and its two direct children
    // violates the heap property
    //https://www.techiedelight.com/min-heap-max-heap-implementation-c/
    void heapify_down_max(int i)
    {
        // get left and right child of node at index `i`
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        int largest = i;

        // compare `max[i]` with its left and right child
        // and find the largest value
        if (left < max.size() && max[left]->match > max[i]->match)
        {
            largest = left;
        }

        if (right < max.size() && max[right]->match > max[largest]->match)
        {
            largest = right;
        }

        // swap with max child having greater value and
        // call heapify-down on the child
        if (largest != i)
        {
            swap(max[i], max[largest]);
            heapify_down_max(largest);
        }
    }

    // Recursive heapify-up algorithm
    //https://www.techiedelight.com/min-heap-max-heap-implementation-c/https://www.techiedelight.com/min-heap-max-heap-implementation-c/
    void heapify_up_max(int i)
    {
        // check if the node at index `i` and its parent violate the heap property
        if (i && max[(i - 1) / 2]->match < max[i]->match)
        {
            // swap the two if heap property is violated
            swap(max[i], max[(i - 1) / 2]);

            // call heapify-up on the parent
            heapify_up_max((i - 1) / 2);
        }
    }

    // Recursive heapify-down algorithm.
    // The node at index `i` and its two direct children
    // violates the heap property
    //https://www.techiedelight.com/min-heap-max-heap-implementation-c/
    void heapify_down_min(int i)
    {
        // get left and right child of node at index `i`
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        int smallest = i;

        // compare `min[i]` with its left and right child
        // and find the smallest value
        if (left < min.size() && min[left]->match < min[i]->match)
        {
            smallest = left;
        }

        if (right < min.size() && min[right]->match < min[smallest]->match)
        {
            smallest = right;
        }

        // swap with min child having lesser value and
        // call heapify-down on the child
        if (smallest != i)
        {
            swap(min[i], min[smallest]);
            heapify_down_min(smallest);
        }
    }

    // Recursive heapify-up algorithm
    //https://www.techiedelight.com/min-heap-max-heap-implementation-c/
    void heapify_up_min(int i)
    {
        // check if the node at index `i` and its parent violate the heap property
        if (i && min[(i - 1) / 2]->match > min[i]->match)
        {
            // swap the two if heap property is violated
            swap(min[i], min[(i - 1) / 2]);

            // call heapify-up on the parent
            heapify_up_min((i - 1) / 2);
        }
    }

    // insert key into the heap
    //https://www.techiedelight.com/min-heap-max-heap-implementation-c/
    void push_min(Movie *key)
    {
        // insert min new element at the end of the vector
        min.push_back(key);

        // get element index and call heapify-up procedure
        int index = min.size() - 1;
        heapify_up_min(index);
    }

    // Function to remove an element with the lowest priority (present at the root)
    //https://www.techiedelight.com/min-heap-max-heap-implementation-c/
    void pop_min()
    {
        // replace the root of the heap with the last element
        // of the vector
        min[0] = min.back();
        min.pop_back();

        // call heapify-down on the root node
        heapify_down_min(0);
    }

    // Function to return an element with the lowest priority (present at the root)
    //https://www.techiedelight.com/min-heap-max-heap-implementation-c/
    Movie *top_min()
    {
        // otherwise, return the top_min (first) element
        return min.at(0); // or return min[0];
    }

    void show_max()
    {
        copy = max;
        while (number > 0)
        {
            cout << top_max()->title << ": " << top_max()->match << endl;
            pop_max();
            number--;
        }
    }

    // insert key into the heap
    //https://www.techiedelight.com/min-heap-max-heap-implementation-c/
    void push_max(Movie *key)
    {
        // insert max new element at the end of the vector
        max.push_back(key);

        // get element index and call heapify-up procedure
        int index = max.size() - 1;
        heapify_up_max(index);
    }

    // Function to remove an element with the highest priority (present at the root)
    //https://www.techiedelight.com/min-heap-max-heap-implementation-c/
    void pop_max()
    {
        // replace the root of the heap with the last element
        // of the vector
        max[0] = max.back();
        max.pop_back();

        // call heapify-down on the root node
        heapify_down_max(0);
    }

    // Function to return an element with the highest priority (present at the root)
    //https://www.techiedelight.com/min-heap-max-heap-implementation-c/
    Movie *top_max()
    {
        // otherwise, return the top (first) element
        return max.at(0); // or return max[0];
    }

    void makeMatch(Movie *movie)
    {
        movie->match = ((double)rand() / (RAND_MAX));
    }

    void buildMax()
    {
        while (min.size() > 0)
        {
            push_max(top_min());
            pop_min();
        }
        show_max();
    }

    Movie *find(string title)
    {
        bool found = false;
        for (Movie *m : copy)
        {
            if (m->title == title)
            {
                found = true;
                return m;
            }
        }
        return nullptr;
    }

public:
    PriorityQueue()
    {
        //size = 0;
        capacity = 100;

        duration = 0;
        rating = 0.0;
        number = 0;
        age = 0;
        gender = 0;
    }

    ~PriorityQueue()
    { //destructor
        min.clear();
        max.clear();
    }

    void setDuration(int d)
    {
        duration = d;
    }

    void setRating(double r)
    {
        rating = r;
    }

    void setNumber(int n)
    {
        number = n;
    }

    void setAge(int a)
    {
        age = a;
    }

    void setGender(int g)
    {
        gender = g;
    }

    void setGenres(unordered_set<string> g)
    {
        genres = g;
    }

    void setLanguages(unordered_set<string> l)
    {
        languages = l;
    }

    void setCountries(unordered_set<string> c)
    {
        countries = c;
    }

    void setDirectors(unordered_set<string> d)
    {
        directors = d;
    }

    void setWriters(unordered_set<string> w)
    {
        writers = w;
    }

    void setActors(vector<string> a)
    {
        actors = a;
    }

    void buildMinMap(UnorderedMap &movies)
    {
        for (int i = 0; i < movies.table.size(); i++)
        {
            for (int j = 0; j < movies.table[i].size(); j++)
            {
                makeMatch(movies.table[i][j].second);
                if (min.size() > number && movies.table[i][j].second->match < top_min()->match)
                {
                    continue;
                }
                push_min(movies.table[i][j].second);
                if (min.size() > number)
                {
                    pop_min();
                };
            }
        }
        cout << min.size() << endl;
        buildMax();
    }

    void buildMinQueue()
    {
        string line;
        vector<string> myString;
        ifstream inFile;
        inFile.open("C:\\Users\\Pandu\\source\\repos\\NaviGATOR\\NaviGATOR\\imdb_movies.tsv"); //first file; contains main movie data, but no detailed rating data, about 86k lines
        if (inFile.is_open())
        {
            //pop heading
            //cout << "file is open!" << endl;
            getline(inFile, line);
            getline(inFile, line);

            string id; //holds movie ID, which the map is organized by

            while (getline(inFile, line))
            { //for each row in the data file
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

                getline(ss, data, '\t'); //gets the duration
                currMovie->duration = intConv(data);
                getline(ss, data, '\t'); //gets the countries
                currMovie->countries = setConv(testQuotes(data));
                getline(ss, data, '\t'); //gets the languages
                unordered_set<string> languages = setConv(testQuotes(data));
                currMovie->languages = languages;

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

                makeMatch(currMovie);
                push_min(currMovie);
            }
        }
        inFile.close();
        cout << min.size() << endl;
        buildMax();
    }

    void printMovieInfo(string title)
    {
        cout << copy.size() << endl;
        Movie *current = find(title);
        if (!current)
        {
            cout << "Movie not found/recommended." << endl;
        }
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
            // Creating a iterator pointing to start of vector
            vector<string>::iterator it = current->actors.begin();
            // Iterate till the end of vector
            while (it != current->actors.end())
            {
                // Print the element
                cout << (*it) << ", ";
                //Increment the iterator
                it++;
            }
            cout << endl;
            cout << "Description: " << current->description << endl;
            //cout << "Average Vote: " << current->avg_vote << endl;
            //cout << "Votes: " << current->votes << endl;
        }
    }

    void printSet(unordered_set<string> temp)
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
};

class MovieNaviGator
{
public:
    UnorderedMap movies = UnorderedMap(110000); //holds each movie, with the key being the movie ID
    PriorityQueue *recommendations = new PriorityQueue();
    //initialize it with size 110000 as we know 86,000 values will be added, this will save the time of resizing
    //(although this functionality is available)
    MovieNaviGator()
    { //constructor; reads through files and sets up movie unordered_map
        string line;
        vector<string> myString;
        ifstream inFile;
        inFile.open("C:\\Users\\Pandu\\source\\repos\\NaviGATOR\\NaviGATOR\\imdb_movies.tsv"); //first file; contains main movie data, but no detailed rating data, about 86k lines
        if (inFile.is_open())
        {
            //pop heading
            //cout << "file is open!" << endl;
            getline(inFile, line);
            getline(inFile, line);

            string id; //holds movie ID, which the map is organized by

            while (getline(inFile, line))
            { //for each row in the data file
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

                getline(ss, data, '\t'); //gets the duration
                currMovie->duration = intConv(data);
                getline(ss, data, '\t'); //gets the countries
                currMovie->countries = setConv(testQuotes(data));
                getline(ss, data, '\t'); //gets the languages
                unordered_set<string> languages = setConv(testQuotes(data));
                currMovie->languages = languages;

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
        /*inFile.open("C:\\Users\\Pandu\\source\\repos\\NaviGATOR\\NaviGATOR\\imdb_ratings.tsv"); //second file; contains detailed rating information for each movie (referenced by ID)

        if (inFile.is_open())
        {
            //pop heading
            getline(inFile, line);
            getline(inFile, line);

            string id;    //holds movie ID, which the map is organized by
            string data;  //holds various pieces of important data
            string trash; //holds lines that are ignored

            while (getline(inFile, line))
            {
                istringstream ss(line);

                getline(ss, id, '\t');              //collect the ID for the current movie
                Movie *currMovie = movies.find(id); //find the movie with this ID in the map

                getline(ss, trash, '\t'); //trash the following line (irrelevant info)
                getline(ss, data, '\t');  //collect number of votes across all ages and genders
                currMovie->num_votes = floatConv(data);
                getline(ss, data, '\t'); //collect total number of votes across all ages and genders
                currMovie->avg_votes[0][0] = floatConv(data);

                for (int i = 0; i < 11; i++)
                { //ignore next 11 lines
                    getline(ss, data, '\t');
                }

                //set data that includes both genders (not including what has already been set)
                for (int i = 1; i < 5; i++)
                {
                    getline(ss, data, '\t');
                    currMovie->avg_votes[0][i] = floatConv(data);
                    getline(ss, trash, '\t');
                }

                //insert male then female data
                for (int i = 1; i < 3; i++)
                {
                    for (int j = 0; j < 5; j++)
                    {
                        getline(ss, data, '\t');
                        currMovie->avg_votes[i][j] = floatConv(data);
                        getline(ss, trash, '\t');
                    }
                }
            }
        }
        inFile.close();*/
    }

    ~MovieNaviGator()
    { //destructor
        //can remain empty because all memory management is taken care of in the unordered map
    }

    //prints out top contents of frequency map, from most to least frequent
    void displayMap(unordered_map<string, int> myMap, int length)
    {
        unordered_map<string, int> mapPrint = myMap;
        for (int i = 0; i < length; i++)
        { //"length" number of contents should be printed
            auto it = mapPrint.begin();
            auto max = it;
            while (it != mapPrint.end())
            {
                if ((it->second) > (max->second))
                { //if it is the smallest so far, replace the max
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

    void displayGenres()
    { //displays all genres
        displayMap(genreMap, genreMap.size());
    }

    void displayLangs()
    { //displays top 10 languages
        displayMap(languageMap, 10);
    }

private:
    unordered_map<string, int> genreMap;    //holds frequencies of genres
    unordered_map<string, int> languageMap; //holds frequencies of languages

    int intConv(string &input)
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

    float floatConv(string &input)
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

    unordered_set<string> setConv(string input)
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

    vector<string> vectorConv(string input)
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

    string testQuotes(string &input)
    { //constructor helper function; pops quotes from a string, if present
        int lastChar = input.size() - 1;
        string result;
        if ((input[0] == '\"') && (input[lastChar] == '\"'))
        {
            return input.substr(1, lastChar - 1);
        }
        return input;
    }

    void insertMap(unordered_set<string> &newSet, unordered_map<string, int> &mainMap)
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
};

int main()
{
    MovieNaviGator session;
    cout << "Welcome to MovieNaviGator!" << endl;

    int n = 1;
    int duration;
    double rating;
    int number;
    int age;
    int gender;
    int implementation;

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
            session.recommendations->setLanguages(languages);
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
            session.recommendations->setGenres(genres);
        }
        else if (n == 3)
        {
            cout << "Range of durations (in minutes) in database: 40 - 800" << endl;
            cout << "Enter preferred duration in minutes (e.g. 120)" << endl;
            cin >> duration;
            session.recommendations->setDuration(duration);
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
            session.recommendations->setActors(actors);
        }
        else if (n == 5)
        {
            cout << "Which age group do you fall into?" << endl;
            cout << "1. 0-18 years" << endl;
            cout << "2. 19-30 years" << endl;
            cout << "3. 31-45 years" << endl;
            cout << "4. 46+ years" << endl;
            cin >> age;
            session.recommendations->setAge(age);
            cout << "What gender are you?" << endl;
            cout << "1. Female" << endl;
            cout << "2. Male" << endl;
            cin >> gender;
            session.recommendations->setGender(gender);
            cout << "Enter preferred ranking (e.g. 7.5)" << endl;
            cin >> rating;
            session.recommendations->setRating(rating);
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
            session.recommendations->setCountries(countries);
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
            directors.insert(director);
            session.recommendations->setDirectors(directors);
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
            session.recommendations->setWriters(writers);
        }
        else if (n == 9)
        {
            cout << "Enter number of recommendations" << endl;
            cin >> number;
            session.recommendations->setNumber(number);
            cout << "Which implementation would you like?" << endl;
            cout << "1. Unordered Map" << endl;
            cout << "2. Priority Queue" << endl;
            cin >> implementation;
            cout << "Your top " << number << " movies are:" << endl;
            if (implementation == 1)
                session.recommendations->buildMinMap(session.movies);
            else
                session.recommendations->buildMinQueue();
            cout << "Would you like more information about any of these movies? (Y/N)" << endl;
            cin >> data;
            if (data == "Y")
            {
                cin.ignore();
                cout << "Which of the above movies would you like more information on?" << endl;
                getline(cin, data);
                cout << data << endl;
                session.recommendations->printMovieInfo(data);
            }
        }
        cout << endl;
    }
    cout << "Thank you for using MovieNaviGator!" << endl;
    return 0;
};