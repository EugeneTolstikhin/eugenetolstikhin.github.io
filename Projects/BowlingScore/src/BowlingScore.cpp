#include "SocketBasic.h"
#include "Lane.h"
#include "GameInitialiser.h"
#include <memory>
#include <iostream>
#include <csignal>
#include <cstring>

static bool keepRunning = true;

static void intHandler(int dummy)
{
    keepRunning = false;
}

int main(int argc, char** argv)
{
#ifdef __linux__
    struct sigaction act;
    memset(&act, '\0', sizeof(act));
    act.sa_handler = intHandler;
    sigaction(SIGINT, &act, NULL);
#elif defined _WIN32
    signal(SIGINT, intHandler);
#endif

    //try
    {
		SocketBasic::init("config.cfg", ConfigType::FILE);
        bool newGame = false;
        std::vector<std::string> players;
        GameInitialiser game;
        while (keepRunning)
        {
            if (argc == 1 || newGame)
            {
                players = game.Init();
				if (!keepRunning) break;
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
                std::unique_ptr<ILane> lane(std::make_unique<Lane>());
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
    //catch(const std::exception& e)
    //{
    //    std::cerr << e.what() << std::endl;
    //    return 1;
    //}
    
    return 0;
}
