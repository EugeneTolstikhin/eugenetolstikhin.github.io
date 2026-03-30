#include "Game.h"
#include "IView.h"

#include <array>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
class RecordingView final : public IView
{
public:
    void Draw(const ViewElement&, void* = nullptr) override
    {
    }

    void UpdateFrameScore(const unsigned short, const Flag&) override
    {
    }

    void UpdateScore(const unsigned short score, const short shift, const bool) override
    {
        const auto absoluteFrame = static_cast<long long>(m_activeFrameIndex) + shift;
        if (absoluteFrame < 0 || absoluteFrame >= static_cast<long long>(MAX_FRAME_AMOUNT))
        {
            throw std::runtime_error("Resolved score targeted an invalid frame index");
        }

        m_cumulativeScores.at(static_cast<std::size_t>(absoluteFrame)) = score;
    }

    void CleanScore() override
    {
    }

    void SetNextFrameActive(const bool last) override
    {
        if (last && m_activeFrameIndex + 1 < MAX_FRAME_AMOUNT)
        {
            ++m_activeFrameIndex;
        }
    }

    const std::array<std::optional<unsigned short>, MAX_FRAME_AMOUNT>& Scores() const noexcept
    {
        return m_cumulativeScores;
    }

private:
    std::size_t m_activeFrameIndex = 0;
    std::array<std::optional<unsigned short>, MAX_FRAME_AMOUNT> m_cumulativeScores{};
};

using Rolls = std::vector<unsigned short>;
using ExpectedScores = std::array<unsigned short, MAX_FRAME_AMOUNT>;

ExpectedScores playGame(const Rolls& rolls)
{
    RecordingView view;
    std::size_t nextRoll = 0;

    Game game(&view, [&rolls, &nextRoll]() -> unsigned short
    {
        if (nextRoll >= rolls.size())
        {
            throw std::runtime_error("The game requested more rolls than the test provided");
        }

        return rolls.at(nextRoll++);
    });

    bool gameOver = false;
    std::size_t completedFrames = 0;
    while (!gameOver)
    {
        while (game.IsAnotherThrowAllowed())
        {
            game.ThrowBall();
        }

        game.UpdateTotalScore();
        game.CloseGame([&gameOver]()
        {
            gameOver = true;
        });

        ++completedFrames;
        if (completedFrames > MAX_FRAME_AMOUNT)
        {
            throw std::runtime_error("Game did not finish within the expected number of frames");
        }
    }

    if (nextRoll != rolls.size())
    {
        throw std::runtime_error("The game did not consume all rolls from the test case");
    }

    ExpectedScores actual{};
    for (std::size_t frameIndex = 0; frameIndex < MAX_FRAME_AMOUNT; ++frameIndex)
    {
        const auto& maybeScore = view.Scores().at(frameIndex);
        if (!maybeScore.has_value())
        {
            std::ostringstream message;
            message << "Missing cumulative score for frame " << (frameIndex + 1);
            throw std::runtime_error(message.str());
        }

        actual.at(frameIndex) = maybeScore.value();
    }

    return actual;
}

void assertScoresEqual(const std::string& caseName, const ExpectedScores& actual, const ExpectedScores& expected)
{
    if (actual == expected)
    {
        return;
    }

    std::ostringstream message;
    message << caseName << " failed.\nExpected: ";
    for (const auto score : expected)
    {
        message << score << ' ';
    }
    message << "\nActual:   ";
    for (const auto score : actual)
    {
        message << score << ' ';
    }

    throw std::runtime_error(message.str());
}

void runCase(const std::string& caseName, const Rolls& rolls, const ExpectedScores& expected)
{
    assertScoresEqual(caseName, playGame(rolls), expected);
}
}

int main()
{
    try
    {
        runCase("gutter game",
            Rolls(20, 0),
            ExpectedScores{0, 0, 0, 0, 0, 0, 0, 0, 0, 0});

        runCase("all ones",
            Rolls(20, 1),
            ExpectedScores{2, 4, 6, 8, 10, 12, 14, 16, 18, 20});

        runCase("opening spare",
            Rolls{5, 5, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            ExpectedScores{13, 16, 16, 16, 16, 16, 16, 16, 16, 16});

        runCase("double strike into open frame",
            Rolls{10, 10, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            ExpectedScores{23, 40, 47, 47, 47, 47, 47, 47, 47, 47});

        runCase("all spares",
            Rolls(21, 5),
            ExpectedScores{15, 30, 45, 60, 75, 90, 105, 120, 135, 150});

        runCase("perfect game",
            Rolls(12, 10),
            ExpectedScores{30, 60, 90, 120, 150, 180, 210, 240, 270, 300});

        runCase("last frame spare fill ball",
            Rolls{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 3, 10},
            ExpectedScores{0, 0, 0, 0, 0, 0, 0, 0, 0, 20});

        runCase("last frame strike with fill balls",
            Rolls{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 7, 2},
            ExpectedScores{0, 0, 0, 0, 0, 0, 0, 0, 0, 19});

        std::cout << "All BowlingScore scoring tests passed." << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}
