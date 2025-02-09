#include <iostream>
#include "CountingBook.hpp"
#include "functions.hpp"

using namespace std;

int main()
{
	string arr1[] = { "Masha", "Pasha", "Mitya", "Vova", "Andrei", "Alice", "Bob", "Charlie", "David", "Eve" };

	CountingBook counting(arr1, 10);

	counting.Rhyme(3);

	vector<string> arr2 = { "Masha", "Pasha", "Mitya", "Vova", "Andrei", "Alice", "Bob", "Charlie", "David", "Eve" };

	Rhyme(3, arr2);

	vector<int> new_arr1 = { 8, 6, 3, 9, 7, 1, 5};
	vector<int> new_arr2 = { 5, -3, 12, 1, 7, -2, 8, 4, 6 };
	BellSort(new_arr1);
	BellShapePrint(new_arr1);

	return 0;
}