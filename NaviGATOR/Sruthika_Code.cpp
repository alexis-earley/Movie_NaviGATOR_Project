#include <iostream>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>
#include <windows.h>
using namespace std;

class Movie
{ //holds the data for each node in the tree
public:
    string id;
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
        id = "";
        match = 0;
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
    double totalscorecalc(double myear, double mgenre, double mdura, double mcoun, double mdirec, double mwriters, double mprodu, double mactors, double mlang, double mrating)
    {
        double votes = this->num_votes;
        double newyear = 0.0, newgenre = 0.0, newdura = 0.0, newcoun = 0.0, newdirec = 0.0, newwriters = 0.0, newprodu = 0.0, newactors = 0.0, newlang = 0.0, newrating = 0.0, newnumvotes = 0.0;
        double percentage = (100.0 / 13.0) / 100.0; // finds percentage for all preferences
        unordered_set<string>::iterator itr;
        double countlang = 0.0, countgenre = 0.0, countcoun = 0.0, countdirec = 0.0, countwriters = 0.0, countactors = 0.0;
        for (itr = this->languages.begin(); itr != this->languages.end(); itr++)
        { // count languages of movie
            countlang++;
        }
        for (itr = this->genres.begin(); itr != this->genres.end(); itr++)
        { // count genres of movie
            countgenre++;
        }
        for (itr = this->countries.begin(); itr != this->countries.end(); itr++)
        { // count countries of movie
            countcoun++;
        }
        for (itr = this->directors.begin(); itr != this->directors.end(); itr++)
        { // count directors of movie
            countdirec++;
        }
        for (itr = this->writers.begin(); itr != this->writers.end(); itr++)
        { // count writers of movie
            countwriters++;
        }
        for (auto itr : this->actors)
        { // count actors of movie
            countactors++;
        }

        // the next statements calculate and return the match score in a range from 0 to 1, 1 being the best match and 0 being the worst match
        // these calculations are done by dividing the match score calculated in matchifelse() by the number of languages so that a movie with several languages is not accounted for several times
        // then, the match score is divided by the number of items in the movie and later multiplied by the percentage so that they all add up to a number less than or equal to 1
        if (myear != 0.0) // change other factors to account for how many actually match (if all match, better score)
            newyear = (myear / myear) * percentage;
        if (mgenre != 0.0)
            newgenre = ((mgenre / countlang) / countgenre) * percentage;
        if (mdura != 0.0)
            newdura = (mdura / countlang) * percentage; // divided by number of languages so that the duration is not added by the number of languages (just one duration per movie)
        if (mcoun != 0.0)
            newcoun = ((mcoun / countlang) / countcoun) * percentage;
        if (mdirec != 0.0)
            newdirec = ((mdirec / countlang) / countdirec) * percentage;
        if (mwriters != 0.0)
            newwriters = ((mwriters / countlang) / countwriters) * percentage;
        if (mprodu != 0.0)
            newprodu = (mprodu / mprodu) * percentage;
        if (mactors != 0.0)
            newactors = ((mactors / countlang) / countactors) * percentage;
        if (mlang != 0.0)
            newlang = (mlang / countlang) * percentage;
        if (mrating != 0.0)                                       // rating is given the most weight in the match score calculation so it is multiplied by 3
            newrating = ((mrating / countlang) * percentage) * 1; // divided by number of languages so that the duration is not added by the number of languages (just one duration per movie)
        if (votes >= 0.0 && votes < 10000.0)
            newnumvotes += 0.1 * percentage;
        if (votes >= 10000.0 && votes < 100000.0)
            newnumvotes += 0.5 * percentage;
        if (votes >= 100000.0)
            newnumvotes += 1.0 * percentage;

        double totalscore = (newyear + newgenre + newdura + newcoun + newdirec + newwriters + newprodu + newactors + newlang + newrating + newnumvotes);

        return totalscore;
    }
    double matchscore(int _yearmax, int _yearmin, unordered_set<string> _genre, int _duration, unordered_set<string> _countries, unordered_set<string> _languages, unordered_set<string> _directors, unordered_set<string> _writers, string _production_company, vector<string> _actor, int _age, int _gender)
    {
        double myear = 0.0, mgenre = 0.0, mdura = 0.0, mcoun = 0.0, mlang = 0.0, mdirec = 0.0, mwriters = 0.0, mprodu = 0.0, mactors = 0.0, mrating = 0.0;
        unordered_set<string> mySet = this->languages, mySet2 = this->genres, mySet3 = this->directors, mySet4 = this->writers, mySet5 = this->countries;

        for (auto i : _languages)
        { // languages
            if (i != "")
            {
                if (mySet.find(i) != mySet.end())
                { // iterate through set of languages
                    mlang++;
                    matchesifelse(_yearmax, _yearmin, _genre, _duration, _countries, _languages, _directors, _writers, _production_company, _actor, _age, _gender, myear, mgenre, mdura, mcoun, mdirec, mwriters, mprodu, mactors, mrating, mySet, mySet2, mySet3, mySet4, mySet5); // calculates match scores for every preference
                }
            }
            else // if no languages specified
                matchesifelse(_yearmax, _yearmin, _genre, _duration, _countries, _languages, _directors, _writers, _production_company, _actor, _age, _gender, myear, mgenre, mdura, mcoun, mdirec, mwriters, mprodu, mactors, mrating, mySet, mySet2, mySet3, mySet4, mySet5);
        }
        return totalscorecalc(myear, mgenre, mdura, mcoun, mdirec, mwriters, mprodu, mactors, mlang, mrating); // calculates total match score in range 0 to 1
    }

