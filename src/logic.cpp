#include "logic.h"
#include <QList>
#include <QByteArray>
#include <QHash>
#include <QPair>
#include <iostream>

#include <QDebug>

struct Figure
{
    Logic::FigureType type;
    Logic::FigurePiece piece;
    int x;
    int y;
};


struct Logic::Impl
{
    QList<Figure> figures;
    QList<QPair<int, int>> availableMoves;
    QMap<QPair<int, int>, FigureType> boardState;

    int findByPosition(int x, int y);
    void initPosition();
    void checkForwardBackward(int x, int y, FigureType type);
    void checkFromLeftToRight(int x, int y, FigureType type);
    void checkDiagonals(int x, int y, FigureType type);
    void checkLShape(int x, int y, FigureType type);
    bool isEnemyInPath(int x, int y, FigureType type, FigurePiece piece1, FigurePiece piece2);
    bool isInCheck(int x, int y, FigureType type);
    void checkKingMove(int x, int y, FigureType type);
    void checkPawnMove(int x, int y, FigureType type, bool isFirstMove = false);
    void calculateAvailableMoves(int index);
    void promote(int index, FigurePiece toPiece);
};

int Logic::Impl::findByPosition(int x, int y)
{
    for (int i(0); i<figures.size(); ++i) {
        if (figures[i].x != x || figures[i].y != y ) {
            continue;
        }
        return i;
    }
    return -1;
}

void Logic::Impl::initPosition()
{
    figures << Figure { FIGURE_WHITE, FIGURE_ROOK, 0, 7 };
    boardState[QPair<int, int>(0, 7)] = FIGURE_WHITE;
    figures << Figure { FIGURE_WHITE, FIGURE_KNIGHT, 1, 7 };
    boardState[QPair<int, int>(1, 7)] = FIGURE_WHITE;
    figures << Figure { FIGURE_WHITE, FIGURE_BISHOP, 2, 7 };
    boardState[QPair<int, int>(2, 7)] = FIGURE_WHITE;
    figures << Figure { FIGURE_WHITE, FIGURE_QUEEN, 3, 7 };
    boardState[QPair<int, int>(3, 7)] = FIGURE_WHITE;
    figures << Figure { FIGURE_WHITE, FIGURE_KING, 4, 7 };
    boardState[QPair<int, int>(4, 7)] = FIGURE_WHITE;
    figures << Figure { FIGURE_WHITE, FIGURE_BISHOP, 5, 7 };
    boardState[QPair<int, int>(5, 7)] = FIGURE_WHITE;
    figures << Figure { FIGURE_WHITE, FIGURE_KNIGHT, 6, 7 };
    boardState[QPair<int, int>(6, 7)] = FIGURE_WHITE;
    figures << Figure { FIGURE_WHITE, FIGURE_ROOK, 7, 7 };
    boardState[QPair<int, int>(7, 7)] = FIGURE_WHITE;

    for (int i = 0; i < 8; ++i) {
        figures << Figure { FIGURE_WHITE, FIGURE_PAWN, i, 6 };
        boardState[QPair<int, int>(i, 6)] = FIGURE_WHITE;
    }

    figures << Figure { FIGURE_BLACK, FIGURE_ROOK, 0, 0 };
    boardState[QPair<int, int>(0, 0)] = FIGURE_BLACK;
    figures << Figure { FIGURE_BLACK, FIGURE_KNIGHT, 1, 0 };
    boardState[QPair<int, int>(1, 0)] = FIGURE_BLACK;
    figures << Figure { FIGURE_BLACK, FIGURE_BISHOP, 2, 0 };
    boardState[QPair<int, int>(2, 0)] = FIGURE_BLACK;
    figures << Figure { FIGURE_BLACK, FIGURE_QUEEN, 3, 0 };
    boardState[QPair<int, int>(3, 0)] = FIGURE_BLACK;
    figures << Figure { FIGURE_BLACK, FIGURE_KING, 4, 0 };
    boardState[QPair<int, int>(4, 0)] = FIGURE_BLACK;
    figures << Figure { FIGURE_BLACK, FIGURE_BISHOP, 5, 0 };
    boardState[QPair<int, int>(5, 0)] = FIGURE_BLACK;
    figures << Figure { FIGURE_BLACK, FIGURE_KNIGHT, 6, 0 };
    boardState[QPair<int, int>(6, 0)] = FIGURE_BLACK;
    figures << Figure { FIGURE_BLACK, FIGURE_ROOK, 7, 0 };
    boardState[QPair<int, int>(7, 0)] = FIGURE_BLACK;

    for (int i = 0; i < 8; ++i) {
        figures << Figure { FIGURE_BLACK, FIGURE_PAWN, i, 1 };
        boardState[QPair<int, int>(i, 1)] = FIGURE_BLACK;
    }
}

