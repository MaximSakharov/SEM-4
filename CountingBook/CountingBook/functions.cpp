#include "functions.hpp"
#include <iomanip>

void Rhyme(int k, vector<string> arr_)
{
	int amount = (int)arr_.size();
	int i = 0;

	// Пока в списке больше одного человека
	while (amount > 1) 
	{
		// Находим индекс человека, которого нужно удалить
		i = (i + k - 1) % amount;

		// Выводим имя человека, который будет удален
		cout << arr_[i] << ' ';

		// Удаляем человека из списка
		arr_.erase(arr_.begin() + i);

		// Обновляем количество оставшихся людей
		amount = (int)arr_.size();
	}

	// Выводим последнего оставшегося человека
	cout << '\n' << arr_[0] << endl;
}

void BellSort(std::vector<int>& arr_)
{
   int size = (int)arr_.size();
   vector<int> new_arr(size);

   int step = 1;
   int pos = size / 2 - 1;

   if (size % 2 != 0)
   {
      pos = size / 2;
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

void BellShapePrint(const vector<int>& arr)
{
   if (arr.empty()) 
   {
      cout << "Массив пуст!" << endl;
      return;
   }

   int size = (int)arr.size();
   int mid = size / 2;

   // Находим максимальную длину числа в массиве
   int max_length = 0;
   for (int num : arr) 
   {
      int length = (int)to_string(num).length();
      if (length > max_length) 
         max_length = length;
   }

   for (int i = 0; i < max_length - 1; ++i)
      cout << ' ';

   // Выводим элементы в виде перевернутой "ёлки"
   for (int i = mid; i >= 0; --i)
   {
      // Выводим пробелы для выравнивания
      for (int j = 0; j < i; ++j)
         cout << string(max_length, ' ');

      // Выводим левый элемент
      if (i < mid) 
         cout << setw(max_length) << arr[i];

      // Выводим пробелы между элементами
      for (int j = 0; j < 2 * (mid - i) - 1; ++j)
         cout << std::string(max_length, ' ');

      // Выводим правый элемент
      if (i < mid)
      {
         for (int i = 0; i < max_length - 1; ++i)
            cout << ' ';
         cout << setw(max_length) << arr[size - 1 - i];
      }

      // Если массив нечетный, выводим средний элемент
      if (size % 2 != 0 && i == mid) 
         cout << setw(max_length) << arr[mid];

      cout << endl;
   }
}
