// Student name: Yang Kai Yam
// Student ID  : 55676537

/*
	Submission deadline: Friday, 6 March 2020, 12 noon.

	Upload your .cpp file to Canvas.

	Put down your name and student ID at the top of your program file.
	10% marks will be deducted if your program file does not contain your name and ID.

	Submitted program that cannot be compiled (with syntax errors) by Visual Studio
	will receive no more than 30% marks.

	If your program crashes when executed with Visual Studio, you should state the
 	problem at the top of your program file (right below student name and ID); otherwise
	significant percentage (30% or more) of marks will be deducted.

	Late submissions or submissions by other means are not accepted.	
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>
#include "my_bsearch.h"

using namespace std;
string getCurDate();
string adjustYear(const string& date, int y);

// Acceptable to use global variable that is read only.
// today is a constant global variable, set to the date of current local time.
const string today = getCurDate();  

struct person
{
	string dob;     // date of birth, yyyy-mm-dd
	char gender;    // 'F' for female, 'M' for male
	double income;  

	// Define your own public member function(s) if appropriate.
};

ostream& operator<<(ostream& os, const person& p)
{
	os << p.dob << ' ' << p.gender << ' ' << p.income << endl;
	return os;
}

// Other supporting functions required in your design.
int comp(const void* a, const void* b) {
	string *s1 = (string *)a;
	string *s2 = (string *)b;
	if (*s1 < *s2) {
		return -1;
	}
	else if (*s1 > *s2) { 
		return 1; 
	}
	else return 0;
}

void analyse(person *list, int n)
{
	// Analyse persons that belong to the age-groups 20-29, 30-39, 40-49, 50-59, and 60-69.
	// You may assume that the above age-groups are fixed in this exercise.

	// Find the average income of perons in the top quartile of the above age-groups.
	// Print the results to standard output. (See the jpeg file for the requirements).

	// If there are k persons in a group, the number of persons in the top-quartile is k/4.
	// That is the 25% of persons with highest income in the group.

	// If you want to sort the array, use the qsort() function to do the sorting.
	// If you want to search the array, use the bsearch_first() function defined in "my_bsearch.h"
	

	cout << "Reference date is " << today << endl << endl;

	// Your codes
	
	int high = 0;

	for (int low = 20; low <= 60; low += 10) {
		high = low + 9;
		string Lowdate = adjustYear(today, -low);
		string* lowd = (string*)bsearch_first(&Lowdate, list, n - 1, sizeof(person), comp);
		string Highdate = adjustYear(today, -high);
		string* highd = (string*)bsearch_first(&Highdate, list, n - 1, sizeof(person), comp);
		cout << "Average income of persons in the top-quartile of the age-group " << low << " to " << high << " = " << endl << endl;
	}
	;
	


	/*
	// Print the sorted list to a file for debugging/testing purpose.
	// Disable the printing in your submitted program.
	string outfile_name = "sorted-list-" + today + ".txt";
	ofstream outfile(outfile_name);
	outfile << "n " << n << endl;
	for (int i = 0; i < n; i++)
		outfile << setw(4) << i << " : " << list[i];
	outfile.close();
	*/

}

// -------------------------------------------------- functions given to you

person* readDataFile(string filename, int& n)
{
	ifstream infile;
	infile.open(filename);
	if (infile.is_open() == false)
	{
		cout << "Cannot open data file" << endl;
		system("pause");
		exit(0);
	}

	string temp;
	infile >> temp >> n;

	person *list = new person[n];
	for (int i = 0; i < n; i++)
		infile >> list[i].dob >> list[i].gender >> list[i].income;

	infile.close();
	return list;
}

string adjustYear(const string& date, int y)
{
	// Return a string that represents the date where 
	// the year is adjusted by adding the value y.

	return to_string(stoi(date.substr(0, 4)) + y) + date.substr(4);
}

string getCurDate()
{
	// Return a string that represents the date of current time in the format yyyy-mm-dd.

	time_t cur = time(NULL);
	struct tm date;
	localtime_s(&date, &cur);
	int year = 1900 + date.tm_year;
	int month = 1 + date.tm_mon;
	int day = date.tm_mday;

	char s[11];
	_itoa_s(year, s, 10);  // _itoa_s(int value, char *buffer, int radix)
	s[4] = '-';
	s[5] = month / 10 + '0';
	s[6] = month % 10 + '0';
	s[7] = '-';
	s[8] = day / 10 + '0';
	s[9] = day % 10 + '0';
	s[10] = '\0';
	return string(s);
}

int main()
{		
	int n = 0;
	person *list = readDataFile("persons.txt", n);
	analyse(list, n);

	system("pause");
	return 0;
}