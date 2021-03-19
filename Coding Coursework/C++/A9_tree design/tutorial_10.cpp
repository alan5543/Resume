//Student name: Yang Kai Yam
//Student ID  : 55676537

/*
	Submission deadline: Friday, 10 April 2020, 12 noon.

	This tutorial is on binary tree.
	Implement the functions buildTree(), getMin(), getAvg(), getMaxLevelCount().
	
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
#include <cstdlib>
#include <queue>
#include <stack>

using namespace std;

template<class Type>
struct treeNode
{
	Type info;
	treeNode<Type> *left, *right;
};

// ------------------------ Functions to be implemented by student

treeNode<int>* build(const int *inorder, int s, int e, const int *postorder, int a, int k) {
	if (s > e) {
		return nullptr;
	}

	treeNode<int> *root = new treeNode<int>;
	root->info = postorder[k];
	int i = s;
	while (i <= e) {
		if (inorder[i] == root->info) {
			break;
		}
		i++;
	}

	int LT = i - s;

	root->left = build(inorder, s, i - 1, postorder, a, a + LT - 1);
	root->right = build(inorder, i + 1, e, postorder, a + LT, k-1);
	return root;
}

treeNode<int>* buildTree(const int *inorder, const int *postorder, int n)
{
	// Construct the binary tree from the given inorder and postorder traversal sequences.
	int num = n - 1;
	int kk = 0;
	return build(inorder, kk, num , postorder, kk, num );
}

int getMin(const treeNode<int> *tree)
{
	// Precondition: The tree is not empty.

	// Return the smallest number in the binary tree.
	int min = tree->info;
	
	if (tree->right != nullptr) {
		int R = getMin(tree->right);
		bool rightcorr = R < min;

		if (rightcorr) {
			min = R;
		}
	}

	if (tree->left != nullptr) {
		int L = getMin(tree->left);
		bool leftcorr = L < min;

		if (leftcorr) {
			min = L;
		}
	}

	return min;
}
template<class Type>
double nodeCount(const treeNode<Type> *tree) {
	if (tree == nullptr) {
		return 0;
	}

	return 1 + nodeCount(tree->left) + nodeCount(tree->right);
}

template<class Type>
double calSum(const treeNode<Type> *tree) {
	if (tree == nullptr) {
		return 0;
	}

	int value = tree->info;
	return value + calSum(tree->left) + calSum(tree->right);
}

double getAvg(const treeNode<int> *tree)
{
	// Return the average value of the numbers in the binary tree.

	// Return 0 if the tree is empty.
	if (tree == nullptr) {
		return 0;
	}
	else {
		double sum = calSum(tree);
		double count = nodeCount(tree);
		double avg = sum / count;

		return avg;
	}
}

pair<int, int> getMaxLevelCount(const treeNode<int> *tree)
{
	// struct pair<T1, T2> is defined in C++, http://www.cplusplus.com/reference/utility/pair/

	//	template<class T1, class T2>
	//	struct pair 
	//	{
	//		T1 first;
	//		T2 second;
	//	};

	// Return pair<int, int> ans
	// ans.first = level number, ans.second = node count on the level

	// Root is on level 0.
	pair<int, int> ans;

	if (tree == nullptr) {
		ans.first = 0;
		ans.second = 0;
	}
	else {
		
		queue<const treeNode<int>*> q;
		
			q.push(tree);
			ans.first = 0;
			ans.second = q.size();
		
		int comp = ans.second;


		for (int i = 0; !q.empty(); i++) {
			int number = q.size();
			for (int k = number; k >= 0; k--) {
				const treeNode<int>* p = q.front();
				q.pop();

				if (p->left != nullptr) {
					q.push(p->left);
				}
				if (p->right != nullptr) {
					q.push(p->right);
				}
			}

			if (number > comp) {
				ans.second = number;
				ans.first = i;
			}
			
		}
	}

	return ans;
}


// ----------------------------------- functions given to you

template <class Type>
void printTree(const treeNode<Type> *p, int indent)
{
	if (p != NULL)
	{
		printTree(p->right, indent + 4);
		cout << setw(indent) << p->info << endl;
		printTree(p->left, indent + 4);
	}	
}

int main()
{	
	// Inorder and postorder traversal sequences.
	int inorder[] = {63, 55, 20, 89, 50, 16, 41, 70, 76, 35, 85};
	int postorder[] = {63, 20, 89, 55, 16, 50, 70, 76, 85, 35, 41};
	int n = 11;

	
	treeNode<int> *tree = buildTree(inorder, postorder, n); 
	cout << "Binary tree :\n\n";
	printTree(tree, 4);
	cout << endl;
	
	cout << "Min value in the binary tree = " << getMin(tree) << endl << endl;
	
	cout << "Average value of numbers in the binary tree = " << getAvg(tree) << endl << endl;
	
	pair<int, int> LC = getMaxLevelCount(tree);
	cout << "Level with largest number of nodes : level = " << LC.first << ", node count = " << LC.second << endl << endl;
	

	system("pause");
	exit(0);
}