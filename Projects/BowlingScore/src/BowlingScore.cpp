#include "SocketBasic.h"
#include "Lane.h"
#include "GameInitialiser.h"
#include <memory>
#include <iostream>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <string>
#include <cctype>

#ifdef BOWLING_WITH_QT
#include <QGuiApplication>
#endif

static bool keepRunning = true;

static void intHandler(int)
{
    keepRunning = false;
}

static bool useQtUi()
{
    const char* view = std::getenv("BOWLING_VIEW");
    if (view == nullptr)
    {
        return false;
    }

    std::string value(view);
    for (auto& ch : value)
    {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }

    return value == "ui";
}

int main(int argc, char** argv)
{
#ifdef BOWLING_WITH_QT
    std::unique_ptr<QGuiApplication> qtApplication;
    if (useQtUi())
    {
        qtApplication = std::make_unique<QGuiApplication>(argc, argv);
    }
#endif

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
        const bool singleGameMode = useQtUi();
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

#ifdef BOWLING_WITH_QT
                if (singleGameMode && qtApplication != nullptr)
                {
                    return qtApplication->exec();
                }
#endif
            }

            newGame = true;
            players.clear();

            if (singleGameMode)
            {
                break;
            }
        }
    }
    //catch(const std::exception& e)
    //{
    //    std::cerr << e.what() << std::endl;
    //    return 1;
    //}
    return 0;
}

