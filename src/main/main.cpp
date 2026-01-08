#include "../game/GameManager.h"
#include "../game/json.hpp"
#include <iostream>
#include <string>
#include <fstream>

int main()
{
    // 테스트 모드: input.json 파일에서 읽기
    std::ifstream inputFile("src/game/input/input.json");
    
    if (!inputFile.is_open()) {
        // 파일이 없으면 stdin 모드 (서버 연동용)
        std::string input;
        std::getline(std::cin, input);

        if (input.empty()) {
            std::cerr << "입력이 비어있습니다." << std::endl;
            return 1;
        }

        try {
            nlohmann::json inputJson = nlohmann::json::parse(input);
            static GameManager game;
            nlohmann::json outputJson;
            game.ProcessTurn(inputJson, outputJson);
            std::cout << outputJson.dump() << std::endl;
        }
        catch (const nlohmann::json::parse_error& e) {
            nlohmann::json errorJson;
            errorJson["event"] = "error";
            errorJson["message"] = "JSON 파싱 오류: " + std::string(e.what());
            std::cerr << errorJson.dump() << std::endl;
            return 1;
        }
        catch (const std::exception& e) {
            nlohmann::json errorJson;
            errorJson["event"] = "error";
            errorJson["message"] = "오류 발생: " + std::string(e.what());
            std::cerr << errorJson.dump() << std::endl;
            return 1;
        }

        return 0;
    }

    // 파일 모드: input.json에서 읽어서 output.json에 쓰기
    try {
        nlohmann::json inputData;
        inputFile >> inputData;
        inputFile.close();

        // GameManager 생성 (한 번만 생성하여 상태 유지)
        static GameManager game;
        
        // 배열이면 모든 입력을 순차적으로 처리, 객체면 하나만 처리
        std::vector<nlohmann::json> inputs;
        if (inputData.is_array()) {
            inputs = inputData.get<std::vector<nlohmann::json>>();
            std::cout << "테스트 모드: input.json에서 " << inputs.size() << "개의 입력을 순차 처리" << std::endl;
        } else if (inputData.is_object()) {
            inputs.push_back(inputData);
            std::cout << "테스트 모드: input.json에서 입력 사용" << std::endl;
        } else {
            std::cerr << "input.json 형식이 올바르지 않습니다." << std::endl;
            return 1;
        }

        // 모든 입력을 순차적으로 처리
        std::vector<nlohmann::json> allResults;
        for (size_t i = 0; i < inputs.size(); i++) {
            std::cout << "\n=== 턴 " << (i + 1) << " 처리 ===" << std::endl;
            nlohmann::json outputJson;
            game.ProcessTurn(inputs[i], outputJson);
            allResults.push_back(outputJson);
            
            // 결과 출력
            std::cout << "결과: " << outputJson.dump(2) << std::endl;
            
            // 게임 종료 체크
            if (outputJson.value("event", "") == "game_end" || 
                outputJson.value("event", "") == "game_over") {
                std::cout << "게임 종료!" << std::endl;
                break;
            }
        }

        // 마지막 결과를 output.json에 쓰기
        std::ofstream outputFile("src/game/input/output.json");
        if (outputFile.is_open()) {
            if (allResults.size() == 1) {
                outputFile << allResults[0].dump(4) << std::endl;
            } else {
                // 여러 결과를 배열로 저장
                outputFile << nlohmann::json(allResults).dump(4) << std::endl;
            }
            outputFile.close();
            std::cout << "\n결과가 output.json에 저장되었습니다." << std::endl;
        } else {
            std::cerr << "output.json 파일을 열 수 없습니다." << std::endl;
            return 1;
        }
    }
    catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON 파싱 오류: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "오류 발생: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}