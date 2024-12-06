//
// Created by ghazal on 06/12/24.
//

#ifndef PROJECT4TEST_FLASHCARD_H
#define PROJECT4TEST_FLASHCARD_H

#include "Manual.h"
#include "Box.h"
class Flashcard
{
public:
    Flashcard(string q, string a, Box *b);
    string getQuestion();
    string getAnswer();
    Box *getBox();
    void setBox(Box *box) ;
    int getNumOfWrongAnswers();
    void setToZeroNumOfWrongAnswers();
    void addNumOfWrongAnswers();


private:
    string question;
    string answer;
    Box *currentBox;
    int numOfWrongAnswers;
};

#endif //PROJECT4TEST_FLASHCARD_H
