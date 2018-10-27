// Filename quiz.cpp
#include "quiz.hpp"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

question create_question() {
  question q;
  string text, answer;
  cout << "Please enter your question: ";
  getline(cin, text);
  cout << "Please enter the answer: ";
  getline(cin, answer);
  cout << "Question added!" << endl;
  q.set_text(text);
  q.set_answer(answer);
  return q;
}
void display_questions(question qArray[], int size) {
  cout << "\nQuestion and Answer list\n";
  for (int i = 0; i < size; i++) {
    cout << i + 1 << ". " << (qArray + i)->text() << "\n"
         << "Answer: " << (qArray + i)->answer() << "\n";
  }
}
void save_questions(question qArray[], int size, string filename) {
  ofstream outfile;
  outfile.open(filename);
  outfile << size << "\n";
  for (int i = 0; i < size; i++) {
    outfile << "[SQ]\n"
            << (qArray + i)->text() << "\n"
            << (qArray + i)->answer() << "\n";
  }
  outfile.close();
  cout << "File saved successfully!\n";
}
