#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <locale>
#include <stack>
#include <list>
#include <algorithm>

#include "BoolVector.hpp"

using namespace std;

// Перечисление содержит константы для обозначения классов символьных лексем: БУКВА, ЦИФРА, КОМПОНЕНТЫ ВЕКТОРА, ОПЕРАЦИИ НАД ВЕКТОРОМ, ЗАПЯТАЯ,АРИФМЕТИЧЕСКАЯ ОПЕРАЦИЯ, ОТНОШЕНИЕ, ПРОБЕЛ, КОНЕЦ СТРОКИ (LF), КОММЕНТАРИЙ (;), ОШИБКА, КОНЕЦ.
enum SymbolicTokenClass { Letter, Digit, Brace, VectorComponents, VectorOperations, Comma, ArithOp, Relation, Space, Semicolon, Colon, LF, Error, EndOfFile };

/*
A1. Ожидается команда в начале строки. Это первая команда программы.
A2. Ожидается очередная (не первая) команда в начале строки.
B1. Считывается продолжение ключевого слова.
C1. Ожидается конец строки после правильной команды.
D1. Был прочитан первый символ отношения. Нужно завершить обработку отношения.
E1. Только что прочитано слово push, нужно дождаться пробела.
E2. Только что прочитано слово jmp или ji, нужно дождаться пробела.
E3. Только что прочитано слово pop, нужно дождаться пробела.
F1. Ожидается первый символ переменной или константы (после push).
F2. Ожидается начало константы (после jmp или ji).
F3. Ожидается начало переменной (после pop).
G1. Считывается константа.
H1. Считывается переменная
I1. Считывается комментарий, перед которым не было команд.
I2. Считывается комментарий, перед которым были команды.
J1. Считывается остаток строки после ошибки до перехода на новую строку.
K1. Считывает '<' вектора после '['
K2. Ожидает булевое число или конец вектора '>'
K3. Ожидает конец вектора ']'
L1. Ожидает запятую или конец вектора '>'
L2. Ожидает запятую или конец вектора '>' после пробелов
*/
enum States { s_A1, s_A2, s_A3, s_A4, s_B1, s_B2, s_B3, s_C1, s_C2, s_D1, s_E1, s_F1, s_I1, s_I2, s_I3, s_Stop, s_Error };

/*
   NONE,
   ARITHMETIC_OPERATION,
   RELATION,
   VARIABLE,
   MARK,
   ASSIGNMENT_MARK,
   SEMICOLON, // Точка с запятой
   COLON, // Двоеточие
   THE_OPEN_CURLY_BRACE,
   THE_CLOSING_CURLY_BRACE,
   THE_OPEN_ROUND_BRACE,
   THE_CLOSING_ROUND_BRACE,
   DECLARE,
   AS,
   TYPE,
   LET,
   WHILE,
   DO,
   STEP,
   IF,
   ELSE,
   LOAD,
   PUT,
   JUMP,
   SWITCH,
   WHEN,
   DEFAULT,
   END,
   BAD,
   COMMENT,
   ERROR
*/
enum ListTokenClass
{
   NONE, ARITHMETIC_OPERATION, VECTOR_OPERATION, RELATION, VARIABLE, CONSTANT, BOOLEAN_VECTOR, COMMA, MARK, ASSIGNMENT_MARK, SEMICOLON, COLON, THE_OPEN_CURLY_BRACE, THE_CLOSING_CURLY_BRACE, ROUND_BRACE,
   DECLARE, AS, TYPE, LET, WHILE, DO, FOR, TO, STEP, IF, ELSE, LOAD, PUT, JUMP, SWITCH, WHEN, DEFAULT, END, BAD, COMMENT,
   SUBVECTOR, SHIFTLEFT, SHIFTRIGHT, CONCATENATE, POPBACK, PUSHBACK, AT, SET, SCALAR,
   ERROR, END_OF_FILE
};
//"shiftright", "concatenate", "popback", "pushback", "at", "set", "scalar",

enum ErrorList
{
   NOT_BOOLEAN_VECTOR_ERROR, NOT_VARIABLE_ERROR, NOT_MARK_ERROR, NOT_ARITHMETIC_OPERATION_ERROR, NOT_VECTOR_OPERATION_ERROR, NOT_RELATION_ERROR, NOT_EXISTING_COMMAND_ERROR
};

class KeyWord
{
   string name;
   ListTokenClass class_name;
public:
   KeyWord(string name_, ListTokenClass class_name_)
      : name(name_), class_name(class_name_) {}
   ~KeyWord()
   {
      name.erase();
   }
   string GetName() const
   {
      return name;
   }
   ListTokenClass GetClassName() const
   {
      return class_name;
   }
};

static const vector<KeyWord> keywords =
{ KeyWord("declare", DECLARE),
   KeyWord("as", AS),
   KeyWord("let", LET),
   KeyWord("while", WHILE),
   KeyWord("do", DO),
   KeyWord("for", FOR),
   KeyWord("to", TO),
   KeyWord("step", STEP),
   KeyWord("if", IF),
   KeyWord("else", ELSE),
   KeyWord("load", LOAD),
   KeyWord("put", PUT),
   KeyWord("jump", JUMP),
   KeyWord("switch", SWITCH),
   KeyWord("when", WHEN),
   KeyWord("default", DEFAULT),
   KeyWord("end", END),
   KeyWord("bad", BAD),
   KeyWord("subvector", SUBVECTOR),
   KeyWord("shiftleft", SHIFTLEFT),
   KeyWord("shiftright", SHIFTRIGHT),
   KeyWord("concatenate", CONCATENATE),
   KeyWord("popback", POPBACK),
   KeyWord("pushback", PUSHBACK),
   KeyWord("at", AT),
   KeyWord("set", SET),
   KeyWord("scalar", SCALAR),
   KeyWord("CN", TYPE),
   KeyWord("BV", TYPE)
};

/*
ostream& operator<<(std::ostream& out, const ListTokenClass& t)
{
   switch (t)
   {
   case NONE:                  return out << "NONE";
   case PUSH:                  return out << "PUSH";
   case POP:                   return out << "POP";
   case ARITHMETIC_OPERATION:  return out << "ARITHMETIC_OPERATION";
   case RELARATIO:                 return out << "RATIO";
   case JMP:                   return out << "JMP";
   case JI:                    return out << "JI";
   case READ:                  return out << "READ";
   case WRITE:                 return out << "WRITE";
   case END:                   return out << "END";
   case COMMENT:               return out << "COMMENT";
   case ERROR:                 return out << "ERROR";
   case END_OF_FILE:           return out << "END_OF_FILE";
   case SUBVECTOR:             return out << "SUBVECTOR";
   case SHIFT_LEFT:            return out << "SHIFT_LEFT";
   case SHIFT_RIGHT:           return out << "SHIFT_RIGHT";
   case CONCATENATE:           return out << "CONCATENATE";
   case OR_OPERATION:          return out << "OR_OPERATION";
   case AND_OPERATION:         return out << "AND_OPERATION";
   case NOT_OPERATION:         return out << "NOT_OPERATION";
   case POP_BACK:              return out << "POP_BACK";
   case PUSH_BACK:             return out << "PUSH_BACK";
   case AT:                    return out << "AT";
   case SET:                   return out << "SET";
   case VECTOR_OPERATION:      return out << "VECTOR_OPERATION";
   case SCALAR_PRODUCT:        return out << "SCALAR_PRODUCT";
   default:                    return out << "UNKNOWN";
   }
}
*/

