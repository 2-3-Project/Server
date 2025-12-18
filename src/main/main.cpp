#include <iostream>

#include "../game/GameManager.h"

int main()
{

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
    GameManager game;
    game.Run();

    return 0;    
}