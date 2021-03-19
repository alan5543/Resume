// Student name: Yang Kai Yam
// Student ID  : 55676537

/*
	Submission deadline: Friday, 28 Feb 2020, 12 noon.
	
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

using namespace std;

struct person
{
	string dob;     // date of birth, yyyy-mm-dd
	char gender;    // 'F' for female, 'M' for male
	double income;  
};

ostream& operator<<(ostream& os, const person& p)
{
	os << p.dob << ' ' << p.gender << ' ' << p.income << endl;
	return os;
}

// Use of global variable is acceptable if it is read only.
const string today = "2020-02-24";

// --------------------------------------- functions to be implemented by student

// You may define other supporting functions, if required.


void analyse(const person *list, int n, int age_L, int age_H)
{
	// preconditions: list[] is sorted by dob in ascending order,
	//                n = length of list, and age_H >= age_L    
	
	// age_L and age_H (inclusive) define an age-group.
	// Age is determined as of today (a constant value is used in this exercise).

	// Find the average income of females, and average income of males in the age-group.
	// Print the results to standard output.

	// Do not traverse the array from the beginning using sequential search.
	// Use binary search to locate a person within the age-group,
	// and then process persons in the age-group sequentially.

	// For age_L = 50, age_H = 59, and today = "2020-02-24", process persons  
	// that were born after 1960-02-24, and were born on or before 1970-02-24.
	// Do not hardcode the dates in your program. 
	// Values of age_L, age_H and today may vary.


	// Your codes.

	int a = stoi(today.substr(0, 4));
	a = a - age_L;
	
	int f = stoi(today.substr(0, 4));
	f = f - 60;
	

	int upper = n - 1; //using the binary search
	int lower = 0;
	while (upper != lower + 1) {
		int middle = (upper + lower) / 2;
		string k = to_string(a) + today.substr(4, 10); 
		if (k.compare(list[middle].dob) <= 0) {  //comparsion
			upper = middle;
		}
		else {
			lower = middle;
		}
	}
	int high1 = n - 1; // using the binary search
	int low1 = 0;
	while (high1 != low1 + 1) {
		int mid = (high1 + low1) / 2;
		string k = to_string(f) + today.substr(4, 10); 
		if (k.compare(list[mid].dob) <= 0) {    //comparsion
			high1 = mid;
		}
		else {
			low1 = mid;
		}
	}

	double numF = 0;
	double incomeF = 0;
	for (int i = high1; i <= lower; i++) {
		if (list[i].gender == 'F') {
			numF++;                    //find the number of female
			incomeF += list[i].income; //find the average income of female
		}
	}
	double sumF = incomeF / numF;
	

	double numM = 0;
	double incomeM = 0;
	for (int i = high1; i <= lower; i++) {
		if (list[i].gender == 'M') {
			numM++;                    //find the number of males
			incomeM += list[i].income; //find the average income of males
		}
	}
	double sumM = incomeM / numM;
	
	cout << "Reference date is " << today << endl << endl;
	cout << "Index range of persons in the age-group " << age_L << " to " << age_H << ":\n";
	cout << "Start index = " << high1 << " to end index (inclusive) = " << lower << endl;
	cout << "list[start] : " << list[high1].dob << " " << list[high1].gender << " " << list[high1].income << endl;
	cout << "list[end] : " << list[lower].dob << " " << list[lower].gender << " " << list[lower].income << endl << endl;

	cout << "Number of females in age-group " << age_L << " to " << age_H << " = " << numF << endl;
	cout << "Average income of females in the age-group = " << sumF << endl << endl;
	cout << "Number of males in age-group " << age_L << " to " << age_H << " = " << numM << endl;
	cout << "Average income of males in the age-group = " << sumM << endl << endl;

	// Other print statements required. See the jpeg file for the requirements.

	
}

// -------------------------------------------------- functions given to student

person* readDataFile(string filename, int& n)
{
	ifstream infile;
	infile.open(filename);
	if (infile.is_open() == false)
	{
		cout << "Cannot open data file." << endl;
		system("pause");
		exit(0);
	}

	string temp;
	infile >> temp >> n;

	person *list = new person[n];
	for (int i = 0; i < n; i++)
		infile >> list[i].dob >> list[i].gender >> list[i].income;

	return list;
}

int main()
{		
	// Put the data file "persons.txt" in the working directory of your project.
	// Suppose your project is located at c:\myProject,
	// put the data file in c:\myProject\myProject

	int n = 0;
	person *list = readDataFile("persons.txt", n);  // DO not modify this statement
	analyse(list, n, 50, 59);

	system("pause");
	return 0;
}