std::ostream& operator<<(std::ostream& out, const ListTokenClass& t)
{
   switch (t)
   {
   case NONE:                     return out << "NONE";
   case ARITHMETIC_OPERATION:     return out << "ARITHMETIC_OPERATION";
   case VECTOR_OPERATION:         return out << "VECTOR_OPERATION";
   case RELATION:                 return out << "RELATION";
   case VARIABLE:                 return out << "VARIABLE";
   case CONSTANT:                 return out << "CONSTANT";
   case BOOLEAN_VECTOR:           return out << "BOOLEAN_VECTOR";
   case COMMA:                    return out << "COMMA";
   case MARK:                     return out << "MARK";
   case ASSIGNMENT_MARK:          return out << "ASSIGNMENT_MARK";
   case SEMICOLON:               return out << "SEMICOLON";
   case COLON:                    return out << "COLON";
   case THE_OPEN_CURLY_BRACE:    return out << "THE_OPEN_CURLY_BRACE";
   case THE_CLOSING_CURLY_BRACE: return out << "THE_CLOSING_CURLY_BRACE";
   case ROUND_BRACE:             return out << "ROUND_BRACE";
   case DECLARE:                 return out << "DECLARE";
   case AS:                      return out << "AS";
   case TYPE:                    return out << "TYPE";
   case LET:                     return out << "LET";
   case WHILE:                   return out << "WHILE";
   case DO:                      return out << "DO";
   case FOR:                     return out << "FOR";
   case TO:                      return out << "TO";
   case STEP:                    return out << "STEP";
   case IF:                      return out << "IF";
   case ELSE:                    return out << "ELSE";
   case LOAD:                    return out << "LOAD";
   case PUT:                     return out << "PUT";
   case JUMP:                    return out << "JUMP";
   case SWITCH:                  return out << "SWITCH";
   case WHEN:                    return out << "WHEN";
   case DEFAULT:                 return out << "DEFAULT";
   case END:                     return out << "END";
   case BAD:                     return out << "BAD";
   case COMMENT:                 return out << "COMMENT";
   case SUBVECTOR:               return out << "SUBVECTOR";
   case SHIFTLEFT:               return out << "SHIFTLEFT";
   case SHIFTRIGHT:              return out << "SHIFTRIGHT";
   case CONCATENATE:             return out << "CONCATENATE";
   case POPBACK:                 return out << "POPBACK";
   case PUSHBACK:                return out << "PUSHBACK";
   case AT:                      return out << "AT";
   case SET:                     return out << "SET";
   case SCALAR:                  return out << "SCALAR";
   case ERROR:                   return out << "ERROR";
   case END_OF_FILE:             return out << "END_OF_FILE";
   default:                      return out << "UNKNOWN_TOKEN";
   }
}

// Структура для представления символьной лексемы.
struct SymbolicToken
{
   SymbolicTokenClass token_class; // Класс лексемы.
   int value;                      // Значение лексемы.
};

// Глобальные переменные, используемые процедурами автомата.
SymbolicToken s;    // ���������� �������.
ListTokenClass class_regist;
void* pointer_regist;
int number_regist;         // Регистр числа.
int ratio_regist;          // Регистр отношения 
int value_regist;          // Регистр значения 
string variable_register;     // Регистр переменной 
int detection_register;    // Регистр обнаружения для таблицы обнаружения
int line_number;           // Номер строки
int constant_flag;         // Флаг константы
bool error_flag;
bool type_regist;
BoolVector bool_vector_register;

int prev_num_indent_regist = 0;
int cur_num_indent_regist = 0;
char char_regist;
string lexeme_regist;
ErrorList error_regist;

//stack<pair<int, BoolVector>> execution_stack;

int A1(), A1_a(), A2_a(), A2_b(), A3_a(),
Exit1(), Exit2(), Exit3(), Exit4();
//Error1(), Error2(), Error3(), Error4(), Error5(), Error6(), Error7(), Error8(), Error9(), Error10(), Error11(), Error12(), Error13(), Error14();

void AddLexemeProcess();

list<int> constant_table;

list<BoolVector> bool_vector_table;

struct Variable
{
   string name;
   int type;
   pair<int, BoolVector> value;
};

list<Variable> variable_table;

typedef int (*TransitionFunc)();

struct LexemeList
{
   ListTokenClass class_token;
   union
   {
      int value;
      void* pointer;   // Для хранения указателей на переменные или константы.
   };
   int number_lexeme_line;
};

class LexemeVector {
private:
   LexemeList* data;
   size_t capacity;
   size_t size;

   void reallocate(size_t new_capacity) {
      LexemeList* new_data = new LexemeList[new_capacity];
      std::copy(data, data + size, new_data);
      delete[] data;
      data = new_data;
      capacity = new_capacity;
   }

public:
   LexemeVector() : data(nullptr), capacity(0), size(0) {}

   ~LexemeVector() {
      delete[] data;
   }

   void push_back(const LexemeList& lexeme) {
      if (size == capacity) {
         size_t new_capacity = capacity == 0 ? 1 : capacity * 2;
         reallocate(new_capacity);
      }
      data[size++] = lexeme;
   }

   void pop_back() {
      if (size > 0) {
         --size;
      }
      else {
         throw std::out_of_range("Attempt to pop from an empty vector");
      }
   }

   LexemeList& operator[](size_t index) {
      if (index >= size) {
         throw std::out_of_range("Index out of bounds");
      }
      return data[index];
   }

   const LexemeList& operator[](size_t index) const {
      if (index >= size) {
         throw std::out_of_range("Index out of bounds");
      }
      return data[index];
   }

   size_t get_size() const {
      return size;
   }

   size_t get_capacity() const {
      return capacity;
   }

   void set_capacity(size_t new_cap_) 
   {
      capacity = new_cap_;
   }

   void clear() {
      size = 0;
   }

   bool empty() const {
      return size == 0;
   }

   LexemeList& back() {
      if (empty()) {
         throw std::out_of_range("Attempt to access back of an empty vector");
      }
      return data[size - 1];
   }

   const LexemeList& back() const {
      if (empty()) {
         throw std::out_of_range("Attempt to access back of an empty vector");
      }
      return data[size - 1];
   }
};

LexemeVector lexeme_list;

struct Mark
{
   string name;
};

list<Mark> mark_table;

class ErrorClass
{
   ErrorList error_type;
   string error_name;
public:
   ErrorClass(ErrorList error_type_, string error_name_)
      : error_type(error_type_), error_name(error_name_) {}
   ~ErrorClass()
   {
      error_name.erase();
   }
   ErrorList GetErrorType() { return error_type; }
   string GerErrorName() { return error_name; }
};

list<ErrorClass> error_list =
{
   ErrorClass(NOT_BOOLEAN_VECTOR_ERROR, string("Boolean vector set incorrectly!")),
   ErrorClass(NOT_VARIABLE_ERROR, string("Variable cannot be set starting with number!")),
   ErrorClass(NOT_MARK_ERROR, string("A mark cannot start with a number!")),
   ErrorClass(NOT_ARITHMETIC_OPERATION_ERROR, string("No existing arithmetic operation entered!")),
   ErrorClass(NOT_VECTOR_OPERATION_ERROR, string("No existing vector operation entered!")),
   ErrorClass(NOT_RELATION_ERROR, string("No existing relation entered!")),
   ErrorClass(NOT_EXISTING_COMMAND_ERROR, string("No existing command entered!"))
};

/*
int ErrorHandlingProcess()
{
   class_regist = ERROR;

   error_flag = 1;

   AddLexemeProcess();

   return s_J1;
}
*/

