// Student name: Yang Kai Yam
// Student ID  : 55676537

/*
	Submission deadline: Friday, 27 March, 12 noon.
	
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
#include <cstdlib>
#include <string>


using namespace std;

template<class Type>
struct node
{
	Type info;
	node<Type> *link;
};

struct person
{
	string dob;
	char gender;
	double income;
};

// Design a functional class/struct to compare person by gender and then by income.
struct comparePerson
{
	// Your codes
	bool operator()(const person& A,const person& B) {
		if (A.gender != B.gender) {
			return A.gender - B.gender > 0;
		}
		
			return A.income - B.income > 0;
		
	}

} compPerson;

ostream& operator<<(ostream& os, const person& p)
{
	os << p.dob << ' ' << p.gender << ' ' << p.income << endl;
	return os;
}

template<class Type, class Compare>
node<Type>* merge(node<Type>* A, node<Type>* B, Compare comp)
{
	// Lists A and B are not empty. 
	// Lists A and B are sorted according to the compare function.

	// Merge A and B to produce a sorted list.
	// Existing nodes in A and B are linked together to form the merged list.
	// This function is used to support merge sort.

	// *** No new node is created. ***
	
	
	node<Type>*list = nullptr;

	bool success = comp(A->info, B->info);
	if (success) {
		list = B;
		B = B->link;
	}
	else {
		list = A;
		A = A->link;
	}
	node<Type>*r = list;

	while (A != nullptr && B != nullptr) {
		bool success = comp(A->info, B->info);
		if (success) {
			r->link = B;
			r = r->link;
			B = B->link;
			
		}
		else {
			r->link = A;
			r = r->link;
			A = A->link;
			
		}
	}
	while (A != nullptr) {
		r->link = A;
		A = A->link;
		r = r->link;
	}
	while (B != nullptr) {
		r->link = B;
		B = B->link;
		r = r->link;
	}
	return list;  // dummy return statement, replace it by your codes
}

// Other functions required in your design


template<class Type, class Compare>
void msort_list(node<Type>*& list, Compare comp)
{
	// Sort the list using the merge sort algorithm.
	// Your sorting method should be stable.
	// Implement the sorting algorithm using recursion.

	// Your codes
    node<Type> *part1;
	node<Type> *part2;
	if (list->link == nullptr) {
		return;
	}
	
	if (list == nullptr || list->link == nullptr) {
		part1 = list;
		part2 = nullptr;
	}

	node<Type> * p = nullptr;
	node<Type> * prev = list;
	node<Type> * cur = list->link;

	while (cur != nullptr) {
		cur = cur->link;
		if (cur == nullptr) {
			break;
		}
		
		cur = cur->link;
		prev = prev->link;
	}

	part1 = list;
	part2 = prev->link;
	prev->link = nullptr;

	msort_list(part1,comp);
	msort_list(part2,comp);

	list = merge(part1, part2, comp);
}


// ------------------------------------ functions given to you

template<class Type>
void printList(const char *label, const node<Type> *list)
{
	cout << label << endl;
	const node<Type> *p = list;
	while (p != nullptr)
	{
		cout << p->info;		 
		p = p->link;
	}
	cout << endl;
}

node<person>* readDataFile(const string& filename)
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
	int n;
	infile >> temp >> n;

	node<person> *header = new node<person>;
	header->link = nullptr;
	node<person> *last = header;
	for (int i = 0; i < n; i++)
	{
		last->link = new node<person>;
		last = last->link;
		infile >> last->info.dob >> last->info.gender >> last->info.income;
	}
	last->link = nullptr;

	infile.close();
	node<person> *list = header->link;
	delete header;
	return list;
}

int main()
{	
	node<person> *list = readDataFile("tut-08-data.txt");
	printList("List of persons:", list);
	// The list is initially sorted by dob.

	// Sort the list by gender, and then by income using merge sort.
	// Your sorting method should be stable.
	msort_list(list, compPerson);
	printList("List of persons (sorted by gender and then by income):", list);
	cout << endl;

	system("pause");
	return 0;
}