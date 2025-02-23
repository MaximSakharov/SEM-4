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

class CountingBook
{
private:
    int count;
    Node* head;
    Node* tail;

public:
    // ����������� �� ���������
    CountingBook() : count(0), head(nullptr), tail(nullptr) {}

    // ����������� � �������� �����
    CountingBook(const string* arr_, int size)
    {
        if (size <= 0)
        {
            head = nullptr;
            tail = nullptr;
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
        tail = cur; // ������������� �����
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
        tail = nullptr; // �������� ��������� �� �����
        count = 0;
    }

    // ���������� ���� � ����� ������
    void AddNode(const string& name)
    {
        Node* newNode = new Node{ name };
        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
            head->next = head; // �������� ������
        }
        else
        {
            tail->next = newNode;
            newNode->next = head;
            tail = newNode;
        }
        ++count;
    }

    // �������� ����
    void DeleteNode(Node* to_delete)
    {
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
                    tail->next = head->next; // ��������� ��������� ���������� ����
                    head = head->next; // ��������� ������
                }
                delete cur;
                --count;
                if (count == 0)
                {
                    head = nullptr;
                    tail = nullptr;
                }
                return;
            }
            prev = cur;
            cur = cur->next;

            // ���� ��������� � ������ � �� ����� ����
            if (cur == head) break;
        }
    }

    // ������� ����������
    void Rhyme(int k)
    {
        if (count == 0)
        {
            cout << "List is empty" << endl;
            return; // ���� ������ ������
        }
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

    // ����� ���� ��������� ������
    void PrintList() const
    {
        if (head == nullptr) return;

        Node* cur = head;
        do
        {
            cout << cur->name << " ";
            cur = cur->next;
        } while (cur != head);
        cout << endl;
    }

    // ��������� ���������� ��������� � ������
    int GetCount() const
    {
        return count;
    }

    // ��������� ������ ������
    Node* GetHead() const
    {
        return head;
    }

    // ��������� ������ ������
    Node* GetTail() const
    {
        return tail;
    }
};
