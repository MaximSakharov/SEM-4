#ifndef BOOL_VECTOR_HPP
#define BOOL_VECTOR_HPP

#include <iostream>
#include "Exception.hpp"

using namespace std;

class BoolVector {
private:
   bool* elements;
   size_t size;
   size_t capacity;

public:
   class Iterator {
   private:
      bool* current;

   public:
      Iterator(bool* ptr);
      bool& operator*();
      const bool& operator*() const;
      Iterator& operator++();
      Iterator operator++(int);
      bool operator==(const Iterator& other) const;
      bool operator!=(const Iterator& other) const;
   };

   BoolVector(size_t capacity_ = 0);
   BoolVector(bool* vector_, size_t capacity_);
   ~BoolVector();

   BoolVector& operator=(const BoolVector& other_);
   BoolVector(const BoolVector& other_);

   bool& operator[](size_t i) const;   // +
   bool& operator[](size_t i);         // +
   bool At(size_t pos_);               // +
   bool Front();
   bool Back();

   void SetElement(size_t pos_, bool new_element_); // +
   void PushBack(bool element_); // +
   void PopBack();   // +

   BoolVector OrOperation(const BoolVector& other_) const;  // +
   BoolVector AndOperation(const BoolVector& other_) const; // +
   bool operator==(const BoolVector& other_) const;   // +
   bool operator!=(const BoolVector& other_) const;   // +
   BoolVector operator!() const;    // +

   void Print();
   size_t ScalarProduct(const BoolVector& other_) const; // +
   BoolVector Concatenate(const BoolVector& other_) const; // +
   BoolVector ShiftLeftOperation(int n) const;  // +
   BoolVector ShiftRightOperation(int n) const; // +
   BoolVector SubvectorExtraction(size_t left_, size_t right_); // +

   Iterator Begin();
   Iterator End();

   friend ostream& operator<<(ostream& stream, const BoolVector& vector_);
   friend istream& operator>>(istream& stream, BoolVector& vector_);
};

#endif // BOOL_VECTOR_HPP