void Logic::Impl::checkForwardBackward(int x, int y, FigureType type)
{
    for (int i = 1; y - i >= 0; ++i) {
        if (boardState[QPair<int, int>(x, y - i)] != type) {
            availableMoves << QPair<int, int>(x, y - i);
        }

        if (boardState[QPair<int, int>(x, y - i)] != FIGURE_NONE) {
            break;
        }
    }

    for (int i = 1; y + i < BOARD_SIZE; ++i) {
        if (boardState[QPair<int, int>(x, y + i)] != type) {
            availableMoves << QPair<int, int>(x, y + i);
        }

        if (boardState[QPair<int, int>(x, y + i)] != FIGURE_NONE) {
            break;
        }
    }
}

void Logic::Impl::checkFromLeftToRight(int x, int y, FigureType type)
{
    for (int i = 1; x - i >= 0; ++i) {
        if (boardState[QPair<int, int>(x - i, y)] != type) {
            availableMoves << QPair<int, int>(x - i, y);
        }

        if (boardState[QPair<int, int>(x - i, y)] != FIGURE_NONE) {
            break;
        }
    }

    for (int i = 1; x + i < BOARD_SIZE; ++i) {
        if (boardState[QPair<int, int>(x + i, y)] != type) {
            availableMoves << QPair<int, int>(x + i, y);
        }

        if (boardState[QPair<int, int>(x + i, y)] != FIGURE_NONE) {
            break;
        }
    }
}

void Logic::Impl::checkDiagonals(int x, int y, FigureType type)
{
    for (int i = 1; x + i < BOARD_SIZE && y - i >= 0; ++i) {
        if (boardState[QPair<int, int>(x + i, y - i)] != type) {
            availableMoves << QPair<int, int>(x + i, y - i);
        }

        if (boardState[QPair<int, int>(x + i, y - i)] != FIGURE_NONE) {
            break;
        }
    }

    for (int i = 1; x - i >= 0 && y + i < BOARD_SIZE; ++i) {
        if (boardState[QPair<int, int>(x - i, y + i)] != type) {
            availableMoves << QPair<int, int>(x - i, y + i);
        }

        if (boardState[QPair<int, int>(x - i, y + i)] != FIGURE_NONE) {
            break;
        }
    }

    for (int i = 1; x - i >= 0 && y - i >= 0; ++i) {
        if (boardState[QPair<int, int>(x - i, y - i)] != type) {
            availableMoves << QPair<int, int>(x - i, y - i);
        }

        if (boardState[QPair<int, int>(x - i, y - i)] != FIGURE_NONE) {
            break;
        }
    }

    for (int i = 1; x + i < BOARD_SIZE && y + i < BOARD_SIZE; ++i) {
        if (boardState[QPair<int, int>(x + i, y + i)] != type) {
            availableMoves << QPair<int, int>(x + i, y + i);
        }

        if (boardState[QPair<int, int>(x + i, y + i)] != FIGURE_NONE) {
            break;
        }
    }
}

