#include <iostream>
#include <string>
#include <locale>

#include "StateMachine.hpp"
#include "StateMachine2.hpp"

using namespace std;

int main()
{
   setlocale(LC_ALL, "RUS");

   Avto1 obj1;
   //obj1.start("00110");
   obj1.start_from_file("test.txt");

   Avto2 obj2;
   //obj2.start("100111101");

   Avto3 obj3;
   //obj3.start("01100011");

   Avto4 obj4;
   //obj4.start("1111101");

   Parser1 obj2_1;
   //obj2_1.parse("00110");
   //obj2_1.parse_file("test.txt");

   Parser2 obj2_2;
   //obj2_2.parse("011001");

   Parser3 obj2_3;
   //obj2_3.parse("11000101");

   Parser4 obj2_4;
   //obj2_4.parse("11100100");
   return 0;
}