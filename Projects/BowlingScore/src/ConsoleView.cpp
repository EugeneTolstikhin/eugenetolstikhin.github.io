#include "ConsoleView.h"
#include <stdexcept>

#include <iostream>
#include <iomanip>

ConsoleView::ConsoleView()
{
    //
}

ConsoleView::~ConsoleView()
{
    //
}

void ConsoleView::InitScoreTable(const std::vector<std::string>& players)
{
    auto drawRow = [](const std::string& name, unsigned nameWidth){
        constexpr unsigned ROW_WIDTH = 150;
        std::string displayName((name.length() <= nameWidth ? name : (name.substr(0, nameWidth - 3) + std::string("..."))));

        std::string picture;

        auto drawHeaderLine = [&picture]{
            picture.append(1, '|');
            picture.append(ROW_WIDTH - 1, '-');
            picture.append(1, '|');
            picture.append(1, '\n');
        };

        auto drawAboveName = [&picture, &nameWidth]{
            picture.append(1, '|');
            picture.append(nameWidth + 1, ' ');
            picture.append(1, '|');

            for (size_t i = 0; i < 9; ++i)
            {
                picture.append(6, ' ');
                picture.append(1, '|');
                picture.append(6, ' ');
                picture.append(1, '|');
            }

            for (size_t i = 0; i < 3; ++i)
            {
                picture.append(3, ' ');
                picture.append(1, '|');
            }

            picture.append(1, '\n');
        };

        auto drawName = [&picture, &displayName, &nameWidth]{
            picture.append(1, '|');
            picture.append(displayName);
            picture.append(nameWidth - displayName.length() + 1, ' ');
            picture.append(1, '|');

            for (size_t i = 0; i < 9; ++i)
            {
                picture.append(6, ' ');
                picture.append(1, '|');
                picture.append(6, '-');
                picture.append(1, '|');
            }

            for (size_t i = 0; i < 3; ++i)
            {
                picture.append(3, '-');
                picture.append(1, '|');
            }

            picture.append(1, '\n');
        };

        auto drawBelowName = [&picture, &nameWidth]{
            picture.append(1, '|');
            picture.append(nameWidth + 1, ' ');
            picture.append(1, '|');

            for (size_t i = 0; i < 9; ++i)
            {
                picture.append(13, ' ');
                picture.append(1, '|');
            }

            picture.append(ROW_WIDTH - nameWidth + 1 - 10 * 13, ' ');
            picture.append(1, '|');
            picture.append(1, '\n');
        };

        auto drawFooterLine = [&picture]{
            picture.append(1, '|');
            picture.append(ROW_WIDTH - 1, '-');
            picture.append(1, '|');
            picture.append(1, '\n');
        };

        drawHeaderLine();
        drawAboveName();
        drawName();
        drawBelowName();
        drawFooterLine();

        std::cout << std::setw(ROW_WIDTH) << picture << std::flush;
    };

    constexpr unsigned MAX_LENGTH = 10;
    for (auto& player : players)
    {
        drawRow(player, MAX_LENGTH);
    }
}

void ConsoleView::UpdateScore()
{
    throw std::runtime_error("Unimplemented functionality");
}

void ConsoleView::CleanScore()
{
    throw std::runtime_error("Unimplemented functionality");
}
