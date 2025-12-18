#include "GameManager.h"
#include <iostream>

GameManager::GameManager()
    : mirimE("미림이", 100000, 10000, 15000),
      currentEnemyIndex(0)
{
    SetupEnemies();
}

void GameManager::SetupEnemies()
{
    enemies.emplace_back("진하쌤, 윤지쌤", 50000, 10000, 20000);
    enemies.emplace_back("성래쌤", 70000, 25000, 35000);
    enemies.emplace_back("교장쌤", 100000, 30000, 40000);
}

void GameManager::Run()
{
    InputHandler input;

    std::cout << "전투 시작\n";

    while (currentEnemyIndex < enemies.size())
    {
        Character& enemy = enemies[currentEnemyIndex];
        std::cout << enemy.GetName() << " 등장\n";

        battle.Start(mirimE, enemy);

        while (battle.IsRunning())
        {
            AttackResult action = input.ResolveFromJson( "src\\main\\test.json" );
            battle.Update( action );
        }

        if (mirimE.IsDead())
        {
            std::cout << "패배\n";
            return;
        }

        std::cout << enemy.GetName() << " 처치\n";
        currentEnemyIndex++;
    }

    std::cout << "모든 적 처치\n";
}
