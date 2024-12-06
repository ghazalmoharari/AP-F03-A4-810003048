#include "../include/Box.h"

Box::Box(string t)
{
    boxType = t;
}
string Box::getType() { return boxType; };
vector<Flashcard *> & Box::getFlashcards()
{
    return boxFlashcards;
};
void Box::addFlashcard(Flashcard *flashcard)
{
    boxFlashcards.push_back(flashcard);
};
void Box::removeFlashcard(Flashcard *flashcard)
{
    boxFlashcards.erase(find(boxFlashcards.begin(), boxFlashcards.end(), flashcard));
};