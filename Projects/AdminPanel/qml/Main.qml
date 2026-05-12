import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    width: 640
    height: 420
    minimumWidth: 480
    minimumHeight: 340
    visible: true
    title: "AdminPanel Qt"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 14

        Label {
            text: "AdminPanel"
            font.pixelSize: 24
            font.bold: true
            Layout.fillWidth: true
        }

        GridLayout {
            columns: 2
            columnSpacing: 12
            rowSpacing: 10
            Layout.fillWidth: true

            Label {
                text: "Host"
                Layout.alignment: Qt.AlignRight
            }

            TextField {
                id: hostField
                text: adminClient.defaultHost
                selectByMouse: true
                Layout.fillWidth: true
            }

            Label {
                text: "Port"
                Layout.alignment: Qt.AlignRight
            }

            SpinBox {
                id: portField
                from: 1
                to: 65535
                value: adminClient.defaultPort
                editable: true
                Layout.fillWidth: true
            }

            Label {
                text: "Players"
                Layout.alignment: Qt.AlignRight
            }

            TextField {
                id: playersField
                text: "Ada Linus Grace"
                selectByMouse: true
                Layout.fillWidth: true
                onAccepted: sendButton.clicked()
            }
        }

        RowLayout {
            Layout.fillWidth: true

            Item {
                Layout.fillWidth: true
            }

            Button {
                id: sendButton
                text: adminClient.busy ? "Sending..." : "Send Players"
                enabled: !adminClient.busy
                onClicked: adminClient.sendPlayers(hostField.text, portField.value, playersField.text)
            }
        }

        Label {
            text: "Status"
            font.bold: true
            Layout.fillWidth: true
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            TextArea {
                text: adminClient.statusLog
                readOnly: true
                wrapMode: TextArea.Wrap
                selectByMouse: true
            }
        }
    }
}
