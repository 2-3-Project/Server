#include <fstream>
#include <iostream>

#include "InputHandler.h"
#include "json.hpp"


using json = nlohmann::json;

AttackResult InputHandler::ResolveFromJson( const std::string& filePath )
{
    std::ifstream file( filePath );
    if ( !file.is_open() )
    {
        std::cerr << "JSON 파일 열기 실패\n";
        return { AttackType::Fail, 0 };
    }

    try
    {
        json j;
        file >> j;

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
    catch( ... )
    {
        return { AttackType::Fail, "" };
    }    
}