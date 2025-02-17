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
    // Конструктор по умолчанию
    CountingBook() : count(0), head(nullptr), tail(nullptr) {}

    // Конструктор с массивом строк
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
        cur->next = head; // Замыкаем список
        tail = cur; // Устанавливаем хвост
        count = size;
    }

    // Деструктор
    ~CountingBook()
    {
        if (head == nullptr) return; // Если список пустой

        Node* cur = head;
        Node* nextNode = nullptr;

        // Удаляем узлы
        nextNode = cur->next; // Запоминаем следующий узел
        while (nextNode != head)
        { // Удаляем узлы до тех пор, пока не вернемся к голове
            delete cur; // Удаляем текущий узел
            cur = nextNode; // Переходим к следующему узлу
            nextNode = cur->next; // Запоминаем следующий узел
        }
        delete cur; // Удаляем последний узел (голову)

        head = nullptr; // Обнуляем указатель на голову
        tail = nullptr; // Обнуляем указатель на хвост
        count = 0;
    }

    // Добавление узла в конец списка
    void AddNode(const string& name)
    {
        Node* newNode = new Node{ name };
        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
            head->next = head; // Замыкаем список
        }
        else
        {
            tail->next = newNode;
            newNode->next = head;
            tail = newNode;
        }
        ++count;
    }

    // Удаление узла
    void DeleteNode(Node* to_delete)
    {
        if (count == 0) return; // Если список пустой

        Node* cur = head;
        Node* prev = nullptr;

        // Находим узел для удаления
        while (true)
        {
            if (cur == to_delete)
            {
                if (prev)
                    prev->next = cur->next;

                else { // Если удаляем голову
                    tail->next = head->next; // Обновляем указатель последнего узла
                    head = head->next; // Обновляем голову
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

            // Если вернулись к голове и не нашли узел
            if (cur == head) break;
        }
    }

    // Функция считалочки
    void Rhyme(int k)
    {
        if (count == 0)
        {
            cout << "List is empty" << endl;
            return; // Если список пустой
        }
        Node* cur = head;
        while (count > 1) {
            for (int i = 0; i < k - 1; ++i) {
                cur = cur->next; // Перемещаемся на k-1 узел
            }
            cout << cur->name << " "; // Выводим имя удаляемого узла
            Node* next_cur = cur->next; // Запоминаем следующий узел
            DeleteNode(cur); // Удаляем текущий узел
            cur = next_cur; // Переходим к следующему узлу
        }
        cout << '\n' << head->name << endl; // Выводим последнего оставшегося человека
    }

    // Вывод всех элементов списка
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

    // Получение количества элементов в списке
    int GetCount() const
    {
        return count;
    }

    // Получение головы списка
    Node* GetHead() const
    {
        return head;
    }

    // Получение хвоста списка
    Node* GetTail() const
    {
        return tail;
    }
};
