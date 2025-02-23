#include <iostream>
#include <string>
#include <fstream>

using namespace std;

enum SymbolicTokenClass { Zero, One, LF, Other};

// Структура для представления символьной лексемы.
struct SymbolicToken
{
   SymbolicTokenClass token_class; // Класс лексемы.
};

// Класс лексического анализатора.
class Parser1
{
   // Перечисление содержит константы для обозначения состояний автомата и одну дополнительную константу s_Stop.
   enum States { s_q0, s_q1, s_q2, s_q3, s_Stop };

   SymbolicToken s;                                    // Символьная лексема.

   // Размеры таблицы автомата.
   const int state_number = 4; // Число состояний (без s_Stop).
   const int class_number = 4; // Число символов входного алфавита.

   typedef int (Parser1::* function_pointer)();
   function_pointer** table; // Таблица указателей на процедуры автомата.

   static SymbolicToken transliterator(int ch)
   {
      SymbolicToken s;
      if (ch == '0')
      {
         s.token_class = Zero;
      }
      else if (ch == '1')
      {
         s.token_class = One;
      }
      else if (ch == '\0')
      {
         s.token_class = LF;
      }
      else
      {
         s.token_class = Other;
      }
      return s;
   }

   // Процедуры автомата (от A1 до Exit1).
   int q0()
   {
      return s_q0;
   }

   int q1()
   {
      return s_q1;
   }

   int q2()
   {
      return s_q2;
   }

   int q3()
   {
      return s_q3;
   }

   int Error1()
   {
      cout << "error: 0 - even" << endl;
      return s_Stop;
   }

   int Error2()
   {
      cout << "error: 1 - odd" << endl;
      return s_Stop;
   }

   int Error3()
   {
      cout << "error: after 0 there 1" << endl;
      return s_Stop;
   }

   int Error()
   {
      cout << "Error symbol!" << endl;
      return s_Stop;
   }

   int Exit1()
   {
      cout << "Good!" << endl;
      return s_Stop;
   }

public:
   // Конструктор инициализирует таблицу указателей на процедуры автомата.
   Parser1()
   {
      // Выделение памяти для таблицы
      table = new function_pointer * [state_number];
      // Сначала во все ячейки заносится указатель на процедуру обработки ошибок.
      for (int i = 0; i < state_number; ++i)
      {
         table[i] = new function_pointer[class_number];
         for (int j = 0; j < class_number; ++j)
            table[i][j] = &Parser1::Error;
      }
      // Далее заполняются ячейки для остальных процедур.
      table[s_q0][Zero] = &Parser1::q1; table[s_q0][One] = &Parser1::q3; table[s_q0][LF] = &Parser1::Error1;  table[s_q0][Other] = &Parser1::Error;
      table[s_q1][Zero] = &Parser1::q2; table[s_q1][One] = &Parser1::Error3; table[s_q1][LF] = &Parser1::Exit1;  table[s_q1][Other] = &Parser1::Error;
      table[s_q2][Zero] = &Parser1::q1; table[s_q2][One] = &Parser1::Error3; table[s_q2][LF] = &Parser1::Error1;  table[s_q2][Other] = &Parser1::Error;
      table[s_q3][Zero] = &Parser1::Error2; table[s_q3][One] = &Parser1::q0; table[s_q3][LF] = &Parser1::Error2;  table[s_q3][Other] = &Parser1::Error;
   }

   // Основная функция анализа. Функция запускает конечный автомат на тексте из файла filename.
   void parse(string str_)
   {
      int ch;             // Входной символ.
      int state = s_q0;   // Текущее состояние автомата.
      int i = 0;
      while (state != s_Stop)
      {
         ch = str_[i];
         s = transliterator(ch);
         state = (this->*table[state][s.token_class])();
         ++i;
      }
   }

   // Основная функция анализа. Функция запускает конечный автомат на тексте из файла filename.
   void parse_file(const char* filename_)
   {
       ifstream in(filename_);
       string str;
       if (!in)
       {
           cout << "Can't open file" << filename_ << endl;
           return;
       }

       //getline(in, str);
       while (!in.eof())
       {
           getline(in, str);
           cout << str << endl;
           parse(str);
       }

       in.close();
   }
};

// Класс лексического анализатора.
class Parser2
{
   // Перечисление содержит константы для обозначения состояний автомата и одну дополнительную константу s_Stop.
   enum States { s_q0, s_q1, s_q2, s_Stop };

   SymbolicToken s;                                    // Символьная лексема.

   // Размеры таблицы автомата.
   const int state_number = 3; // Число состояний (без s_Stop).
   const int class_number = 4; // Число символов входного алфавита.

   typedef int (Parser2::* function_pointer)();
   function_pointer** table; // Таблица указателей на процедуры автомата.

