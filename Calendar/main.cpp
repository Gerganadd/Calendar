#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

const string events_file_name = "events.txt";
const string settings_file_name = "settings.txt";

const string short_format = "MM/YYYY";
const string full_format = "DD/MM/YYYY";

const string greeting_message = "Welcome! Today is ";
const string tomorrow_events_message = "Yours events tomorrow : ";
const string choose_menu_option_text1 = "Choose an option : ";
const string choose_menu_option_text2 = "Enter your chose : ";
const string enter_short_date_short_format = "Enter month (MM/YYYY) : ";
const string events_message = "You have the followings events: ";
const string event_name_message = "Enter name : ";
const string add_event_start_date_message = "Enter start date (DD/MM/YYYY) : ";
const string add_event_end_date_message = "Enter end date (DD/MM/YYYY) : ";
const string problem_end_date_message = "Error! end date must be after start date";
const string problem_event_name_message = "Error! There is already an event with that name!";
const string problem_open_file = "Failed to open file";
const string problem_delete_event = "There is no such event!";
const string add_successfully_message = "Event added successfully!";
const string delete_successfully_message = "Event deleted successfully!";
const string save_successfully_message = "Saved!";
const string change_first_weekday_message1 = "The first day of the week is currently ";
const string enter_first_week_message = "Enter new (0 - for Monday/ 1 - for Sunday) : ";