// Функция для проверки, является ли строка числом
bool isNumber(const std::string& str)
{
   // Удаляем пробелы
   std::string trimmed = str;
   trimmed.erase(std::remove(trimmed.begin(), trimmed.end(), ' '), trimmed.end());

   // Если строка пустая, это не число
   if (trimmed.empty())
      return false;

   // Проверяем, что все символы — цифры
   return std::all_of(trimmed.begin(), trimmed.end(), ::isdigit);
}

// Функция для проверки, является ли строка булевым вектором
bool isBoolVector(const string& str, BoolVector& boolVec)
{
   boolVec = BoolVector();

   string trimmed = str;

   // Удаляем лишние пробелы
   trimmed.erase(remove(trimmed.begin(), trimmed.end(), ' '), trimmed.end());

   // Проверяем, начинается и заканчивается ли строка с '[' и ']'
   if (trimmed.size() < 4 || trimmed.front() != '[' || trimmed.back() != ']' || trimmed[1] != '<' || trimmed[trimmed.size() - 2] != '>')
      return false;

   if (trimmed.size() == 4)
      return true;

   int i = 2;

   while (trimmed[i] == ' ')
      ++i;

   if (trimmed[i] == '>')
   {
      ++i;
      if (trimmed[i] == ']')
      {
         //class_regist = ERROR;
         //error_regist = VECTOR_ERROR;
         return true;
      }
   }

   if (trimmed[i] != '0' && trimmed[i] != '1')
   {
      //class_regist = ERROR;
      //error_regist = NOT_BOOLEAN_VECTOR_ERROR;
      return false;
   }

   boolVec.PushBack(trimmed[i] == '1');

   ++i;

   while (trimmed[i] == ' ')
      ++i;

   while (trimmed[i] == ',')
   {
      ++i;
      while (trimmed[i] == ' ')
         ++i;

      if (trimmed[i] != '0' && trimmed[i] != '1')
         return false;

      boolVec.PushBack(trimmed[i] == '1');

      ++i;

      while (trimmed[i] == ' ')
         ++i;
   }

   if (trimmed[i] != '>')
   {
      //class_regist = ERROR;
      //error_regist = VECTOR_ERROR;
      return false;
   }
   else
   {
      ++i;
      if (trimmed[i] != ']')
      {
         //class_regist = ERROR;
         //error_regist = VECTOR_ERROR;
         return false;
      }
   }

   return true;
}

/*
void KeyWordCheck()
{
   for (const KeyWord& keyword : keywords)
   {
      if (lexeme_regist == keyword.GetName())
      {
         class_regist = keyword.GetClassName();
         return;
      }
   }
}
*/

// Процедура ДОБАВИТЬ_КОНСТАНТУ
void AddConstantProcess()
{
   //if (isNumber(lexeme_regist))
  //    return;

   int number = 0;

   for (int i = 0; lexeme_regist[i] != '\0'; ++i)
   {
      number *= 10;
      number += lexeme_regist[i] - '0';
   }

   for (int& constant : constant_table)
   {
      if (constant == number)
      {
         pointer_regist = &constant;  // Установить указатель на существующую константу.
         return;
      }
   }

   constant_table.push_back(number);
   pointer_regist = &constant_table.back();
   //cout << "Pointer assigned: " << pointer_regist << endl;
}

void AddBoolVectorProcess()
{
   if (!isBoolVector(lexeme_regist, bool_vector_register))
   {
      class_regist = ERROR;
      error_regist = NOT_BOOLEAN_VECTOR_ERROR;

      return;
   }

   for (BoolVector& bool_vector : bool_vector_table)
   {
      if (bool_vector == bool_vector_register)
      {
         pointer_regist = &bool_vector;
         bool_vector_register = BoolVector();
         return;
      }
   }

   bool_vector_table.push_back(bool_vector_register);
   //delete &bool_vector_register;
   bool_vector_register = BoolVector();
   pointer_regist = &bool_vector_table.back();
   //cout << "Pointer assigned: " << pointer_regist << endl;
}

void AddMarkProcess()
{
   // Проверить, совпадает ли РЕГИСТР_ПЕРЕМЕННОЙ с ключевым словом.
   //static const vector<string> keywords = { "declare","as", "let","while","do","for","to","step","if","else","load","put","jump", "switch","when","default","end", "bad" ,"subvector", "shiftleft", "shiftright", "concatenate", "popback", "pushback", "at", "set", "scalar", "CN","BV"};
   /*
   for (const KeyWord& keyword : keywords)
   {
      if (lexeme_regist == keyword.GetName())
      {
         class_regist = keyword.GetClassName();
         AddLexemeProcess();
         class_regist = COLON;
         lexeme_regist = '%';
         return;
      }
   }
   */

   // Проверить, есть ли переменная в таблице меток.
   for (Mark& var : mark_table)
   {
      if (var.name == lexeme_regist)
      {
         pointer_regist = &var;
         return;
      }
   }

   mark_table.push_back(Mark{ lexeme_regist });
   pointer_regist = &mark_table.back();
   //cout << "Pointer assigned: " << pointer_regist << ", Variable name: " << variable_table.back().name << endl;
}

// Процедура ДОБАВИТЬ_ПЕРЕМЕННУЮ

void AddVariableProcess()
{
   // Проверить, совпадает ли РЕГИСТР_ПЕРЕМЕННОЙ с ключевым словом.
   //static const vector<string> keywords = { "declare", "as", "let","while","do","for","to","step","if","else","load","put","jump", "switch","when","default","end", "bad" ,"subvector", "shiftleft", "shiftright", "concatenate", "popback", "pushback", "at", "set", "scalar","CN","BV"};

   /*
   for (const KeyWord& keyword : keywords)
   {
      if (lexeme_regist == keyword.GetName())
      {
         //ErrorHandlingProcess();
         class_regist = keyword.GetClassName();
         AddLexemeProcess();

         return;
      }
   }
   */

   // Проверить, есть ли переменная в таблице переменных.
   for (Variable& var : variable_table)
   {
      if (var.name == lexeme_regist)
      {
         pointer_regist = &var;
         return;
      }
   }

   // ���� ���������� ���, ��������� �.
   variable_table.push_back(Variable{ lexeme_regist, type_regist, pair<int, BoolVector>(0, BoolVector()) });
   pointer_regist = &variable_table.back();
   //cout << "Pointer assigned: " << pointer_regist << ", Variable name: " << variable_table.back().name << endl;
}

void AddErrorProcess()
{
   for (ErrorClass& err : error_list)
   {
      if (err.GetErrorType() == error_regist)
      {
         pointer_regist = &err;
         return;
      }
   }

   //variable_table.push_back(Variable{ lexeme_regist, type_regist, pair<int, BoolVector>(0, BoolVector()) });
   //pointer_regist = &variable_table.back();
   //cout << "Pointer assigned: " << pointer_regist << ", Variable name: " << variable_table.back().name << endl;
}

