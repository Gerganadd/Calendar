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
vector<string> months{"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
//vector<string> day_of_week{"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

const int day_of_week_start_index = 0;
const int month_start_index = 4;
const int day_of_month_start_index = 8;
const int year_start_index = 20;

int user_input;
vector<vector <string>> events_information;

//help functions
string format_month_and_year(string input);
string format_date(char * date_time);
string format_event(vector<string> event);
int compare_dates(string date1, string date2);
bool validate_dates(string start, string end);
bool contains_event_name(string event_name);

int parse_string_to_integer(string s);
long pow(int base, int power);

int difference_between(string start_date, string end_date);
string weekday(int day, int month, int year);
bool is_leap_year(int year);
int day_of_month(int month, int year);
int get_day(string date);
int get_month(string date);
int get_year(string date);

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
void show_events_for_month(); // to-do
void list_events();
void add_event();
void remove_event();
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
}

void show_events_for_month()
{
    cout << "Enter month (MM/YYYY) : " << endl;
    string input;
    getline(cin, input);

    string month_year = format_month_and_year(input);
    cout << month_year << endl;
    cout << string(month_year.size(), '-') << endl;

    sort_events();

    int current_month = get_month(input);
    int current_year = get_year(input);
    int month_total_days = day_of_month(current_month, current_year);

    int day = 0;
    int month = 0;
    int year = 0;
    int days_difference = 0;

    vector<string> * event_names_array = new vector<string>[month_total_days]{};

    // save information in event_names_array
    for (int i = 0; i < events_information.size(); i++)
    {
        bool match = events_information[i][0].substr(3, events_information[i][0].size() - 1) == input;
        if (!match)
            continue;

        day = get_day(events_information[i][0]);
        month = get_month(events_information[i][0]);
        year = get_year(events_information[i][0]);

        string info = events_information[i][2];

        // when start and end date are different
        if (compare_dates(events_information[i][0], events_information[i][1]) != 2)
        {
            days_difference = difference_between(events_information[i][0], events_information[i][1]);

            for (int k = 0; k < days_difference; k++)
            {
                if (day + k > month_total_days)
                {
                    break;
                }

                string current_day = " (Day " + to_string(k + 1) + "/" + to_string(days_difference) + ")";
                event_names_array[day + k].push_back(info + current_day);
            }
        }
        else
        {
            event_names_array[day + i].push_back(info);
        }
    }

    // print
    for (int i = 0; i < month_total_days; i++)
    {
        if (event_names_array[i].empty())
            continue;

        string day_of_week = weekday(i, month, year);

        cout << day_of_week << ", " << i << string(3, ' ');

        for (int k = 0; k < event_names_array[i].size(); k++)
        {
            if (k != 0) cout << string(9, ' ');
            cout << event_names_array[i][k] << endl;
        }
    }

    // delete event_name_array
    for (int i = 0; i < month_total_days; i++)
    {
        if (!event_names_array[i].empty())
        {
            event_names_array[i].clear();
        }
    }
    delete[] event_names_array;
}

