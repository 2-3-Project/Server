#include <iostream>
#include <cstdlib>

#include "Battle.h"

Battle::Battle()
    : player(nullptr), enemy(nullptr),
      currentTurn(Turn::PLAYER),
      running(false)
{
}

void Battle::Start(Character& p, Character& e)
{
    player = &p;
    enemy = &e;
    currentTurn = Turn::PLAYER;
    running = true;

    std::cout << "배틀 시작\n";
}

bool Battle::IsRunning() const
{
    return running;
}

void Battle::Update( const AttackResult& playerAction )
{
    if (!running) return;

    if (currentTurn == Turn::PLAYER)
        PlayerTurn( playerAction );
    else
        EnemyTurn();

    CheckEnd();
}

void Battle::PlayerTurn( const AttackResult& result )
{
    std::cout << "[플레이어 턴]\n";

    if (result.type == AttackType::Fail)
    {
        std::cout << "공격 실패\n";
    }
    else if (result.type == AttackType::Normal)
    {
        std::cout << result.spellId << " 기본 공격\n";
        enemy->TakeDamage(player->GetAttack());
    }
    else if (result.type == AttackType::Strong)
    {
        std::cout << result.spellId << " 강공격\n";
        enemy->TakeDamage(player->GetStrongAttack());
    }

    currentTurn = Turn::ENEMY;
}

void Battle::EnemyTurn()
{
    std::cout << "[적 턴]\n";

    int roll = rand() % 100;

    if (roll < 10)
        player->TakeDamage(enemy->GetStrongAttack());
    else
        player->TakeDamage(enemy->GetAttack());

    currentTurn = Turn::PLAYER;
}

void Battle::CheckEnd()
{
    if (player->IsDead() || enemy->IsDead())
    {
        running = false;
        std::cout << "배틀 종료\n";
    }
}
