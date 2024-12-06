

#ifndef PROJECT4TEST_DAY_H
#define PROJECT4TEST_DAY_H
class Day
{
public:
    Day(int n);

    void addCorrect() ;
    void addIncorrect() ;
    void markPracticed() ;
    bool isPracticed() ;
    int getCorrect() ;
    int getIncorrect() ;
    int getTotal() ;
    int getDayNumber();

private:
    int dayNumber;
    int correctAnswers;
    int incorrectAnswers;
    bool practiced;
};
#endif //PROJECT4TEST_DAY_H
