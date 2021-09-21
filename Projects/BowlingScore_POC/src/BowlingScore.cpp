#include "Lane.h"
#include "GameInitialiser.h"
#include <memory>
#include <iostream>

int main(int argc, char** argv)
{
    //try
    {
        bool newGame = false;
        std::vector<std::string> players;
        while (true)
        {
            if (argc == 1 || newGame)
            {
                GameInitialiser game;
                players = game.Init();
            }
            else if (argc > 1)
            {
                players.reserve(argc - 1);

                for (int i = 1; i < argc; ++i)
                {
                    players.push_back(argv[i]);
                }
            }

            // Let lane destroy itself after finishing is completed
            {
                std::unique_ptr<ILane> lane(new Lane);
                lane->Init(players);
                lane->Play([&lane]()
                {
                    lane->Finish();
                });
            }

            newGame = true;
            players.clear();
        }
    }
    /*catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }*/
    
    return 0;
}