void list_events()
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
    bool is_contains_name = contains_event_name(event_name);

    if (is_valid && !is_contains_name)
    {
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
    else if (is_contains_name)
    {
        int counter_invalid_inputs = 0;
        do
        {
            if (counter_invalid_inputs > 5)
            {
                cout << "You try to input fifth times invalid name!" << endl;
                close_program();
            }
            cout << "Error! There is already an event with that name!" << endl;
            cout << "Enter event name : ";
            cin >> event_name;

            is_contains_name = contains_event_name(event_name);
            counter_invalid_inputs++;
        }
        while (is_contains_name);
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

    cout << "Event added successfully!" << endl;
}

void remove_event()
{
    string event_name;
    cout << "Enter name : ";
    getline(cin, event_name);

    // find index of event
    int index = -1;
    for (int i = 0; i < events_information.size(); i++)
    {
        string current_name = events_information[i][2];
        bool has_match = (current_name == event_name);
        if (has_match)
        {
            index = i;
            break;
        }
    }

    if (index != -1)
    {
        // delete from event_information
        int size = events_information.size();

        events_information[index] = events_information[size - 1];
        events_information.pop_back();

        // delete from file
        fstream file_delete;

        file_delete.open(events_file_name, fstream::out);

        if (file_delete.is_open())
        {
            for (int i = 0; i < events_information.size(); i++)
            {
                file_delete << events_information[i][0] << " " << events_information[i][1] << " " << events_information[i][2] << "\n";
            }

            file_delete.close();
        }

        cout << "Event deleted successfully!" << endl;
    }
    else
    {
        cout << "There is no such event!" << endl;
    }
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
        case 2: show_events_for_month(); break;
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
    for (int i = 0; i < events_information.size(); i++)
    {
        if (events_information[i][2] == event_name)
            return true;
    }

    return false;
}

string weekday(int day, int month, int year)
{
    int leap_years = (int) year/ 4;
    long a = (year - leap_years) * 365 + leap_years * 366;

    if(month >= 2) a += 31;
    if(month >= 3 && (int)year/4 == year/4) a += 29;
    else if(month >= 3) a += 28;
    if(month >= 4) a += 31;
    if(month >= 5) a += 30;
    if(month >= 6) a += 31;
    if(month >= 7) a += 30;
    if(month >= 8) a += 31;
    if(month >= 9) a += 31;
    if(month >= 10) a += 30;
    if(month >= 11) a += 31;
    if(month == 12) a += 30;
    a += day;

    int n = (a - 2)  % 7;
    switch(n)
    {
        case 0:
            return "Sat";
        case 1:
            return "Sun";
        case 2:
            return "Mon";
        case 3:
            return "Tue";
        case 4:
            return "Wed";
        case 5:
            return "Thu";
        case 6:
            return "Fri";
    }

    return " ";
}

string get_month_name(string month)
{
    if (month == "01") return months.at(0);
    if (month == "02") return months.at(1);
    if (month == "03") return months.at(2);
    if (month == "04") return months.at(3);
    if (month == "05") return months.at(4);
    if (month == "06") return months.at(5);
    if (month == "07") return months.at(6);
    if (month == "08") return months.at(7);
    if (month == "09") return months.at(8);
    if (month == "10") return months.at(9);
    if (month == "11") return months.at(10);
    if (month == "12") return months.at(11);

    return "Error! Invalid month";
}

string format_month_and_year(string input) // input - MM/YYYY
{
    string year =  input.substr(3, input.size());
    string month = get_month_name(input.substr(0, 2));

    return month + " " + year;
}

long pow(int base, int power)
{
    if (power == 0) return 1;

    long result = 1l;
    for (int i = 0; i < power; i++)
    {
        result *= base;
    }

    return result;
}

int parse_string_to_integer(string s)
{
    unsigned int result = 0u;
    int power = s.size() - 1;

    int size = s.size();
    int current_n;

    for (int i = 0; i < size; i++)
    {
        current_n = (s[size - i - 1]  - '0');
        result +=  current_n * pow(10, i);
    }

    return result;
}

bool is_leap_year(int year)
{
    bool is_leap_year = false;

    if (year % 4 == 0 && year % 100 != 0) is_leap_year = true;
    else if (year % 100 == 0) is_leap_year = (year % 400 == 0);

    return is_leap_year;
}

int day_of_month(int month, int year)
{
    bool leap_year = is_leap_year(year); //false = 0; true = 1;

    if (month > 7)
    {
        return (30 + (month % 2 == 0));
    }
    else if (month == 2)
    {
        return (28 + (leap_year % 2));
    }
    else
    {
        return (30 + (month % 2));
    }
}

//day1 and month1 - start date
//day2 and month2 - end date
int days_between_dates_in_same_year(int day1, int month1, int day2, int month2, int year)
{
    int diff_days_in_current_month = day_of_month(month1, year) - day1;
    int diff_months = month2 - month1;
    int sum_days = 0;

    if (diff_months > 1)
    {
        for (int i = 1; i < diff_months; i++)
        {
            sum_days += day_of_month(month2 - i, year);
        }
    }

    return day2 + diff_days_in_current_month + sum_days + 1;
}

int difference_between(string start_date, string end_date)
{
    int day1 = get_day(start_date);
    int month1 = get_month(start_date);
    int year1 = get_year(start_date);

    int day2 = get_day(end_date);
    int month2 = get_month(end_date);
    int year2 = get_year(end_date);

    if (month1 == month2 && year1 == year2)
    {
        return day2 - day1 + 1;
    }

    if (year1 == year2)
    {
        return days_between_dates_in_same_year(day1, month1, day2, month2, year2);
    }

    // when year1 != year2
    int diff_years = year2 - year1;

    int days_till_end_year1 = days_between_dates_in_same_year(day1, month1, 31, 12, year1);
    int days_from_begin_year2 = days_between_dates_in_same_year(1, 1, day2, month2, year2);

    int sum_days = 0;

    if (diff_years > 1)
    {
        for (int i = 1; i < diff_years; i++)
        {
            sum_days += 365 + is_leap_year(year2 - i);
        }
    }

    return days_till_end_year1 + sum_days + days_from_begin_year2;
}

int get_day(string date)
{
    return parse_string_to_integer(date.substr(0, 2));
}

int get_month(string date)
{
    return parse_string_to_integer(date.substr(3, 2));
}

int get_year(string date)
{
    return parse_string_to_integer(date.substr(6, 4));
}