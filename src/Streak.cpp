
#include "../include/Streak.h"


    Streak::Streak(int s = 0) { streakDays = s; }
    int Streak::getStreak() { return streakDays; }
    void Streak::addStreak() { streakDays++; }
    void Streak::breakStreak() { streakDays = 0; }
