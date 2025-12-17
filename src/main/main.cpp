#include <iostream>
#include <string>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

int main()
{
    string input;
    getline(cin, input);

    json data = json::parse(input);
    int volume = data["volume"];

    if( volume >= 60 )
    {
        cout << "통과, volume : " << volume << endl;
    }
    else
    {
        cout << "실패, volume : " << volume << endl;
    }
}