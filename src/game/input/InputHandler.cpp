#include <fstream>
#include <iostream>

#include "InputHandler.h"
#include "../json.hpp"


using json = nlohmann::json;

InputHandler::InputHandler(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Input JSON 파일 열기 실패: " << filePath << "\n";
        return;
    }

    try
    {
        json j;
        file >> j;
        if (j.is_array())
        {
            inputs = j.get<std::vector<json>>();
        }
        else
        {
            inputs.push_back(j);
        }
    }
    catch (...)
    {
        std::cerr << "Input JSON 파싱 실패\n";
    }
}

AttackResult InputHandler::ResolveFromJson()
{
    if (index >= inputs.size())
    {
        return { AttackType::Fail, "" };
    }

    json j = inputs[index++];
    std::string spellId = j.value("spellId", "");
    float pronunciation = j.value("pronunciation", 0.0f);
    int volume = j.value("volume", 0);

    // 1차
    if (pronunciation < 70)
        return { AttackType::Fail, spellId };

    // 2차
    if (volume >= 85)
        return { AttackType::Fail, spellId };
    else if (volume >= 70)
        return { AttackType::Strong, spellId };
    else if (volume >= 40)
        return { AttackType::Normal, spellId };

    return { AttackType::Fail, spellId };
}

AttackResult InputHandler::ResolveFromJsonInput(const nlohmann::json& input)
{
    std::string spellId = input.value("spellId", "");
    float pronunciation = input.value("pronunciation", 0.0f);
    int volume = input.value("volume", 0);

    // 1차: 발음 정확도 체크
    if (pronunciation < 70)
        return { AttackType::Fail, spellId };

    // 2차: 볼륨 체크
    if (volume >= 85)
        return { AttackType::Fail, spellId };  // 너무 크면 실패
    else if (volume >= 70)
        return { AttackType::Strong, spellId };  // 강한 공격
    else if (volume >= 40)
        return { AttackType::Normal, spellId };  // 일반 공격

    return { AttackType::Fail, spellId };  // 볼륨이 너무 작으면 실패
}