void Logic::Impl::checkLShape(int x, int y, FigureType type)
{
    if (x + 1 < BOARD_SIZE && y - 2 >= 0 && boardState[QPair<int, int>(x + 1, y - 2)] != type) {
        availableMoves << QPair<int, int>(x + 1, y - 2);
    }

    if (x - 1 >= 0 && y - 2 >= 0 && boardState[QPair<int, int>(x - 1, y - 2)] != type) {
        availableMoves << QPair<int, int>(x - 1, y - 2);
    }

    if (x + 1 < BOARD_SIZE && y + 2 < BOARD_SIZE && boardState[QPair<int, int>(x + 1, y + 2)] != type) {
        availableMoves << QPair<int, int>(x + 1, y + 2);
    }

    if (x - 1 >= 0 && y + 2 < BOARD_SIZE && boardState[QPair<int, int>(x - 1, y + 2)] != type) {
        availableMoves << QPair<int, int>(x - 1, y + 2);
    }

    if (x + 2 < BOARD_SIZE && y - 1 >= 0 && boardState[QPair<int, int>(x + 2, y - 1)] != type) {
        availableMoves << QPair<int, int>(x + 2, y - 1);
    }

    if (x + 2 < BOARD_SIZE && y + 1 < BOARD_SIZE && boardState[QPair<int, int>(x + 2, y + 1)] != type) {
        availableMoves << QPair<int, int>(x + 2, y + 1);
    }

    if (x - 2 >= 0 && y - 1 >= 0 && boardState[QPair<int, int>(x - 2, y - 1)] != type) {
        availableMoves << QPair<int, int>(x - 2, y - 1);
    }

    if (x - 2 >= 0 && y + 1 < BOARD_SIZE && boardState[QPair<int, int>(x - 2, y + 1)] != type) {
        availableMoves << QPair<int, int>(x - 2, y + 1);
    }
}

bool Logic::Impl::isEnemyInPath(int x, int y, FigureType type, FigurePiece piece1, FigurePiece piece2)
{
    if (boardState[QPair<int, int>(x, y)] != type) {
        int index = findByPosition(x, y);
        if (index >= 0 && (figures[index].piece == piece1 || figures[index].piece == piece2)) {
            return true;
        }
    }

    return false;
}

