#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;
const string events_file_name = "events.txt";
string greeting_message = "Welcome! Today is ";
// to-do: add other messages as a constants

vector<string> menu_options{"Show calendar", "Show schedule", "List events", "Add event", "Remove event", "Set first weekday"};

const int day_of_week_start_index = 0;
const int month_start_index = 4;
const int day_of_month_start_index = 8;
const int year_start_index = 20;

int user_input;
vector<vector <string>> events_information;

//help functions
string format_date(char * date_time);
string format_event(vector<string> event);
int compare_dates(string date1, string date2);
bool validate_dates(string start, string end);

void sort_events();
bool compare_events(const vector<string> & event1, const vector<string> & event2);
void parse_events();
string parse_date(int & index, string & buffer);
string parse_event_name(int & index, string & buffer);

void start();
void print_menu();
void check_menu_option(int n);
void invalid_menu_option();
void close_program();

// menu functions
void show_calendar(); // to-do
void show_schedule(); // to-do
void list_events(); // to-do
void add_event(); // to-do
void remove_event(); // to-do
void set_first_weekday(); // to-do

int main()
{
    start();

    string input;
    getline(cin, input);
    user_input = input.at(0) - '0';

    check_menu_option(user_input);

    return 0;
}

void print_menu()
{
    cout << "Choose an option : " << endl;

    for (int i = 0; i < menu_options.size(); i++)
    {
        cout << "\t" << i + 1 << ". " << menu_options.at(i) << endl;
    }

    cout << "Enter your chose : " << endl;

}

void show_calendar()
{
    // to-do
}

void show_schedule()
{
    // to-do
}
void list_events() // to-do: sort it
{
    sort_events();

    cout << "You have the followings events: " << endl;

    for (int i = 0; i < events_information.size(); i++)
    {
        cout << i + 1 << ". ";
        cout << format_event(events_information[i]) << endl;
    }
}

void add_event()
{
    string event_name;
    string start_date;
    string end_date;

    cout << "Enter name : " << endl;
    getline(cin, event_name, '\n');
    cout << "Enter start date (DD/MM/YYYY) : " << endl;
    getline(cin, start_date);
    cout << "Enter end date (DD/MM/YYYY) : " << endl;
    getline(cin, end_date);

    bool is_valid = validate_dates(start_date, end_date);
    if (is_valid)
    {
        //to-do: check if events contains current event name
        //to-do: add events multiple times
        fstream events_file_write;

        events_file_write.open(events_file_name, std::fstream::out | std::fstream::app);

        if (events_file_write.is_open() == false)
        {
            std::cerr << "Failed to open file";

            return;
        }

        // save information to file
        events_file_write << start_date << " " << end_date << " " << event_name << "\n";

        events_file_write.close();

        // add information into events_information
        vector<string> event = {start_date, end_date, event_name};
        events_information.push_back(event);
    }
    else
    {
        int counter_invalid_inputs = 0;
        do
        {
            if (counter_invalid_inputs > 5)
            {
                cout << "You try to input fifth times invalid date!" << endl;
                close_program();
            }
            cout << "Error! end date must be after start date" << endl;
            cout << "Enter end date (DD/MM/YYYY) : ";
            cin >> end_date;

            is_valid = validate_dates(start_date, end_date);
            counter_invalid_inputs++;
        }
        while (!is_valid);

    }
}

void remove_event()
{
    // to-do
}

void set_first_weekday()
{
    // to-do
}

/*
 * return 1  if date1 is before date2
 * return -1 if date1 is after date2
 * return 2  if date1 is equal to date2
 * return -2 for invalid date format
 */
int compare_dates(string date1, string date2)
{
    if (date1.size() != date2.size())
        return -2;

    int c1 = 0;
    int c2 = 0;

    for (int i = 0; i < date1.size(); i++)
    {
        c1 = date1.at(i) - '0';
        c2 = date2.at(i) - '0';

        // 12/11/2023 or 12.11.2023
        if ( (c1 == '/' - '0' || c1 == '.' - '0')
             && (c2 == '/' - '0' || c2 == '.' - '0'))
            continue;

        if (c1 > c2) return -1;
        else if (c1 < c2) return 1;
    }

    return 2;
}

// check if end date is before start date
bool validate_dates(string start, string end)
{
    int compare = compare_dates(start, end);

    if (compare == -2 || compare == -1)
        return false;

    return true;
}

void start()
{
    time_t now = time(0);
    char* date_time = ctime(&now);
    string time = format_date(date_time);

    // parse information of events_file if it exists
    fstream fileStream;
    fileStream.open(events_file_name);
    if (!fileStream.fail())
    {
       parse_events();
    }

    cout << greeting_message << time << endl;

    // to-do: You have 1 event tomorrow.

    print_menu();
}

