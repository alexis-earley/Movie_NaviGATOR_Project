#include "Movie.cpp"

using namespace std;

int main()
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
    cout << "Thank you for using MovieNaviGATOR!" << endl;
};