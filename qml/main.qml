import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import Figure 1.0

ApplicationWindow {
    title: qsTr("Chess")
    visible: true
    width: 800
    height: 600
    
    property int squareSize: 70
    property int turnType: Figure.FIGURE_TYPE_WHITE

    function getImgPath(type, piece)
    {
        var imgPath = ""

        switch (type) {
        case Figure.FIGURE_TYPE_BLACK: imgPath += "/images/black"; break;
        case Figure.FIGURE_TYPE_WHITE:
        default:
            imgPath += "/images/white"; break;
        }

        switch (piece) {
        case Figure.FIGURE_PIECE_KING: imgPath += "_king.svg"; break;
        case Figure.FIGURE_PIECE_QUEEN: imgPath += "_queen.svg"; break;
        case Figure.FIGURE_PIECE_ROOK: imgPath += "_rook.svg"; break;
        case Figure.FIGURE_PIECE_KNIGHT: imgPath += "_knight.svg"; break;
        case Figure.FIGURE_PIECE_BISHOP: imgPath += "_bishop.svg"; break;
        case Figure.FIGURE_PIECE_PAWN:
        default:
            imgPath += "_pawn.svg"; break;
        }

        return imgPath
    }

    Item {
        id: gameBoard
        x: 0
        y: 0
        width : board.size * squareSize
        height: board.size * squareSize

        property bool figureChosen: false

        Image {
            source: "/images/chess_board.jpg"
            height: gameBoard.height
            width: gameBoard.width
        }


        Repeater {
            model: board

            FigureTile {
                iconPath: getImgPath(type, piece)
                x: squareSize * positionX
                y: squareSize * positionY
                enabled: turnType === type
                property int startX: 0
                property int startY: 0

                onPressed: {
                    gameBoard.figureChosen = true;
                    startX = x;
                    startY = y;
                    console.log("Pick on", x, y)
                    //logic.calculateAvailableMoves(startX, startY)
                }

                onReleased: {
                    gameBoard.figureChosen = false;
                    if (board.move(startX, startY, x, y)) {
                        turnType = turnType === Figure.FIGURE_TYPE_WHITE ? Figure.FIGURE_TYPE_BLACK : Figure.FIGURE_TYPE_WHITE;
                    }
                }
            }
        }

        Repeater {
            model: logic.availableMoves

            Rectangle {
                visible: hint.checked && gameBoard.figureChosen
                height: squareSize
                width : squareSize
                color: "red"
                opacity: 0.3
                x: squareSize * modelData.x
                y: squareSize * modelData.y
            }
        }
    }

    Button {
        id: startButton
        anchors.left: gameBoard.right
        anchors.right: parent.right
        anchors.leftMargin: 10
        anchors.rightMargin: 10

        text: "Clear"

        onClicked: {
            board.clear();
        }
    }

    CheckBox {
        id: hint
        anchors {
            top: startButton.bottom
            topMargin: 10
            left: startButton.left
        }

        text: qsTr("Hint")
        checked: true
    }

    Button {
        id: prevButton
        anchors {
            bottom: nextButton.top
            bottomMargin: 10
            left: startButton.left
        }

        text: "Previous"

        onClicked: {
            board.previous();
        }
    }

    Button {
        id: nextButton
        anchors {
            bottom: parent.bottom
            bottomMargin: 10
            left: startButton.left
        }

        text: "Next"

        onClicked: {
            board.next();
        }
    }
}