void close_program()
{
    //to-do : save information into files and close files

    exit(-1);
}

string format_date(char * date_time)
{
    string year = {date_time[year_start_index], date_time[year_start_index + 1], date_time[year_start_index + 2], date_time[year_start_index + 3]};
    string day = {date_time[day_of_month_start_index], date_time[day_of_month_start_index + 1]};
    string day_of_week = "";
    string month = "";

    // for day of week
    if (date_time[day_of_week_start_index] == 'M') day_of_week = "Monday";
    else if(date_time[day_of_week_start_index] == 'T' && date_time[day_of_week_start_index + 1] == 'u') day_of_week = "Tuesday";
    else if(date_time[day_of_week_start_index] == 'W') day_of_week = "Wednesday";
    else if(date_time[day_of_week_start_index] == 'T' && date_time[day_of_week_start_index + 1] == 'h') day_of_week = "Thursday";
    else if(date_time[day_of_week_start_index] == 'F') day_of_week = "Friday";
    else if(date_time[day_of_week_start_index] == 'S' && date_time[day_of_week_start_index + 1] == 'a') day_of_week = "Saturday";
    else if(date_time[day_of_week_start_index] == 'S' && date_time[day_of_week_start_index + 1] == 'u') day_of_week = "Sunday";

    // for month
    if (date_time[month_start_index] == 'J') month = "January";
    else if (date_time[month_start_index] == 'F') month = "February";
    else if (date_time[month_start_index] == 'M' && date_time[month_start_index + 2] == 'r') month = "March";
    else if (date_time[month_start_index] == 'A' && date_time[month_start_index + 2] == 'r') month = "April";
    else if (date_time[month_start_index] == 'M' && date_time[month_start_index + 2] == 'y') month = "May";
    else if (date_time[month_start_index] == 'J' && date_time[month_start_index + 2] == 'n') month = "June";
    else if (date_time[month_start_index] == 'J' && date_time[month_start_index + 2] == 'l') month = "July";
    else if (date_time[month_start_index] == 'A' && date_time[month_start_index + 2] == 'g') month = "August";
    else if (date_time[month_start_index] == 'S') month = "September";
    else if (date_time[month_start_index] == 'O') month = "October";
    else if (date_time[month_start_index] == 'N') month = "November";
    else if (date_time[month_start_index] == 'D') month = "December";

    return day_of_week + ", " + day + " " + month + " " + year + ".";
}

void check_menu_option(int n)
{
    if (n == -1)
    {
        close_program();
    }
    switch (n)
    {
        case 1: show_calendar(); break;
        case 2: show_schedule(); break;
        case 3: list_events(); break;
        case 4: add_event(); break;
        case 5: remove_event(); break;
        case 6: set_first_weekday(); break;

        default: invalid_menu_option(); break;
    }
}

void invalid_menu_option()
{
    cout << "Your choose was invalid." << endl;
    cout << "Please enter correct number or -1 to close the program : ";
    cin >> user_input;
}

bool compare_events(const vector<string> & event1, const vector<string> & event2)
{
    int compare_start_dates = compare_dates(event1[0], event2[0]);

    if (compare_start_dates == 2)
    {
        int compare_event_names = event1[2].compare(event2[2]);
        return compare_event_names >= 0; //? of just >
    }

    return compare_start_dates > 0;
}

void sort_events()
{
    sort(events_information.begin(), events_information.end(), compare_events);
}
void parse_events()
{
    string buffer1, start_date, end_date, event_name;
    fstream file_events_reading;
    file_events_reading.open(events_file_name, fstream::in);

    if (file_events_reading.is_open())
    {
        while(getline(file_events_reading, buffer1)) // read line by line
        {
            int current_char_index = 0;

            start_date = parse_date(current_char_index, buffer1);
            end_date = parse_date(++current_char_index, buffer1);
            event_name = parse_event_name(++current_char_index, buffer1);

            vector<string> event = {start_date, end_date, event_name};
            events_information.push_back(event);
        }
    }

    file_events_reading.close();
}

string parse_date(int & index, string & buffer)
{
    string date;

    while (index < buffer.size() && buffer[index] != ' ')
    {
        date += buffer[index++];
    }

    return date;
}

string parse_event_name(int & index, string & buffer)
{
    string event;

    while (index < buffer.size())
    {
        event += buffer[index++];
    }

    return event;
}

string format_event(vector<string> event)
{
    string result = event[2] + " ("; // event[2] - event_name

    int compare = compare_dates(event[0], event[1]); // event[0] - start date; event[1] - end date

    if (compare == 2) // if start date = end date
    {
        result += event[0];
    }
    else
    {
        result += event[0] + " - " + event[1];
    }

    result += ")";

    return result;
}


bool contains_event_name(string event_name)
{
    // to-do
    return false;
}