vector<string> menu_options{"Show calendar", "Show schedule", "List events", "Add event", "Remove event", "Set first weekday"};
vector<string> months{"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
string array_days_of_week[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

const int days_of_week = 7;
const int month_start_index = 4;
const int day_of_month_start_index = 8;
const int year_start_index = 20;

//help functions
void start(vector<vector<string>> & all_events, vector<vector<string>> & current_month_events, vector<vector<string>> & events_today);
void sort_events(vector<vector<string>> & events);
void print_menu();
void check_menu_option(int n, bool is_sunday_format, vector<vector<string>> & all_events);
void close_program();

string parse_date(int & index, string & buffer);
string parse_event_name(int & index, string & buffer);
vector<vector<string>> parse_events();
bool parse_first_day_of_week();
unsigned int parse_string_to_integer(string s);

string format_today_date();
string format_month_and_year(string & input);
string format_event(vector<string> event);

int compare_dates(string & date1, string & date2);
bool is_valid_date(string & date);
bool validate_dates(string & start, string & end);

bool compare_events(const vector<string> & event1, const vector<string> & event2);
bool contains_event_name(string & event_name, vector<vector<string>> & events);
vector<vector<string>> events_for_month(unsigned int month, unsigned int year, vector<vector<string>> & all_events);
vector<vector<string>> events_for_day(unsigned int day, unsigned int month, unsigned int year, vector<vector<string>> & all_events);
int * convert_events_of_month_into_array(unsigned int month, unsigned int year, vector<vector<string>> month_events);
void print_events(vector<vector<string>> & events);

bool is_leap_year(unsigned int year);
long pow(int base, int power);

unsigned int difference_between(string & start_date, string & end_date);
int get_month_by_name(char * date_time);
int int_day_of_week(unsigned int day, unsigned int month, unsigned int year);
string string_day_of_week(int n, int length);

unsigned int days_of_month(unsigned int month, unsigned int year);
unsigned int get_month(string & date, bool is_short_format = false);
unsigned int get_year(string & date, bool is_short_format = false);
unsigned int get_day(string & date);
unsigned int get_today();
unsigned int get_current_month();
unsigned int get_current_year();
unsigned int get_tomorrow();
unsigned int get_tomorrow_month();
unsigned int get_tomorrow_year();
string get_event_start_date(vector<string> event);
string get_event_end_date(vector<string> event);
string get_event_name(vector<string> event);

void print_calendar(bool is_sunday_format, string input, int * events_array, int days, int first_day_of_week,
                    int today, int today_day_of_week, bool is_current_month_calendar);

// menu functions
void show_calendar(bool is_sunday_format, vector<vector<string>> all_events);
void show_events_for_month(vector<vector<string>> & all_events);
void list_events(vector<vector<string>> & all_events);
void add_event(vector<vector<string>> & all_events);
void remove_event(vector<vector<string>> & all_events);
void set_first_weekday(bool is_sunday_format);

int main()
{
    string time = format_today_date();
    cout << greeting_message << time << endl;

    vector<vector<string>> all_events = parse_events();

    bool is_sunday_format = parse_first_day_of_week();

    vector<vector<string>> current_month_events = events_for_month(get_current_month(), get_current_year(), all_events);
    vector<vector<string>> events_today = events_for_day(get_today(), get_current_month(), get_current_year(), current_month_events);

    start(all_events, current_month_events, events_today);

    print_menu();

    string input;
    getline(cin, input);
    int selected_option = parse_string_to_integer(input);

    check_menu_option(selected_option, is_sunday_format, all_events);

    return 0;
}

void print_menu()
{
    cout << choose_menu_option_text1 << endl;

    for (int i = 0; i < menu_options.size(); i++)
    {
        cout << "\t" << i + 1 << ". " << menu_options.at(i) << endl;
    }

    cout << choose_menu_option_text2 << endl;
}

void show_calendar(bool is_sunday_format, vector<vector<string>> all_events)
{
    // gets today's date
    unsigned int today = get_today();
    unsigned int current_month = get_current_month();
    unsigned int current_year = get_current_year();

    int today_day_of_week = int_day_of_week(today, current_month, current_year);

    // input month and year
    cout << enter_short_date_short_format << endl;
    string input;
    getline(cin, input);

    unsigned int month = get_month(input, true);
    unsigned int year = get_year(input, true);
    unsigned int days = days_of_month(month, year);
    int first_day_of_week = int_day_of_week(1, month, year);

    bool is_current_month = (month == current_month) && (year == current_year);

    vector<vector<string>> current_month_events = events_for_month(month, year, all_events);

    unsigned int events_array_size = days_of_month(month, year) + 1;
    int * events_array = convert_events_of_month_into_array(month, year, current_month_events);

    print_calendar(is_sunday_format, input, events_array, days, first_day_of_week, today, today_day_of_week, is_current_month);
}

void show_events_for_month(vector<vector<string>> & all_events)
{
    // input month and year
    cout << enter_short_date_short_format << endl;
    string input;
    getline(cin, input);

    unsigned int current_month = get_month(input, true);
    unsigned int current_year = get_year(input, true);
    unsigned int month_total_days = days_of_month(current_month, current_year);

    unsigned int day = 0;
    unsigned int month = 0;
    unsigned int year = 0;
    unsigned int days_difference = 0;

    // print name of month and year
    string month_year = format_month_and_year(input);
    cout << month_year << endl;
    cout << string(month_year.size(), '-') << endl;

    vector<vector<string>> month_events = events_for_month(current_month, current_year, all_events);

    sort_events(month_events);

    int array_size = month_total_days + 1;
    vector<string> * event_names_array = new vector<string>[array_size]{};

    // save information in event_names_array
    for (int i = 0; i < month_events.size(); i++)
    {
        string event_start_date = get_event_start_date(month_events[i]);
        string event_end_date = get_event_end_date(month_events[i]);
        string info = get_event_name(month_events[i]);

        day = get_day(event_start_date);
        month = get_month(event_start_date);
        year = get_year(event_start_date);

        // when start and end date are different
        if (compare_dates(event_start_date, event_end_date) != 2)
        {
            days_difference = difference_between(event_start_date, event_end_date);

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
            event_names_array[day].push_back(info);
        }
    }

    // print events
    for (int i = 0; i < array_size; i++)
    {
        if (event_names_array[i].empty())
            continue;

        int d = int_day_of_week(i, month, year);
        string day_of_week = string_day_of_week(d, 2);

        cout << day_of_week << ", " << i << ((i < 10) ? " " : "") << string(3, ' ');

        for (int k = 0; k < event_names_array[i].size(); k++)
        {
            if (k != 0) cout << string(9, ' ');
            cout << event_names_array[i][k] << endl;
        }
    }

    // delete event_name_array
    for (int i = 0; i < array_size; i++)
    {
        if (!event_names_array[i].empty())
        {
            event_names_array[i].clear();
        }
    }
    delete[] event_names_array;
}

void list_events(vector<vector<string>> & all_events)
{
    sort_events(all_events);

    cout << events_message << endl;

    for (int i = 0; i < all_events.size(); i++)
    {
        cout << i + 1 << ". ";
        cout << format_event(all_events[i]) << endl;
    }
}

void add_event(vector<vector<string>> & all_events)
{
    string event_name;
    string start_date;
    string end_date;

    cout << event_name_message << endl;
    getline(cin, event_name, '\n');
    cout << add_event_start_date_message << endl;
    getline(cin, start_date);
    cout << add_event_end_date_message << endl;
    getline(cin, end_date);

    bool is_valid = validate_dates(start_date, end_date);
    bool is_contains_name = contains_event_name(event_name, all_events);

    if (is_valid && !is_contains_name)
    {
        fstream events_file_write;
        events_file_write.open(events_file_name, std::fstream::out | std::fstream::app);
        if (!events_file_write.is_open())
        {
            std::cerr << problem_open_file;

            return;
        }

        // save information to file
        events_file_write << start_date << " " << end_date << " " << event_name << "\n";
        events_file_write.close();

        cout << add_successfully_message << endl;
    }
    else if (is_contains_name)
    {
        cout << problem_event_name_message << endl;
        close_program();
    }
    else
    {
        cout << problem_end_date_message << endl;
        close_program();
    }
}

void remove_event(vector<vector<string>> & all_events)
{
    string event_name;
    cout << event_name_message;
    getline(cin, event_name);

    // find index of event
    int index = -1;
    for (int i = 0; i < all_events.size(); i++)
    {
        string current_name = get_event_name(all_events[i]);
        bool has_match = (current_name == event_name);
        if (has_match)
        {
            index = i;
            break;
        }
    }

    string current_start_date;
    string current_end_date;
    string current_name;

    if (index != -1)
    {
        // delete from events
        int size = all_events.size();

        all_events[index] = all_events[size - 1];
        all_events.pop_back();

        // delete from file
        fstream file_delete;

        file_delete.open(events_file_name, fstream::out);

        if (file_delete.is_open())
        {
            for (int i = 0; i < all_events.size(); i++)
            {
                current_start_date = get_event_start_date(all_events[i]);
                current_end_date = get_event_end_date(all_events[i]);
                current_name = get_event_name(all_events[i]);
                file_delete << current_start_date << " " << current_end_date << " " << current_name << "\n";
            }

            file_delete.close();
        }

        cout << delete_successfully_message << endl;
    }
    else
    {
        cout << problem_delete_event << endl;
    }
}

void set_first_weekday(bool is_sunday_format)
{
    cout << change_first_weekday_message1 << (is_sunday_format ? "Sunday" : "Monday") << endl;
    cout << enter_first_week_message << endl;

    string input;
    getline(cin, input);

    // save in file
    is_sunday_format = parse_string_to_integer(input);

    fstream file_write;
    file_write.open(settings_file_name, fstream::out);
    if (file_write.is_open())
    {
        file_write << is_sunday_format;
    }
    file_write.close();

    cout << save_successfully_message << endl;
}

/*
 * return 1  if date1 is before date2
 * return -1 if date1 is after date2
 * return 2  if date1 is equal to date2
 * return -2 for invalid date format
 */
int compare_dates(string & date1, string & date2)
{
    if (date1.size() != date2.size())
        return -2;

    if (date1.size() == short_format.size())
    {
        unsigned int year1 = get_year(date1, true);
        unsigned int year2 = get_year(date2, true);

        if (year1 < year2) return 1;
        else if (year1 > year2) return -1;

        unsigned int month1 = get_month(date1, true);
        unsigned int month2 = get_month(date2, true);

        if (month1 < month2) return 1;
        else if (month1 > month2) return -1;
    }
    else if (date1.size() == full_format.size())
    {
        unsigned int year1 = get_year(date1, false);
        unsigned int year2 = get_year(date2, false);

        if (year1 < year2) return 1;
        else if (year1 > year2) return -1;

        unsigned int month1 = get_month(date1, false);
        unsigned int month2 = get_month(date2, false);

        if (month1 < month2) return 1;
        else if (month1 > month2) return -1;

        unsigned int day1 = get_day(date1);
        unsigned int day2 = get_day(date2);

        if (day1 < day2) return 1;
        else if (day1 > day2) return -1;
    }
    else
    {
        return -2;
    }

    return 2;
}

bool is_valid_date(string & date)
{
    // to-do;
}

// check if end date is before start date
bool validate_dates(string & start, string & end)
{
    int compare = compare_dates(start, end);

    if (compare == -2 || compare == -1)
        return false;

    return true;
}

void close_program()
{
    exit(-1);
}

string format_today_date()
{
    unsigned int year = get_current_year();
    unsigned int month = get_current_month();
    unsigned int day = get_today();
    int s = int_day_of_week(day, month, year);

    string day_of_week = array_days_of_week[s];
    string m = months[month - 1];

    return day_of_week + ", " + to_string(day) + " " + m + " " + to_string(year) + ".";
}

void check_menu_option(int n, bool is_sunday_format, vector<vector<string>> & all_events)
{
    switch (n)
    {
        case 1: show_calendar(is_sunday_format, all_events); break;
        case 2: show_events_for_month(all_events); break;
        case 3: list_events(all_events); break;
        case 4: add_event(all_events); break;
        case 5: remove_event(all_events); break;
        case 6: set_first_weekday(is_sunday_format); break;

        default: break;
    }
}

bool compare_events(const vector<string> & event1, const vector<string> & event2)
{
    string event1_start = event1[0];
    string event2_start = event2[0];

    int compare_start_dates = compare_dates(event1_start, event2_start);

    if (compare_start_dates == 2)
    {
        int compare_event_names = event1[2].compare(event2[2]);
        return compare_event_names >= 0; //? of just >
    }

    return compare_start_dates > 0;
}

void sort_events(vector<vector<string>> & events)
{
    sort(events.begin(), events.end(), compare_events);
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
    //example 1: shopping (28/02/2023)
    //example 2: trip (10/12/2023 - 24/12/2023)
    string event_name = event[2];
    string event_start = event[0];
    string event_end = event[1];

    string result = event_name + " (";

    int compare = compare_dates(event_start, event_end);

    if (compare == 2) // if start date = end date
    {
        result += event_start;
    }
    else
    {
        result += event_start + " - " + event_end;
    }

    result += ")";

    return result;
}

bool contains_event_name(string & event_name, vector<vector<string>> & events)
{
    string current_name;
    for (int i = 0; i < events.size(); i++)
    {
        current_name = get_event_name(events[i]);
        if (current_name == event_name)
            return true;
    }

    return false;
}

string string_day_of_week(int n, int length = 3)
{
    if (n > days_of_week - 1)
    {
        return " ";
    }

    return array_days_of_week[n].substr(0,length);
}

int int_day_of_week(unsigned int day, unsigned int month, unsigned int year)
{
    month = (month + 9) % 12;
    year -= month / 10;

    unsigned int result = (365*year) + (year/4) - (year/100) + (year/400) + ((month*306 + 5)/10) + (day - 1);
    result %= 7;

    if (result >= 0 && result <= 3) result += 3;
    else result -= 4;

    return result;
}

string format_month_and_year(string & input) // input - MM/YYYY
{
    unsigned int year = get_year(input, true);
    unsigned int month = get_month(input, true);

    return months[month - 1] + " " + to_string(year);
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

unsigned int parse_string_to_integer(string s)
{
    unsigned int result = 0u;
    unsigned int power = s.size() - 1;

    unsigned int size = s.size();
    int current_n;

    for (int i = 0; i < size; i++)
    {
        current_n = (s[size - i - 1]  - '0');
        result +=  current_n * pow(10, i);
    }

    return result;
}

bool is_leap_year(unsigned int year)
{
    bool is_leap_year = false;

    if (year % 4 == 0 && year % 100 != 0) is_leap_year = true;
    else if (year % 100 == 0) is_leap_year = (year % 400 == 0);

    return is_leap_year;
}

unsigned int days_of_month(unsigned int month, unsigned int year)
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
unsigned int days_between_dates_in_same_year(unsigned int day1, unsigned int month1, unsigned int day2, unsigned int month2, unsigned int year)
{
    unsigned int diff_days_in_current_month = days_of_month(month1, year) - day1;
    unsigned int diff_months = month2 - month1;
    unsigned int sum_days = 0;

    if (diff_months > 1)
    {
        for (int i = 1; i < diff_months; i++)
        {
            sum_days += days_of_month(month2 - i, year);
        }
    }

    return day2 + diff_days_in_current_month + sum_days + 1;
}

unsigned int difference_between(string & start_date, string & end_date)
{
    unsigned int day1 = get_day(start_date);
    unsigned int month1 = get_month(start_date);
    unsigned int year1 = get_year(start_date);

    unsigned int day2 = get_day(end_date);
    unsigned int month2 = get_month(end_date);
    unsigned int year2 = get_year(end_date);

    if (month1 == month2 && year1 == year2)
    {
        return day2 - day1 + 1;
    }

    if (year1 == year2)
    {
        return days_between_dates_in_same_year(day1, month1, day2, month2, year2);
    }

    // when year1 != year2
    unsigned int diff_years = year2 - year1;

    unsigned int days_till_end_year1 = days_between_dates_in_same_year(day1, month1, 31, 12, year1);
    unsigned int days_from_begin_year2 = days_between_dates_in_same_year(1, 1, day2, month2, year2);

    unsigned int sum_days = 0;

    if (diff_years > 1)
    {
        for (int i = 1; i < diff_years; i++)
        {
            sum_days += 365 + is_leap_year(year2 - i);
        }
    }

    return days_till_end_year1 + sum_days + days_from_begin_year2;
}

unsigned int get_day(string & date)
{
    string substring = date.substr(0, 2);
    return parse_string_to_integer(substring);
}

// full_format = "dd/mm/yyyy";
// short_format = "mm/yyyy";
unsigned int get_month(string & date, bool is_short_format)
{
    if (is_short_format)
    {
        return parse_string_to_integer(date.substr(0, 2));
    }

    return parse_string_to_integer(date.substr(3, 2));
}

unsigned int get_year(string & date, bool is_short_format)
{
    if (is_short_format)
    {
        return parse_string_to_integer(date.substr(3, 4));
    }

    return parse_string_to_integer(date.substr(6, 4));
}

unsigned int get_today()
{
    time_t now = time(0);
    char * date_time = ctime(& now);

    string str_today = {date_time[day_of_month_start_index], date_time[day_of_month_start_index + 1]};

    return parse_string_to_integer(str_today);
}

unsigned int get_current_month()
{
    time_t now = time(0);
    char * date_time = ctime(& now);

    return get_month_by_name(date_time);
}

unsigned int get_current_year()
{
    time_t now = time(0);
    char * date_time = ctime(& now);

    string str_this_year = {date_time[year_start_index], date_time[year_start_index + 1],
                            date_time[year_start_index + 2], date_time[year_start_index + 3]};

    return parse_string_to_integer(str_this_year);
}

unsigned int get_tomorrow()
{
    unsigned int today = get_today();
    unsigned int days_in_month = days_of_month(get_current_month(), get_current_year());

    if (today + 1 <= days_in_month)
    {
        return today + 1;
    }
    else
    {
        return 1;
    }
}

unsigned int get_tomorrow_month()
{
    if (get_tomorrow() != 1)
    {
        return get_current_month();
    }

    unsigned int month = get_current_month();
    if (month + 1 <= months.size())
    {
        return month + 1;
    }
    else
    {
        return 1;
    }

}

unsigned int get_tomorrow_year()
{
    if (get_tomorrow() == 1 && get_tomorrow_month() == 1)
    {
        return get_current_year() + 1;
    }

    return get_current_year();
}

string get_event_start_date(vector<string> event)
{
    return event[0];
}

string get_event_end_date(vector<string> event)
{
    return event[1];
}

string get_event_name(vector<string> event)
{
    return event[2];
}

vector<vector<string>> parse_events()
{
    vector<vector<string>> events;

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
            events.push_back(event);
        }
    }

    file_events_reading.close();

    return events;
}

bool parse_first_day_of_week()
{
    string text;
    fstream file_read;
    file_read.open(settings_file_name, fstream::in);
    if (file_read.is_open())
    {
        getline(file_read, text);
    }
    file_read.close();

    int n = parse_string_to_integer(text);

    return n > 0;
}

vector<vector<string>> events_for_month(unsigned int month,unsigned int year, vector<vector<string>> & all_events)
{
    vector<vector<string>> events;
    unsigned int current_month;
    unsigned int current_year;
    string current_start_date;

    for (int i = 0; i < all_events.size(); i++)
    {
        current_start_date = get_event_start_date(all_events[i]);
        current_month = get_month(current_start_date, false);
        current_year = get_year(current_start_date, false);

        if (month == current_month && year == current_year)
        {
            vector<string> current_event = all_events[i];

            events.push_back(current_event);
        }
    }

    return events;
}

vector<vector<string>> events_for_day(unsigned int day, unsigned int month, unsigned int year, vector<vector<string>> & all_events)
{
    vector<vector<string>> events;
    unsigned int current_day;
    unsigned int current_month;
    unsigned int current_year;
    string current_start_date;

    for (int i = 0; i < all_events.size(); i++)
    {
        current_start_date = get_event_start_date(all_events[i]);

        current_day = get_day(current_start_date);
        current_month = get_month(current_start_date, false);
        current_year = get_year(current_start_date, false);

        if (day == current_day && month == current_month && year == current_year)
        {
            vector<string> current_event = all_events[i];

            events.push_back(current_event);
        }
    }

    return events;
}

void print_events(vector<vector<string>> & events)
{
    for (int i = 0; i < events.size(); i++)
    {
        cout << i + 1 << ". " << get_event_name(events[i]) << endl;
    }
}

void start(vector<vector<string>> & all_events, vector<vector<string>> & current_month_events, vector<vector<string>> & events_today)
{
    unsigned int today = get_today();
    unsigned int current_month = get_current_month();
    unsigned int current_year = get_current_year();

    if (events_today.empty())
    {
        unsigned int tomorrow = get_tomorrow();
        unsigned int tomorrow_month = get_tomorrow_month();
        unsigned int tomorrow_year = get_tomorrow_year();

        vector<vector<string>> events_tomorrow = events_for_day(tomorrow, tomorrow_month, tomorrow_year, all_events);
        int count_events_tomorrow = events_tomorrow.size();

        cout << tomorrow_events_message << count_events_tomorrow << endl;
    }
    else
    {
        cout << events_message << endl;
        print_events(events_today);
    }
}

int get_month_by_name(char * date_time)
{
    if (date_time[month_start_index] == 'J') return 1;
    else if (date_time[month_start_index] == 'F') return 2;
    else if (date_time[month_start_index] == 'M' && date_time[month_start_index + 2] == 'r') return 3;
    else if (date_time[month_start_index] == 'A' && date_time[month_start_index + 2] == 'r') return 4;
    else if (date_time[month_start_index] == 'M' && date_time[month_start_index + 2] == 'y') return 5;
    else if (date_time[month_start_index] == 'J' && date_time[month_start_index + 2] == 'n') return 6;
    else if (date_time[month_start_index] == 'J' && date_time[month_start_index + 2] == 'l') return 7;
    else if (date_time[month_start_index] == 'A' && date_time[month_start_index + 2] == 'g') return 8;
    else if (date_time[month_start_index] == 'S') return 9;
    else if (date_time[month_start_index] == 'O') return 10;
    else if (date_time[month_start_index] == 'N') return 11;
    else if (date_time[month_start_index] == 'D') return 12;

    return -1;
}

void print_day_of_week(bool is_sunday_format, int today_day_of_week, bool is_current_month_calendar, string & space)
{
    if (is_sunday_format)
    {
        for (int i = 0; i < days_of_week; i++)
        {
            if (is_current_month_calendar && i == today_day_of_week)
            {
                int count = space.size() - 1;
                string current_spaces = string(count, ' ');

                cout << current_spaces << "[" << string_day_of_week(i, 2) << "]" << current_spaces;
            }
            else
            {
                cout << space << string_day_of_week(i, 2) << space;
            }
        }
    }
    else
    {
        for (int i = 1; i <= days_of_week; i++)
        {
            if (is_current_month_calendar && (i % 7) == today_day_of_week)
            {
                int count = space.size() - 1;
                string current_spaces = string(count, ' ');

                cout << current_spaces << "[" << string_day_of_week((i % 7), 2) << "]" << current_spaces;
            }
            else
            {
                cout << space << string_day_of_week((i % 7), 2) << space;
            }
        }
    }
}

void print_empty_days(string & empty_days, int first_day_of_week, bool is_sunday_format)
{
    if (is_sunday_format)
    {
        for (int i = 0; i < first_day_of_week; i++)
        {
            cout << empty_days;
        }

        return;
    }

    if (first_day_of_week == 0) first_day_of_week = 7;

    for (int i = 1; i < first_day_of_week; i++)
    {
        cout << empty_days;
    }
}

void print_days(bool is_sunday_format, int first_day_of_week, int today, bool is_current_month_calendar, string & spaces, int days, int * & events_array)
{
    if (!is_sunday_format)
    {
        if (first_day_of_week == 0)
            first_day_of_week = 7;

        --first_day_of_week;
    }

    for (int i = 1; i <= days; i++)
    {
        if ((i + first_day_of_week) % 7 == 1)
        {
            cout << endl;
        }

        int count_events = events_array[i];

        if (i == today && is_current_month_calendar)
        {
            int count = spaces.size() - 1;
            string current_spaces = string(count, ' ');
            cout << current_spaces << "[" << today << "]";
        }
        else
        {
            cout << spaces << ((i < 10) ? " " : "") << i;
        }

        if (count_events > 0)
        {
            cout << "(" << count_events << ")";

            // when current day has events - symbols are more than we want
            if (i == today && is_current_month_calendar) continue;

            cout << ((count_events < 10) ? " " : "");
        }
        else if (!is_current_month_calendar || i != today)
        {
            cout << spaces;
        }
    }
}

void print_calendar(bool is_sunday_format, string input, int * events_array, int days, int first_day_of_week, int today, int today_day_of_week, bool is_current_month_calendar)
{
    // print name of month and year
    string spaces = string(4, ' ');
    unsigned int count = (spaces.size() * 2 + 2) * days_of_week;

    cout << format_month_and_year(input) << endl;
    cout << string(count, '-') << endl;

    // Mo Tu We Th Fr Sa Su
    print_day_of_week(is_sunday_format, today_day_of_week, is_current_month_calendar, spaces);
    cout << endl;

    string empty_day = spaces + "**" + spaces;
    print_empty_days(empty_day, first_day_of_week, is_sunday_format);

    print_days(is_sunday_format, first_day_of_week, today, is_current_month_calendar, spaces, days, events_array);
}

int * convert_events_of_month_into_array(unsigned int month, unsigned int year, vector<vector<string>> month_events)
{
    unsigned int days = days_of_month(month, year) + 1;
    int * array = new int[days]{};

    unsigned int current_start_date = 0u;
    unsigned int current_end_date = 0u;

    int size = month_events.size();
    for (int i = 0; i < size; i++)
    {
        string start_date = get_event_start_date(month_events[i]);
        string end_date = get_event_end_date(month_events[i]);

        current_start_date = get_day(start_date);
        current_end_date = get_day(end_date);

        unsigned int diff = current_end_date - current_start_date;
        if (diff > 0)
        {
            for (int k = 0; k <= diff; k++)
            {
                array[k + current_start_date]++;
            }
        }
        else
        {
            array[current_start_date]++;
        }
    }

    return array;
}
