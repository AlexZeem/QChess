#include <QList>
#include <QMap>
#include "history.h"
#include "board.h"

#include <QDebug>

enum GlobalConstants {
    BOARD_SIZE = 8
};

enum Roles {
    Type = Qt::UserRole,
    Piece,
    PositionX,
    PositionY,
};

struct Position
{
    int x;
    int y;
};

struct Figure
{
    Board::FigureType type;
    Board::FigurePiece piece;
};

struct Board::Impl
{
    QList<Position> items;
    QMap<QPair<int, int>, Figure> figures;
    History history;

    void initPosition();
    int  findByPosition(int x, int y);
};

void Board::Impl::initPosition()
{
    items << Position { 0, 7 };
    figures[QPair<int, int>(0, 7)] = Figure { FIGURE_TYPE_WHITE, FIGURE_PIECE_ROOK   };
    items << Position { 1, 7 };
    figures[QPair<int, int>(1, 7)] = Figure { FIGURE_TYPE_WHITE, FIGURE_PIECE_KNIGHT };
    items << Position { 2, 7 };
    figures[QPair<int, int>(2, 7)] = Figure { FIGURE_TYPE_WHITE, FIGURE_PIECE_BISHOP };
    items << Position { 3, 7 };
    figures[QPair<int, int>(3, 7)] = Figure { FIGURE_TYPE_WHITE, FIGURE_PIECE_QUEEN  };
    items << Position { 4, 7 };
    figures[QPair<int, int>(4, 7)] = Figure { FIGURE_TYPE_WHITE, FIGURE_PIECE_KING   };
    items << Position { 5, 7 };
    figures[QPair<int, int>(5, 7)] = Figure { FIGURE_TYPE_WHITE, FIGURE_PIECE_BISHOP };
    items << Position { 6, 7 };
    figures[QPair<int, int>(6, 7)] = Figure { FIGURE_TYPE_WHITE, FIGURE_PIECE_KNIGHT };
    items << Position { 7, 7 };
    figures[QPair<int, int>(7, 7)] = Figure { FIGURE_TYPE_WHITE, FIGURE_PIECE_ROOK   };

    for (int i = 0; i < BOARD_SIZE; ++i) {
        items << Position { i, 6 };
        figures[QPair<int, int>(i, 6)] = Figure { FIGURE_TYPE_WHITE, FIGURE_PIECE_PAWN };
    }

    items << Position { 0, 0 };
    figures[QPair<int, int>(0, 0)] = Figure { FIGURE_TYPE_BLACK, FIGURE_PIECE_ROOK   };
    items << Position { 1, 0 };
    figures[QPair<int, int>(1, 0)] = Figure { FIGURE_TYPE_BLACK, FIGURE_PIECE_KNIGHT };
    items << Position { 2, 0 };
    figures[QPair<int, int>(2, 0)] = Figure { FIGURE_TYPE_BLACK, FIGURE_PIECE_BISHOP };
    items << Position { 3, 0 };
    figures[QPair<int, int>(3, 0)] = Figure { FIGURE_TYPE_BLACK, FIGURE_PIECE_QUEEN  };
    items << Position { 4, 0 };
    figures[QPair<int, int>(4, 0)] = Figure { FIGURE_TYPE_BLACK, FIGURE_PIECE_KING   };
    items << Position { 5, 0 };
    figures[QPair<int, int>(5, 0)] = Figure { FIGURE_TYPE_BLACK, FIGURE_PIECE_BISHOP };
    items << Position { 6, 0 };
    figures[QPair<int, int>(6, 0)] = Figure { FIGURE_TYPE_BLACK, FIGURE_PIECE_KNIGHT };
    items << Position { 7, 0 };
    figures[QPair<int, int>(7, 0)] = Figure { FIGURE_TYPE_BLACK, FIGURE_PIECE_ROOK   };

    for (int i = 0; i < BOARD_SIZE; ++i) {
        items << Position { i, 1 };
        figures[QPair<int, int>(i, 1)] = Figure { FIGURE_TYPE_BLACK, FIGURE_PIECE_PAWN };
    }
}

int Board::Impl::findByPosition(int x, int y)
{
    for (int i = 0; i < items.size(); ++i) {
        if (items[i].x != x || items[i].y != y) {
            continue;
        }
        return i;
    }
    return -1;
}

Board::Board(QObject *parent)
    : QAbstractListModel(parent)
    , impl(new Impl())
{
    impl->initPosition();
}

Board::~Board()
{ }

int Board::size() const
{
    return BOARD_SIZE;
}

int Board::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return impl->items.size();
}

QVariant Board::data(const QModelIndex &modelIndex, int role) const
{
    if (!modelIndex.isValid()) {
        return QVariant();
    }

    int index = modelIndex.row();

    if (index >= impl->items.size() || index < 0) {
        return QVariant();
    }

    Position & position = impl->items[index];
    Figure   & figure   = impl->figures[QPair<int, int>(position.x, position.y)];

    switch (role) {
    case Roles::Type     : return figure.type;
    case Roles::Piece    : return figure.piece;
    case Roles::PositionX: return position.x;
    case Roles::PositionY: return position.y;
    }

    return QVariant();
}

QHash<int, QByteArray> Board::roleNames() const
{
    QHash<int, QByteArray> names;
    names.insert(Roles::Type      , "type");
    names.insert(Roles::Piece     , "piece");
    names.insert(Roles::PositionX , "positionX");
    names.insert(Roles::PositionY , "positionY");
    return names;
}

void Board::clear()
{
    beginResetModel();
    impl->figures.clear();
    impl->initPosition();
    endResetModel();
}

bool Board::move(int fromX, int fromY, int toX, int toY)
{
    int index = impl->findByPosition(fromX, fromY);

    if (index < 0 || toX < 0 || toX >= BOARD_SIZE || toY < 0 || toY >= BOARD_SIZE) {
        return false;
    }
    impl->items[index] = Position { toX, toY };

    Figure figure = impl->figures[QPair<int, int>(fromX, fromY)];
    impl->figures.remove(QPair<int, int>(fromX, fromY));
    impl->figures[QPair<int, int>(toX, toY)] = figure;
    //qDebug() << "move [" << cast(fromX) << fromY << "] to [" << cast(toX) << toY << "] type" << figure.type << "piece" << cast(figure.piece);

    impl->history.update(fromX, fromY, toX, toY, figure.piece, figure.type == FIGURE_TYPE_WHITE);

    QModelIndex topLeft = createIndex(index, 0);
    QModelIndex bottomRight = createIndex(index, 0);
    emit dataChanged(topLeft, bottomRight);

    return true;
}
