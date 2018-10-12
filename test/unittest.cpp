#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include <cstdio>
#include <cstring>
#include "gtest_ext.h"
#include "../quiz.hpp"
using ::testing::HasSubstr;
using ::testing::StartsWith;
using ::testing::MatchesRegex;
using ::testing::ContainsRegex;

TEST(Quiz, PublicMethodsPresent) {
  question q;
  ASSERT_NO_THROW({
    q.text();
    q.answer();
    q.set_text("a");
    q.set_answer("b");
  });
}

TEST(Quiz, MutatorsAndAccessors) {
  question q;
  std::string text, answer;
  text = generate_string(10);
  answer = generate_string(10);
  ASSERT_NO_THROW({
    q.set_text(text);
    q.set_answer(answer);
  });
  ASSERT_EQ(q.text(), text);
  ASSERT_EQ(q.answer(), answer);
}

TEST(Quiz, CreateQuestion) {
  for(int i = 0; i < 10; i++) {
    std::string text, answer;
    text = generate_string(10);
    answer = generate_string(10);
    question temp, actual;
    temp.set_text(text);
    temp.set_answer(answer);
    std::string input = text+"\n"+answer;
    SIMULATE_CIN(input, {
      actual = create_question();
    });
    ASSERT_EQ(temp.text(), actual.text());
    ASSERT_EQ(temp.answer(), actual.answer());
  }
}


TEST(Quiz, SaveQuestion) {
  for(int i = 0; i < 10; i++) {
    int repetitions = rand() % 10 + 1;
    question questions[repetitions];
    for (int i = 0; i < repetitions; i++) {
      std::string text, answer;
      text = generate_string(10);
      answer = generate_string(10);
      questions[i].set_text(text);
      questions[i].set_answer(answer);
    }
    std::string filename = generate_string(5) + ".txt";
    ASSERT_DURATION_LE(10000, {
      ASSERT_SIO_EQ("", "File saved successfully!\n\n", {
        save_questions(questions, repetitions, filename);
      });
    });
    ifstream test_file(filename.c_str());
    ASSERT_TRUE(test_file.good());
    remove(filename.c_str());
  }
}

TEST(Quiz, SaveFileFormat) {
  for(int i = 0; i < 10; i++) {
    int repetitions = rand() % 10 + 1;
    question questions[repetitions];
    std::string expected = std::to_string(repetitions) + "\n";
    for (int i = 0; i < repetitions; i++) {
      std::string text, answer;
      text = generate_string(10);
      answer = generate_string(10);
      questions[i].set_text(text);
      questions[i].set_answer(answer);
      expected += "[SQ]\n";
      expected += text + "\n";
      expected += answer + "\n";
    }

    std::string filename = generate_string(5) + ".txt";
    ASSERT_DURATION_LE(10000, {
      ASSERT_SIO_EQ("", "File saved successfully!\n\n", {
        save_questions(questions, repetitions, filename);
      });
    });

    ifstream save_file(filename);
    ASSERT_TRUE(save_file.good()) << "Save file was not created";
    std::string file_contents, temp_holder;
    while(save_file >> temp_holder) {
      file_contents += temp_holder + "\n";
    }
    save_file.close();
    remove(filename.c_str());

    ASSERT_EQ(file_contents, expected);
  }
}



TEST(Quiz, DisplayQuestions) {
  for(int i = 0; i < 10; i++) {
    int repetitions = rand() % 10 + 1;
    question questions[repetitions];
    std::string expected = "Question and Answer list\n";
    for (int i = 0; i < repetitions; i++) {
      std::string text, answer;
      text = generate_string(10);
      answer = generate_string(10);
      questions[i].set_text(text);
      questions[i].set_answer(answer);
      expected += std::to_string(i+1) + ". "+ text +"\nAnswer: "
                  + answer + "\n";
    }
    expected += "\n";
    ASSERT_DURATION_LE(10000, {
      ASSERT_SIO_EQ("", expected, display_questions(questions, repetitions));
    });
  }
}

TEST(Quiz, FiniteChoiceLoop) {
  srand (time(NULL));
  ASSERT_DURATION_LE(10000, {
    main_output("quizgen", "a\nX\nX\nd");
  });
}

TEST(Quiz, CreateQuestionChoice) {
  std::string expected = "Welcome to QuizMaker\n\nWhat would you like to do?\n"
                         "a. Create a question\nb. Display questions\n"
                         "c. Save questions\nd. Quit\nChoice: "
                         "\nPlease enter your question: Please enter the "
                         "answer: Question added!\n\nWhat would you like to do?"
                         "\na. Create a question\nb. Display questions\n"
                         "c. Save questions\nd. Quit"
                         "\nChoice: \nThank you for using QuizMaker!\n";
  ASSERT_DURATION_LE(10000, {
    ASSERT_EXECIO_EQ("quizgen", "a\nX\nX\nd", expected);
  });
}

TEST(Quiz, DisplayQuestionsChoice) {
  std::string expected = "Welcome to QuizMaker\n\nWhat would you like to do?\n"
                         "a. Create a question\nb. Display questions\n"
                         "c. Save questions\nd. Quit\nChoice: "
                         "\nPlease enter your question: Please enter the "
                         "answer: Question added!\n\nWhat would you like to do?\n"
                         "a. Create a question\nb. Display questions\n"
                         "c. Save questions\nd. Quit\nChoice: \nQuestion and Answer list\n"
                         "1. X\nAnswer: X\n\nWhat would you like to do?\n"
                         "a. Create a question\nb. Display questions\n"
                         "c. Save questions\nd. Quit\nChoice: \nThank you for using QuizMaker!\n";
  ASSERT_DURATION_LE(10000, {
    ASSERT_EXECIO_EQ("quizgen", "a\nX\nX\nb\nd", expected);
  });
}

