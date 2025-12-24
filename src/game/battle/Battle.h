#pragma once

#include "../core/Character.h"
#include "../input/InputHandler.h"

#include <iostream>
#include <cstdlib>
#include "../json.hpp"

const int TURN_DELAY_MS = 3000;

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
        nlohmann::json lastTurnResult;  // 마지막 턴 결과 저장

        void PlayerTurn( const AttackResult& action );
        void EnemyTurn();
        void CheckEnd();

    public :
        Battle();

        void Start(Character& player, Character& enemy);
        void Update( const AttackResult& action );
        bool IsRunning() const;
        nlohmann::json GetLastTurnResult() const;  // 마지막 턴 결과 반환
        Character* GetPlayer() const;
        Character* GetEnemy() const;
        Turn GetCurrentTurn() const { return currentTurn; }
        void SetRunning(bool r) { running = r; }
        void SetCurrentTurn(Turn t) { currentTurn = t; }
        void SetPlayer(Character* p) { player = p; }
        void SetEnemy(Character* e) { enemy = e; }
};