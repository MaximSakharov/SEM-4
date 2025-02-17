#include "functions.hpp"
#include <iomanip>

void Rhyme(int k, vector<string> arr_)
{
	int amount = (int)arr_.size();
	int i = 0;

	// ���� � ������ ������ ������ ��������
	while (amount > 1) 
	{
		// ������� ������ ��������, �������� ����� �������
		i = (i + k - 1) % amount;

		// ������� ��� ��������, ������� ����� ������
		cout << arr_[i] << ' ';

		// ������� �������� �� ������
		arr_.erase(arr_.begin() + i);

		// ��������� ���������� ���������� �����
		amount--;
	}

	// ������� ���������� ����������� ��������
	cout << '\n' << arr_[0] << endl;
}

void BellSort(vector<int>& arr_)
{
   int size = (int)arr_.size();
   vector<int> new_arr(size);

   int step = 1;
   int pos = size / 2 - 1;

   if (size % 2 != 0)
   {
      pos = size / 2;
      step = -1;
   }

   for (int i = 0; i < size; ++i)
   {
      int max_num = arr_[0];
      int max_pos = 0;

      for (int j = 1; j < size; ++j)
      {
         if (max_num < arr_[j])
         {
            max_num = arr_[j];
            max_pos = j;
         }
      }

      arr_[max_pos] = INT_MIN;
      new_arr[pos] = max_num;

      pos += step;
      step = (step > 0) ? -(step + 1) : -(step - 1);
   }

   swap(arr_, new_arr);
}

void BellSort2(vector<int>& arr_) {
    int size = (int)arr_.size();
    int left = 0, right = size - 1;

    while (left <= right) {
        // ����� ������������ �������� �� left �� right
        int min_pos = left;
        for (int j = left + 1; j <= right; ++j) {
            if (arr_[j] < arr_[min_pos]) {
                min_pos = j;
            }
        }
        swap(arr_[min_pos], arr_[right]); // ��������� ������� � ������ �������
        --right;

        if (left > right) break;

        // ����� ������������ �������� �� left �� right (����� ��������)
        min_pos = left;
        for (int j = left + 1; j <= right; ++j) {
            if (arr_[j] < arr_[min_pos]) {
                min_pos = j;
            }
        }
        swap(arr_[min_pos], arr_[left]); // ��������� ������� � ����� �������
        ++left;
    }
}

void BellShapePrint(const vector<int>& arr)
{
   if (arr.empty()) 
   {
      cout << "������ ����!" << endl;
      return;
   }

   int size = (int)arr.size();
   int mid = size / 2;

   // ������� ������������ ����� ����� � �������
   int max_length = 0;
   for (int num : arr) 
   {
      int length = (int)to_string(num).length();
      if (length > max_length) 
         max_length = length;
   }

   for (int i = 0; i < max_length - 1; ++i)
      cout << ' ';

   for (int i = mid; i >= 0; --i)
   {
      // ������� ������� ��� ������������
      for (int j = 0; j < i; ++j)
         cout << string(max_length, ' ');

      // ������� ����� �������
      if (i < mid) 
         cout << setw(max_length) << arr[i];

      // ������� ������� ����� ����������
      for (int j = 0; j < 2 * (mid - i) - 1; ++j)
         cout << std::string(max_length, ' ');

      // ������� ������ �������
      if (i < mid)
      {
         for (int i = 0; i < max_length - 1; ++i)
            cout << ' ';
         cout << setw(max_length) << arr[size - 1 - i];
      }

      
      // ���� ������ ��������, ������� ������� �������
      if (size % 2 != 0 && i == mid) 
         cout << setw(max_length) << arr[mid];
      

      cout << endl;
   }
}
