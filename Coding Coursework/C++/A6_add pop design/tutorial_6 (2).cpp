// Student name: Yang Kai Yam
// Student ID  : 55676537

/*
	Submission deadline: Friday, 13 March 2020, 12 noon

	We shall use the data structure vector<T> in this exercise.
	http://www.cplusplus.com/reference/vector/vector/

	Study the notes and watch the videos that explain vector, and
	recursion and backtracking.

	The storage capacity of a vector is expanded automatically when more
	elements are added to it.
	You can access an element in a vector using the subscript operator,
	e.g. v[i] references the element at index i of vector v.

	Operators that are relevant to this exercise:
		push_back(item) : add the item at the end of the vector
		pop_back()      : remove the last item in the vector
		back()          : get the last item in the vector
		size()          : return the number of items in the vector
		empty()         : return true if the vector is empty, otherwise return false
*/

#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

struct tube  // Implement the member functions add() and pop()
{
	unsigned len;          // length of the tube
	unsigned avail_space;  // vacant space in the tube
	vector<unsigned> list; // cylindrical pins added to the tube

	tube(unsigned size)  // constructor
	{
		len = avail_space = size;
	}


	bool add(unsigned e)
	{
		bool value = true;
		if (!(e > len || e > avail_space))
			return value;
		else
			return !value;

		// Return true if a cylindrical pin with length e can be added to list
		// of the tube (appen at the end), otherwise return false.

	}
	/*
	void pop(){

	}
	*/
};


bool check(const vector<unsigned>& pins, vector<unsigned> order, vector<tube>& tubes, int i, unsigned tubelen)
{
	int count = 0;
	int k = 0;
	while (k < pins.size()) {
		if (order[k] == 1) {
			count++;
		}
		k++;
	}
	if (count == pins.size())
		return true;
	if (count != pins.size())
	{
		tube t(tubelen);
		tubes.push_back(t);

		bool op;
		for (unsigned num = 0; num < pins.size(); num++) {
			op = false;
			int k = tubes[i].avail_space;
			int pos = pins[num];
			if (order[num] != 1)
				op = tubes[i].add(pins[num]);

			if (op)
			{
				order[num] ++;
				k = k - pos;
				tubes[i].list.push_back(pos);
				tubes[i].avail_space = k;
			}
		}
		int a = i + 1;
		return check(pins, order, tubes, a, tubelen);
	}
}

void abc(const vector<unsigned>& pins, vector<unsigned> order) {
	int a = 0;
	for (int i = 0; i < pins.size(); i++)
		order.push_back(a);
}

ostream& operator<<(ostream& os, const tube& t)
{
	// Function to print a tube.

	os << "len = " << t.len << ", pins : ";
	for (int i = 0; i < t.list.size(); i++)
		os << t.list[i] << ", ";

	os << endl;
	return os;
}



// Other supporting functions required in your design


void packing(const vector<unsigned>& pins, unsigned tubeLen)
{
	// Determine the minimum number of tubes requried to store all
	// the cylindrical pins.
	// Print the results to standard output (reference the pdf file).

	// Use a vector to store the tube in your program.
	vector<tube> tubes; vector<unsigned> order;  // the vector is empty initially
	bool Checking;
	int first = pins[0];
	cout << "tubeLen = " << tubeLen << " cm" << endl;

	
	if (tubeLen < first) {
		cout << "No Solution" << endl;
	}
	else {
		// Sample statements to define a tube and add it to the vector
		//   tube t(tubeLen);   
		//   tubes.push_back(t);

		tube t(tubeLen);
		tubes.push_back(t);

		
		int a = 0;
		int i = 0;
		while (i < pins.size()) {
			i++;
            order.push_back(a);
		}
			

		int numi = 0;
		Checking = check( pins,order, tubes, numi, tubeLen);
		


		// Statements to print the results, if a solution is found.	
        tubes.pop_back();
		unsigned spaceUsed = 0;
		unsigned count = 0;
		for (unsigned i = 0; i < tubes.size(); i++)
		{
			cout << "Tube " << i + 1 << " : " << tubes[i];
			spaceUsed += tubes[i].len - tubes[i].avail_space;
			count += tubes[i].list.size();
		}
		unsigned totalSpace = tubes.size() * tubeLen;
		double utilization = totalSpace > 0 ? spaceUsed * 100.0 / totalSpace : 0;
		cout << "Total no. of pins = " << count << "; space utilization = " << utilization << "%\n";
	}
	
}

int main()
{
	unsigned a[] = { 4, 10, 7, 12, 6, 10, 10, 8, 5, 13, 13, 11, 3, 14 };
	unsigned n = 14;

	/*
		You may test your program with 1 more test case:
		unsigned a[] = {3, 3, 4, 6, 5, 3};
		unsigned n = 6;
		unsigned tubeLen = 12;
		Result: packed in 2 tubes (6, 3, 3), (5, 4, 3)
	*/

	vector<unsigned> pins;
	for (unsigned i = 0; i < n; i++)
		pins.push_back(a[i]);

	cout << "Lengths of cylindrical pins (cm) :" << endl;
	for (unsigned i = 0; i < pins.size(); i++)
	{
		if (i > 0 && i % 10 == 0)
			cout << endl;
		cout << setw(3) << pins[i] << ", ";
	}
	cout << endl << endl;

	//sorting
	sort(pins.begin(), pins.end(), greater<>());
	
	// *** Question to think about ***
	//    Possible way to enhance computation efficiency ?
	//    Results for this test data set can be obtained in less than 1 second.



	// DO NOT modify this for-loop.
	for (unsigned tubeLen = 22; tubeLen >= 13; tubeLen -= 3)
	{
		cout << "-----------------------------------------------------\n";
		packing(pins, tubeLen);
	}

	system("pause");
	return 0;
}