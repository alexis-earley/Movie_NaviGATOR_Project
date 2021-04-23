#include <iostream>
#include <vector>
#include "Movie.cpp"
using namespace std;

//min PQ implementation
class PriorityQueue
{
private:
    vector<Movie *> minPQ;
    vector<Movie *> maxPQ;
    int size;

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
        for (Movie *m : heap)
            cout << m->title << ": " << m->match << endl;
    }

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

    // Recursive heapify-up algorithm
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

    void PQRemove(vector<Movie *> &heap)
    {
        heap[0] = heap.back();
        //cout << "Removing " << heap.back()->title << ": " << heap.back()->match << endl;
        heap.pop_back();
        size--;
        heapifyDown(heap, size, 0);
    }

    void PQInsert(vector<Movie *> &heap, Movie *temp)
    {
        heap.push_back(temp);
        size++;
        heapifyUp(heap, size - 1);
    }

    void makeMatch(unordered_map<string, Movie *> &movies, string id)
    {
        movies[id]->match = ((double)rand() / (RAND_MAX));
    }

    //Code referenced to GeeksforGeeks
    void buildMinPQ(unordered_map<string, Movie *> &movies)
    {

        for (auto it = movies.begin(); it != movies.end(); ++it)
        {
            current++;
            makeMatch(movies, it->first);
            PQInsert(minPQ, it->second);
            //cout << "Inserting " << it->second->title << ": " << it->second->match << endl;
            if (minPQ.size() > number)
                PQRemove(minPQ);
            it++;
        }
        buildMaxPQ();
    }

    void buildMaxPQ()
    {
        maxPQ = minPQ;
        for (int i = (size - 2) / 2; i >= 0; --i)
            heapifyDown(maxPQ, i, size);
        PQPrint(maxPQ);
    }
};

int main()
{
    MovieNaviGATOR session;
    PriorityQueue Q;
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
            Q.setLanguages(languages);
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
            Q.setGenres(genres);
        }
        else if (n == 3)
        {
            cout << "Range of durations (in minutes) in database: 40 - 800" << endl;
            cout << "Enter preferred duration in minutes (e.g. 120)" << endl;
            cin >> duration;
            Q.setDuration(duration);
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
            Q.setActors(actors);
        }
        else if (n == 5)
        {
            cout << "Which age group do you fall into?" << endl;
            cout << "1. 0-18 years" << endl;
            cout << "2. 19-30 years" << endl;
            cout << "3. 31-45 years" << endl;
            cout << "4. 46+ years" << endl;
            cin >> age;
            Q.setAge(age);
            cout << "What gender are you?" << endl;
            cout << "1. Female" << endl;
            cout << "2. Male" << endl;
            cin >> gender;
            Q.setGender(gender);
            cout << "Enter preferred ranking (e.g. 7.5)" << endl;
            cin >> rating;
            Q.setRating(rating);
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
            Q.setCountries(countries);
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
            Q.setDirectors(directors);
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
            Q.setWriters(writers);
        }
        else if (n == 9)
        {
            cout << "Enter number of recommendations" << endl;
            cin >> number;
            Q.setNumber(number);
            cout << "Your top " << number << " movies are:" << endl;
            Q.buildMinPQ(session.movies);
            /*cout << "Would you like more information about any of these movies? (Y/N)" << endl;
            cin >> data;
            if (data == "Y")
            {
                cin.ignore();
                cout << "Which of the above movies would you like more information on?" << endl;
                getline(cin, data);
                cout << data << endl;
                session.printMovieInfo(data);
            }*/
        }
        cout << endl;
    }
    cout << "Thank you for using MovieNaviGATOR!" << endl;
};