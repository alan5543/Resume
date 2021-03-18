#include <iostream>
#include <set> 
using namespace std;

//*******************************************************************************************************************
//Method 1: Using a simple nested loops to determine whether it is subset or not
/*
Please explain the computational complexity of your answer in Big-O notation, i.e. O(log n) or O(n ^2)?
 According to the big O notation, one nested for-loop cost for O(n^2). 
Time complexity of nested loops is equal to the number of times the innermost statement is executed.*/

bool isSubset_1(char a[], char b[], int c1, int c2) {
	if (a != NULL && b != NULL) {
		int counter = 0;
		for (int s2 = 0; s2 < c2; s2++) {
			char test = b[s2];
			for (int s1 = 0; s1 < c1; s1++) {
				if (test == a[s1]) {
					counter++;
					break;
				}
			}
		}

		if (counter == c2)
			return true;
		else
			return false;
	}
	else
		return false;
}
 
//*******************************************************************************************************************
// Method 2: Using set to implement the isSubset() function
// As we know set will not save duplicate cases. Due to this property, we can compare the set size to determine subset
// The time complexity of using set is O(m+n).

bool isSubset_2(char a[], char b[], int c1, int c2)
{
	set<char> s1;
	int temp_size = 0;

	for (int i = 0; i < c1; i++) {
		s1.insert(a[i]);
	}
	temp_size = s1.size();

	for (int i = 0; i < c2; i++) {
		s1.insert(b[i]);
	}
	if (s1.size() == temp_size)
		return true;
	else
		return false;
}


//*******************************************************************************************************************
//Method 3: Using qsort and bsearch to implement the isSubset()
/*
Using qsort and bsearch is better to minimize the execution time. The time taken by qsort depends upon
the inut array and partition strategy. Therefore, the time complexity is O(mLogm + nLogn). In the worst
case, the complexity will be O(m^2). */

void swap(char x[], int i, int j)
{
	char t = x[i];
	x[i] = x[j];
	x[j] = t;
}


int partition(char x[], int s, int e)
{
	// precondition: s < e 
	int i = s + 1;
	int j = e;
	bool done = false;
	while (!done)
	{
		while (i < j && x[i] <= x[s])
			i++;
		while (x[j] > x[s]) // j will NOT go out of bound
			j--;
		if (i < j)
			swap(x, i, j);
		else
			done = true;
	}
	swap(x, s, j); // swap x[s] and x[j] 
	return j;
}

//Best Case for quicksort is O(nlogn), Worst Case for qsort is O(n^2)
void simpleQuicksort(char x[], int start, int end)
{
	if (start < end)
	{
		int j = partition(x, start, end);
		simpleQuicksort(x, start, j - 1);
		simpleQuicksort(x, j + 1, end);
	}
}

//Time complexity of binary search is O(log N)
int binSearch(char A[], int N, char x)
{
	int low = 0;
	int high = N - 1;
	int loc = -1;
	// loop invariant:
	// if x is contained in A[], A[low] <= x <= A[high]
	while (low <= high && loc < 0)
	{
		int mid = (low + high) / 2;
		if (A[mid] < x)
			low = mid + 1;
		else if (A[mid] > x)
			high = mid - 1;
		else
			loc = mid; // x == A[mid]
	}
	return loc;
}

bool isSubset_3(char a[], char b[], int c1, int c2)
{
	simpleQuicksort(a, 0, c1 - 1);
	for (int i = 0; i < c2; i++) {
		if (binSearch(a, c1, b[i]) == -1) {
			return false;
		}
	}
	return true;
}

//*******************************************************************************************************************

int main() //driver function
{

	char a1[] = { 'A','B','C','D','E' };
	char b1[] = { 'A','A', 'D' };
	if (isSubset_1(a1, b1, sizeof(a1), sizeof(b1)))
		cout << "True" << endl;
	else
		cout << "False"<< endl;

	char a2[] = { 'A','B','C','D','E' };
	char b2[] = { 'A','D', 'Z' };
	if (isSubset_2(a2, b2, sizeof(a2), sizeof(b2)))
		cout << "True" << endl;
	else
		cout << "False" << endl;

	char a3[] = { 'A','D', 'E' };
	char b3[] = { 'A','A', 'D', 'E' };
	if (isSubset_3(a3, b3, sizeof(a3), sizeof(b3)))
		cout << "True" << endl;
	else
		cout << "False" << endl;

	char a4[]{ 'E', 'Q', 'D', 'A'};
	char b4[] = { 'A','A', 'D', 'E' };
	if (isSubset_3(a4, b4, sizeof(a4), sizeof(b4)))
		cout << "True" << endl;
	else
		cout << "False" << endl;

	return 0;
}
