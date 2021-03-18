#include <iostream>
using namespace std;


//recursion design in Question 3
double recur(double n, double cur) {
	if (!cur) {
		cur = 0;
	}
	if (n < 2) {
		throw "Invalid input";
	}
	if (n == 2) {
		return 1 / n + cur;
	}
	return recur(n - 1, cur + 1 / (n * (n - 1)));
}


//Same calculation with recursion (answer3.cpp)
double non_recur(double n, double cur) {
	if (!cur) {
		cur = 0;
	}
	if (n < 2) {                       // process for n < 2
		throw "Invalid input";
	}
	while (n > 2) {                    // process for n > 2
		cur = cur + 1 / (n * (n - 1));
		n--;
	}
	return 1 / n + cur;                // process for n = 2
}


//***********************************************************
//driver function
int main()
{
	double recur_res = 0;   //result in recursion
	double n_recur_res = 0; //result in non-recursion

	try {
		recur_res = recur(7, NULL);
		cout << recur_res << endl;
	}
	catch (const char* msg) {
		cerr << msg << endl;
	}

	try {
		n_recur_res = non_recur(7, NULL);
		cout << n_recur_res << endl;
	}
	catch (const char* msg) {
		cerr << msg << endl;
	}

	return 0;
}


