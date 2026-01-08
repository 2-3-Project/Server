#include "GameManager.h"
#include <iostream>
#include <fstream>
#include <windows.h>

std::ofstream outputFile("src/game/input/output.json");

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
    InputHandler input("src\\game\\input\\input.json");

    std::cout << "전투 시작\n";

    while (currentEnemyIndex < enemies.size())
    {
        Character& enemy = enemies[currentEnemyIndex];
        std::cout << enemy.GetName() << " 등장\n";

        battle.Start(mirimE, enemy);

        while (battle.IsRunning())
        {
            AttackResult action = input.ResolveFromJson();
            battle.Update( action );
        }

        if (mirimE.IsDead())
        {
            std::cout << "패배\n";
            return;
        }

        std::cout << enemy.GetName() << " 처치\n";

        // 스토리 진행 (마지막 적이 아니면)
        if (currentEnemyIndex < enemies.size() - 1) {
            outputFile << R"({"event": "story", "message": ")" << (currentEnemyIndex + 1) << R"(번째 선생님을 물리쳤다! 스토리 진행..."} )" << std::endl;
            std::cout << "스토리 진행...\n";
            Sleep(3000);
        }

        currentEnemyIndex++;
    }

    std::cout << "모든 적 처치\n";
}

void GameManager::ProcessTurn(const nlohmann::json& inputJson, nlohmann::json& outputJson) {
    LoadState();

    // 게임 종료 체크
    if (currentEnemyIndex >= enemies.size()) {
        outputJson["event"] = "game_end";
        outputJson["message"] = "모든 적을 처치했습니다!";
        return;
    }

    Character& enemy = enemies[currentEnemyIndex];

    // 전투가 시작되지 않았으면 시작
    std::cout << "디버깅: battle.IsRunning() = " << (battle.IsRunning() ? "true" : "false") << std::endl;
    if (!battle.IsRunning()) {
        std::cout << "전투 시작!" << std::endl;
        battle.Start(mirimE, enemy);
        std::cout << "디버깅: battle.IsRunning() (시작 후) = " << (battle.IsRunning() ? "true" : "false") << std::endl;
    } else {
        // 상태 복원 시 player와 enemy 설정
        battle.SetPlayer(&mirimE);
        battle.SetEnemy(&enemy);
    }
    
    std::cout << "전투 진행 중 - 플레이어 턴 처리 시작" << std::endl;

    // 플레이어가 죽었는지 체크
    if (mirimE.IsDead()) {
        outputJson["event"] = "game_over";
        outputJson["message"] = "패배했습니다...";
        return;
    }

    // 플레이어 턴 처리 (마이크 입력 받아서 공격)
    InputHandler inputHandler("");
    AttackResult result = inputHandler.ResolveFromJsonInput(inputJson);
    
    // 디버깅: 입력 값 확인
    std::cout << "입력 값 - spellId: " << inputJson.value("spellId", "") 
              << ", pronunciation: " << inputJson.value("pronunciation", 0.0f)
              << ", volume: " << inputJson.value("volume", 0) << std::endl;
    
    // 턴 결과를 저장할 배열
    nlohmann::json turns = nlohmann::json::array();
    
    // 현재 턴을 플레이어로 설정 (입력에 따라 플레이어 턴 처리)
    battle.SetCurrentTurn(Turn::PLAYER);
    std::cout << "currentTurn after set: " << static_cast<int>(battle.GetCurrentTurn()) << std::endl;
    
    // 플레이어 턴
    battle.Update(result);
    turns.push_back(battle.GetLastTurnResult());
    
    // 디버깅: 플레이어 턴 결과 확인
    std::cout << "플레이어 턴 결과: " << turns.back().dump() << std::endl;
    
    // 추가 턴 진행 (적 턴 1번)
    for(int i = 0; i < 1 && battle.IsRunning(); i++) {
        AttackResult dummyResult = { AttackType::Fail, "" };
        battle.Update(dummyResult);
        turns.push_back(battle.GetLastTurnResult());
    }
    
    // 모든 턴 결과를 outputJson에 넣기
    outputJson["turns"] = turns;
    outputJson["event"] = "battle_update";

    SaveState();
}

void GameManager::SaveState() {
    nlohmann::json state;
    state["currentEnemyIndex"] = currentEnemyIndex;
    state["battleRunning"] = battle.IsRunning();
    state["battleCurrentTurn"] = static_cast<int>(battle.GetCurrentTurn());
    state["playerHp"] = mirimE.GetHp();
    if (currentEnemyIndex < enemies.size()) {
        state["enemyHp"] = enemies[currentEnemyIndex].GetHp();
    }
    std::ofstream file("game_state.json");
    file << state.dump();
}

void GameManager::LoadState() {
    std::ifstream file("game_state.json");
    if (file) {
        nlohmann::json state;
        file >> state;
        currentEnemyIndex = state.value("currentEnemyIndex", 0);
        bool running = state.value("battleRunning", false);
        battle.SetRunning(running);
        if (running) {
        Turn turn = static_cast<Turn>(state.value("battleCurrentTurn", 0));
        battle.SetCurrentTurn(turn);
        }
        mirimE.SetHp(state.value("playerHp", 100000));
        if (currentEnemyIndex < enemies.size()) {
            enemies[currentEnemyIndex].SetHp(state.value("enemyHp", 50000));
        }
    }
}