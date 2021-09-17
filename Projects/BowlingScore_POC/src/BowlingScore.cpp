#include "Lane.h"
#include <memory>
#include <iostream>

int main(int argc, char** argv)
{
    try
    {
        std::vector<std::string> players;

        if (argc > 1)
        {
            players.reserve(argc - 1);
        }
        
        for (int i = 1; i < argc; ++i)
        {
            players.push_back(argv[i]);
        }

        std::unique_ptr<ILane> lane(new Lane);
        lane->Init(players);
        lane->Play([&lane]{
            lane->Finish();
        });
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