// Процедура СОЗДАТЬ_ЛЕКСЕМУ
void AddLexemeProcess()
{
   if (class_regist == VARIABLE || class_regist == MARK)
   {
      for (const KeyWord& keyword : keywords)
      {
         if (lexeme_regist == keyword.GetName())
         {
            //ErrorHandlingProcess();
            class_regist = keyword.GetClassName();
            break;
         }
      }
      if (isdigit(lexeme_regist[0]) && class_regist == VARIABLE)
      {
         class_regist = ERROR;
         error_regist = NOT_VARIABLE_ERROR;
      }
      if (isdigit(lexeme_regist[0]) && class_regist == MARK)
      {
         class_regist = ERROR;
         error_regist = NOT_MARK_ERROR;
      }
   }

   if (!lexeme_list.empty() && class_regist == VARIABLE && lexeme_list.back().class_token == JUMP)
      class_regist = MARK;

   LexemeList lexeme;
   lexeme.class_token = class_regist;
   lexeme.number_lexeme_line = line_number;

   //cout << pointer_regist << endl;

   // Ключевые слова и односвязные лексемы можно не добавлять
   // Сделай case() для упрощения


   switch (class_regist)
   {
   case ARITHMETIC_OPERATION:
      if (lexeme_regist.size() > 1)
      {
         class_regist = ERROR;
         error_regist = NOT_ARITHMETIC_OPERATION_ERROR;
         AddLexemeProcess();
         return;
      }
      lexeme.value = lexeme_regist[0];
      break;

   case VECTOR_OPERATION:
      if (lexeme_regist.size() > 1)
      {
         class_regist = ERROR;
         error_regist = NOT_VECTOR_OPERATION_ERROR;
         AddLexemeProcess();
         return;
      }
      lexeme.value = lexeme_regist[0];
      break;

   case RELATION:
      if (lexeme_regist.size() == 1 && lexeme_regist[0] == '=')
      {
         class_regist = ASSIGNMENT_MARK;
         AddLexemeProcess();
         return;
      }
      else if (lexeme_regist.size() == 1)
      {
         if (lexeme_regist[0] == '<')
            ratio_regist = 3;
         else if (lexeme_regist[0] == '>')
            ratio_regist = 4;
      }
      else if (lexeme_regist.size() == 2)
      {
         if (lexeme_regist[0] == '=' && lexeme_regist[1] == '=')
            ratio_regist = 1;
         else if (lexeme_regist[0] == '!' && lexeme_regist[1] == '=')
            ratio_regist = 2;
         else if (lexeme_regist[0] == '<' && lexeme_regist[1] == '=')
            ratio_regist = 5;
         else if (lexeme_regist[0] == '>' && lexeme_regist[1] == '=')
            ratio_regist = 6;
      }
      else
      {
         class_regist = ERROR;
         error_regist = NOT_RELATION_ERROR;
         AddLexemeProcess();
         return;
      }
      lexeme.value = ratio_regist;
      break;

   case VARIABLE:
      /*
      if (isdigit(lexeme_regist[0]))
      {
         class_regist = ERROR;
         error_regist = NOT_VARIABLE_ERROR;
         AddLexemeProcess();
         return;
      }
      */

      if (!lexeme_list.empty() && lexeme_list.back().class_token == JUMP)
         AddMarkProcess();
      else
         AddVariableProcess();

      lexeme.pointer = pointer_regist;
      break;

   case CONSTANT:
      AddConstantProcess();
      lexeme.pointer = pointer_regist;
      break;

   case BOOLEAN_VECTOR:
      AddBoolVectorProcess();
      lexeme.class_token = class_regist;
      lexeme.pointer = pointer_regist;
      break;

   case COMMA:
      lexeme.value = ',';
      break;

   case MARK:
      if (isdigit(lexeme_regist[0]))
      {
         class_regist = ERROR;
         error_regist = NOT_MARK_ERROR;
         AddLexemeProcess();
         return;
      }

      AddMarkProcess();

      lexeme.pointer = pointer_regist;
      break;

   case ASSIGNMENT_MARK:
      lexeme.value = '=';
      break;

   case SEMICOLON:
      lexeme.value = ';';
      break;

   case COLON:
      lexeme.value = ':';
      break;

   case THE_OPEN_CURLY_BRACE:
      lexeme.value = '{';
      break;

   case THE_CLOSING_CURLY_BRACE:
      lexeme.value = '}';
      break;

   case ROUND_BRACE:
      lexeme.value = (int)lexeme_regist[0];;
      break;
   case TYPE:
      if (lexeme_regist == "CN")
         lexeme.value = 0;
      else if (lexeme_regist == "BV")
         lexeme.value = 1;
      break;
   case ERROR:
      //cout << "Обнаружена ошибка в строке : " << line_number << endl;
      AddErrorProcess();
      break;

   default:
      // Обработка случая, если class_regist не соответствует ни одному из вышеуказанных значений
      lexeme.value = s.value;
      //lexeme.pointer = nullptr;
      break;
   }

   /*
   cout << "Adding lexeme: " << lexeme.class_token
      << ", Value: " << lexeme.value
      << ", Pointer: " << lexeme.pointer
      << ", Line: " << lexeme.number_lexeme_line << endl;
   */

   lexeme_list.push_back(lexeme);
   lexeme_regist.clear();

   if (class_regist != THE_OPEN_CURLY_BRACE && class_regist != THE_CLOSING_CURLY_BRACE)
      class_regist = NONE;
}

/* Функция транслитерации.
 * ch - символ
 * Возвращаемое значение - результат транслитерации символа ch
 */
SymbolicToken transliterator(int ch)
{
   SymbolicToken s;
   s.value = 0;
   if (isalpha(ch))
   {
      s.token_class = Letter;
      s.value = ch;
   }
   else if (isdigit(ch))
   {
      s.token_class = Digit;
      s.value = ch;
   }
   else if (ch == '(' || ch == ')')
   {
      s.token_class = Brace;
      s.value = ch;
   }
   else if (ch == ',')
   {
      s.token_class = Comma;
      s.value = ch;
   }
   else if (ch == '[' || ch == ']')
   {
      s.token_class = VectorComponents;
      s.value = ch;
   }
   else if (class_regist == BOOLEAN_VECTOR && (ch == '[' || ch == ']' || ch == '<' || ch == '>'))
   {
      s.token_class = VectorComponents;
      s.value = ch;
   }
   else if (ch == '&' || ch == '|' || ch == '~')
   {
      s.token_class = VectorOperations;
      s.value = ch;
   }
   else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%')
   {
      s.token_class = ArithOp;
      s.value = ch;
   }
   else if (ch == '=' || ch == '!' || ch == '<' || ch == '>')
   {
      s.token_class = Relation;
      s.value = ch;
   }
   else if (ch == ' ')
   {
      s.token_class = Space;
   }
   else if (ch == ';')
   {
      s.token_class = Semicolon;
      s.value = ch;
   }
   else if (ch == ':')
   {
      s.token_class = Colon;
      s.value = ch;
   }
   else if (ch == '\t' || ch == '\n')
   {
      s.token_class = LF;

   }
   else if (ch == EOF)
   {
      s.token_class = EndOfFile;
   }
   else
   {
      s.token_class = Error;
   }
   return s;
}

// Процедуры автомата (от A1 до Exit1).
int A1()
{
   ++line_number;

   return s_A1;
}

int A1_a()
{
   ++cur_num_indent_regist;
   return s_A1;
}

int A1_b()
{
   AddLexemeProcess();

   ++line_number;

   return s_A1;
}

int A2_a()
{
   int indent = cur_num_indent_regist - prev_num_indent_regist;
   if (indent == 0)
      ;
   else if (indent < 0)
   {
      indent *= -1;
      class_regist = THE_CLOSING_CURLY_BRACE;
      lexeme_regist = '{';
   }
   else
   {
      class_regist = THE_OPEN_CURLY_BRACE;
      lexeme_regist = '}';
   }

   for (int i = 0; i < indent; ++i)
   {
      AddLexemeProcess();
   }

   prev_num_indent_regist = cur_num_indent_regist;
   cur_num_indent_regist = 0;

   class_regist = CONSTANT;

   lexeme_regist.clear();
   lexeme_regist = (char)s.value;

   return s_A2;
}

