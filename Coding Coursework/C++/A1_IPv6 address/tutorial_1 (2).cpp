// Student name: Yang Kai Yam
// Student ID  : 55676537

/*
	Submission deadline: Thursday, 23 Jan 2020, 5 pm

	Part-1 : simple operations on char array.
	Part-2 : conversion of IPv6 address from binary to text format.

	Upload your .cpp file to Canvas.

	Put down your name and student ID at the top of your program file.
	10% marks will be deducted if your program file does not contain your name and ID.

	Submitted program that cannot be compiled (with syntax errors) by Visual Studio
	will receive no more than 30% marks.

	Late submissions or submissions by other means are not accepted.
*/

#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std;
char toHex(unsigned d);

// ------------------------------------ functions to be implemented by student
void trim(char *a)
{
	// Remove leading and trailing white-space characters in a[].
	// Example:
	//		char a[] = "  Test 1  2 3   ";
	//		After trimming, a[] = "Test 1  2 3";

	// Your codes.
	if (a[0] != '\0') {
		int first = 0;
		while (isspace(a[first]))
			first++;

		int last = 0;
		for (int k = 0; a[k] != '\0'; k++) {
			if (a[k] != ' ') {
				last = k;
			}
		}
		int n = 0;
		for (int k = first; k <= last; k++) {
			a[n] = a[k];
			n++;
		}
		a[n] = '\0';
	}
}

void squeeze(char *a)
{
	// Remove all white-space characters in a[].
	// Example:
	//		char a[] = "  Test 1  2 3   ";
	//		After squeezing, a[] = "Test123";

	// Your codes.
	int n = 0;
	int k = 0;
	while (a[n] != '\0') {
		if (a[n] != ' ') {
			a[k] = a[n];
			k++;
		}
		n++;
	}
	a[k] = '\0';
	
}

char* bin2Text_IPv6(const unsigned char *v)
{
	// Convert an IPv6 address from binary to text format.
	// See the pdf file for the explanation on the conversion requirements.

	// IPv6 address is an 128-bit bit-vector represented by an array of 16 unsigned char.
	// v[0] represents the left most 8 bits of the IPv6 address, and so on.
	// Bits in a byte are numbered from 7 down to 0 (from left to right).
	// The 7-th bit of v[0] is the first bit of the IPv6 address.

	char *a = new char[40];  // IPv6 address in text format is up to 40 chars, including '\0'

	 // dummy statement, replace it by your own codes

	// Your codes.
	int i = 0;
	int s = 0;
	for (int n = 0; n < 39; n += 2) {
		a[n] = toHex(v[i] / 16);
		a[n + 1] = toHex(v[i] % 16);
		int mp = n + 2;
		if (i % 2 && i < 14) {
			a[mp] = ':';
			n = mp - 1;
		}
		i++;
		s = n;
	}
	a[s + 2] = '\0';

	int p = 38;

	while ((a[p] == '0') || (a[p] == ':')) {
		p--;
	}

	if (p < 38) {
		a[p + 1] = ':';
		a[p + 2] = ':';
		a[p + 3] = '\0';
	}

	int op = 0;
	while ((a[op] == '0') || (a[op] == ':')) {
		op++;
	}
	if (op > 0) {
		a[0] = ':';
		a[1] = ':';
		int n = 2;
		for (int b = op; b < 40; b++) {
			a[n] = a[b];
			n++;
		}
	}
	
	int f = 0;
	while (a[f] != '\0') {
		if (a[f] == ':') {
			
			if ((a[f + 1] == '0') && (a[f + 2] == '0') ) {
				for (int num = f + 1; a[num] != '\0'; num+=2) {
					a[num] = a[num+2];
					a[num+1] = a[num+3];
				}
			}
			if ((a[f + 1] == '0') ) {
				for (int num = f + 1; a[num] != '\0'; num++) {
					a[num] = a[num + 1];
				}
			}
		}
		f++;
	}

	int mj = 0;
	while (a[mj] != '\0') {
		if (a[mj] == ':') {
			if ((a[mj + 1] == '0') && (a[mj + 5] == '0')) {
				for (int num = mj + 1; a[num] != '\0'; num++) {
					a[num] = a[num + 5];
				}
			}
		}
		mj++;
	}
	
	return a;
}

// ------------------------------------ functions given to student

void test1(char *a, char *b)
{
	cout << "-------------------------------------\n";
	
	cout << "Character array: \"" << a << '\"' << endl;
	trim(a);
	cout << "After trimming : \"" << a << '\"' << endl;
	squeeze(b);
	cout << "After squeezing: \"" << b << '\"' << endl;
}

void part_1()
{
	cout << "Part-1: char array\n\n";

	char a1[] = "Testing    ";
	char a2[] = "Testing    ";
	test1(a1, a2);

	char b1[] = "   Test";
	char b2[] = "   Test";
	test1(b1, b2);

	char c1[] = "   Test  1   2 3  ";
	char c2[] = "   Test  1   2 3  ";
	test1(c1, c2);

	char d1[] = "  ";
	char d2[] = "  ";
	test1(d1, d2);

	char e1[] = "";
	char e2[] = "";
	test1(e1, e2);
}

char toHex(unsigned d)  
{
	// Precondition: 0 <= d <= 15

	if (d >= 10)
		return d - 10 + 'a';
	return d + '0';
}

void printIPv6address_hex(const unsigned char *v)
{
	cout << "IPv6 address (bin)  ";
	for (int i = 0; i < 16; i++)
	{
		cout << toHex(v[i] /16 ) << toHex(v[i] % 16);
		if (i % 2 && i < 14)
			cout << ':';
	}
	cout << endl;
}

void test2(unsigned char *v)
{	
	cout << "-----------------------------------------------------------\n";
	printIPv6address_hex(v);
	char *a = bin2Text_IPv6(v);
	cout << "IPv6 address (text) " << a << endl << endl;	
	delete[] a;
}

void part_2()
{
	cout << "\n===========================================================\n";
	cout << "Part-2: IPv6 address\n\n";

	// An IPv6 address (in binary) is represented by an array of unsigned char of size 16.

	unsigned char v1[] = {0x12, 0x34, 0x0a, 0xbc, 0x00, 0x00, 0x00, 0x78, 0xff, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x05};
	unsigned char v2[] = {0x20, 0x01, 0x0d, 0xb8, 0x00, 0xa3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x08};
	unsigned char v3[] = {0x20, 0x01, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned char v4[] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x70, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04};
	unsigned char v5[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
	unsigned char v6[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	test2(v1);
	test2(v2);
	test2(v3);
	test2(v4);
	test2(v5);
	test2(v6);
}

int main()
{	
	using namespace std;

	int main() {
		int low, high, i;
		bool isPrime = true;

		cout << "Enter two numbers (intervals): ";
		cin >> low >> high;

		cout << "\nPrime numbers between " << low << " and " << high << " are: " << endl;

		while (low < high) {
			isPrime = true;
			if (low == 0 || low == 1) {
				isPrime = false;
			}
			else {
				for (i = 2; i <= low / 2; ++i) {
					if (low % i == 0) {
						isPrime = false;
						break;
					}
				}
			}

			if (isPrime)
				cout << low << " ";

			++low;
		}

		return 0;
	}
}