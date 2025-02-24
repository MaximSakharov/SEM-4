#include "BoolVector.hpp"

// Реализация методов

BoolVector::Iterator::Iterator(bool* ptr) : current(ptr) {}

bool& BoolVector::Iterator::operator*() 
{
   return *current;
}

const bool& BoolVector::Iterator::operator*() const 
{
   return *current;
}

/*
BoolVector::Iterator& BoolVector::Iterator::operator++() 
{
   ++current;
   return *this;
}

BoolVector::Iterator BoolVector::Iterator::operator++(int) 
{
   Iterator old = *this;
   current++;
   return old;
}
*/

BoolVector::Iterator& BoolVector::Iterator::operator++()
{
   // Проверяем, что текущий итератор не указывает за пределы вектора
   if (current == nullptr) {
      throw IteratorOutOfRangeException();  // Или любое другое подходящее исключение
   }

   ++current;
   return *this;
}

BoolVector::Iterator BoolVector::Iterator::operator++(int)
{
   Iterator old = *this;
   ++(*this);  // Используем префиксную версию, чтобы избежать повторного кода
   return old;
}


bool BoolVector::Iterator::operator==(const Iterator& other) const 
{
   return current == other.current;
}

bool BoolVector::Iterator::operator!=(const Iterator& other) const 
{
   return current != other.current;
}

BoolVector::BoolVector(size_t capacity_) : capacity(capacity_), size(0) 
{
   elements = new bool[capacity];
}

BoolVector::BoolVector(bool* vector_, size_t capacity_) : capacity(capacity_), size(capacity_) 
{
   elements = new bool[capacity];
   for (size_t i = 0; i < capacity; ++i) 
      elements[i] = vector_[i];
}

BoolVector::~BoolVector() 
{
   if (elements)
   {
      delete[] elements;
      elements = nullptr;
   }
}

BoolVector& BoolVector::operator=(const BoolVector& other_) 
{
   if (this != &other_) 
   {
      delete[] elements;
      size = other_.size;
      capacity = other_.capacity;
      elements = new bool[capacity];
      for (size_t i = 0; i < size; ++i) 
         elements[i] = other_.elements[i];
   }
   return *this;
}

BoolVector::BoolVector(const BoolVector& other_) : size(other_.size), capacity(other_.capacity), elements(new bool[other_.capacity]) 
{
   for (size_t i = 0; i < size; ++i) 
      elements[i] = other_.elements[i];
}

bool& BoolVector::operator[](size_t i) const 
{
   if (i >= size)
      throw IndexOutOfRangeException();
   return elements[i];
}


bool& BoolVector::operator[](size_t i) 
{
   if (i >= size)
      throw IndexOutOfRangeException();
   return elements[i];
}


bool BoolVector::At(size_t pos_) 
{
   if (pos_ >= size)
      throw IndexOutOfRangeException();
   return elements[pos_];
}

bool BoolVector::Front() 
{
   if (size == 0)
      throw EmptyVectorException();
   return elements[0];
}

bool BoolVector::Back() 
{
   if (size == 0)
      throw EmptyVectorException();
   return elements[size - 1];
}

void BoolVector::SetElement(size_t pos_, bool new_element_) 
{
   if (pos_ >= size)
      throw IndexOutOfRangeException();
   elements[pos_] = new_element_;
}

void BoolVector::PushBack(bool element_) 
{
   if (size < capacity) 
   {
      elements[size++] = element_;
      return;
   }
   if (capacity == 0) 
   {
      elements = new bool[1];
      capacity = size = 1;
      elements[0] = element_;
   }
   else 
   {
      bool* new_elements = new bool[capacity *= 2];
      if (!new_elements) return;

      for (size_t i = 0; i < size; ++i) 
         new_elements[i] = elements[i];

      delete[] elements;
      elements = new_elements;
      elements[size++] = element_;
   }
}

void BoolVector::PopBack() 
{
   if (size == 0)
      return;
   --size;
}

BoolVector BoolVector::OrOperation(const BoolVector& other_) const 
{
   if (size != other_.size)
      throw SizeMismatchException();

   BoolVector result(size);
   result.size = size;
   for (size_t i = 0; i < size; ++i) 
      result[i] = (elements[i] || other_.elements[i]);

   return result;
}

BoolVector BoolVector::AndOperation(const BoolVector& other_) const 
{
   if (size != other_.size)
      throw SizeMismatchException();

   BoolVector result(size);
   result.size = size;
   for (size_t i = 0; i < size; ++i) 
      result[i] = (elements[i] && other_.elements[i]);

   return result;
}

bool BoolVector::operator==(const BoolVector& other_) const 
{
   if (size != other_.size)
      return false;

   for (size_t i = 0; i < size; ++i)
      if (elements[i] != other_[i])
         return false;

   return true;
}

