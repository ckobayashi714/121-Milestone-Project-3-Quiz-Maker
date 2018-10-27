#include "quiz.hpp"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main() {
  /* This will create an array of 100 question objects*/
  question q;
  question qArray[100];
  /* This program will reapteadly ask the user to input a question or exit. If
  the user inputs something other than a or b (capital letters too), they will
  be notified with an invalid message, but will then be asked again what option
  they would like to choose.*/
  string filename;
  char choice;
  int size = 0;
  cout << "Welcome to QuizMaker\n";

  do {
    cout << "\nWhat would you like to do?\n"
         << "a. Create a question\n"
         << "b. Display questions\n"
         << "c. Save questions\n"
         << "d. Quit\n"
         << "Choice: ";
    cin >> choice;
    cin.ignore();
    switch (choice) {
    case 'a':
    case 'A':
      cout << "\n";
      qArray[size] = create_question();
      // cout << qArray[size].text();
      // cout << qArray[size].answer();
      size++;
      break;
    case 'b':
    case 'B':
      display_questions(qArray, size);
      break;
    case 'c':
    case 'C':
      cout << "\nWhat filename would you like to use? ";
      getline(cin, filename);
      save_questions(qArray, size, filename);
      break;
    case 'd':
    case 'D':
      break;
    default:
      cout << "\nInvalid option\n";
    }
  } while (choice != 'd' && choice != 'D');
  cout << "\nThank you for using QuizMaker!\n";
  return 0;
}
