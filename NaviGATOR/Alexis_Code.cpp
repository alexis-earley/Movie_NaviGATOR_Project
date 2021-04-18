/* #include <iostream>
#include <unordered_map>
#include <map>
#include<set>
using namespace std;
class Node { //holds the data for each node in the tree
public:
    long id;
    string name;
    Node* left;
    Node* right;
    Node() { //default constructor
        id = -1;
        name = "";
        left = nullptr;
        right = nullptr;
    }
    Node(long id, string name) { //constructor with name and ID
        this->id = id;
        this->name = name;
        left = nullptr;
        right = nullptr;
    }
    Node(long id, string name, Node* left, Node* right) { //constructor with name, ID, left and right
        this->id = id;
        this->name = name;
        this->left = left;
        this->right = right;
    }
};
struct comp
{
    template<typename T>
    bool operator()(const T &l, const T &r) const
    {
        return l->id < r->id;
    }
};
int main() {
    Node node1 = Node(1, "node1");
    Node node2 = Node(2, "node2");
    Node node0 = Node(0, "node0");
    set<Node*, comp> st;
    st.insert(&node2);
    st.insert(&node1);
    for (int i = 0; i < 2; i++) {
        auto it = st.begin();
        while (it != st.end()) {
            // Print the element
            std::cout << (*it)->name << endl;
            //Increment the iterator
            it++;
        }
        st.insert(&node0);
        cout << endl;
    }
    //map<Node, Node*> umap;
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
void printSet(unordered_set<string>& mySet) {
    auto it = mySet.begin();
    while (it != mySet.end()) {
        cout << *it << " ";
        it++;
    }
    cout << endl;
}
class Actor {
public:
    int importance;
    string name;
    string character;
    Actor() { //default constructor
        importance = -1;
        name = "";
        character = "";
    }
};
 */

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
        unordered_set<string> actors;
        string description;
        float avg_vote;
        int votes;
        //pair<double, double> ratings[3][5];
        double avg_votes[3][5];
        double num_votes[3][5];

        Movie() { //default constructor
            title = "";
            original_title = "";
            year = -1;
            date_published = "";
            duration = -1;
            production_company = "";
            description = "";
            //avg_vote = -1.0;
            //votes = -1;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 5; j++) {
                    avg_votes[i][j] = -1;
                }
            }
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 5; j++) {
                    num_votes[i][j] = -1;
                }
            }
        }
    };

    MovieRec();
    ~MovieRec();

private:
    unordered_map<string, Movie*> movies;
    int intConv(string& input);
    float floatConv(string& input);
    unordered_set<string> setConv(string input);
    string testQuotes(string& input);
};

int MovieRec::intConv(string& input) { //converts string to integer
    int result;
    try {
        result = stoi(input);
    }
    catch(exception &err) {
        result = -1;
    }
    return result;
}

float MovieRec::floatConv(string& input) {
    float result;
    try {
        result = stof(input);
    }
    catch(exception &err) {
        result = -1;
    }
    return result;
}

unordered_set<string> MovieRec::setConv(string input) { //converts set to integer
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

string MovieRec::testQuotes(string& input) { //removes quotes, if present
    int lastChar = input.size() - 1;
    string result;
    if ((input[0] == '\"') && (input[lastChar] == '\"')) {
        return input.substr(1, lastChar - 1);
    }
    return input;
}

MovieRec::MovieRec() {
    string line;
    vector<string> myString;
    ifstream inFile;
    inFile.open("imdb_movies.tsv");
    if (inFile.is_open()) {
        //remove header
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
            currMovie->actors = setConv(testQuotes(data));
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
    inFile.open("imdb_ratings.tsv");

    if (inFile.is_open()) {
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

MovieRec::~MovieRec() {
    auto it = movies.begin();
    while (it != movies.end()) {
        delete it->second;
        it++;
    }
    movies.clear();
}

int main() {
    MovieRec test;
}