   static SymbolicToken transliterator(int ch)
   {
      SymbolicToken s;
      if (ch == '0')
      {
         s.token_class = Zero;
      }
      else if (ch == '1')
      {
         s.token_class = One;
      }
      else if (ch == '\0')
      {
         s.token_class = LF;
      }
      else
      {
         s.token_class = Other;
      }
      return s;
   }

   // Процедуры автомата (от A1 до Exit1).
   int q0()
   {
      return s_q0;
   }

   int q1()
   {
      return s_q1;
   }

   int q2()
   {
      return s_q2;
   }

   int Error1()
   {
      cout << "error: odd number of '0' between '1's" << endl;
      return s_Stop;
   }

   int Error2()
   {
      cout << "error: '0' not between '1's" << endl;
      return s_Stop;
   }

   int Error()
   {
      cout << "Error symbol!" << endl;
      return s_Stop;
   }

   int Exit1()
   {
      cout << "Good!" << endl;
      return s_Stop;
   }

public:
   // Конструктор инициализирует таблицу указателей на процедуры автомата.
   Parser2()
   {
      // Выделение памяти для таблицы
      table = new function_pointer * [state_number];
      // Сначала во все ячейки заносится указатель на процедуру обработки ошибок.
      for (int i = 0; i < state_number; ++i)
      {
         table[i] = new function_pointer[class_number];
         for (int j = 0; j < class_number; ++j)
            table[i][j] = &Parser2::Error;
      }
      // Далее заполняются ячейки для остальных процедур.
      table[s_q0][Zero] = &Parser2::q0; table[s_q0][One] = &Parser2::q1; table[s_q0][LF] = &Parser2::Exit1;  table[s_q0][Other] = &Parser2::Error;
      table[s_q1][Zero] = &Parser2::q2; table[s_q1][One] = &Parser2::q1; table[s_q1][LF] = &Parser2::Exit1;  table[s_q1][Other] = &Parser2::Error;
      table[s_q2][Zero] = &Parser2::q1; table[s_q2][One] = &Parser2::Error1; table[s_q2][LF] = &Parser2::Exit1;  table[s_q2][Other] = &Parser2::Error;
   }

   // Основная функция анализа. Функция запускает конечный автомат на тексте из файла filename.
   void parse(string str_)
   {
      int ch;             // Входной символ.
      int state = s_q0;   // Текущее состояние автомата.
      int i = 0;
      while (state != s_Stop)
      {
         ch = str_[i];
         s = transliterator(ch);
         state = (this->*table[state][s.token_class])();
         ++i;
      }
   }
   // Основная функция анализа. Функция запускает конечный автомат на тексте из файла filename.
   void parse_file(const char* filename_)
   {
       ifstream in(filename_);
       string str;
       if (!in)
       {
           cout << "Can't open file" << filename_ << endl;
           return;
       }

       getline(in, str);
       while (!in.eof())
       {
           parse(str);
       }

       in.close();
   }
};

// Класс лексического анализатора.
class Parser3
{
   // Перечисление содержит константы для обозначения состояний автомата и одну дополнительную константу s_Stop.
   enum States { s_q0, s_q1, s_q2, s_q3, s_Stop };

   SymbolicToken s;                                    // Символьная лексема.

   // Размеры таблицы автомата.
   const int state_number = 4; // Число состояний (без s_Stop).
   const int class_number = 4; // Число символов входного алфавита.

   typedef int (Parser3::* function_pointer)();
   function_pointer** table; // Таблица указателей на процедуры автомата.

   static SymbolicToken transliterator(int ch)
   {
      SymbolicToken s;
      if (ch == '0')
      {
         s.token_class = Zero;
      }
      else if (ch == '1')
      {
         s.token_class = One;
      }
      else if (ch == '\0')
      {
         s.token_class = LF;
      }
      else
      {
         s.token_class = Other;
      }
      return s;
   }

   // Процедуры автомата (от A1 до Exit1).
   int q0()
   {
      return s_q0;
   }

   int q1()
   {
      return s_q1;
   }

   int q2()
   {
      return s_q2;
   }

   int Error1()
   {
      cout << "error: after a pair of '1's there is no '0'" << endl;
      return s_Stop;
   }

   int Error()
   {
      cout << "Error symbol!" << endl;
      return s_Stop;
   }

