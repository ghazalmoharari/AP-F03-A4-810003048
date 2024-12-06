#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
const string DAILY_BOX_NAME="Daily";
const string EVERY_3_DAYS_BOX_NAME="Every3Days";
const string WEEKLY_BOX_NAME="Weekly";
const string MONTHLY_BOX_NAME="Monthly";
const string MASTERED_BOX_NAME="Mastered";
void error(string message)
{
    cerr << message << endl;
    abort();
}

class Flashcard;

class Box
{
public:
    Box(string t)
    {
        boxType = t;
    }
    string getType() { return boxType; };
    vector<Flashcard *> &getFlashcards()
    {
        return boxFlashcards;
    };
    void addFlashcard(Flashcard *flashcard)
    {
        boxFlashcards.push_back(flashcard);
    };
    void removeFlashcard(Flashcard *flashcard)
    {
        boxFlashcards.erase(find(boxFlashcards.begin(), boxFlashcards.end(), flashcard));
    };

private:
    string boxType;
    vector<Flashcard *> boxFlashcards;
};

class Flashcard
{
public:
    Flashcard(string q, string a, Box *b)
    {
        question = q;
        answer = a;
        currentBox = b;
        numOfWrongAnswers=0;
    };
    string getQuestion() { return question; };
    string getAnswer() { return answer; };
    Box *getBox() { return currentBox; };
    void setBox(Box *box) { currentBox = box; };
    int getNumOfWrongAnswers(){return numOfWrongAnswers;};
    void setToZeroNumOfWrongAnswers(){numOfWrongAnswers=0;};
    void addNumOfWrongAnswers(){numOfWrongAnswers++;};


private:
    string question;
    string answer;
    Box *currentBox;
    int numOfWrongAnswers;
};

class Streak
{
public:
    Streak(int s = 0) { streakDays = s; }
    int getStreak() { return streakDays; }
    void addStreak() { streakDays++; }
    void breakStreak() { streakDays = 0; }

private:
    int streakDays = 0;
};

class Day
{
public:
    Day(int n)
    {
        dayNumber = n;
        correctAnswers = 0;
        incorrectAnswers = 0;
        practiced = false;
    }

    void addCorrect() { correctAnswers++; }
    void addIncorrect() { incorrectAnswers++; }
    void markPracticed() { practiced = true; }
    bool isPracticed() { return practiced; }
    int getCorrect() { return correctAnswers; }
    int getIncorrect() { return incorrectAnswers; }
    int getTotal() { return correctAnswers + incorrectAnswers; }
    int getDayNumber() { return dayNumber; }

private:
    int dayNumber;
    int correctAnswers;
    int incorrectAnswers;
    bool practiced;
};

class LeitnerSystem
{
public:
    LeitnerSystem()
    {
        boxes.push_back(new Box(DAILY_BOX_NAME));
        boxes.push_back(new Box(EVERY_3_DAYS_BOX_NAME));
        boxes.push_back(new Box(WEEKLY_BOX_NAME));
        boxes.push_back(new Box(MONTHLY_BOX_NAME));
        boxes.push_back(new Box(MASTERED_BOX_NAME));
        streak = new Streak();
        currentDay = 1;
        days.push_back(new Day(currentDay));
    };

    ~LeitnerSystem()
    {
        for (auto box : boxes)
        {
            delete box;
        }
        delete streak;
        for (auto day : days)
        {
            delete day;
        }
    };

    Box *getBox(const string &type)
    {
        for (auto box : boxes)
        {
            if (box->getType() == type)
                return box;
        }
        return nullptr; // so we dont get warnings
    };
    Streak *getStreak() { return streak; };
    void addFlashcard(string question, string answer)
    {
        Box *dailyBox = getBox(DAILY_BOX_NAME);
        if (!dailyBox)
            error("Daily box not found!");
        Flashcard *newCard = new Flashcard(question, answer, dailyBox);
        dailyBox->addFlashcard(newCard);
    };
    void moveFlashcard(Flashcard *flashcard, Box *newBox)
    {
        flashcard->getBox()->removeFlashcard(flashcard);
        newBox->addFlashcard(flashcard);
        flashcard->setBox(newBox);
    };

