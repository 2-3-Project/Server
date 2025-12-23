#pragma once

#include <vector>

#include "battle/Battle.h"
#include "../game/core/Character.h"
#include "../game/input/InputHandler.h"
#include "../game/json.hpp"

class GameManager
{
    private :
        Battle battle;
        Character mirimE;

        std::vector<Character> enemies;
        int currentEnemyIndex;

        void SetupEnemies();

    public :
        GameManager();
        void Run();

        void ProcessTurn( const nlohmann::json& inputJson, nlohmann::json& outputJson );
};