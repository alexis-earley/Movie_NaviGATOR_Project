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
    double avg_votes[3][5] = {0}; //holds ratings across different age groups and genders
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

    void remove(string id)
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

//min PQ implementation
class PriorityQueue
{
private:
    vector<Movie *> minPQ;
    vector<Movie *> maxPQ;
    int size;
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

public:
    PriorityQueue()
    {
        size = 0;
        capacity = 100;

        duration = 0;
        rating = 0.0;
        number = 0;
        age = 0;
        gender = 0;
    }

    ~PriorityQueue()
    { //destructor
        minPQ.clear();
        maxPQ.clear();
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

    void PQPrint(vector<Movie *> &heap)
    {
        while (number > 0)
        {
            cout << heap[0]->title << ": " << heap[0]->match << endl;
            PQRemove(heap);
            number--;
        }
    }

    //https://www.geeksforgeeks.org/insertion-and-deletion-in-heaps/
    void heapifyDown(vector<Movie *> &heap, int &size, int index)
    {
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        // If left child is larger than root
        if (left < size && heap[left] > heap[largest])
            largest = left;

        // If right child is larger than largest so far
        if (right < size && heap[right] > heap[largest])
            largest = right;

        // If largest is not root
        if (largest != index)
        {
            swap(heap[index], heap[largest]);

            // Recursively heapify the affected sub-tree
            heapifyDown(heap, size, largest);
        }
    }

    // https://www.geeksforgeeks.org/insertion-and-deletion-in-heaps/
    void heapifyUp(vector<Movie *> &heap, int i)
    {
        int parent = (i - 1) / 2;

        if (parent >= 0)
        {
            if (heap[i] > heap[parent])
            {
                // swap the two if heap property is violated
                swap(heap[i], heap[parent]);

                // call heapify-up on the parent
                heapifyUp(heap, parent);
            }
        }
    }

    //https://www.geeksforgeeks.org/insertion-and-deletion-in-heaps/
    void PQRemove(vector<Movie *> &heap)
    {
        heap[0] = heap.back();
        //cout << "Removing " << heap.back()->title << ": " << heap.back()->match << endl;
        heap.pop_back();
        size--;
        heapifyDown(heap, size, 0);
    }

    //https://www.geeksforgeeks.org/insertion-and-deletion-in-heaps/
    void PQInsert(vector<Movie *> &heap, Movie *temp)
    {
        heap.push_back(temp);
        //cout << "Inserting " << heap.back()->title << ": " << heap.back()->match << endl;
        size++;
        heapifyUp(heap, size - 1);
    }

    void makeMatch(UnorderedMap &movies, string id)
    {
        movies.find(id)->match = ((double)rand() / (RAND_MAX));
    }

    //Code referenced to GeeksforGeeks
    void buildMinPQ(UnorderedMap &movies)
    {
        for (int i = 0; i < movies.table.size(); i++)
        {
            for (int j = 0; j < movies.table[i].size(); j++)
            {
                makeMatch(movies, movies.table[i][j].first);
                if (minPQ.size() > capacity && movies.table[i][j].second->match < minPQ[0]->match)
                    continue;
                PQInsert(minPQ, movies.table[i][j].second);
                if (minPQ.size() > capacity)
                    PQRemove(minPQ);
            }
        }
        //PQPrint(minPQ);
        buildMaxPQ();
    }

    //https://www.geeksforgeeks.org/convert-min-heap-to-max-heap/
    void buildMaxPQ()
    {
        maxPQ = minPQ;
        for (int i = (size - 2) / 2; i >= 0; --i)
            heapifyDown(maxPQ, i, size);
        PQPrint(maxPQ);
    }

    Movie *find(string title)
    {
        bool found = false;
        for (Movie *m : maxPQ)
        {
            if (m->title == title)
            {
                found = true;
                return m;
            }
        }
        return nullptr;
    }

    void printMovieInfo(string title)
    {
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
            cout << "file is open!" << endl;
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
    { //constructor helper function; removes quotes from a string, if present
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
            cout << "Your top " << number << " movies are:" << endl;
            session.recommendations->buildMinPQ(session.movies);
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
};