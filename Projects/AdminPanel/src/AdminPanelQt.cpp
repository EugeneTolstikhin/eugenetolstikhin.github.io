#include <QtCore/QFile>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtNetwork/QTcpSocket>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlApplicationEngine>
#include <QtGui/QGuiApplication>

namespace
{
struct Config
{
    QString host = "host.docker.internal";
    int port = 12002;
};

Config readConfig()
{
    Config config;
    QFile file("config.cfg");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return config;
    }

    QTextStream input(&file);
    while (!input.atEnd())
    {
        const QString line = input.readLine().trimmed();
        const auto separator = line.indexOf(':');
        if (separator < 0)
        {
            continue;
        }

        const QString key = line.left(separator).trimmed();
        const QString value = line.mid(separator + 1).trimmed();
        if (key == "HOST")
        {
            config.host = value;
        }
        else if (key == "PORT")
        {
            bool ok = false;
            const int parsed = value.toInt(&ok);
            if (ok)
            {
                config.port = parsed;
            }
        }
    }

    return config;
}
}

class AdminClient final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString defaultHost READ defaultHost CONSTANT)
    Q_PROPERTY(int defaultPort READ defaultPort CONSTANT)
    Q_PROPERTY(QString statusLog READ statusLog NOTIFY statusLogChanged)
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)

public:
    explicit AdminClient(QObject* parent = nullptr)
        : QObject(parent)
        , m_config(readConfig())
        , m_statusLog(QString("Ready. Default target: %1:%2").arg(m_config.host).arg(m_config.port))
    {
    }

    QString defaultHost() const
    {
        return m_config.host;
    }

    int defaultPort() const
    {
        return m_config.port;
    }

    QString statusLog() const
    {
        return m_statusLog;
    }

    bool busy() const
    {
        return m_busy;
    }

    Q_INVOKABLE void sendPlayers(const QString& host, int port, const QString& players)
    {
        const QString cleanHost = host.trimmed();
        const QString cleanPlayers = players.trimmed();
        if (cleanHost.isEmpty())
        {
            appendStatus("Host is required.");
            return;
        }

        if (port < 1 || port > 65535)
        {
            appendStatus("Port must be between 1 and 65535.");
            return;
        }

        if (cleanPlayers.isEmpty())
        {
            appendStatus("Enter at least one player name.");
            return;
        }

        setBusy(true);
        appendStatus(QString("Connecting to %1:%2...").arg(cleanHost).arg(port));

        QTcpSocket socket;
        socket.connectToHost(cleanHost, static_cast<quint16>(port));
        if (!socket.waitForConnected(5000))
        {
            appendStatus(QString("Connection failed: %1").arg(socket.errorString()));
            setBusy(false);
            return;
        }

        const QByteArray payload = QString("Test %1\n").arg(cleanPlayers).toUtf8();
        socket.write(payload);
        if (!socket.waitForBytesWritten(5000))
        {
            appendStatus(QString("Send failed: %1").arg(socket.errorString()));
            socket.disconnectFromHost();
            setBusy(false);
            return;
        }

        if (!socket.waitForReadyRead(5000))
        {
            appendStatus(QString("No answer from server: %1").arg(socket.errorString()));
            socket.disconnectFromHost();
            setBusy(false);
            return;
        }

        appendStatus(QString("Server answer: %1").arg(QString::fromUtf8(socket.readAll()).trimmed()));
        socket.disconnectFromHost();
        setBusy(false);
    }

signals:
    void statusLogChanged();
    void busyChanged();

private:
    void appendStatus(const QString& message)
    {
        if (!m_statusLog.isEmpty())
        {
            m_statusLog.append('\n');
        }
        m_statusLog.append(message);
        emit statusLogChanged();
    }

    void setBusy(bool busy)
    {
        if (m_busy == busy)
        {
            return;
        }

        m_busy = busy;
        emit busyChanged();
    }

    Config m_config;
    QString m_statusLog;
    bool m_busy = false;
};

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    AdminClient adminClient;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("adminClient", &adminClient);
    engine.load(QUrl(QStringLiteral("qrc:/AdminPanel/Main.qml")));
    if (engine.rootObjects().isEmpty())
    {
        return 1;
    }

    return app.exec();
}

#include "AdminPanelQt.moc"