   int Exit1()
   {
      cout << "Good!" << endl;
      return s_Stop;
   }

public:
   // Конструктор инициализирует таблицу указателей на процедуры автомата.
   Parser3()
   {
      // Выделение памяти для таблицы
      table = new function_pointer * [state_number];
      // Сначала во все ячейки заносится указатель на процедуру обработки ошибок.
      for (int i = 0; i < state_number; ++i)
      {
         table[i] = new function_pointer[class_number];
         for (int j = 0; j < class_number; ++j)
            table[i][j] = &Parser3::Error;
      }
      // Далее заполняются ячейки для остальных процедур.
      table[s_q0][Zero] = &Parser3::q0; table[s_q0][One] = &Parser3::q1; table[s_q0][LF] = &Parser3::Exit1;  table[s_q0][Other] = &Parser3::Error;
      table[s_q1][Zero] = &Parser3::q0; table[s_q1][One] = &Parser3::q2; table[s_q1][LF] = &Parser3::Exit1;  table[s_q1][Other] = &Parser3::Error;
      table[s_q2][Zero] = &Parser3::q0; table[s_q2][One] = &Parser3::Error1; table[s_q2][LF] = &Parser3::Error1;  table[s_q2][Other] = &Parser3::Error;
   }

   // Основная функция анализа. Функция запускает конечный автомат на тексте из файла filename.
   void parse(string str_)
   {
      int ch;             // Входной символ.
      int state = s_q0;   // Текущее состояние автомата.
      int i = 0;
      while (state != s_Stop)
      {
         ch = str_[i];
         s = transliterator(ch);
         state = (this->*table[state][s.token_class])();
         ++i;
      }
   }
   // Основная функция анализа. Функция запускает конечный автомат на тексте из файла filename.
   void parse_file(const char* filename_)
   {
       ifstream in(filename_);
       string str;
       if (!in)
       {
           cout << "Can't open file" << filename_ << endl;
           return;
       }

       getline(in, str);
       while (!in.eof())
       {
           parse(str);
       }

       in.close();
   }
};

// Класс лексического анализатора.
class Parser4
{
   // Перечисление содержит константы для обозначения состояний автомата и одну дополнительную константу s_Stop.
   enum States { s_q0, s_q1, s_q2, s_q3, s_Stop };

   SymbolicToken s;                                    // Символьная лексема.

   // Размеры таблицы автомата.
   const int state_number = 4; // Число состояний (без s_Stop).
   const int class_number = 4; // Число символов входного алфавита.

   typedef int (Parser4::* function_pointer)();
   function_pointer** table; // Таблица указателей на процедуры автомата.

   static SymbolicToken transliterator(int ch)
   {
      SymbolicToken s;
      if (ch == '0')
      {
         s.token_class = Zero;
      }
      else if (ch == '1')
      {
         s.token_class = One;
      }
      else if (ch == '\0')
      {
         s.token_class = LF;
      }
      else
      {
         s.token_class = Other;
      }
      return s;
   }

   // Процедуры автомата (от A1 до Exit1).
   int q0()
   {
      return s_q0;
   }

   int q1()
   {
      return s_q1;
   }

   int q2()
   {
      return s_q2;
   }

   int Error1()
   {
      cout << "error: not every third character is '1'" << endl;
      return s_Stop;
   }

   int Error()
   {
      cout << "Error symbol!" << endl;
      return s_Stop;
   }

   int Exit1()
   {
      cout << "Good!" << endl;
      return s_Stop;
   }

public:
   // Конструктор инициализирует таблицу указателей на процедуры автомата.
   Parser4()
   {
      // Выделение памяти для таблицы
      table = new function_pointer * [state_number];
      // Сначала во все ячейки заносится указатель на процедуру обработки ошибок.
      for (int i = 0; i < state_number; ++i)
      {
         table[i] = new function_pointer[class_number];
         for (int j = 0; j < class_number; ++j)
            table[i][j] = &Parser4::Error;
      }
      // Далее заполняются ячейки для остальных процедур.
      table[s_q0][Zero] = &Parser4::q1; table[s_q0][One] = &Parser4::q1; table[s_q0][LF] = &Parser4::Exit1;  table[s_q0][Other] = &Parser4::Error;
      table[s_q1][Zero] = &Parser4::q2; table[s_q1][One] = &Parser4::q2; table[s_q1][LF] = &Parser4::Exit1;  table[s_q1][Other] = &Parser4::Error;
      table[s_q2][Zero] = &Parser4::Error1; table[s_q2][One] = &Parser4::q0; table[s_q2][LF] = &Parser4::Exit1;  table[s_q2][Other] = &Parser4::Error;
   }

   // Основная функция анализа. Функция запускает конечный автомат на тексте из файла filename.
   void parse(string str_)
   {
      int ch;             // Входной символ.
      int state = s_q0;   // Текущее состояние автомата.
      int i = 0;
      while (state != s_Stop)
      {
         ch = str_[i];
         s = transliterator(ch);
         state = (this->*table[state][s.token_class])();
         ++i;
      }
   }
   // Основная функция анализа. Функция запускает конечный автомат на тексте из файла filename.
   void parse_file(const char* filename_)
   {
       ifstream in(filename_);
       string str;
       if (!in)
       {
           cout << "Can't open file" << filename_ << endl;
           return;
       }

       getline(in, str);
       while (!in.eof())
       {
           parse(str);
       }

       in.close();
   }
};