int A2_b()
{
   lexeme_regist += (char)s.value;

   return s_A2;
}

int A2_c()
{
   class_regist = COLON;
   lexeme_regist = (char)s.value;

   AddLexemeProcess();

   return s_A2;
}

int A2_d()
{
   AddLexemeProcess();

   class_regist = CONSTANT;
   lexeme_regist = (char)s.value;

   return s_A2;
}

int A2_g()
{
   AddLexemeProcess();

   class_regist = COLON;
   lexeme_regist = (char)s.value;

   return s_A2;
}

int A3_a()
{
   int indent = cur_num_indent_regist - prev_num_indent_regist;
   if (indent == 0)
      ;
   else if (indent < 0)
   {
      indent *= -1;
      class_regist = THE_CLOSING_CURLY_BRACE;
   }
   else
      class_regist = THE_OPEN_CURLY_BRACE;

   for (int i = 0; i < indent; ++i)
   {
      AddLexemeProcess();
   }

   prev_num_indent_regist = cur_num_indent_regist;
   cur_num_indent_regist = 0;

   class_regist = VARIABLE;

   lexeme_regist.clear();
   lexeme_regist = (char)s.value;

   return s_A3;
}

int A3_b()
{
   class_regist = VARIABLE;

   lexeme_regist += (char)s.value;

   return s_A3;
}

int A3_c()
{
   AddLexemeProcess();

   class_regist = VARIABLE;

   lexeme_regist = (char)s.value;

   return s_A3;
}


int A3_e()
{
   lexeme_regist += (char)s.value;

   return s_A3;
}

int A3_f()
{
   AddLexemeProcess();

   class_regist = VARIABLE;
   lexeme_regist = (char)s.value;

   return s_A3;
}

int A4_a()
{
   class_regist = MARK;

   //AddMarkProcess();
   //AddLexemeProcess();

   return s_A4;
}

int A4_b()
{
   //class_regist = CONSTANT;
   AddLexemeProcess();

   class_regist = COLON;
   lexeme_regist = (char)s.value;

   return s_A4;
}

int B1()
{
   return s_B1;
}

int B1_a()
{
   int indent = cur_num_indent_regist - prev_num_indent_regist;
   if (indent == 0)
      ;
   else if (indent < 0)
   {
      indent *= -1;
      class_regist = THE_CLOSING_CURLY_BRACE;
   }
   else
      class_regist = THE_OPEN_CURLY_BRACE;

   for (int i = 0; i < indent; ++i)
   {
      AddLexemeProcess();
   }

   prev_num_indent_regist = cur_num_indent_regist;
   cur_num_indent_regist = 0;

   class_regist = BOOLEAN_VECTOR;

   lexeme_regist.clear();
   lexeme_regist = (char)s.value;

   return s_B1;
}

int B1_b()
{
   AddLexemeProcess();

   class_regist = BOOLEAN_VECTOR;

   lexeme_regist.clear();
   lexeme_regist = (char)s.value;

   return s_B1;
}

int B2()
{
   return s_B2;
}

int B2_a()
{
   lexeme_regist += (char)s.value;

   return s_B2;
}

int B3_a()
{
   if (s.value == '[')
   {
      class_regist = ERROR;
      AddLexemeProcess();

      class_regist = BOOLEAN_VECTOR;
      lexeme_regist = (char)s.value;
      return s_B1;
   }
   else if (s.value == '<')
   {
      class_regist = ERROR;
      AddLexemeProcess();

      class_regist = ARITHMETIC_OPERATION;
      lexeme_regist = (char)s.value;
      return s_E1;
   }
   else if (s.value == ']')
   {
      //class_regist = ERROR;
      //lexeme_regist += (char)s.value;
      AddLexemeProcess();

      return s_I1;
   }
   lexeme_regist += (char)s.value;

   return s_B3;
}

int C1_a()
{
   lexeme_regist += (char)(s.value);

   return s_C1;
}

int C2()
{
   return s_C2;
}
int D1_a()
{
   int indent = cur_num_indent_regist - prev_num_indent_regist;
   if (indent == 0)
      ;
   else if (indent < 0)
   {
      indent *= -1;
      class_regist = THE_CLOSING_CURLY_BRACE;
      lexeme_regist = '{';
   }
   else
   {
      class_regist = THE_OPEN_CURLY_BRACE;
      lexeme_regist = '}';
   }

   for (int i = 0; i < indent; ++i)
   {
      AddLexemeProcess();
   }

   prev_num_indent_regist = cur_num_indent_regist;
   cur_num_indent_regist = 0;

   class_regist = ROUND_BRACE;
   lexeme_regist = (char)s.value;

   return s_D1;
}

int D1_b()
{
   //AddLexemeProcess();

   class_regist = VECTOR_OPERATION;
   lexeme_regist = (char)s.value;

   return s_D1;
}

int D1_c()
{
   //AddLexemeProcess();

   class_regist = COMMA;
   lexeme_regist = (char)s.value;

   return s_D1;
}

int D1_d()
{
   AddLexemeProcess();

   class_regist = ROUND_BRACE;
   lexeme_regist = (char)s.value;

   return s_D1;
}

int D1_e()
{
   AddLexemeProcess();

   class_regist = VECTOR_OPERATION;
   lexeme_regist = (char)s.value;

   return s_D1;
}

int D1_f()
{
   AddLexemeProcess();

   class_regist = COMMA;
   lexeme_regist = (char)s.value;

   return s_D1;
}

int D1_g()
{
   AddLexemeProcess();

   class_regist = SEMICOLON;
   lexeme_regist = (char)s.value;

   return s_D1;
}

int D1_h()
{
   int indent = cur_num_indent_regist - prev_num_indent_regist;
   if (indent == 0)
      ;
   else if (indent < 0)
   {
      indent *= -1;
      class_regist = THE_CLOSING_CURLY_BRACE;
      lexeme_regist = '{';
   }
   else
   {
      class_regist = THE_OPEN_CURLY_BRACE;
      lexeme_regist = '}';
   }

   for (int i = 0; i < indent; ++i)
   {
      AddLexemeProcess();
   }

   prev_num_indent_regist = cur_num_indent_regist;
   cur_num_indent_regist = 0;

   class_regist = SEMICOLON;
   lexeme_regist = (char)s.value;

   return s_D1;
}

int E1_a()
{
   int indent = cur_num_indent_regist - prev_num_indent_regist;
   if (indent == 0)
      ;
   else if (indent < 0)
   {
      indent *= -1;
      class_regist = THE_CLOSING_CURLY_BRACE;
      lexeme_regist = '{';
   }
   else
   {
      class_regist = THE_OPEN_CURLY_BRACE;
      lexeme_regist = '}';
   }

   for (int i = 0; i < indent; ++i)
   {
      AddLexemeProcess();
   }

   prev_num_indent_regist = cur_num_indent_regist;
   cur_num_indent_regist = 0;

   class_regist = ARITHMETIC_OPERATION;

   lexeme_regist.clear();
   lexeme_regist = (char)s.value;

   return s_E1;
}

int E1_b()
{
   AddLexemeProcess();

   class_regist = ARITHMETIC_OPERATION;

   lexeme_regist.clear();
   lexeme_regist = (char)s.value;

   return s_E1;
}

