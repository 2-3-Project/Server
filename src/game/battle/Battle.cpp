#include <windows.h>
#include <iostream>
#include <fstream>

#include "../json.hpp"

#include "Battle.h"

extern std::ofstream outputFile;

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

    // lastTurnResult에도 저장 (ProcessTurn에서 사용)
    lastTurnResult.clear();
    lastTurnResult["event"] = "battle_start";
    lastTurnResult["player"] = {{"name", player->GetName()}, {"hp", player->GetHp()}, {"maxHp", player->GetMaxHp()}};
    lastTurnResult["enemy"] = {{"name", enemy->GetName()}, {"hp", enemy->GetHp()}, {"maxHp", enemy->GetMaxHp()}};
    
    std::string jsonStr = lastTurnResult.dump();
    outputFile << jsonStr << std::endl;
}

bool Battle::IsRunning() const
{
    return running;
}

void Battle::Update( const AttackResult& action )
{
    if (!running) return;

    if (currentTurn == Turn::PLAYER)
        PlayerTurn( action );
    else
        EnemyTurn();

    CheckEnd();
}

void Battle::PlayerTurn( const AttackResult& result )
{
    lastTurnResult.clear();
    lastTurnResult["event"] = "player_turn";
    lastTurnResult["spellId"] = result.spellId;
    Sleep(TURN_DELAY_MS);

    if (result.type == AttackType::Fail)
    {
        lastTurnResult["result"] = "fail";
    }
    else if (result.type == AttackType::Normal)
    {
        int damage = player->GetAttack();
        enemy->TakeDamage(damage);
        lastTurnResult["result"] = "normal";
        lastTurnResult["damage"] = damage;
        lastTurnResult["enemyHp"] = enemy->GetHp();
    }
    else if (result.type == AttackType::Strong)
    {
        int damage = player->GetStrongAttack();
        enemy->TakeDamage(damage);
        lastTurnResult["result"] = "strong";
        lastTurnResult["damage"] = damage;
        lastTurnResult["enemyHp"] = enemy->GetHp();
    }

    lastTurnResult["playerHp"] = player->GetHp();

    std::string jsonStr = lastTurnResult.dump();
    outputFile << jsonStr << std::endl;
    currentTurn = Turn::ENEMY;
}

void Battle::EnemyTurn()
{
    lastTurnResult.clear();
    lastTurnResult["event"] = "enemy_turn";
    Sleep(TURN_DELAY_MS);

    int roll = rand() % 100;

    if (roll < 10)
    {
        int damage = enemy->GetStrongAttack();
        player->TakeDamage(damage);
        lastTurnResult["result"] = "strong";
        lastTurnResult["damage"] = damage;
        lastTurnResult["playerHp"] = player->GetHp();
    }
    else
    {
        int damage = enemy->GetAttack();
        player->TakeDamage(damage);
        lastTurnResult["result"] = "normal";
        lastTurnResult["damage"] = damage;
        lastTurnResult["playerHp"] = player->GetHp();
    }

    lastTurnResult["enemyHp"] = enemy->GetHp();

    std::string jsonStr = lastTurnResult.dump();
    outputFile << jsonStr << std::endl;
    currentTurn = Turn::PLAYER;
}

void Battle::CheckEnd()
{
    if (player->IsDead() || enemy->IsDead())
    {
        running = false;
        lastTurnResult.clear();
        lastTurnResult["event"] = "battle_end";
        lastTurnResult["winner"] = player->IsDead() ? "enemy" : "player";
        lastTurnResult["playerHp"] = player->GetHp();
        lastTurnResult["enemyHp"] = enemy->GetHp();
        std::string jsonStr = lastTurnResult.dump();
        outputFile << jsonStr << std::endl;
    }
}

nlohmann::json Battle::GetLastTurnResult() const
{
    return lastTurnResult;
}

Character* Battle::GetPlayer() const
{
    return player;
}

Character* Battle::GetEnemy() const
{
    return enemy;
}
