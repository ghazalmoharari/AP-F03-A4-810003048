#include "../include/Flashcard.h"


    Flashcard::Flashcard(string q, string a, Box *b)
    {
        question = q;
        answer = a;
        currentBox = b;
        numOfWrongAnswers=0;
    };
    string Flashcard::getQuestion() { return question; };
    string Flashcard::getAnswer() { return answer; };
    Box* Flashcard::getBox() { return currentBox; };
    void Flashcard::setBox(Box *box) { currentBox = box; };
    int Flashcard::getNumOfWrongAnswers(){return numOfWrongAnswers;};
    void Flashcard::setToZeroNumOfWrongAnswers(){numOfWrongAnswers=0;};
    void Flashcard::addNumOfWrongAnswers(){numOfWrongAnswers++;};

