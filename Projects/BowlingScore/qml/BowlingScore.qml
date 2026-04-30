import QtQuick
import QtQuick.Window

Window {
    id: window
    width: 1280
    height: 720
    minimumWidth: 940
    minimumHeight: 560
    visible: true
    title: "BowlingScore"
    color: "#101418"

    property int frameCount: 10
    property int activePlayer: 0
    property int activeFrame: 0
    property int activeRoll: 0
    property string status: "Starting"
    property var players: []

    function emptyFrames() {
        var frames = []
        for (var frame = 0; frame < frameCount; ++frame)
            frames.push({ rolls: ["", "", ""], total: "", gameOver: false })
        return frames
    }

    function addPlayer(name) {
        players = players.concat([{ name: name, frames: emptyFrames() }])
    }

    function recordRoll(playerIndex, frameIndex, rollIndex, value, flagName, pins) {
        if (playerIndex >= players.length || frameIndex >= frameCount)
            return

        var copy = players.slice()
        var player = copy[playerIndex]
        var frames = player.frames.slice()
        var frame = {
            rolls: frames[frameIndex].rolls.slice(),
            total: frames[frameIndex].total,
            gameOver: frames[frameIndex].gameOver
        }
        frame.rolls[Math.min(rollIndex, 2)] = value
        frames[frameIndex] = frame
        copy[playerIndex] = { name: player.name, frames: frames }
        players = copy
    }

    function updateTotal(playerIndex, frameIndex, total, gameOver) {
        if (playerIndex >= players.length || frameIndex >= frameCount)
            return

        var copy = players.slice()
        var player = copy[playerIndex]
        var frames = player.frames.slice()
        var frame = {
            rolls: frames[frameIndex].rolls.slice(),
            total: String(total),
            gameOver: gameOver
        }
        frames[frameIndex] = frame
        copy[playerIndex] = { name: player.name, frames: frames }
        players = copy
    }

    Connections {
        target: scoreBridge

        function onPlayerAdded(name) { window.addPlayer(name) }
        function onRollRecorded(playerIndex, frameIndex, rollIndex, value, flagName, pins) {
            window.recordRoll(playerIndex, frameIndex, rollIndex, value, flagName, pins)
        }
        function onScoreUpdated(playerIndex, frameIndex, total, gameOver) {
            window.updateTotal(playerIndex, frameIndex, total, gameOver)
        }
        function onActiveChanged(playerIndex, frameIndex, rollIndex) {
            activePlayer = playerIndex
            activeFrame = frameIndex
            activeRoll = rollIndex
        }
        function onStatusChanged(message) { status = message }
    }

    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#101418" }
            GradientStop { position: 0.58; color: "#203039" }
            GradientStop { position: 1.0; color: "#2f3d42" }
        }
    }

    Text {
        id: title
        x: 28
        y: 24
        text: "BowlingScore"
        color: "#f8fafc"
        font.pixelSize: 34
        font.bold: true
    }

    Text {
        id: subtitle
        x: 28
        y: 68
        width: parent.width - 320
        text: status
        color: "#b7c4cc"
        font.pixelSize: 15
        elide: Text.ElideRight
    }

    Rectangle {
        id: frameBadge
        x: parent.width - width - 28
        y: 28
        width: 230
        height: 54
        radius: 8
        color: "#d8b45f"

        Text {
            anchors.centerIn: parent
            text: activeFrame < 10 ? "Frame " + (activeFrame + 1) : "Final"
            color: "#15191d"
            font.pixelSize: 22
            font.bold: true
        }
    }

    Rectangle {
        id: board
        x: 28
        y: 112
        width: parent.width - 56
        height: parent.height - 140
        radius: 8
        color: "#ecf0f3"
        border.color: "#c5cdd3"

        readonly property real gap: 4
        readonly property real leftWidth: 168
        readonly property real headerHeight: 38
        readonly property real cellWidth: (width - 28 - leftWidth - gap * frameCount) / frameCount
        readonly property real rowHeight: Math.max(72, (height - 32 - headerHeight - 8 - gap * Math.max(players.length - 1, 0)) / Math.max(players.length, 1))

        Rectangle {
            x: 14
            y: 14
            width: board.leftWidth
            height: board.headerHeight
            radius: 4
            color: "#24333a"

            Text {
                anchors.centerIn: parent
                text: "Player"
                color: "#f8fafc"
                font.pixelSize: 14
                font.bold: true
            }
        }

        Repeater {
            model: frameCount

            Rectangle {
                x: 14 + board.leftWidth + board.gap + index * (board.cellWidth + board.gap)
                y: 14
                width: board.cellWidth
                height: board.headerHeight
                radius: 4
                color: index === activeFrame ? "#d8b45f" : "#24333a"

                Text {
                    anchors.centerIn: parent
                    text: index + 1
                    color: index === activeFrame ? "#15191d" : "#f8fafc"
                    font.pixelSize: 14
                    font.bold: true
                }
            }
        }

        Repeater {
            model: players

            Item {
                id: playerRow
                required property int index
                required property var modelData
                property int playerIndex: index

                x: 14
                y: 14 + board.headerHeight + 8 + index * (board.rowHeight + board.gap)
                width: board.width - 28
                height: board.rowHeight

                Rectangle {
                    x: 0
                    y: 0
                    width: board.leftWidth
                    height: parent.height
                    radius: 6
                    color: playerRow.playerIndex === activePlayer ? "#2f7d73" : "#d7dee3"
                    border.color: playerRow.playerIndex === activePlayer ? "#8be0d4" : "#bdc7ce"

                    Text {
                        anchors.fill: parent
                        anchors.margins: 12
                        text: playerRow.modelData.name
                        color: playerRow.playerIndex === activePlayer ? "#f8fafc" : "#1c252b"
                        font.pixelSize: 18
                        font.bold: true
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Repeater {
                    model: playerRow.modelData.frames

                    Rectangle {
                        id: frameCell
                        required property int index
                        required property var modelData
                        property int frameIndex: index
                        property var frameData: modelData

                        x: board.leftWidth + board.gap + frameIndex * (board.cellWidth + board.gap)
                        y: 0
                        width: board.cellWidth
                        height: playerRow.height
                        radius: 6
                        color: playerRow.playerIndex === activePlayer && frameIndex === activeFrame ? "#fff8e1" : "#ffffff"
                        border.color: frameData.gameOver ? "#2f7d73" : "#bdc7ce"
                        border.width: playerRow.playerIndex === activePlayer && frameIndex === activeFrame ? 2 : 1

                        Repeater {
                            model: frameCell.frameIndex === 9 ? 3 : 2

                            Rectangle {
                                x: 5 + index * ((frameCell.width - 13) / (frameCell.frameIndex === 9 ? 3 : 2) + 3)
                                y: 5
                                width: (frameCell.width - 13) / (frameCell.frameIndex === 9 ? 3 : 2)
                                height: 26
                                radius: 4
                                color: "#eef2f4"
                                border.color: "#d0d8dd"

                                Text {
                                    anchors.centerIn: parent
                                    text: frameCell.frameData.rolls[index]
                                    color: "#152026"
                                    font.pixelSize: 17
                                    font.bold: true
                                }
                            }
                        }

                        Text {
                            x: 5
                            y: 35
                            width: parent.width - 10
                            height: parent.height - 40
                            text: frameCell.frameData.total
                            color: "#15191d"
                            font.pixelSize: 24
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }
        }
    }
}
