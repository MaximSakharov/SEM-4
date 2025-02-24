#include <iostream>

using namespace std;

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

// Базовый класс исключений
class Exception
{
public:
   virtual ~Exception() { }
   virtual void Print() const
   {
      cout << "Unknown error." << endl;
   }
};

// Исключение для неверного индекса
class IndexOutOfRangeException : public Exception
{
public:
   void Print() const override
   {
      cout << "Error: Index out of vector." << endl;
   }
};

// Исключение для операций с несоответствующими размерами векторов
class SizeMismatchException : public Exception
{
public:
   void Print() const override
   {
      cout << "Error: Vector size mismatch." << endl;
   }
};

// Исключение для пустого вектора
class EmptyVectorException : public Exception
{
public:
   void Print() const override
   {
      cout << "Error: Vector is empty" << endl;
   }
};

// Исключение для неверного формата ввода
class InvalidInputFormatException : public Exception
{
public:
   void Print() const override
   {
      cout << "Error: Invalid input format." << endl;
   }
};

class InvalidShiftException : public Exception
{
public:
   void Print() const override
   {
      cout << "Error: Shift cannot be negative" << endl;
   }
};

class IteratorOutOfRangeException : public Exception
{
public:
   void Print() const override
   {
      cout << "Error: Iterator Out Of Range" << endl;
   }
};

#endif