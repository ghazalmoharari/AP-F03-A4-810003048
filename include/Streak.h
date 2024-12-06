

#ifndef PROJECT4TEST_STREAK_H
#define PROJECT4TEST_STREAK_H
class Streak
{
public:
    Streak(int s = 0) ;
    int getStreak() ;
    void addStreak();
    void breakStreak() ;

private:
    int streakDays = 0;
};
#endif //PROJECT4TEST_STREAK_H
