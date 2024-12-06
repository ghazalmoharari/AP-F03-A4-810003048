

#ifndef PROJECT4_BOX_H
#define PROJECT4_BOX_H

#include "Manual.h"
#include "Flashcard.h"
class Box
{
public:
    Box(string t);
    string getType() ;
    vector<Flashcard *> &getFlashcards();
    void addFlashcard(Flashcard *flashcard);
    void removeFlashcard(Flashcard *flashcard);

private:
    string boxType;
    vector<Flashcard *> boxFlashcards;
};
#endif //PROJECT4_BOX_H