    void moveToNextBox(Flashcard *flashcard)
    {
        Box *currentBox = flashcard->getBox();
        string currentType = currentBox->getType();

        if (currentType == DAILY_BOX_NAME)
            moveFlashcard(flashcard, getBox(EVERY_3_DAYS_BOX_NAME));
        else if (currentType == EVERY_3_DAYS_BOX_NAME)
            moveFlashcard(flashcard, getBox(WEEKLY_BOX_NAME));
        else if (currentType == WEEKLY_BOX_NAME)
            moveFlashcard(flashcard, getBox(MONTHLY_BOX_NAME));
        else if (currentType == MONTHLY_BOX_NAME)
            moveFlashcard(flashcard, getBox(MASTERED_BOX_NAME));
    };

    void moveToPrevBox(Flashcard *flashcard)
    {
        Box *currentBox = flashcard->getBox();
        string currentType = currentBox->getType();

        if (currentType == EVERY_3_DAYS_BOX_NAME)
            moveFlashcard(flashcard, getBox(DAILY_BOX_NAME));
        else if (currentType == WEEKLY_BOX_NAME)
            moveFlashcard(flashcard, getBox(EVERY_3_DAYS_BOX_NAME));
        else if (currentType == MONTHLY_BOX_NAME)
            moveFlashcard(flashcard, getBox(WEEKLY_BOX_NAME));
    };

   void moveCardsToPrevBox(string boxType)
{
    Box *box = getBox(boxType);
    Box *prevBox = nullptr;
    if (boxType == EVERY_3_DAYS_BOX_NAME)
        prevBox = getBox(DAILY_BOX_NAME);
    else if (boxType == WEEKLY_BOX_NAME)
        prevBox = getBox(EVERY_3_DAYS_BOX_NAME);
    else if (boxType == MONTHLY_BOX_NAME)
        prevBox = getBox(WEEKLY_BOX_NAME);


    vector<Flashcard *> &currentFlashcards = box->getFlashcards();
    vector<Flashcard *> &prevFlashcards = prevBox->getFlashcards();
    for (auto flashcard : currentFlashcards)
    {
        prevFlashcards.push_back(flashcard);
        flashcard->setBox(prevBox); 
    }
    currentFlashcards.clear(); 
}

    void noPracticeBoxChange()
{
    if (currentDay % 3 == 0)
    {
        moveCardsToPrevBox(EVERY_3_DAYS_BOX_NAME);
    }
    if (currentDay % 7 == 0)
    {
        moveCardsToPrevBox(WEEKLY_BOX_NAME);
    }
    if (currentDay % 30 == 0)
    {
        moveCardsToPrevBox(MONTHLY_BOX_NAME);
    }
}

    Day *getDay(int dayNumber)
    {
        for (auto day : days)
        {
            if (day->getDayNumber() == dayNumber)
                return day;
        }
        return nullptr;
    };

    void addAnswerResult(bool isCorrect)
    {
        Day *current = getDay(currentDay);
        if (!current)
        {
            current = new Day(currentDay);
            days.push_back(current);
        }
        current->markPracticed();
        if (isCorrect)
        {
            current->addCorrect();

        }
        else
        {
            current->addIncorrect();
        }
    };

    void reviewToday(int numQuestions)
    {
        vector<Flashcard *> todayCards;
        for (auto boxType : {MONTHLY_BOX_NAME, WEEKLY_BOX_NAME, EVERY_3_DAYS_BOX_NAME, DAILY_BOX_NAME})
        {
            Box *box = getBox(boxType);
            if (box)
            {
                vector<Flashcard *> &cards = box->getFlashcards();
                if (boxType == MONTHLY_BOX_NAME && currentDay % 30 != 0)
                    continue;
                if (boxType == WEEKLY_BOX_NAME && currentDay % 7 != 0)
                    continue;
                if (boxType == EVERY_3_DAYS_BOX_NAME && currentDay % 3 != 0)
                    continue;

                todayCards.insert(todayCards.end(), cards.begin(), cards.end());
            }
        }

        int reviewedCount = 0;
        for (auto card : todayCards)
        {
            if (reviewedCount >= numQuestions)
                break;

            cout << "Flashcard: " << card->getQuestion() << endl;
            cout << "Your answer: " << endl;
            string userAnswer;
            getline(cin, userAnswer);

            if (userAnswer == card->getAnswer())
            {
                cout << "Your answer was correct! Well done, keep it up!" << endl;
                addAnswerResult(true);
                moveToNextBox(card);
            }
            else
            {
                cout << "Your answer was incorrect. Don't worry! The correct answer is: " << card->getAnswer() << ". Keep practicing!" << endl;
                addAnswerResult(false);
                card->addNumOfWrongAnswers();
                if (card->getNumOfWrongAnswers()==2){
                    card->setToZeroNumOfWrongAnswers();
                    moveToPrevBox(card);
                }
            }
            reviewedCount++;
        }
        cout << "You’ve completed today’s review! Keep the momentum going and continue building your knowledge, one flashcard at a time!" << endl;
    };