//
int E1_c()
{

   lexeme_regist += (char)s.value;

   if (lexeme_regist.size() >= 2 && lexeme_regist[lexeme_regist.size() - 2] == '%' && lexeme_regist[lexeme_regist.size() - 1] == '%')
   {
      lexeme_regist.pop_back();
      lexeme_regist.pop_back();

      if (lexeme_regist.size() != 0)
      {
         AddLexemeProcess();
      }

      class_regist = COMMENT;
      return s_I2;
   }

   return s_E1;
}

int F1_a()
{
   int indent = cur_num_indent_regist - prev_num_indent_regist;
   if (indent == 0)
      ;
   else if (indent < 0)
   {
      indent *= -1;
      class_regist = THE_CLOSING_CURLY_BRACE;
      lexeme_regist = '{';
   }
   else
   {
      class_regist = THE_OPEN_CURLY_BRACE;
      lexeme_regist = '}';
   }

   for (int i = 0; i < indent; ++i)
   {
      AddLexemeProcess();
   }

   prev_num_indent_regist = cur_num_indent_regist;
   cur_num_indent_regist = 0;

   class_regist = RELATION;

   lexeme_regist.clear();
   lexeme_regist = (char)s.value;

   return s_F1;
}

int F1_b()
{
   AddLexemeProcess();

   class_regist = RELATION;

   lexeme_regist.clear();
   lexeme_regist = (char)s.value;

   return s_F1;
}

int F1_c()
{
   lexeme_regist += (char)s.value;

   return s_F1;
}

/*
int G1_a()
{
   int indent = cur_num_indent_regist - prev_num_indent_regist;
   if (indent == 0)
      ;
   else if (indent < 0)
   {
      indent *= -1;
      class_regist = THE_CLOSING_CURLY_BRACE;
      lexeme_regist = '{';
   }
   else
   {
      class_regist = THE_OPEN_CURLY_BRACE;
      lexeme_regist = '}';
   }

   for (int i = 0; i < indent; ++i)
   {
      AddLexemeProcess();
   }

   prev_num_indent_regist = cur_num_indent_regist;
   cur_num_indent_regist = 0;

   class_regist = SEMICOLON;

   lexeme_regist.clear();
   lexeme_regist = (char)s.value;

   return s_G1;
}

int G1_b()
{
   AddLexemeProcess();

   class_regist = SEMICOLON;

   lexeme_regist.clear();
   lexeme_regist = (char)s.value;

   return s_G1;
}
*/

int I1()
{

   return s_I1;
}

int I1_a()
{
   if (s.value == ']')
   {
      lexeme_regist += (char)s.value;
      //AddLexemeProcess();

      return s_I1;
   }

   //AddLexemeProcess();

   return s_I1;
}

int I2()
{
   return s_I2;
}

int I3_a()
{
   int indent = cur_num_indent_regist - prev_num_indent_regist;
   if (indent == 0)
      ;
   else if (indent < 0)
   {
      indent *= -1;
      class_regist = THE_CLOSING_CURLY_BRACE;
      lexeme_regist = '{';
   }
   else
   {
      class_regist = THE_OPEN_CURLY_BRACE;
      lexeme_regist = '}';
   }

   for (int i = 0; i < indent; ++i)
   {
      AddLexemeProcess();
   }

   prev_num_indent_regist = cur_num_indent_regist;
   cur_num_indent_regist = 0;

   class_regist = ERROR;
   error_regist = NOT_EXISTING_COMMAND_ERROR;

   lexeme_regist.clear();
   lexeme_regist = (char)s.value;

   return s_I3;
}

int I3_b()
{
   AddLexemeProcess();

   class_regist = ERROR;
   error_regist = NOT_EXISTING_COMMAND_ERROR;

   lexeme_regist.clear();
   lexeme_regist = (char)s.value;

   return s_I3;
}

int I3_c()
{
   lexeme_regist += (char)s.value;

   return s_I3;
}

int Exit1()
{
   AddLexemeProcess();

   class_regist = END_OF_FILE;

   AddLexemeProcess();
   return s_Stop;
}
int Exit2()
{
   /*
   if (ratio_regist == '!')
      return ErrorHandlingProcess();
   */

   //AddLexemeProcess();
   class_regist = END_OF_FILE;
   AddLexemeProcess();

   return s_Stop;
}
int Exit3()
{
   AddConstantProcess();

   AddLexemeProcess();
   class_regist = END_OF_FILE;

   AddLexemeProcess();
   return s_Stop;
}
int Exit4()
{
   //AddVariableProcess();

   AddLexemeProcess();
   class_regist = END_OF_FILE;

   AddLexemeProcess();
   return s_Stop;
}

const int state_number = 15; // Число состояний (без s_Stop).
const int class_number = 14; // Число символов входного алфавита.

// Таблица указателей на процедуры автомата.
typedef int (*function_pointer)();
function_pointer table[state_number][class_number];