bool Logic::Impl::isInCheck(int x, int y, FigureType type) {
    for (int i = 1; y - i >= 0; ++i) {
        if (boardState[QPair<int, int>(x, y - i)] == FIGURE_NONE) {
            continue;
        }
        if (isEnemyInPath(x, y - i, type, FIGURE_QUEEN, FIGURE_ROOK)) {
            return true;
        }
        break;
    } // forward

    for (int i = 1; y + i < BOARD_SIZE; ++i) {
        if (boardState[QPair<int, int>(x, y + i)] == FIGURE_NONE) {
            continue;
        }
        if (isEnemyInPath(x, y + i, type, FIGURE_QUEEN, FIGURE_ROOK)) {
            return true;
        }
        break;
    } // backward

    for (int i = 1; x - i >= 0; ++i) {
        if (boardState[QPair<int, int>(x - i, y)] == FIGURE_NONE) {
            continue;
        }
        if (isEnemyInPath(x - i, y, type, FIGURE_QUEEN, FIGURE_ROOK)) {
            return true;
        }
        break;
    } // left

    for (int i = 1; x + i < BOARD_SIZE; ++i) {
        if (boardState[QPair<int, int>(x + i, y)] == FIGURE_NONE) {
            continue;
        }
        if (isEnemyInPath(x + i, y, type, FIGURE_QUEEN, FIGURE_ROOK)) {
            return true;
        }
        break;
    } // right

    for (int i = 1; x + i < BOARD_SIZE && y - i >= 0; ++i) {
        if (boardState[QPair<int, int>(x + i, y - i)] == FIGURE_NONE) {
            continue;
        }
        if (isEnemyInPath(x + i, y - i, type, FIGURE_QUEEN, FIGURE_BISHOP)) {
            return true;
        }
        break;
    } // to top right

    for (int i = 1; x - i >= 0 && y + i < BOARD_SIZE; ++i) {
        if (boardState[QPair<int, int>(x - i, y + i)] == FIGURE_NONE) {
            continue;
        }
        if (isEnemyInPath(x - i, y + i, type, FIGURE_QUEEN, FIGURE_BISHOP)) {
            return true;
        }
        break;
    } // to bottom left

    for (int i = 1; x - i >= 0 && y - i >= 0; ++i) {
        if (boardState[QPair<int, int>(x - i, y - i)] == FIGURE_NONE) {
            continue;
        }
        if (isEnemyInPath(x - i, y - i, type, FIGURE_QUEEN, FIGURE_BISHOP)) {
            return true;
        }
        break;
    } // to top left

    for (int i = 1; x + i < BOARD_SIZE && y + i < BOARD_SIZE; ++i) {
        if (boardState[QPair<int, int>(x + i, y + i)] == FIGURE_NONE) {
            continue;
        }
        if (isEnemyInPath(x + i, y + i, type, FIGURE_QUEEN, FIGURE_BISHOP)) {
            return true;
        }
        break;
    } // to bottom right

    if (x + 1 < BOARD_SIZE && y - 2 >= 0 && boardState[QPair<int, int>(x + 1, y - 2)] != FIGURE_NONE) {
        if (isEnemyInPath(x + 1, y - 2, type, FIGURE_KNIGHT, FIGURE_KNIGHT)) {
            return true;
        }
    }

    if (x - 1 >= 0 && y - 2 >= 0 && boardState[QPair<int, int>(x - 1, y - 2)] != FIGURE_NONE) {
        if (isEnemyInPath(x - 1, y - 2, type, FIGURE_KNIGHT, FIGURE_KNIGHT)) {
            return true;
        }
    }

    if (x + 1 < BOARD_SIZE && y + 2 < BOARD_SIZE && boardState[QPair<int, int>(x + 1, y + 2)] != FIGURE_NONE) {
        if (isEnemyInPath(x + 1, y + 2, type, FIGURE_KNIGHT, FIGURE_KNIGHT)) {
            return true;
        }
    }

    if (x - 1 >= 0 && y + 2 < BOARD_SIZE && boardState[QPair<int, int>(x - 1, y + 2)] != FIGURE_NONE) {
        if (isEnemyInPath(x - 1, y + 2, type, FIGURE_KNIGHT, FIGURE_KNIGHT)) {
            return true;
        }
    }

    if (x + 2 < BOARD_SIZE && y - 1 >= 0 && boardState[QPair<int, int>(x + 2, y - 1)] != FIGURE_NONE) {
        if (isEnemyInPath(x + 2, y - 1, type, FIGURE_KNIGHT, FIGURE_KNIGHT)) {
            return true;
        }
    }

    if (x + 2 < BOARD_SIZE && y + 1 < BOARD_SIZE && boardState[QPair<int, int>(x + 2, y + 1)] != FIGURE_NONE) {
        if (isEnemyInPath(x + 2, y + 1, type, FIGURE_KNIGHT, FIGURE_KNIGHT)) {
            return true;
        }
    }

    if (x - 2 >= 0 && y - 1 >= 0 && boardState[QPair<int, int>(x - 2, y - 1)] != FIGURE_NONE) {
        if (isEnemyInPath(x - 2, y - 1, type, FIGURE_KNIGHT, FIGURE_KNIGHT)) {
            return true;
        }
    }

    if (x - 2 >= 0 && y + 1 < BOARD_SIZE && boardState[QPair<int, int>(x - 2, y + 1)] != FIGURE_NONE) {
        if (isEnemyInPath(x - 2, y + 1, type, FIGURE_KNIGHT, FIGURE_KNIGHT)) {
            return true;
        }
    }

    if (x - 1 >= 0 && y - 1 >= 0 && boardState[QPair<int, int>(x - 1, y - 1)] != FIGURE_NONE) {
        if (isEnemyInPath(x - 1, y - 1, type, FIGURE_PAWN, FIGURE_PAWN)) {
            return true;
        }
    }

    if (x + 1 >= 0 && y - 1 >= 0 && boardState[QPair<int, int>(x + 1, y - 1)] != FIGURE_NONE) {
        if (isEnemyInPath(x + 1, y - 1, type, FIGURE_PAWN, FIGURE_PAWN)) {
            return true;
        }
    }

    if (x - 1 >= 0 && y + 1 < BOARD_SIZE && boardState[QPair<int, int>(x - 1, y + 1)] != FIGURE_NONE) {
        if (isEnemyInPath(x - 1, y + 1, type, FIGURE_PAWN, FIGURE_PAWN)) {
            return true;
        }
    }

    if (x + 1 >= 0 && y + 1 < BOARD_SIZE && boardState[QPair<int, int>(x + 1, y + 1)] != FIGURE_NONE) {
        if (isEnemyInPath(x + 1, y + 1, type, FIGURE_PAWN, FIGURE_PAWN)) {
            return true;
        }
    }

    return false;
}

