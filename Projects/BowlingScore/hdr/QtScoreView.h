#ifndef __QT_SCORE_VIEW_H__
#define __QT_SCORE_VIEW_H__

#include "IView.h"

#ifdef BOWLING_WITH_QT

#include <QObject>
#include <QQmlApplicationEngine>
#include <QString>

#include <memory>
#include <string>
#include <vector>

class QtScoreBridge final : public QObject
{
    Q_OBJECT

public:
    explicit QtScoreBridge(QObject* parent = nullptr);

signals:
    void playerAdded(const QString& name);
    void rollRecorded(int playerIndex, int frameIndex, int rollIndex, const QString& value, const QString& flagName, int pins);
    void scoreUpdated(int playerIndex, int frameIndex, int total, bool gameOver);
    void activeChanged(int playerIndex, int frameIndex, int rollIndex);
    void boardCleared();
    void statusChanged(const QString& status);
};

class QtScoreView final : public IView
{
public:
    QtScoreView();
    virtual ~QtScoreView();

    QtScoreView(const QtScoreView&) = delete;
    QtScoreView(QtScoreView&&) = delete;
    QtScoreView& operator = (const QtScoreView&) = delete;
    QtScoreView&& operator = (QtScoreView&&) = delete;

    virtual void Draw(const ViewElement&, void* params = nullptr) override;
    virtual void UpdateFrameScore(const unsigned short, const Flag&) override;
    virtual void UpdateScore(const unsigned short, const short, const bool gameover = false) override;
    virtual void CleanScore() override;
    virtual void SetNextFrameActive(const bool) override;

private:
    static QString flagToText(const Flag&);
    static QString rollToText(const unsigned short, const Flag&);
    void pumpUi(const int delayMs = 0) const;
    void announceActive() const;

    QQmlApplicationEngine m_engine;
    QtScoreBridge* m_bridge = nullptr;

    short m_activePlayerIdx = 0;
    short m_activeFrameIdx = 0;
    short m_activeRollIdx = 0;
    short m_pendingFrames = 0;
    std::vector<std::string> m_players;
};

#endif // BOWLING_WITH_QT

#endif // __QT_SCORE_VIEW_H__
