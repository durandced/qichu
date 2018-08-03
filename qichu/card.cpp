#include "card.h"

bool Card::operator<(const Card& other)
{
    return this->value < other.value;
}