void Logic::Impl::checkKingMove(int x, int y, FigureType type)
{
    if (y - 1 >= 0 && boardState[QPair<int, int>(x, y - 1)] != type && !isInCheck(x, y - 1, type)) {
        availableMoves << QPair<int, int>(x, y - 1);
    }

    if (y + 1 < BOARD_SIZE && boardState[QPair<int, int>(x, y + 1)] != type) {
        availableMoves << QPair<int, int>(x, y + 1);
    }

    if (x - 1 >= 0 && boardState[QPair<int, int>(x - 1, y)] != type) {
        availableMoves << QPair<int, int>(x - 1, y);
    }

    if (x + 1 < BOARD_SIZE && boardState[QPair<int, int>(x + 1, y)] != type) {
        availableMoves << QPair<int, int>(x + 1, y);
    }

    if (x + 1 < BOARD_SIZE && y - 1 >= 0 && boardState[QPair<int, int>(x + 1, y - 1)] != type) {
        availableMoves << QPair<int, int>(x + 1, y - 1);
    }

    if (x + 1 < BOARD_SIZE && y + 1 < BOARD_SIZE && boardState[QPair<int, int>(x + 1, y + 1)] != type) {
        availableMoves << QPair<int, int>(x + 1, y + 1);
    }

    if (x - 1 >= 0 && y + 1 < BOARD_SIZE && boardState[QPair<int, int>(x - 1, y + 1)] != type) {
        availableMoves << QPair<int, int>(x - 1, y + 1);
    }

    if (x - 1 >= 0 && y - 1 >= 0 && boardState[QPair<int, int>(x - 1, y - 1)] != type) {
        availableMoves << QPair<int, int>(x - 1, y - 1);
    }
}

void Logic::Impl::checkPawnMove(int x, int y, FigureType type, bool isFirstMove)
{
    int newY = y - 1 * (type == FIGURE_WHITE ? 1 : -1);
    if (newY >= 0 && newY < BOARD_SIZE && boardState[QPair<int, int>(x, newY)] == FIGURE_NONE) {
        availableMoves << QPair<int, int>(x, newY);
    }

    if (x - 1 >= 0 && newY >= 0 && newY < BOARD_SIZE
            && boardState[QPair<int, int>(x - 1, newY)] != FIGURE_NONE
            && boardState[QPair<int, int>(x - 1, newY)] != type) {
        availableMoves << QPair<int, int>(x - 1, newY);
    }

    if (x + 1 < BOARD_SIZE && newY >= 0 && newY < BOARD_SIZE
            && boardState[QPair<int, int>(x + 1, newY)] != FIGURE_NONE
            && boardState[QPair<int, int>(x + 1, newY)] != type) {
        availableMoves << QPair<int, int>(x + 1, newY);
    }

    newY = y - 2 * (type == FIGURE_WHITE ? 1 : -1);
    if (isFirstMove && !availableMoves.isEmpty()
            && newY >= 0 && newY < BOARD_SIZE && boardState[QPair<int, int>(x, newY)] == FIGURE_NONE) {
        availableMoves << QPair<int, int>(x, newY);
    }
}

void Logic::Impl::calculateAvailableMoves(int index)
{
    Figure figure = figures[index];
    availableMoves.clear();

    switch (figure.piece) {
    case FIGURE_KING:
        checkKingMove(figure.x, figure.y, figure.type);
        break;
    case FIGURE_QUEEN:
        checkForwardBackward(figure.x, figure.y, figure.type);
        checkFromLeftToRight(figure.x, figure.y, figure.type);
        checkDiagonals(figure.x, figure.y, figure.type);
        break;
    case FIGURE_ROOK:
        checkForwardBackward(figure.x, figure.y, figure.type);
        checkFromLeftToRight(figure.x, figure.y, figure.type);
        break;
    case FIGURE_KNIGHT:
        checkLShape(figure.x, figure.y, figure.type);
        break;
    case FIGURE_BISHOP:
        checkDiagonals(figure.x, figure.y, figure.type);
        break;
    case FIGURE_PAWN:
        checkPawnMove(figure.x, figure.y, figure.type, figure.y == (figure.type == FIGURE_WHITE ? 6 : 1));
        break;
    default:
        break;
    }
}

