#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

struct Node 
{
   string name;
   Node* next = nullptr;
};

class CountingBook {
private:
   int count;
   Node* head;

public:
   // ����������� �� ���������
   CountingBook() : count(0), head(nullptr) {}

   // ����������� � �������� �����
   CountingBook(const std::string* arr_, int size) 
   {
      if (size <= 0) 
      {
         head = nullptr;
         count = 0;
         return;
      }

      head = new Node{ arr_[0] };
      Node* cur = head;

      for (int i = 1; i < size; ++i) 
      {
         cur->next = new Node{ arr_[i] };
         cur = cur->next;
      }
      cur->next = head; // �������� ������
      count = size;
   }

   // ����������
   ~CountingBook() 
   {
      if (head == nullptr) return; // ���� ������ ������

      Node* cur = head;
      Node* nextNode = nullptr;

      // ������� ����
      nextNode = cur->next; // ���������� ��������� ����
      while (nextNode != head) 
      { // ������� ���� �� ��� ���, ���� �� �������� � ������
         delete cur; // ������� ������� ����
         cur = nextNode; // ��������� � ���������� ����
         nextNode = cur->next; // ���������� ��������� ����
      }
      delete cur; // ������� ��������� ���� (������)

      head = nullptr; // �������� ��������� �� ������
      count = 0;
   }


   // �������� ����
   void DeleteNode(Node* to_delete) {
      if (count == 0) return; // ���� ������ ������

      Node* cur = head;
      Node* prev = nullptr;

      // ������� ���� ��� ��������
      while (true) 
      {
         if (cur == to_delete) 
         {
            if (prev) 
               prev->next = cur->next;
            
            else { // ���� ������� ������
               Node* last = head;
               while (last->next != head) last = last->next; // ������� ��������� ����
               last->next = head->next; // ��������� ��������� ���������� ����
               head = head->next; // ��������� ������
            }
            delete cur;
            --count;
            return;
         }
         prev = cur;
         cur = cur->next;

         // ���� ��������� � ������ � �� ����� ����
         if (cur == head) break;
      }
   }

   // ������� ����������
   void Rhyme(int k) {
      if (count == 0) return; // ���� ������ ������

      Node* cur = head;
      while (count > 1) {
         for (int i = 0; i < k - 1; ++i) {
            cur = cur->next; // ������������ �� k-1 ����
         }
         cout << cur->name << " "; // ������� ��� ���������� ����
         Node* next_cur = cur->next; // ���������� ��������� ����
         DeleteNode(cur); // ������� ������� ����
         cur = next_cur; // ��������� � ���������� ����
      }
      cout << '\n' << head->name << endl; // ������� ���������� ����������� ��������
   }
};
