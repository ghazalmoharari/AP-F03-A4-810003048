
#ifndef PROJECT4TEST_LEITNERSYSTEM_H
#define PROJECT4TEST_LEITNERSYSTEM_H

#include "Manual.h"
#include "Box.h"
#include "Day.h"
#include "Flashcard.h"
#include "Streak.h"

class LeitnerSystem
{
public:
    LeitnerSystem();

    ~LeitnerSystem();

    Box *getBox(const string &type);
    Streak *getStreak() ;
    void addFlashcard(string question, string answer);
    void moveFlashcard(Flashcard *flashcard, Box *newBox);

    void moveToNextBox(Flashcard *flashcard);

    void moveToPrevBox(Flashcard *flashcard);

    void moveCardsToPrevBox(string boxType);

    void noPracticeBoxChange();

    Day *getDay(int dayNumber);

    void addAnswerResult(bool isCorrect);

    void reviewToday(int numQuestions);

    void getReport(int startDay, int endDay);
    void getProgressReport();
    void nextDay();
    void run();

private:
    vector<Box *> boxes;
    int currentDay;
    Streak *streak;
    vector<Day *> days;
};
#endif //PROJECT4TEST_LEITNERSYSTEM_H
