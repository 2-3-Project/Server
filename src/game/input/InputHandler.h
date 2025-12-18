#pragma once

#include <string>

enum class AttackType
{
    Fail,
    Normal,
    Strong
};

struct AttackResult
{
    AttackType type;
    std::string spellId;
};

class InputHandler
{
    public:
        AttackResult ResolveFromJson( const std::string& filePath );
};