    void getReport(int startDay, int endDay)
    {
        int correct = 0, incorrect = 0, total = 0;
        for (int dayNum = startDay; dayNum <= endDay; dayNum++)
        {
            Day *day = getDay(dayNum);
            if (day)
            {
                correct += day->getCorrect();
                incorrect += day->getIncorrect();
                total += day->getTotal();
            }
        }
        if (startDay == endDay)
        {
            cout << "Day: " << startDay << endl;
        }
        else
        {
            cout << "Day: " << startDay << " to " << endDay << endl;
        }
        cout << "Correct Answers: " << correct << endl;
        cout << "Incorrect Answers: " << incorrect << endl;
        cout << "Total: " << total << endl;
    };
    void getProgressReport()
    {
        Day *current = getDay(currentDay);

        int masteredCount = 0;
        Box *masteredBox = getBox(MASTERED_BOX_NAME);
        masteredCount = masteredBox->getFlashcards().size();
        int totalParticipatedDays = 0;
        for (auto day : days)
        {
            if (day->isPracticed())
            {
                totalParticipatedDays++;
            }
        }
        cout << "Challenge Progress Report:" << endl;
        cout << endl
             << "Day of the Challenge: " << current->getDayNumber() << endl;
        cout << "Streak: " << streak->getStreak() << endl;
        cout << "Total Days Participated: " << totalParticipatedDays << endl;
        cout << "Mastered Flashcards: " << masteredCount << endl;
        cout << endl
             << "Keep up the great work! You're making steady progress toward mastering your flashcards." << endl;
    }
    void nextDay()
    {
        Day *current = getDay(currentDay);
        if (current && current->isPracticed())
        {
            streak->addStreak();
        }
        else
        {
            noPracticeBoxChange();
            streak->breakStreak();
        }
        currentDay++;
        days.push_back(new Day(currentDay));
        cout << "Good morning! Today is day " << currentDay << " of our journey." << endl;
    };
    void run(){
    LeitnerSystem leitnerSystem;
    string command;

    while (cin >> command)
    {
        if (command == "add_flashcard")
        {
            int numQuestions;
            cin >> numQuestions;
            cin.ignore();

            for (int i = 0; i < numQuestions; ++i)
            {
                string question, answer;
                getline(cin, question);
                getline(cin, answer);
                leitnerSystem.addFlashcard(question, answer);
            }
            cout << "flashcards added to the daily box" << endl;
        }
        else if (command == "streak")
        {
            Streak *streak = leitnerSystem.getStreak();
            cout << "Your current streak is: " << streak->getStreak() << endl;
            cout << "Keep going!" << endl;
        }
        if (command == "review_today")
        {
            int numQuestions;
            cin >> numQuestions;
            cin.ignore();
            leitnerSystem.reviewToday(numQuestions);
        }
        else if (command == "get_report")
        {
            int startDay, endDay;
            cin >> startDay >> endDay;

            leitnerSystem.getReport(startDay, endDay);
        }
        else if (command == "next_day")
        {
            leitnerSystem.nextDay();
            Streak *streak = leitnerSystem.getStreak();
            cout << "Your current streak is: " << streak->getStreak() << endl;
            cout << "Start reviewing to keep your streak!"<<endl;
        }
        else if (command == "get_progress_report")
        {
            leitnerSystem.getProgressReport();
        }
    }
}

private:
    vector<Box *> boxes;
    int currentDay;
    Streak *streak;
    vector<Day *> days;
};


int main()
{
    LeitnerSystem* leitner = new LeitnerSystem();
    leitner->run();
}