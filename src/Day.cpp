
#include "../include/Day.h"

    Day::Day(int n)
    {
        dayNumber = n;
        correctAnswers = 0;
        incorrectAnswers = 0;
        practiced = false;
    };

    void Day::addCorrect() { correctAnswers++; };
    void Day::addIncorrect() { incorrectAnswers++; };
    void Day::markPracticed() { practiced = true; };
    bool Day::isPracticed() { return practiced; };
    int Day::getCorrect() { return correctAnswers; };
    int Day::getIncorrect() { return incorrectAnswers; };
    int Day::getTotal() { return correctAnswers + incorrectAnswers; };
    int Day::getDayNumber() { return dayNumber; };