// Функция инициализации таблицы указателей на процедуры автомата.
void initialize_table()
{
   // Сначала во все ячейки заносится указатель на процедуру обработки ошибок.
   /*
   for (int i = 0; i < state_number; i++)
   {
      for (int j = 0; j < class_number; j++)
      {
         table[i][j] = Error1;
      }
   }
   */

   // Далее заполняются ячейки для остальных процедур.
   table[s_A1][Letter] = A3_a; table[s_A1][Digit] = A2_a; table[s_A1][Brace] = D1_a; table[s_A1][VectorComponents] = B1_a; table[s_A1][VectorOperations] = D1_b; table[s_A1][Comma] = D1_c; table[s_A1][ArithOp] = E1_a; table[s_A1][Relation] = F1_a; table[s_A1][Space] = A1_a; table[s_A1][Semicolon] = D1_h; table[s_A1][Colon] = A2_c; table[s_A1][LF] = A1; table[s_A1][ERROR] = I3_a; table[s_A1][EndOfFile] = Exit2;
   table[s_A2][Letter] = A3_b; table[s_A2][Digit] = A2_b; table[s_A2][Brace] = D1_d; table[s_A2][VectorComponents] = B1_b; table[s_A2][VectorOperations] = D1_e; table[s_A2][Comma] = D1_f; table[s_A2][ArithOp] = E1_b; table[s_A2][Relation] = F1_b; table[s_A2][Space] = I1; table[s_A2][Semicolon] = D1_g; table[s_A2][Colon] = A4_b; table[s_A2][LF] = A1_b; table[s_A2][ERROR] = I3_b; table[s_A2][EndOfFile] = Exit1;
   table[s_A3][Letter] = A3_e; table[s_A3][Digit] = A3_e; table[s_A3][Brace] = D1_d; table[s_A3][VectorComponents] = B1_b; table[s_A3][VectorOperations] = D1_e; table[s_A3][Comma] = D1_f; table[s_A3][ArithOp] = E1_b; table[s_A3][Relation] = F1_b; table[s_A3][Space] = I1; table[s_A3][Semicolon] = D1_g; table[s_A3][Colon] = A4_a;  table[s_A3][LF] = A1_b; table[s_A3][ERROR] = I3_b; table[s_A3][EndOfFile] = Exit1;
   table[s_A4][Letter] = A3_f; table[s_A4][Digit] = A2_g; table[s_A4][Brace] = D1_d; table[s_A4][VectorComponents] = B1_b; table[s_A4][VectorOperations] = D1_e; table[s_A4][Comma] = D1_f; table[s_A4][ArithOp] = E1_b; table[s_A4][Relation] = F1_b; table[s_A4][Space] = I1; table[s_A4][Semicolon] = D1_g; table[s_A4][Colon] = A4_b; table[s_A4][LF] = A1_b; table[s_A4][ERROR] = I3_b; table[s_A4][EndOfFile] = Exit1;

   table[s_B1][Letter] = A3_f; table[s_B1][Digit] = A2_g; table[s_B1][Brace] = D1_d; table[s_B1][VectorComponents] = B2_a; table[s_B1][VectorOperations] = D1_e; table[s_B1][Comma] = D1_f; table[s_B1][ArithOp] = E1_b; table[s_B1][Relation] = F1_b; table[s_B1][Space] = I1; table[s_B1][Semicolon] = D1_g; table[s_B1][Colon] = A2_g; table[s_B1][LF] = A1_b; table[s_B1][ERROR] = I3_b; table[s_B1][EndOfFile] = Exit1;
   table[s_B2][Letter] = A3_f; table[s_B2][Digit] = C1_a; table[s_B2][Brace] = D1_d; table[s_B2][VectorComponents] = B3_a; table[s_B2][VectorOperations] = D1_e; table[s_B2][Comma] = D1_f; table[s_B2][ArithOp] = E1_b; table[s_B2][Relation] = F1_b; table[s_B2][Space] = B2; table[s_B2][Semicolon] = D1_g; table[s_B2][Colon] = A2_g; table[s_B2][LF] = A1_b; table[s_B2][ERROR] = I3_b; table[s_B2][EndOfFile] = Exit1;
   table[s_B3][Letter] = A3_f; table[s_B3][Digit] = A2_a; table[s_B3][Brace] = D1_d; table[s_B3][VectorComponents] = I1_a; table[s_B3][VectorOperations] = D1_e; table[s_B3][Comma] = D1_f; table[s_B3][ArithOp] = E1_b; table[s_B3][Relation] = F1_b; table[s_B3][Space] = I1; table[s_B3][Semicolon] = D1_g; table[s_B3][Colon] = A2_g; table[s_B3][LF] = A1_b; table[s_B3][ERROR] = I3_b; table[s_B3][EndOfFile] = Exit1;

   table[s_C1][Letter] = A3_f; table[s_C1][Digit] = C1_a; table[s_C1][Brace] = D1_d; table[s_C1][VectorComponents] = B3_a; table[s_C1][VectorOperations] = D1_e; table[s_C1][Comma] = B2_a; table[s_C1][ArithOp] = E1_b; table[s_C1][Relation] = F1_b; table[s_C1][Space] = C2; table[s_C1][Semicolon] = D1_g; table[s_C1][Colon] = A2_g; table[s_C1][LF] = A1_b; table[s_C1][ERROR] = I3_b; table[s_C1][EndOfFile] = Exit1;
   table[s_C2][Letter] = A3_f; table[s_C2][Digit] = A2_g; table[s_C2][Brace] = D1_d; table[s_C2][VectorComponents] = B3_a; table[s_C2][VectorOperations] = D1_e; table[s_C2][Comma] = D1_f; table[s_C2][ArithOp] = E1_b; table[s_C2][Relation] = F1_b; table[s_C2][Space] = C2; table[s_C2][Semicolon] = D1_g; table[s_C2][Colon] = A2_g; table[s_C2][LF] = A1_b; table[s_C2][ERROR] = I3_b; table[s_C2][EndOfFile] = Exit1;

   table[s_D1][Letter] = A3_c; table[s_D1][Digit] = A2_d; table[s_D1][Brace] = D1_d; table[s_D1][VectorComponents] = B1_b; table[s_D1][VectorOperations] = D1_e; table[s_D1][Comma] = D1_f; table[s_D1][ArithOp] = E1_b; table[s_D1][Relation] = F1_b; table[s_D1][Space] = I1; table[s_D1][Semicolon] = D1_g; table[s_D1][Colon] = A4_b; table[s_D1][LF] = A1_b; table[s_D1][ERROR] = I3_b; table[s_D1][EndOfFile] = Exit1;

   table[s_E1][Letter] = A3_c; table[s_E1][Digit] = A2_d; table[s_E1][Brace] = D1_d; table[s_E1][VectorComponents] = B1_b; table[s_E1][VectorOperations] = D1_e; table[s_E1][Comma] = D1_f; table[s_E1][ArithOp] = E1_c; table[s_E1][Relation] = F1_b; table[s_E1][Space] = I1; table[s_E1][Semicolon] = D1_g; table[s_E1][Colon] = A4_b; table[s_E1][LF] = A1_b; table[s_E1][ERROR] = I3_b; table[s_E1][EndOfFile] = Exit1;

   table[s_F1][Letter] = A3_c; table[s_F1][Digit] = A2_d; table[s_F1][Brace] = D1_d; table[s_F1][VectorComponents] = B1_b; table[s_F1][VectorOperations] = D1_e; table[s_F1][Comma] = D1_f; table[s_F1][ArithOp] = E1_b; table[s_F1][Relation] = F1_c; table[s_F1][Space] = I1; table[s_F1][Semicolon] = D1_g; table[s_F1][Colon] = A4_b; table[s_F1][LF] = A1_b; table[s_F1][ERROR] = I3_b; table[s_F1][EndOfFile] = Exit1;

   //table[s_G1][Letter] = A3_c; table[s_G1][Digit] = A2_d; table[s_G1][Brace] = D1_d; table[s_G1][VectorComponents] = B1_b; table[s_G1][VectorOperations] = D1_e; table[s_G1][Comma] = D1_f; table[s_G1][ArithOp] = E1_b; table[s_G1][Relation] = F1_b; table[s_G1][Space] = I1_a; table[s_G1][Semicolon] = G1_b; table[s_G1][Colon] = A4_b; table[s_G1][LF] = A1_b; table[s_G1][EndOfFile] = Exit1;

   table[s_I1][Letter] = A3_c; table[s_I1][Digit] = A2_d; table[s_I1][Brace] = D1_d; table[s_I1][VectorComponents] = B1_b; table[s_I1][VectorOperations] = D1_e; table[s_I1][Comma] = D1_f; table[s_I1][ArithOp] = E1_b; table[s_I1][Relation] = F1_b; table[s_I1][Space] = I1; table[s_I1][Semicolon] = D1_g; table[s_I1][Colon] = A2_c; table[s_I1][LF] = A1_b; table[s_I1][ERROR] = I3_b; table[s_I1][EndOfFile] = Exit1;
   table[s_I2][Letter] = I2; table[s_I2][Digit] = I2; table[s_I2][Brace] = I2; table[s_I2][VectorComponents] = I2; table[s_I2][VectorOperations] = I2; table[s_I2][Comma] = I2; table[s_I2][ArithOp] = I2; table[s_I2][Relation] = I2; table[s_I2][Space] = I2; table[s_I2][Semicolon] = I2; table[s_I2][Colon] = I2; table[s_I2][LF] = A1_b; table[s_I2][ERROR] = I3_b; table[s_I2][EndOfFile] = Exit1;
   table[s_I3][Letter] = A3_c; table[s_I3][Digit] = A2_d; table[s_I3][Brace] = D1_d; table[s_I3][VectorComponents] = B1_b; table[s_I3][VectorOperations] = D1_e; table[s_I3][Comma] = D1_f; table[s_I3][ArithOp] = E1_b; table[s_I3][Relation] = F1_b; table[s_I3][Space] = I1; table[s_I3][Semicolon] = D1_g; table[s_I3][Colon] = A2_c; table[s_I3][LF] = A1_b; table[s_I3][ERROR] = I3_c; table[s_I3][EndOfFile] = Exit1;
}

