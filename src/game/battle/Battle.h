#pragma once

#include "../core/Character.h"
#include "../input/InputHandler.h"

enum class Turn {
    PLAYER,
    ENEMY
};

class Battle
{
    private :
        Character* player;
        Character* enemy;
        Turn currentTurn;
        bool running;

        void PlayerTurn( const AttackResult& action );
        void EnemyTurn();
        void CheckEnd();

    public :
        Battle();

        void Start(Character& player, Character& enemy);
        void Update( const AttackResult& playerAction );
        bool IsRunning() const;

};