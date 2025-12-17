#include <iostream>
#include <string>
#include "Character.h"

Character::Character( std::string name, int hp, int atk, int strong ) : name( name ), maxHp( hp ), attack( atk ), strongAttack( strong )
{
    this -> hp = maxHp;
}

void Character::Attack( Character& target )
{
    target.takeDamage( this -> attack );
}
void Character::StrongAttack( Character& target )
{

}
void Character::takeDamage( int dmg )
{
    this -> hp -= dmg;
    if( this -> hp < 0 ) 
    {
        this -> hp = 0;
    }
}
bool Character::isDead() const
{
    return this -> hp <= 0;
}

void Character::PrintStatus()
{
    std::cout << "name : " << this -> name << std::endl;
    std::cout << "hp : " << this -> hp << std::endl;
    std::cout << "atk : " << this -> attack << std::endl;
}
