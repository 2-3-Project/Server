#include <iostream>
#include <string>

#include "../core/Character.h"
#include "../battle/Battle.h"

#include "json.hpp"

using json = nlohmann::json;
using namespace std;

int main()
{
    // string input;
    // getline(cin, input);

    // json data = json::parse(input);
    // int volume = data["volume"];
    // float pronunciation = data["pronunciation"];

    // if( pronunciation >= 70 )
    // {
    //     cout << "성공" << endl;
    //     if( volume > 85 )
    //     {
    //         cout << "공격 실패" << endl;
    //     } // if
    //     else if( volume >= 70 )
    //     {
    //         cout << "강공격" << endl;
    //     } // else if
    //     else if( volume >= 40 )
    //     {
    //         cout << "기본 공격력" << endl;
    //     } // else if
    //     else
    //     {
    //         cout << "실패" << endl;
    //     } // else
    // } // if
    // else
    // {
    //     cout << "실패" << endl;
    // } // else

    Battle battle;

    Character mirimE( "미림", 10000, 1000, 1000 );
    Character teacher1( "진하쌤, 윤지쌤", 10000, 1000, 1000 );
    Character teacher2( "성래쌤", 10000, 1000, 1000 );
    Character teacher3( "교장쌤", 10000, 1000, 1000 );

    battle.ProcessTurn( mirimE, teacher1 );

    teacher1.PrintStatus();

    return 0;    
}