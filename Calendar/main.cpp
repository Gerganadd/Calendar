#include <iostream>
#include <ctime>
#include <vector>
#include <fstream>

using namespace std;

string greeting_message = "Welcome! Today is ";

vector<string> menu_options{"Show calendar", "Show schedule", "List events", "Add event", "Remove event", "Set first weekday"};

const int day_of_week_start_index = 0;
const int month_start_index = 4;
const int day_of_month_start_index = 8;
const int year_start_index = 20;

// functions
string format_date(char * date_time);
void print_menu();
void start();
void show_calendar(); // to-do
void show_schedule(); // to-do
void list_events(); // to-do
void add_event(); // to-do
void remove_event(); // to-do
void set_first_weekday(); // to-do

int main()
{
    start();

    return 0;
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

void print_menu() // may have enum or vector as a parameter
{
    cout << "Choose an option :" << endl;

    for (int i = 0; i < menu_options.size(); i++)
    {
        cout << "\t" << i + 1 << ". " << menu_options.at(i) << endl;
    }

    cout << "Enter your chose :" << endl;

}

void start()
{
    time_t now = time(0);
    char* date_time = ctime(&now);
    string time = format_date(date_time);

    cout << greeting_message << time << endl;

    // to-do: You have 1 event tomorrow.

    print_menu();
}

void show_calendar()
{
    // to-do
}

void show_schedule()
{
    // to-do
}
void list_events()
{
    // to-do
}
void add_event()
{
    // to-do
}

void remove_event()
{
    // to-do
}

void set_first_weekday()
{
    // to-do
}

