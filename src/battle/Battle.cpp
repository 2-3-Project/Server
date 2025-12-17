#include "Battle.h"

void Battle::ProcessTurn( Character& attacker, Character& defender )
{
    attacker.Attack( defender );
    defender.Attack( attacker );
}