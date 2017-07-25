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

    function getImgPath(type, piece)
    {
        var imgPath = ""

        switch (type) {
        case Figure.FIGURE_BLACK: imgPath += "/images/black"; break;
        case Figure.FIGURE_WHITE:
        default:
            imgPath += "/images/white"; break;
        }

        switch (piece) {
        case Figure.FIGURE_KING: imgPath += "_king.svg"; break;
        case Figure.FIGURE_QUEEN: imgPath += "_queen.svg"; break;
        case Figure.FIGURE_ROOK: imgPath += "_rook.svg"; break;
        case Figure.FIGURE_KNIGHT: imgPath += "_knight.svg"; break;
        case Figure.FIGURE_BISHOP: imgPath += "_bishop.svg"; break;
        case Figure.FIGURE_PAWN:
        default:
            imgPath += "_pawn.svg"; break;
        }

        return imgPath
    }

    Item {
      id: gameBoard
      x: 0
      y: 0
      width : logic.boardSize * squareSize
      height: logic.boardSize * squareSize
      
      Image {
        source: "/images/chess_board.jpg"
        height: gameBoard.height
        width: gameBoard.width
      }
      
      Repeater {
        model: logic

        Image {
          height: squareSize
          width : squareSize

          x: squareSize * positionX
          y: squareSize * positionY

          source: getImgPath(type, piece)
          
          MouseArea {
            anchors.fill: parent
            drag.target: parent

            property int startX: 0
            property int startY: 0

            onPressed: {
              startX = parent.x;
              startY = parent.y;
            }

            onReleased: {
              var fromX = startX / squareSize;
              var fromY = startY / squareSize;
              var toX   = (parent.x + mouseX) / squareSize;
              var toY   = (parent.y + mouseY) / squareSize;

              if (!logic.move(fromX, fromY, toX, toY)) {
                parent.x = startX;
                parent.y = startY;
              }
            }
          }
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
        logic.clear();
      }
    }
}
