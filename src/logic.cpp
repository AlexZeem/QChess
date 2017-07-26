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

    void initPosition();
    int findByPosition(int x, int y);
    bool isAvailable(int x, int y, FigureType type);
    void calculateAvailableMoves(int index);
    void promote(int index, FigurePiece toPiece);
};

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

bool Logic::Impl::isAvailable(int x, int y, FigureType type)
{
    if (x >= BOARD_SIZE || x < 0 || y >= BOARD_SIZE || y < 0) {
        return false;
    }

    if (boardState[QPair<int, int>(x, y)] != type) {
        return true;
    }

    return false;
}

void Logic::Impl::calculateAvailableMoves(int index)
{
    Figure figure = figures[index];
    availableMoves.clear();

    switch (figure.piece) {
    case FIGURE_KING:
        if (isAvailable(figure.x, figure.y + 1, figure.type)) {
            availableMoves << QPair<int, int>(figure.x, figure.y + 1);
        }

        if (isAvailable(figure.x + 1, figure.y + 1, figure.type)) {
            availableMoves << QPair<int, int>(figure.x + 1, figure.y + 1);
        }

        if (isAvailable(figure.x + 1, figure.y, figure.type)) {
            availableMoves << QPair<int, int>(figure.x + 1, figure.y);
        }

        if (isAvailable(figure.x + 1, figure.y - 1, figure.type)) {
            availableMoves << QPair<int, int>(figure.x + 1, figure.y - 1);
        }

        if (isAvailable(figure.x, figure.y - 1, figure.type)) {
            availableMoves << QPair<int, int>(figure.x, figure.y - 1);
        }

        if (isAvailable(figure.x - 1, figure.y - 1, figure.type)) {
            availableMoves << QPair<int, int>(figure.x - 1, figure.y - 1);
        }

        if (isAvailable(figure.x - 1, figure.y, figure.type)) {
            availableMoves << QPair<int, int>(figure.x - 1, figure.y);
        }

        if (isAvailable(figure.x - 1, figure.y + 1, figure.type)) {
            availableMoves << QPair<int, int>(figure.x - 1, figure.y + 1);
        }

        break;
    case FIGURE_QUEEN:
        break;
    case FIGURE_ROOK:
        break;
    case FIGURE_KNIGHT:
        break;
    case FIGURE_BISHOP:
        break;
    case FIGURE_PAWN:
        if (isAvailable(figure.x, figure.y - 1 * (figure.type == FIGURE_WHITE ? 1 : -1), figure.type)) {
            availableMoves << QPair<int, int>(figure.x, figure.y - 1 * (figure.type == FIGURE_WHITE ? 1 : -1));
        }

        //first move
        if (figure.y == (figure.type == FIGURE_WHITE ? 6 : 1) && !availableMoves.isEmpty()
            && isAvailable(figure.x, figure.y - 2 * (figure.type == FIGURE_WHITE ? 1 : -1), figure.type)) {
            availableMoves << QPair<int, int>(figure.x, figure.y - 2 * (figure.type == FIGURE_WHITE ? 1 : -1));
        }

        if (boardState[QPair<int, int>(figure.x - 1, figure.y - 1 * (figure.type == FIGURE_WHITE ? 1 : -1))] != FIGURE_NONE
                && isAvailable(figure.x - 1, figure.y - 1 * (figure.type == FIGURE_WHITE ? 1 : -1), figure.type)) {
            availableMoves << QPair<int, int>(figure.x - 1, figure.y - 1 * (figure.type == FIGURE_WHITE ? 1 : -1));
        }

        if (boardState[QPair<int, int>(figure.x + 1, figure.y - 1 * (figure.type == FIGURE_WHITE ? 1 : -1))] != FIGURE_NONE
                && isAvailable(figure.x + 1, figure.y - 1 * (figure.type == FIGURE_WHITE ? 1 : -1), figure.type)) {
            availableMoves << QPair<int, int>(figure.x + 1, figure.y - 1 * (figure.type == FIGURE_WHITE ? 1 : -1));
        }
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
    qDebug() << "calculateAvailableMoves" << index << fromX << fromY;
    if (index < 0) return;

    impl->calculateAvailableMoves(index);
    qDebug() << "calculateAvailableMoves" << index << fromX << fromY << "(" << impl->availableMoves.size() << ")";
    emit availableMovesChanged();
}