    void matchesifelse(int _yearmax, int _yearmin, unordered_set<string> _genre, int _duration, unordered_set<string> _countries, unordered_set<string> _languages, unordered_set<string> _directors, unordered_set<string> _writers, string _production_company, vector<string> _actor, int _age, int _gender, double &myear, double &mgenre, double &mdura, double &mcoun, double &mdirec, double &mwriters, double &mprodu, double &mactors, double &mrating, unordered_set<string> mySet, unordered_set<string> mySet2, unordered_set<string> mySet3, unordered_set<string> mySet4, unordered_set<string> mySet5)
    {
        for (auto j : _genre) // genre
        {
            if (mySet2.find(j) != mySet2.end())
            {
                mgenre++;
            }
        }
        if (this->year <= _yearmax && this->year > _yearmin)
        { // year
            myear++;
        }
        if (this->duration <= _duration)
        { // duration
            mdura += ((double)this->duration) / (_duration);
        }
        for (auto k : _countries) //countries
        {
            if (mySet5.find(k) != mySet5.end())
            {
                mcoun++;
            }
        }
        for (auto l : _directors) //directors
        {
            if (mySet3.find(l) != mySet3.end())
            {
                mdirec++;
            }
        }
        for (auto m : _writers) //writers
        {
            if (mySet4.find(m) != mySet4.end())
            {
                mwriters++;
            }
        }
        if (this->production_company == _production_company) //production company
            mprodu++;
        for (int i = 0; i < this->actors.size(); i++) // actors
        {
            for (int j = 0; j < _actor.size(); j++)
            {
                if (this->actors.at(i) == _actor.at(j))
                {
                    mactors++;
                }
            }
        }
        if (_gender == 1) // male
        {
            if (_age == 1) // 18 under
            {
                mrating += this->avg_votes[1][1] / 10.0;
            }
            else if (_age == 2) // 18-30
            {
                mrating += this->avg_votes[1][2] / 10.0;
            }
            else if (_age == 3) // 30-45
            {
                mrating += this->avg_votes[1][3] / 10.0;
            }
            else if (_age == 4) // 45+ under
            {
                mrating += this->avg_votes[1][4] / 10.0;
            }
            else // either, if inputted empty string or number other than 1,
            {
                mrating += this->avg_votes[1][0] / 10.0;
            }
        }
        else if (_gender == 2) // female
        {
            if (_age == 1) // 18 under
            {
                mrating += this->avg_votes[2][1] / 10.0;
            }
            else if (_age == 2) // 18-30
            {
                mrating += this->avg_votes[2][2] / 10.0;
            }
            else if (_age == 3) // 30-45
            {
                mrating += this->avg_votes[2][3] / 10.0;
            }
            else if (_age == 4) // 45+ under
            {
                mrating += this->avg_votes[2][4] / 10.0;
            }
            else // either
            {
                mrating += this->avg_votes[2][0] / 10.0;
            }
        }
        else // either
        {
            if (_age == 1) // 18 under
            {
                mrating += this->avg_votes[0][1] / 10.0;
            }
            else if (_age == 2) // 18-30
            {
                mrating += this->avg_votes[0][2] / 10.0;
            }
            else if (_age == 3) // 30-45
            {
                mrating += this->avg_votes[0][3] / 10.0;
            }
            else if (_age == 4) // 45+ under
            {
                mrating += this->avg_votes[0][4] / 10.0;
            }
            else // either
            {
                mrating += this->avg_votes[0][0] / 10.0;
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
    // vectors to store heap elements
    vector<Movie *> min;
    vector<Movie *> max;
    vector<Movie *> copy;
    int number; //user input for number of recomendations specifies size of min and max heap
    //begin other user inputs for match preferences
    int duration;
    double rating;
    int age;
    int gender;
    int year_max;
    int year_min;
    string production_company;
    unordered_set<string> genres;
    unordered_set<string> languages;
    unordered_set<string> countries;
    unordered_set<string> directors;
    unordered_set<string> writers;
    vector<string> actors;

    //same as for unordered map file parsing
    string testQuotes(string &input)
    { //buildMinQueue helper function; pops quotes from a string, if present
        int lastChar = input.size() - 1;
        string result;
        if ((input[0] == '\"') && (input[lastChar] == '\"'))
        {
            return input.substr(1, lastChar - 1);
        }
        return input;
    }
    //same as for unordered map file parsing
    int intConv(string &input)
    { //buildMinQueue helper function; converts string to integer, if possible
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
    //same as for unordered map file parsing
    unordered_set<string> setConv(string input)
    { //buildMinQueue helper function; breaks apart string by commas and places into an unordered set
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
    //same as for unordered map file parsing
    vector<string> vectorConv(string input)
    { //buildMinQueue helper function; breaks apart string by commas and places into a vector
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

    // Recursively sifts down through the max heap to ensure that the each node's children are smaller than the node, and if not, swapping them
    //https://www.techiedelight.com/min-heap-max-heap-implementation-c/
    void heapify_down_max(int i)
    {
        // get left and right child of node at index i (parent)
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        int largest = i;

        // compare the parent with its left and right child and find the index of the largest value
        if (left < max.size() && max[left]->match > max[i]->match)
            largest = left;

        if (right < max.size() && max[right]->match > max[largest]->match)
            largest = right;

        // if largest isn't the parent, swap with child having greater value and call heapify-down on the child
        if (largest != i)
        {
            swap(max[i], max[largest]);
            heapify_down_max(largest);
        }
    }

    // Recursively sifts down through the max heap to ensure that the each node's children are smaller than the node, and if not, swapping them
    //https://www.techiedelight.com/min-heap-max-heap-implementation-c/
    void heapify_down_copy(int i)
    {
        // get left and right child of node at index i (parent)
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        int largest = i;

        // compare the parent with its left and right child and find the index of the largest value
        if (left < copy.size() && copy[left]->match > copy[i]->match)
            largest = left;

        if (right < copy.size() && copy[right]->match > copy[largest]->match)
            largest = right;

        // if largest isn't the parent, swap with child having greater value and call heapify-down on the child
        if (largest != i)
        {
            swap(copy[i], copy[largest]);
            heapify_down_copy(largest);
        }
    }

    // Recursively sifts up through the max heap to ensure that the each node's children are smaller than the node, and if not, swapping them
    //https://www.techiedelight.com/min-heap-max-heap-implementation-c/https://www.techiedelight.com/min-heap-max-heap-implementation-c/
    void heapify_up_max(int i)
    {
        // check if the parent is smaller than the child
        if (i && max[(i - 1) / 2]->match < max[i]->match)
        {
            // swap the child and the parent
            swap(max[i], max[(i - 1) / 2]);

            // call heapify-up on the parent
            heapify_up_max((i - 1) / 2);
        }
    }

    // Recursively sifts down through the min heap to ensure that the each node's children are larger than the node, and if not, swapping them
    //https://www.techiedelight.com/min-heap-max-heap-implementation-c/
    void heapify_down_min(int i)
    {
        // get the left and right child of node at index i (parent)
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        int smallest = i;

        // compare the parent with its left and right child and find the smallest value
        if (left < min.size() && min[left]->match < min[i]->match)
            smallest = left;

        if (right < min.size() && min[right]->match < min[smallest]->match)
            smallest = right;

        // if smallest isn't the parent, swap with child having smaller value and call heapify-down on the child
        if (smallest != i)
        {
            swap(min[i], min[smallest]);
            heapify_down_min(smallest);
        }
    }

    // Recursively sifts up through the min heap to ensure that the each node's children are larger than the node, and if not, swapping them
    //https://www.techiedelight.com/min-heap-max-heap-implementation-c/
    void heapify_up_min(int i)
    {
        // check if the parent is larger than the child
        if (i && min[(i - 1) / 2]->match > min[i]->match)
        {
            // swap the child and the parent
            swap(min[i], min[(i - 1) / 2]);

            // call heapify-up on the parent
            heapify_up_min((i - 1) / 2);
        }
    }

    // insert Movie into the min heap
    //https://www.techiedelight.com/min-heap-max-heap-implementation-c/
    void push_min(Movie *m)
    {
        // insert new Movie at the end of the vector
        min.push_back(m);

        // get element index and call heapify-up procedure
        int index = min.size() - 1;
        heapify_up_min(index);
    }

    // Function to remove the Movie with the lowest match score (present at the root or index = 0)
    //https://www.techiedelight.com/min-heap-max-heap-implementation-c/
    void pop_min()
    {
        // replace the root of the heap with the last element of the vector
        min[0] = min.back();
        min.pop_back();

        // call heapify-down on the root node
        heapify_down_min(0);
    }

    // Function to remove the Movie with the highest match score (present at the root or index = 0)
    //https://www.techiedelight.com/min-heap-max-heap-implementation-c/
    void pop_max()
    {
        // replace the root of the heap with the last element of the vector
        max[0] = max.back();
        max.pop_back();

        // call heapify-down on the root node
        heapify_down_max(0);
    }

    // Function to remove the Movie with the highest match score (present at the root or index = 0)
    //https://www.techiedelight.com/min-heap-max-heap-implementation-c/
    void pop_copy()
    {
        // replace the root of the heap with the last element of the vector
        copy[0] = copy.back();
        copy.pop_back();

        // call heapify-down on the root node
        heapify_down_copy(0);
    }

    void buildMax()
    {
        //cout << "building max!" << endl;
        while (min.size() > 0)
        {
            push_max(min[0]);
            pop_min();
        }
        show_max();
    }

public:
    //Constructor initializes class variables (user preferences) to default values
    PriorityQueue()
    {
        //number = 0;
        number = 3;
        duration = 0;
        rating = 0.0;
        number = 0;
        age = 0;
        gender = 0;
        year_max = 0;
        year_min = 0;
    }
    //Destructor clears both the min and max heaps
    ~PriorityQueue()
    {
        min.clear();
        max.clear();
    }

    //Redefine class variable per user input
    void setDuration(int d)
    {
        duration = d;
        //cout << "duration!" << endl;
        //cout << this->duration << endl;
    }

    //Redefine class variable per user input
    void setYearMaxMin(int max, int min)
    {
        year_max = max;
        year_min = min;
    }

    //Redefine class variable per user input
    void setProductionCompany(string p) { production_company = p; }

    //Redefine class variable per user input
    void setRating(double r) { rating = r; }

    //Redefine class variable per user input
    void setNumber(int n) { number = n; }

    //Redefine class variable per user input
    void setAge(int a) { age = a; }

    //Redefine class variable per user input
    void setGender(int g) { gender = g; }

    //Redefine class variable per user input
    void setGenres(unordered_set<string> g) { genres = g; }

    //Redefine class variable per user input
    void setLanguages(unordered_set<string> l) { languages = l; }

    //Redefine class variable per user input
    void setCountries(unordered_set<string> c) { countries = c; }

    //Redefine class variable per user input
    void setDirectors(unordered_set<string> d) { directors = d; }

    //Redefine class variable per user input
    void setWriters(unordered_set<string> w) { writers = w; }

    //Redefine class variable per user input
    void setActors(vector<string> a) { actors = a; }

    // insert Movie into the max heap
    //https://www.techiedelight.com/min-heap-max-heap-implementation-c/
    void push_max(Movie *key)
    {
        // insert new Movie at the end of the vector
        max.push_back(key);

        // get element index and call heapify-up procedure
        int index = max.size() - 1;
        heapify_up_max(index);
    }

    void show_max()
    {
        copy = max;
        int i = 1;         //create copy of max heap to use in printMovieInfo function (so that we are not trying to iterate through an empty vector)
        while (number > 0) //return the top 'number' movies in the max heap (number is user input)
        {
            cout << i << ". " << max[0]->title << ": " << max[0]->match << endl;
            pop_max();
            number--;
            i++;
        }
    }

    void buildMinMap(UnorderedMap &movies)
    {
        //cout << this->duration << endl;
        for (int i = 0; i < movies.table.size(); i++)
        {
            for (int j = 0; j < movies.table[i].size(); j++)
            {
                movies.table[i][j].second->match = movies.table[i][j].second->matchscore(this->year_max, this->year_min, this->genres, this->duration, this->countries, this->languages, this->directors, this->writers, this->production_company, this->actors, this->age, this->gender); // calculate match score for each movie in unordered map
                if (min.size() > number && movies.table[i][j].second->match < min[0]->match)
                    continue;
                push_min(movies.table[i][j].second);
                if (min.size() > number)
                    pop_min();
            }
        }
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
            getline(inFile, line);

            string id;                            //holds movie ID, which the map is organized by
            const clock_t begin_time_1 = clock(); //start clock

            while (getline(inFile, line))
            { //for each row in the data file
                Movie *currMovie = new Movie();
                istringstream ss(line);

                string data;
                getline(ss, data, '\t'); //gets the ID
                currMovie->id = data;
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

                currMovie->match = currMovie->matchscore(this->year_max, this->year_min, this->genres, this->duration, this->countries, this->languages, this->directors, this->writers, this->production_company, this->actors, this->age, this->gender); // calculate match score for each movie in unordered map
                push_max(currMovie);
            }
            float resultTime = float(clock() - begin_time_1) / CLOCKS_PER_SEC;

            cout << "It took: " << resultTime << " seconds to parse through the first file, inserting data into the priority queue 85855 times." << endl;
        }
        inFile.close();
        /*inFile.open("C:\\Users\\Pandu\\source\\repos\\NaviGATOR\\NaviGATOR\\imdb_ratings.tsv"); //second file; contains detailed rating information for each movie (referenced by ID)

        if (inFile.is_open())
        {
            //pop heading
            //cout << "ratings is open!" << endl;
            getline(inFile, line);
            //getline(inFile, line);

            string id;    //holds movie ID, which the map is organized by
            string data;  //holds various pieces of important data
            string trash; //holds lines that are ignored

            while (getline(inFile, line))
            {
                istringstream ss(line);

                getline(ss, id, '\t'); //collect the ID for the current movie
                //cout << id << endl;
                Movie *currMovie = find(id); //find the movie with this ID in the map
                //cout << currMovie->id << endl;

                getline(ss, data, '\t'); //collect number of votes across all ages and genders
                currMovie->num_votes = floatConv(data);
                getline(ss, data, '\t'); //collect total number of votes across all ages and genders
                currMovie->avg_votes[0][0] = floatConv(data);

                //set data that includes both genders (not including what has already been set)
                for (int i = 1; i < 5; i++)
                {
                    getline(ss, data, '\t');
                    currMovie->avg_votes[0][i] = floatConv(data);
                }

                //insert male then female data
                for (int i = 1; i < 3; i++)
                {
                    for (int j = 0; j < 5; j++)
                    {
                        getline(ss, data, '\t');
                        currMovie->avg_votes[i][j] = floatConv(data);
                    }
                }
            }
        }
        inFile.close();*/
        //buildMax();
        show_max();
    }

    Movie *find(string id)
    {
        for (Movie *current : min)
        {
            if (current->id == id)
            {
                return current;
            }
        }
        return nullptr;
    }

    void printMovieInfo3(vector<int> indices) //print multiple movies
    {
        vector<Movie *> printing;
        while (copy.size() > 0)
        {
            printing.push_back(copy[0]);
            pop_copy();
        }
        for (int i = 0; i < indices.size(); i++)
        {
            Movie *current = printing[indices[i] - 1];
            if (!current)
                cout << "Movie not found/recommended." << endl;
            else
            {
                cout << "----------------------------" << endl;
                cout << "Title: " << current->title << endl;
                cout << "Original Title: " << current->original_title << endl;
                cout << "Year: " << current->year << endl;
                cout << "Date Published: " << current->date_published << endl;
                cout << "Genres: ";
                printSet(current->genres, current->genres.size() - 1);
                cout << "Duration: " << current->duration << endl;
                cout << "Countries: ";
                printSet(current->countries, current->countries.size() - 1);
                cout << "Languages: ";
                printSet(current->languages, current->languages.size() - 1);
                cout << "Directors: ";
                printSet(current->directors, current->directors.size() - 1);
                cout << "Writers: ";
                printSet(current->writers, current->writers.size() - 1);
                cout << "Production Company: " << current->production_company << endl;
                cout << "Actors: ";
                for (string actor : current->actors)
                    cout << actor << ", ";
                cout << endl;
                cout << "Description: " << current->description << endl;
            }
        }
    }

    void printSet(unordered_set<string> temp, int length)
    {
        for (string s : temp)
            cout << s << ", ";
        cout << endl;
    }

    void show_more()
    {
        int n = 5;
        int i = 1;    //create copy of max heap to use in printMovieInfo function (so that we are not trying to iterate through an empty vector)
        while (n > 0) //return the top 'number' movies in the max heap (number is user input)
        {
            cout << i << ". " << max[0]->title << ": " << max[0]->match << endl;
            pop_max();
            n--;
            i++;
        }
    }
};

class MovieNaviGator
{
public:
    UnorderedMap movies = UnorderedMap(110000); //holds each movie, with the key being the movie ID

    MovieNaviGator()
    { //constructor; reads through files and sets up movie unordered_map
        string line;
        vector<string> myString;
        ifstream inFile;
        inFile.open("C:\\Users\\Pandu\\source\\repos\\NaviGATOR\\NaviGATOR\\imdb_movies.tsv"); //first file; contains main movie data, but no detailed rating data, about 86k lines
        if (inFile.is_open())
        {
            //pop heading
            getline(inFile, line);
            string id; //holds movie ID, which the map is organized by

            const clock_t begin_time_1 = clock(); //start clock

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
            float resultTime = float(clock() - begin_time_1) / CLOCKS_PER_SEC;

            cout << "It took: " << resultTime << " seconds to parse through the first file, inserting data into the unordered map 85855 times." << endl;
        }
        inFile.close();
        /*inFile.open("C:\\Users\\Pandu\\source\\repos\\NaviGATOR\\NaviGATOR\\imdb_ratings.tsv"); //second file; contains detailed rating information for each movie (referenced by ID)

        if (inFile.is_open())
        {
            //pop heading
            getline(inFile, line);
            //cout << "ratings is open!" << endl;
            string id;    //holds movie ID, which the map is organized by
            string data;  //holds various pieces of important data
            string trash; //holds lines that are ignored

            while (getline(inFile, line))
            {
                istringstream ss(line);

                getline(ss, id, '\t');              //collect the ID for the current movie
                Movie *currMovie = movies.find(id); //find the movie with this ID in the map

                getline(ss, data, '\t'); //collect number of votes across all ages and genders
                currMovie->num_votes = floatConv(data);
                getline(ss, data, '\t'); //collect total number of votes across all ages and genders
                currMovie->avg_votes[0][0] = floatConv(data);

                //set data that includes both genders (not including what has already been set)
                for (int i = 1; i < 5; i++)
                {
                    getline(ss, data, '\t');
                    currMovie->avg_votes[0][i] = floatConv(data);
                }

                //insert male then female data
                for (int i = 1; i < 3; i++)
                {
                    for (int j = 0; j < 5; j++)
                    {
                        getline(ss, data, '\t');
                        currMovie->avg_votes[i][j] = floatConv(data);
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
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    cout << "----------------------------" << endl;
    SetConsoleTextAttribute(hConsole, 9);
    cout << "*Welcome to MovieNaviGator!*" << endl;
    SetConsoleTextAttribute(hConsole, 15);
    cout << "----------------------------" << endl;

    int n = 1;
    int duration = 0;
    int yearmin = 0;
    int yearmax = 0;
    double rating = 0;
    int number = 0;
    int age = 0;
    int gender = 0;
    int implementation = 0;
    string proco = "";

    unordered_set<string> genres;
    vector<string> actors;
    unordered_set<string> languages;
    unordered_set<string> countries;
    unordered_set<string> directors;
    unordered_set<string> writers;

    while (n <= 11 && n >= 1)
    {
        string data;
        int dataNum;
        cout << endl;
        SetConsoleTextAttribute(hConsole, 15);
        cout << "Note that input is case-sensitive and multiple inputs should be separated by commas" << endl;
        cout << endl;
        SetConsoleTextAttribute(hConsole, 9);
        cout << "Menu:" << endl;
        SetConsoleTextAttribute(hConsole, 15);
        cout << "1. Enter preferred languages (e.g. English)" << endl;
        cout << "2. Enter preferred genres (e.g. Action)" << endl;
        cout << "3. Enter maximum duration in minutes (e.g. 120)" << endl;
        cout << "4. Enter preferred actors/actresses (e.g. George Clooney)" << endl;
        cout << "5. Enter preferred age/gender" << endl;
        cout << "6. Enter preferred countries (e.g. USA)" << endl;
        cout << "7. Enter preferred directors (e.g. Steven Spielberg)" << endl;
        cout << "8. Enter preferred writers (e.g. Quentin Tarantino)" << endl;
        cout << "9. Enter preferred production company (e.g. Warner Bros)" << endl;
        cout << "10. Enter preferred minimum and maximum year (e.g. 2015)" << endl;
        cout << "11. Recommend movies" << endl;
        cout << "12. Exit" << endl;
        cout << "----------------------------" << endl;
        SetConsoleTextAttribute(hConsole, 9);
        cout << "Please enter a menu option (e.g. 3)" << endl;
        SetConsoleTextAttribute(hConsole, 15);
        cout << "Your choice : ";
        cin >> n;
        if (n == 1)
        {
            cin.ignore();
            SetConsoleTextAttribute(hConsole, 9);
            cout << "Enter preferred language (e.g. English)" << endl;
            SetConsoleTextAttribute(hConsole, 15);
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
            SetConsoleTextAttribute(hConsole, 9);
            cout << "Enter preferred genre (e.g. Action)" << endl;
            SetConsoleTextAttribute(hConsole, 15);
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
            SetConsoleTextAttribute(hConsole, 9);
            cout << "Enter max duration in minutes (e.g. 120)" << endl;
            SetConsoleTextAttribute(hConsole, 15);
            cin >> duration;
        }
        else if (n == 4)
        {
            cin.ignore();
            SetConsoleTextAttribute(hConsole, 9);
            cout << "Enter preferred main actor/actress (e.g. George Clooney)" << endl;
            SetConsoleTextAttribute(hConsole, 15);
            getline(cin, data);
            istringstream ss(data);
            string actor;
            while (getline(ss, actor, ','))
            {
                if (actor[0] == ' ')
                    actor.erase(0, 1);
                actors.push_back(actor);
            }
        }
        else if (n == 5)
        {
            SetConsoleTextAttribute(hConsole, 9);
            cout << "Which age group do you fall into?" << endl;
            SetConsoleTextAttribute(hConsole, 15);
            cout << "1. 0-18 years" << endl;
            cout << "2. 19-30 years" << endl;
            cout << "3. 31-45 years" << endl;
            cout << "4. 46+ years" << endl;
            cin >> age;
            SetConsoleTextAttribute(hConsole, 9);
            cout << "What gender are you?" << endl;
            SetConsoleTextAttribute(hConsole, 15);
            cout << "1. Female" << endl;
            cout << "2. Male" << endl;
            cin >> gender;
        }
        else if (n == 6)
        {
            cin.ignore();
            SetConsoleTextAttribute(hConsole, 9);
            cout << "Enter preferred countries (e.g. USA)" << endl;
            SetConsoleTextAttribute(hConsole, 15);
            getline(cin, data);
            istringstream ss(data);
            string country;
            while (getline(ss, country, ','))
            {
                if (country[0] == ' ')
                    country.erase(0, 1);
                countries.insert(country);
            }
        }
        else if (n == 7)
        {
            cin.ignore();
            SetConsoleTextAttribute(hConsole, 9);
            cout << "Enter preferred directors (e.g. Steven Spielberg)" << endl;
            SetConsoleTextAttribute(hConsole, 15);
            getline(cin, data);
            istringstream ss(data);
            string director;
            while (getline(ss, director, ','))
            {
                if (director[0] == ' ')
                    director.erase(0, 1);
                directors.insert(director);
            }
        }
        else if (n == 8)
        {
            cin.ignore();
            SetConsoleTextAttribute(hConsole, 9);
            cout << "Enter preferred writers (e.g. Quentin Tarantino)" << endl;
            SetConsoleTextAttribute(hConsole, 15);
            getline(cin, data);
            istringstream ss(data);
            string writer;
            while (getline(ss, writer, ','))
            {
                if (writer[0] == ' ')
                    writer.erase(0, 1);
                writers.insert(writer);
            }
        }
        else if (n == 9)
        {
            cin.ignore();
            SetConsoleTextAttribute(hConsole, 9);
            cout << "Enter preferred production company (e.g. Warner Bros)" << endl;
            SetConsoleTextAttribute(hConsole, 15);
            getline(cin, proco);
        }
        else if (n == 10)
        {
            cin.ignore();
            SetConsoleTextAttribute(hConsole, 9);
            cout << "Enter preferred minimum year (e.g. 2012)" << endl;
            SetConsoleTextAttribute(hConsole, 15);
            cin >> yearmin;
            SetConsoleTextAttribute(hConsole, 9);
            cout << "Enter preferred maximum year (e.g. 2012)" << endl;
            SetConsoleTextAttribute(hConsole, 15);
            cin >> yearmax;
        }
        else if (n == 11)
        {
            SetConsoleTextAttribute(hConsole, 9);
            cout << "Enter number of recommendations" << endl;
            SetConsoleTextAttribute(hConsole, 15);
            cin >> number;
            SetConsoleTextAttribute(hConsole, 9);
            cout << "Which implementation would you like?" << endl;
            SetConsoleTextAttribute(hConsole, 15);
            cout << "1. Unordered Map" << endl;
            cout << "2. Priority Queue" << endl;
            cin >> implementation;
            cout << "In case you're curious!" << endl;
            cout << "The MovieNaviGATOR recommends movies by calculating match scores for each movie based on your preferences." << endl;
            cout << "The match score is a number between 0 and 1 that reflects how good of a fit is the movie according to the " << endl;
            cout << "preferences you specified. In this context, a score of 0 represents the worst match and 1 is the best match." << endl;
            cout << "Note that because of the many factors considered, it's normal for your top matches to still be between 0.2 and" << endl;
            cout << "0.6. The following movies are printed in order of best to worst match score." << endl;
            cout << endl;
            cout << "Your top " << number << " movies are:" << endl;

            PriorityQueue recommendations = PriorityQueue();
            recommendations.setLanguages(languages);
            recommendations.setGenres(genres);
            recommendations.setDuration(duration);
            recommendations.setActors(actors);
            recommendations.setAge(age);
            recommendations.setGender(gender);
            recommendations.setCountries(countries);
            recommendations.setDirectors(directors);
            recommendations.setWriters(writers);
            recommendations.setProductionCompany(proco);
            recommendations.setYearMaxMin(yearmax, yearmin);
            recommendations.setNumber(number);

            if (implementation == 1)
            {
                MovieNaviGator *session = new MovieNaviGator();
                recommendations.buildMinMap(session->movies);
            }
            else
            {
                recommendations.buildMinQueue();
            }
            SetConsoleTextAttribute(hConsole, 9);
            cout << "Would you like more information about any of these movies? (Y/N)" << endl;
            SetConsoleTextAttribute(hConsole, 15);
            cin >> data;
            if (data == "Y")
            {
                cin.ignore();
                SetConsoleTextAttribute(hConsole, 9);
                cout << "Which of the above movies would you like more information on? (1, 2, etc.)" << endl;
                SetConsoleTextAttribute(hConsole, 15);
                vector<int> indices;
                getline(cin, data);
                istringstream ss(data);
                string writer;
                while (getline(ss, writer, ','))
                {
                    if (writer[0] == ' ')
                        writer.erase(0, 1);
                    indices.push_back(stoi(writer));
                }
                recommendations.printMovieInfo3(indices);
            }
            break;
        }
        else
        {
            cout << "Invalid input! Please enter a menu option between 1-12: " << endl;
        }
        cout << endl;
    }
    SetConsoleTextAttribute(hConsole, 9);
    cout << "Thank you for using MovieNaviGator!" << endl;
    SetConsoleTextAttribute(hConsole, 15);
    return 0;
};