/*
void PrintLexemeList()
{
   for (int i = 0; i < lexeme_list.size(); ++i)
   {
      cout << lexeme_list[i].number_lexeme_line << ' ' << lexeme_list[i].class_token << ' ';
      if (lexeme_list[i].class_token == PUSH)
      {
         bool flag = 0;
         for (auto it = variable_table.begin(); flag == 0 && it != variable_table.end(); ++it)
         {
            if (&(*it) == lexeme_list[i].pointer)
            {
               Variable* variable = static_cast<Variable*>(lexeme_list[i].pointer);

               cout << variable->name << ' ';
               if (variable->type == 0)
                  cout << variable->value.first;
               else
                  cout << variable->value.second;
               flag = 1;
               break;
            }
         }
         for (auto it = constant_table.begin(); flag == 0 && it != constant_table.end(); ++it)
         {
            if (&(*it) == lexeme_list[i].pointer)
            {
               int value = *it;  // �������� ��������� �� ��������

               cout << value;
               flag = 1;
               break;
            }
         }
         for (auto it = bool_vector_table.begin(); flag == 0 && it != bool_vector_table.end(); ++it)
         {
            if (&(*it) == lexeme_list[i].pointer)
            {
               BoolVector value = *it;  // �������� ��������� �� ��������

               cout << value;

               //cout << "PUSH bool vector: " << value  << endl;
               flag = 1;
               break;
            }
         }
      }
      else if (lexeme_list[i].class_token == POP)
      {
         for (auto it = variable_table.begin(); it != variable_table.end(); ++it)
         {
            if (&(*it) == lexeme_list[i].pointer)
            {
               Variable* variable = static_cast<Variable*>(lexeme_list[i].pointer);

               cout << variable->name << ' ';
               if (variable->type == 0)
                  cout << variable->value.first;
               else
                  cout << variable->value.second;
               break;
            }
         }
      }
      else if (lexeme_list[i].class_token == ARITHMETIC_OPERATION)
      {
         cout << (char)lexeme_list[i].value;

      }
      else if (lexeme_list[i].class_token == RELATION)
      {
         int value = lexeme_list[i].value;
         if (value == 1)
            cout << '=';
         else if (value == 2)
            cout << "!=";
         else if (value == 3)
            cout << '<';
         else if (value == 4)
            cout << ">";
         else if (value == 5)
            cout << "<=";
         else if (value == 6)
            cout << ">=";
      }
      else if (lexeme_list[i].class_token == JMP || lexeme_list[i].class_token == JI)
      {
         cout << lexeme_list[i].value;
      }
      else if (lexeme_list[i].class_token == VECTOR_OPERATION)
      {
         cout << (char)lexeme_list[i].value;
      }
      cout << endl;
   }
}
*/

void PrintLexemeList()
{
   for (int i = 0; i < lexeme_list.get_size(); ++i)
   {
      cout << lexeme_list[i].number_lexeme_line << ' ' << lexeme_list[i].class_token << ' ';

      switch (lexeme_list[i].class_token)
      {
      case ARITHMETIC_OPERATION:
      {
         cout << (char)lexeme_list[i].value;
         break;
      }
      case VECTOR_OPERATION:
      {
         cout << (char)lexeme_list[i].value;
         break;
      }
      case RELATION:
      {
         switch (lexeme_list[i].value)
         {
         case 1:
         {
            cout << "==";
            break;
         }
         case 2:
         {
            cout << "!=";
            break;
         }
         case 3:
         {
            cout << "<";
            break;
         }
         case 4:
         {
            cout << ">";
            break;
         }
         case 5:
         {
            cout << "<=";
            break;
         }
         case 6:
         {
            cout << ">=";
            break;
         }
         default:
            break;
         }
         break;
      }
      case VARIABLE:
      {
         for (auto it = variable_table.begin(); it != variable_table.end(); ++it)
         {
            if (&(*it) == lexeme_list[i].pointer)
            {
               Variable* variable = static_cast<Variable*>(lexeme_list[i].pointer);

               cout << variable->name;
               break;
            }
         }
         break;
      }
      case CONSTANT:
      {
         for (auto it = constant_table.begin(); it != constant_table.end(); ++it)
         {
            if (&(*it) == lexeme_list[i].pointer)
            {
               int value = *it;

               cout << value;
               break;
            }
         }
         break;
      }
      case BOOLEAN_VECTOR:
      {
         for (auto it = bool_vector_table.begin(); it != bool_vector_table.end(); ++it)
         {
            if (&(*it) == lexeme_list[i].pointer)
            {
               BoolVector value = *it;

               cout << value;
               break;
            }
         }
         break;
      }
      case MARK:
      {
         for (auto it = mark_table.begin(); it != mark_table.end(); ++it)
         {
            if (&(*it) == lexeme_list[i].pointer)
            {
               Mark value = *it;

               cout << value.name;
               break;
            }
         }
         break;
      }
      case ROUND_BRACE:
      {
         cout << (char)lexeme_list[i].value;
         break;
      }
      case TYPE:
      {
         if (lexeme_list[i].value == 0)
            cout << "CN";
         else if (lexeme_list[i].value == 1)
            cout << "BV";
         break;
      }
      case ERROR:
      {
         for (auto it = error_list.begin(); it != error_list.end(); ++it)
         {
            if (&(*it) == lexeme_list[i].pointer)
            {
               ErrorClass* err = static_cast<ErrorClass*>(lexeme_list[i].pointer);

               cout << err->GerErrorName();
               break;
            }
         }
         break;
      }
      default:
         break;
      }
      cout << endl;
   }
}

// Основная функция анализа. Функция запускает конечный автомат на тексте из файла filename.
void parse(const char* filename)
{
   ifstream in(filename);
   if (!in)
   {
      cout << "Не удалось открыть файл " << filename << endl;
      return;
   }

   lexeme_list.clear();
   //lexeme_list.reserve(1);
   lexeme_list.set_capacity(0);

   LexemeList lexeme{};
   lexeme.class_token = NONE;
   lexeme.value = 0; // Явная инициализация union
   lexeme.number_lexeme_line = 0;

   lexeme_list.push_back(lexeme); // Убедитесь, что нет опечаток (например, лишняя скобка)

   /*
   cout << lexeme_list.size() << endl;
   lexeme_list.resize(0);
   cout << lexeme_list.size() << endl;
   */

   int ch;             // Входной символ.
   int state = s_A1;   // Текущее состояние автомата.
   while (state != s_Stop)
   {
      // s_Stop - техническое состояние. Когда автомат попадает в s_Stop, цикл останавливается.
      ch = in.get();
      s = transliterator(ch);
      state = (*table[state][s.token_class])();
   }

   PrintLexemeList();

   cout << endl;

   /*
   if (!error_flag)
      ExecuteLexemeList();
   */

   in.close();
}



// Вызов программы имеет вид ./имя-исполняемого-файла имя-входного-файла.
/*
int main(int argc, char* argv[])
{
   setlocale(LC_ALL, "RUS");

   /*
    if (argc != 2)
    {
        cout << "Формат: ./имя-исполняемого-файла имя-входного-файла" << endl;
        return 1;
    }
   



   initialize_table();

   s.value = 'a';
   line_number = 1;
   error_flag = 0;

   //parse(argv[1]);
   const char* filename = "input5";
   //const char* filename = "test2";
   parse(filename);

   return 0;
}

*/