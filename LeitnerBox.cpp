#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

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
    };
    string getQuestion() { return question; };
    string getAnswer() { return answer; };
    Box *getBox() { return currentBox; };
    void setBox(Box *box) { currentBox = box; };

private:
    string question;
    string answer;
    Box *currentBox;
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
        boxes.push_back(new Box("Daily"));
        boxes.push_back(new Box("Every3Days"));
        boxes.push_back(new Box("Weekly"));
        boxes.push_back(new Box("Monthly"));
        boxes.push_back(new Box("Mastered"));
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
        Box *dailyBox = getBox("Daily");
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

        if (currentType == "Daily")
            moveFlashcard(flashcard, getBox("Every3Days"));
        else if (currentType == "Every3Days")
            moveFlashcard(flashcard, getBox("Weekly"));
        else if (currentType == "Weekly")
            moveFlashcard(flashcard, getBox("Monthly"));
        else if (currentType == "Monthly")
            moveFlashcard(flashcard, getBox("Mastered"));
    };

    void moveToPrevBox(Flashcard *flashcard)
    {
        Box *currentBox = flashcard->getBox();
        string currentType = currentBox->getType();

        if (currentType == "Every3Days")
            moveFlashcard(flashcard, getBox("Daily"));
        else if (currentType == "Weekly")
            moveFlashcard(flashcard, getBox("Every3Days"));
        else if (currentType == "Monthly")
            moveFlashcard(flashcard, getBox("Weekly"));
    };

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
        for (auto boxType : {"Monthly", "Weekly", "Every3Days", "Daily"})
        {
            Box *box = getBox(boxType);
            if (box)
            {
                vector<Flashcard *> &cards = box->getFlashcards();
                if (boxType == string("Every3Days") && currentDay % 3 != 0)
                    continue;
                if (boxType == string("Weekly") && currentDay % 7 != 0)
                    continue;
                if (boxType == string("Monthly") && currentDay % 30 != 0)
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
                moveToPrevBox(card);
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
        Box *masteredBox = getBox("Mastered");
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
            streak->breakStreak();
        }
        currentDay++;
        days.push_back(new Day(currentDay));
        cout << "Good morning! Today is day " << currentDay << " of our journey." << endl;
    }

private:
    vector<Box *> boxes;
    int currentDay;
    Streak *streak;
    vector<Day *> days;
};

int main()
{
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