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

void display_questions(question qArray, int size) {
  cout << "Question and Answer list";
  for(int i = 1; i < 100; i++) {
    cout << *(question qArray + i);
  }
}
void save_questions(question qArray, int size, string filename){
  ofstream outfile;
  outfile.open(filename);
  outfile << qArray << size;
  //cout << question number
  outfile.close();
}
