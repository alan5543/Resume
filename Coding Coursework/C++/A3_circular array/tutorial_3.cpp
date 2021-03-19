// Student name: Yang Kai Yam
// Student ID  : 55676537

/*
	Submission deadline: Friday, 21 Feb 2020, 12 noon.
	
	Upload your .cpp file to Canvas.

	Put down your name and student ID at the top of your program file.
	10% marks will be deducted if your program file does not contain your name and ID.

	Submitted program that cannot be compiled (with syntax errors) by Visual Studio
	will receive no more than 30% marks.

	If your program crashes or infinite-loop when executed with Visual Studio, 
 	you should state the problem at the top of your program file 
	(right below student name and ID); otherwise significant percentage (30% or more) 
	of marks will be deducted.

	Late submissions or submissions by other means are not accepted.
*/

#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std;

// -------------------------------------------- functions to be implemented by you

// You can reference the answer of Tut_S1 for the same operations on linear array.

void longest_block(const int* a, int n, int& start, int& len)
{
	// Array a[] is interpreted as circular array.
	// A block is a consecutive sequence of elements with the same value.

	// start = start index of the longest block
	// len = length of the longest block
	// dummy statement, replace it by your own codes
	// Your codes

	int count = 1;
	int open = 0;
	int sum = n ;
	int j = 0;
	if (n != 0) {
		for (int num = 0; num < sum; num++) {
			if (a[num] == a[num + 1]) {
				count++;
			}
			else {
				if (((num + 1) == sum)&&(count != sum)) {
					while (a[num] == a[j]) {
						count++;
						j++;
					}
				}
				if (count > open) {
					open = count;
					len = open;
					start = num +j- open + 1;
				}
				count = 1;
			}
		}
	}
	else {
      len = open;
	  start = n;
	}

}

void longestSubSeq_NoDup(const int* a, int n, int& start, int& len)
{	
	// Array a[] is interpreted as a circular array.

	// start = start index of the longest subsequence without duplicated value
	// len = length of the longest subsequence without duplicated value

	start = 0;  // dummy statement, replace it by your own codes
	len = 1;

	// Your codes

	int threshold = 0;
	bool ret = true;
	int b[40];
	for (int u = 0; u < n; u++) {
		b[u] = a[u];
	}
	for (int u = n, i=0; u < 40,i<n; u++,i++) {
		b[u] = a[i];
	}
	for (int order = 0; order < n; order++) {
        int i = order;
		bool done = n <= 0;

		while (!done) {
			int count = 1;
			bool open = false;
			
				for (int num = order; (num < i) && (!open); num++) {
					if (b[num] != b[i]) {
						count++;
					}
					else {
						open = true;
					}
				}
			
			if (count > threshold) {
				threshold = count;
				len = count;
				start = order;
			}
			
			i = (i + 1) ;
			int aa = i % n;
			done = aa == order;

			if (open) {
				done = true;
			}
		}
	}
	if (n == 0) {
		start = 0;
		len = 0;
	}
}

// -------------------------------------------------- functions given to you
void printArray(const char *header, const int* a, int n)
{
	cout << header << ", size = " << n << endl;
	for (int i = 0; i < n; i++)
	{
		if (i > 0 && i % 10 == 0)
			cout << endl;
		cout << setw(4) << a[i] << ", ";
	}
	cout << endl << endl;
}


void printSeq(const int *a, int n, int start, int len)
{
	cout << "subsequence: ";
	for (int i = 0; i < len; i++)
		cout << a[(start+i)%n] << ", ";
	cout << endl;
}

void test_longest_block(const char *header, const int *a, int n)
{
	printArray(header, a, n);
	int start, len;
	longest_block(a, n, start, len);
	cout << "Longest block:\n";
	cout << "start index = " << start << ", block len = " << len << endl;
	cout << "----------------------------------------------------------\n";
}

void part_1()
{
	cout << "Part_1:\n\n";

	int a[] = {1, 2, 2, 3, 3, 4, 4, 4, 6, 6, 6, 5, 5, 5, 5, 5, 2, 0, 1, 3};
	int n = 20;

	int b[] = {1, 1, 3, 3, 1, 4, 4, 4, 0, 8, 6, 6, 5, 5, 5, 5, 2, 1, 1, 1};
	int m = 20;

	int c[] = {3, 3, 3, 3};
	int p = 4;

	int d[] = {2};
	int q = 1;

	test_longest_block("a[]", a, n);
	test_longest_block("b[]", b, m);
	test_longest_block("c[]", c, p);
	test_longest_block("d[]", d, q);
	test_longest_block("d[]", d, 0);  // empty array
}

void test_longestSubSeq_NoDup(const char *prefix, const int *a, int n)
{
	printArray(prefix, a, n);
	int start, len;
	longestSubSeq_NoDup(a, n, start, len);
	cout << "Longest subsequence with no duplicate value:\n";
	cout << "start index = " << start << ", subSeq len = " << len << endl;
	printSeq(a, n, start, len);
	cout << "----------------------------------------------------------\n";
}

void part_2()
{
	cout << "\n==========================================================\n";
	cout << "Part_2:\n\n";

	int a[] = {1, 2, 3, 0, 2, 7, 3, 4, 5, 6, 4, 8, 0, 2, 3, 1, 6, 8, 3, 2};
	int n = 20;

	int b[] = {5, 9, 7, 4, 5, 7, 3, 4, 5, 6, 4, 0, 9, 0, 4, 1, 6, 8, 3, 2};
	int m = 20;

	int c[] = {3, 3, 3, 3};
	int p = 4;

	int d[] = {1, 0, 2, 3};
	int q = 4;

	int e[] = {2};
	int r = 1;

	test_longestSubSeq_NoDup("a[]", a, n);
	test_longestSubSeq_NoDup("b[]", b, m);
	test_longestSubSeq_NoDup("c[]", c, p);
	test_longestSubSeq_NoDup("d[]", d, q);
	test_longestSubSeq_NoDup("e[]", e, r);
	test_longestSubSeq_NoDup("e[]", e, 0);  // empty array
}

int main()
{		
	part_1();
	part_2();

	system("pause");
	return 0;
}