void Logic::Impl::promote(int index, FigurePiece toPiece)
{
    if (figures[index].type == FIGURE_WHITE ? figures[index].y == 0 : figures[index].y == 7) {
        figures[index].piece = toPiece;
    }
}

Logic::Logic(QObject *parent)
    : QAbstractListModel(parent)
    , impl(new Impl())
{
    impl->initPosition();
}

Logic::~Logic()
{}

int Logic::boardSize() const
{
    return BOARD_SIZE;
}

QVariantList Logic::availableMoves()
{
    QVariantList result;

    for (const auto i : impl->availableMoves)
    {
        QVariantMap cell;
        cell["x"] = i.first;
        cell["y"] = i.second;
        result << cell;
    }

    return result;
}

int Logic::rowCount(const QModelIndex & ) const
{
    return impl->figures.size();
}

QHash<int, QByteArray> Logic::roleNames() const
{
    QHash<int, QByteArray> names;
    names.insert(Roles::Type      , "type");
    names.insert(Roles::Piece     , "piece");
    names.insert(Roles::PositionX , "positionX");
    names.insert(Roles::PositionY , "positionY");
    return names;
}

QVariant Logic::data(const QModelIndex & modelIndex, int role) const
{
    if (!modelIndex.isValid()) {
        return QVariant();
    }
    
    int index = static_cast<int>(modelIndex.row());

    if (index >= impl->figures.size() || index < 0) {
        return QVariant();
    }

    Figure & figure = impl->figures[index];
    
    switch (role) {
    case Roles::Type     : return figure.type;
    case Roles::Piece    : return figure.piece;
    case Roles::PositionX: return figure.x;
    case Roles::PositionY: return figure.y;
    }
    return QVariant();
}

void Logic::clear() {
    beginResetModel();
    impl->figures.clear();
    impl->boardState.clear();
    impl->initPosition();
    endResetModel();
}

bool Logic::move(int fromX, int fromY, int toX, int toY)
{
    int index = impl->findByPosition(fromX, fromY);
    qDebug() << "move" << index << fromX << fromY << toX << toY;

    if (index < 0 || toX < 0 || toX >= BOARD_SIZE || toY < 0 || toY >= BOARD_SIZE || impl->availableMoves.isEmpty()) {
        return false;
    }

    bool available = false;
    for (const auto i : impl->availableMoves) {
        if (i.first == toX && i.second == toY) {
            available = true;
            break;
        }
    }

    if (!available) {
        qDebug() << "move not available!";
        return false;
    }

    int enemyIndex = impl->findByPosition(toX, toY);
    if (available && enemyIndex >= 0) {
        qDebug() << "atack!";
        impl->figures.removeAt(enemyIndex);
        beginRemoveRows(QModelIndex(), enemyIndex, enemyIndex);
        endRemoveRows();
        if (enemyIndex < index) {
            --index;
        }
    }

    impl->boardState.remove(QPair<int, int>(fromX, fromY));
    impl->figures[index].x = toX;
    impl->figures[index].y = toY;
    impl->boardState[QPair<int, int>(toX, toY)] = impl->figures[index].type;

    if (impl->figures[index].piece == FIGURE_PAWN) {
        impl->promote(index, FIGURE_QUEEN);
    }

    QModelIndex topLeft = createIndex(index, 0);
    QModelIndex bottomRight = createIndex(index, 0);
    emit dataChanged(topLeft, bottomRight);
    return true;
}

void Logic::calculateAvailableMoves(int fromX, int fromY)
{
    int index = impl->findByPosition(fromX, fromY);
    if (index < 0) {
        return;
    }

    impl->calculateAvailableMoves(index);
    qDebug() << "calculateAvailableMoves" << index << fromX << fromY << "(" << impl->availableMoves.size() << ")";
    emit availableMovesChanged();
}
