#include "QtScoreView.h"

#ifdef BOWLING_WITH_QT

#include <QCoreApplication>
#include <QDateTime>
#include <QMetaObject>
#include <QQmlContext>
#include <QThread>
#include <QUrl>

#include <algorithm>
#include <stdexcept>

QtScoreBridge::QtScoreBridge(QObject* parent) :
    QObject(parent)
{
}

QtScoreView::QtScoreView()
{
    if (QCoreApplication::instance() == nullptr)
    {
        throw std::runtime_error("QtScoreView requires a QGuiApplication before it is created");
    }

    m_bridge = new QtScoreBridge(&m_engine);
    m_engine.rootContext()->setContextProperty("scoreBridge", m_bridge);
    m_engine.load(QUrl(QStringLiteral("qrc:/qml/BowlingScore.qml")));

    if (m_engine.rootObjects().isEmpty())
    {
        throw std::runtime_error("Cannot load BowlingScore Qt/QML UI");
    }

    emit m_bridge->statusChanged(QStringLiteral("Waiting for the first roll"));
    pumpUi(250);
}

QtScoreView::~QtScoreView()
{
    pumpUi();
}

void QtScoreView::Draw(const ViewElement& element, void* params)
{
    switch (element)
    {
        case ViewElement::FRAME:
        {
            ++m_pendingFrames;
            break;
        }
        case ViewElement::PLAYER:
        {
            const auto* name = static_cast<std::string*>(params);
            if (name == nullptr)
            {
                throw std::runtime_error("QtScoreView cannot draw a player without a name");
            }

            m_players.push_back(*name);
            emit m_bridge->playerAdded(QString::fromStdString(*name));
            m_pendingFrames = 0;
            announceActive();
            pumpUi(100);
            break;
        }
        case ViewElement::FLUSH:
        {
            emit m_bridge->statusChanged(QStringLiteral("Game on"));
            announceActive();
            pumpUi(300);
            break;
        }
        default:
        {
            throw std::runtime_error("Unknown type of view element");
        }
    }
}

void QtScoreView::UpdateFrameScore(const unsigned short score, const Flag& flag)
{
    emit m_bridge->rollRecorded(
        m_activePlayerIdx,
        m_activeFrameIdx,
        m_activeRollIdx,
        rollToText(score, flag),
        flagToText(flag),
        score);
    emit m_bridge->statusChanged(QStringLiteral("%1 rolled %2")
        .arg(QString::fromStdString(m_players.at(m_activePlayerIdx)))
        .arg(score));
    pumpUi(450);
}

void QtScoreView::UpdateScore(const unsigned short score, const short shift, const bool gameover)
{
    const auto frameIndex = static_cast<short>(m_activeFrameIdx + shift);
    if (frameIndex < 0)
    {
        return;
    }

    emit m_bridge->scoreUpdated(m_activePlayerIdx, frameIndex, score, gameover);
    if (gameover)
    {
        emit m_bridge->statusChanged(QStringLiteral("%1 finished with %2")
            .arg(QString::fromStdString(m_players.at(m_activePlayerIdx)))
            .arg(score));
    }
    pumpUi(gameover ? 700 : 250);
}

void QtScoreView::CleanScore()
{
    emit m_bridge->statusChanged(QStringLiteral("Game finished. Close the window to exit."));
    pumpUi();
}

void QtScoreView::SetNextFrameActive(const bool last)
{
    if (m_players.empty())
    {
        return;
    }

    if (!last)
    {
        ++m_activeRollIdx;
    }
    else
    {
        m_activeRollIdx = 0;
        if (m_activePlayerIdx < static_cast<short>(m_players.size() - 1))
        {
            ++m_activePlayerIdx;
        }
        else
        {
            m_activePlayerIdx = 0;
            ++m_activeFrameIdx;
        }
    }

    announceActive();
    pumpUi(150);
}

QString QtScoreView::flagToText(const Flag& flag)
{
    switch (flag)
    {
        case Flag::ZERO:
            return QStringLiteral("zero");
        case Flag::NOTHING:
            return QStringLiteral("open");
        case Flag::SPARE:
            return QStringLiteral("spare");
        case Flag::STRIKE:
            return QStringLiteral("strike");
        default:
            return QStringLiteral("unknown");
    }
}

QString QtScoreView::rollToText(const unsigned short score, const Flag& flag)
{
    switch (flag)
    {
        case Flag::ZERO:
            return QStringLiteral("-");
        case Flag::SPARE:
            return QStringLiteral("/");
        case Flag::STRIKE:
            return QStringLiteral("X");
        case Flag::NOTHING:
        default:
            return QString::number(score);
    }
}

void QtScoreView::pumpUi(const int delayMs) const
{
    QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    if (delayMs > 0)
    {
        QThread::msleep(static_cast<unsigned long>(delayMs));
        QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    }
}

void QtScoreView::announceActive() const
{
    emit m_bridge->activeChanged(m_activePlayerIdx, m_activeFrameIdx, m_activeRollIdx);
}

#endif // BOWLING_WITH_QT