TEST(Quiz, SaveQuestionsChoice) {
  std::string expected = "Welcome to QuizMaker\n\nWhat would you like to do?\n"
                         "a. Create a question\nb. Display questions\n"
                         "c. Save questions\nd. Quit\nChoice: \nWhat filename "
                         "would you like to use? File saved successfully!\n"
                         "\nWhat would you like to do?\n"
                         "a. Create a question\nb. Display questions\n"
                         "c. Save questions\nd. Quit\nChoice: "
                         "\nThank you for using QuizMaker!\n";
  string filename = generate_string(5) + ".txt";
  string input = "c\n" + filename + "\nd";
  ASSERT_DURATION_LE(10000, {
    ASSERT_EXECIO_EQ("quizgen", input, expected);
  });
  std::remove(filename.c_str());
}

TEST(Quiz, QuitChoice) {
  std::string expected = "Welcome to QuizMaker\n\nWhat would you like to do?\n"
                         "a. Create a question\nb. Display questions\n"
                         "c. Save questions\nd. Quit\nChoice: "
                         "\nThank you for using QuizMaker!\n";
  ASSERT_DURATION_LE(10000, {
    ASSERT_EXECIO_EQ("quizgen", "d", expected);
  });
}

TEST(Quiz, InvalidOptionChoice) {
  std::string expected = "Welcome to QuizMaker\n\nWhat would you like to do?\n"
                         "a. Create a question\nb. Display questions\n"
                         "c. Save questions\nd. Quit\nChoice: "
                         "\nInvalid option\n\nWhat would you like to do?\n"
                         "a. Create a question\nb. Display questions\n"
                         "c. Save questions\nd. Quit\nChoice: \n"
                         "Thank you for using QuizMaker!\n";
  ASSERT_DURATION_LE(10000, {
    ASSERT_EXECIO_EQ("quizgen", "x\nd",expected);
  });
}

TEST(Quiz, ChoiceLoopLowercase) {
  int repetitions = rand() % 10 + 1;
  for (int i = 0; i < repetitions; i++) {
    std::string input = "";
    std::string question_list = "Question and Answer list\n";
    std::string expected = "Welcome to QuizMaker\n\n";
    std::vector<std::string> filenames;
    int question_ctr = 1;
    char choice;
    do {
      choice = rand() % 4 + 97;
      expected += "What would you like to do?\na. Create a question\n"
                  "b. Display questions\nc. Save questions\nd. Quit\nChoice: \n";
      std::string s_choice(1, choice);
      input += s_choice + "\\n";
      switch (choice) {
        case 'a':
          expected += "Please enter your question: Please enter the answer: "
                      "Question added!\n\n";
          input += "q\\na\\n";
          question_list += std::to_string(question_ctr) +". q\nAnswer: a\n";
          question_ctr++;
          break;
        case 'b':
          expected += question_list + "\n";
          break;
        case 'c':
          {
            expected += "What filename would you like to use? File saved successfully!\n\n";
            std::string filename = generate_string(5) + ".txt";
            filenames.push_back(filename);
            input += filename+"\n";
            break;
          }
        case 'd':
          expected += "Thank you for using QuizMaker!\n";
          break;
        default:
          expected += "Invalid option\n\n";
      }
    } while(choice!= 'd');
    ASSERT_DURATION_LE(10000, {
      ASSERT_EXECIO_EQ("quizgen", input, expected);
    });
    for(std::string fn : filenames) {
      remove(fn.c_str());
    }
  }
}

TEST(Quiz, ChoiceLoopUppercase) {
  int repetitions = rand() % 10 + 1;
  for (int i = 0; i < repetitions; i++) {
    std::string input = "";
    std::string question_list = "Question and Answer list\n";
    std::string expected = "Welcome to QuizMaker\n\n";
    std::vector<std::string> filenames;
    int question_ctr = 1;
    char choice;
    do {
      choice = rand() % 4 + 65  ;
      expected += "What would you like to do?\na. Create a question\n"
                  "b. Display questions\nc. Save questions\nd. Quit\nChoice: \n";
      std::string s_choice(1, choice);
      input += s_choice + "\\n";
      switch (choice) {
        case 'A':
          expected += "Please enter your question: Please enter the answer: "
                      "Question added!\n\n";
          input += "q\\na\\n";
          question_list += std::to_string(question_ctr) +". q\nAnswer: a\n";
          question_ctr++;
          break;
        case 'B':
          expected += question_list + "\n";
          break;
        case 'C':
          {
            expected += "What filename would you like to use? File saved successfully!\n\n";
            std::string filename = generate_string(5) + ".txt";
            filenames.push_back(filename);
            input += filename+"\n";
            break;
          }
        case 'D':
          expected += "Thank you for using QuizMaker!\n";
          break;
        default:
          expected += "Invalid option\n\n";
      }
    } while(choice!= 'D');
    ASSERT_DURATION_LE(10000, {
        ASSERT_EXECIO_EQ("quizgen", input, expected);
    });
    for(std::string fn : filenames) {
      remove(fn.c_str());
    }

  }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
