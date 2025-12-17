#pragma once

class Character 
{
    public :
        std::string name;
        int maxHp;
        int hp;
        int attack;
        int strongAttack;

    public :
        Character(std::string name, int hp, int atk, int strong);

        void Attack( Character& target );
        void StrongAttack( Character& target );
        void takeDamage(int dmg);
        bool isDead() const;

        void PrintStatus();
};