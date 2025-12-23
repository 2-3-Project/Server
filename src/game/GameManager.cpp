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
        // Battle::Start에서 설정한 lastTurnResult 사용
        outputJson = battle.GetLastTurnResult();
        std::cout << "디버깅: battle.IsRunning() (시작 후) = " << (battle.IsRunning() ? "true" : "false") << std::endl;
        return;
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
    
    battle.Update(result);

    // 플레이어 턴 결과 가져오기
    nlohmann::json playerTurnResult = battle.GetLastTurnResult();
    
    // 디버깅: 결과 확인
    std::cout << "플레이어 턴 결과: " << playerTurnResult.dump() << std::endl;
    
    // 전투가 끝났는지 체크
    if (!battle.IsRunning()) {
        outputJson = playerTurnResult;
        
        if (enemy.IsDead()) {
            currentEnemyIndex++;
            
            // 모든 적 처치 완료
            if (currentEnemyIndex >= enemies.size()) {
                outputJson["event"] = "game_end";
                outputJson["message"] = "모든 적을 처치했습니다!";
            } else {
                // 다음 적으로 진행
                outputJson["event"] = "enemy_defeated";
                outputJson["message"] = enemy.GetName() + " 처치!";
                outputJson["nextEnemy"] = enemies[currentEnemyIndex].GetName();
            }
        } else if (mirimE.IsDead()) {
            outputJson["event"] = "game_over";
            outputJson["message"] = "패배했습니다...";
        }
        return;
    }

    // 적 턴 자동 처리 (플레이어 턴 후 바로 적이 공격)
    AttackResult dummyResult = { AttackType::Fail, "" };  // 적 턴은 입력 불필요
    battle.Update(dummyResult);
    
    // 적 턴 결과 가져오기
    nlohmann::json enemyTurnResult = battle.GetLastTurnResult();

    // 두 턴의 결과를 합쳐서 반환
    outputJson["playerTurn"] = playerTurnResult;
    outputJson["enemyTurn"] = enemyTurnResult;
    outputJson["event"] = "turn_complete";
    
    // 전투 종료 체크
    if (!battle.IsRunning()) {
        if (enemy.IsDead()) {
            currentEnemyIndex++;
            
            // 모든 적 처치 완료
            if (currentEnemyIndex >= enemies.size()) {
                outputJson["event"] = "game_end";
                outputJson["message"] = "모든 적을 처치했습니다!";
            } else {
                // 다음 적으로 진행
                outputJson["event"] = "enemy_defeated";
                outputJson["message"] = enemy.GetName() + " 처치!";
                outputJson["nextEnemy"] = enemies[currentEnemyIndex].GetName();
            }
        } else if (mirimE.IsDead()) {
            outputJson["event"] = "game_over";
            outputJson["message"] = "패배했습니다...";
        }
    }
}