bool BoolVector::operator!=(const BoolVector& other_) const 
{
   return !(*this == other_);
}

BoolVector BoolVector::operator!() const 
{
   BoolVector result(size);
   result.size = size;
   for (size_t i = 0; i < size; ++i)
      result[i] = !elements[i];
   return result;
}

void BoolVector::Print() 
{
   cout << "[< ";
   for (size_t i = 0; i < size - 1; ++i)
      cout << elements[i] << ", ";
   cout << elements[size - 1];
   cout << " >]";
}

size_t BoolVector::ScalarProduct(const BoolVector& other_) const 
{
   if (size != other_.size)
      throw SizeMismatchException();

   size_t result = 0;
   for (size_t i = 0; i < size; ++i)
      result += (elements[i] && other_.elements[i]);

   return result;
}

BoolVector BoolVector::Concatenate(const BoolVector& other_) const 
{
   BoolVector result(size + other_.size);
   result.size = size + other_.size;

   for (size_t i = 0; i < size; ++i)
      result.elements[i] = elements[i];
   for (size_t i = 0; i < other_.size; ++i)
      result.elements[i + size] = other_.elements[i];

   return result;
}

BoolVector BoolVector::ShiftLeftOperation(int n) const 
{
   if (n < 0)
      throw InvalidShiftException();

   BoolVector result(size);
   result.size = size;

   int param = n;

   if (n >= size)
      param = (int)size;

   for (size_t i = 0; i < size - param; ++i)
      result[i] = elements[i + param];

   for (size_t i = size - param; i < size; ++i)
      result[i] = false;

   return result;
}

BoolVector BoolVector::ShiftRightOperation(int n) const 
{
   if (n < 0)
      throw InvalidShiftException();

   BoolVector result(size);
   result.size = size;

   int param = n;

   if (n >= size)
      param = (int)size;

   for (size_t i = size - 1; i >= param; --i)
      result[i] = elements[i - param];

   for (size_t i = 0; i < param; ++i)
      result[i] = false;

   return result;
}

BoolVector BoolVector::SubvectorExtraction(size_t left_, size_t right_) 
{
   if (right_ >= size || left_ > right_)
      throw IndexOutOfRangeException();

   size_t new_capacity = right_ - left_ + 1;
   BoolVector result(new_capacity);
   result.size = new_capacity;

   for (int i = 0; i < new_capacity; ++i)
      result[i] = elements[i + left_];

   return result;
}

BoolVector::Iterator BoolVector::Begin() 
{
   return Iterator(elements);
}

BoolVector::Iterator BoolVector::End() {
   return Iterator(elements + size);
}

ostream& operator<<(ostream& stream, const BoolVector& vector_) 
{
   stream << "[< ";
   if (vector_.size == 0) {
      stream << " >]";
      return stream;
   }

   size_t vector_size = vector_.size - 1;
   for (size_t i = 0; i < vector_size; ++i)
      stream << vector_[i] << ", ";
   stream << vector_[vector_size];
   stream << " >]";
   return stream;
}

istream& operator>>(istream& stream, BoolVector& vector_) 
{
   if (vector_.elements) 
   {
      delete[] vector_.elements;
      vector_.size = 0;
   }
   vector_.elements = new bool[vector_.capacity];

   char ch;

   stream >> ch;
   if (ch != '[') 
      throw InvalidInputFormatException();

   stream >> ch;
   if (ch != '<') 
      throw InvalidInputFormatException();

   if (stream.peek() == '>') 
   {
      stream.ignore();
      if (stream.peek() == ']') 
      {
         stream.ignore();
         return stream;
      }
      else 
         throw InvalidInputFormatException();
   }

   char boolChar;

   while (stream.peek() == ' ')
      stream.ignore();

   stream.get(boolChar);
   if (!stream.good() || (boolChar != '0' && boolChar != '1'))
      throw InvalidInputFormatException();

   vector_.PushBack(boolChar == '1');

   while (stream.peek() == ' ')
      stream.ignore();

   while (stream.peek() == ',') 
   {
      stream.ignore();
      while (stream.peek() == ' ')
         stream.ignore();

      stream.get(boolChar);
      if (!stream.good() || (boolChar != '0' && boolChar != '1'))
         throw InvalidInputFormatException();

      vector_.PushBack(boolChar == '1');

      while (stream.peek() == ' ')
         stream.ignore();
   }

   if (stream.peek() == '>') 
   {
      stream.ignore();
      if (stream.peek() == ']') 
      {
         stream.ignore();
         return stream;
      }
      else 
         throw InvalidInputFormatException();
   }
   else
   {
      throw InvalidInputFormatException();
   }

   return stream;
}