#pragma once
#include <memory>
#include <QAbstractListModel>

class Board: public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(FigureType FigurePiece)

    Q_PROPERTY(int size READ size CONSTANT)

public:
    enum FigureType
    {
        FIGURE_TYPE_NONE = 0,
        FIGURE_TYPE_WHITE,
        FIGURE_TYPE_BLACK
    };

    enum FigurePiece
    {
        FIGURE_PIECE_NONE = 0,
        FIGURE_PIECE_KING,
        FIGURE_PIECE_QUEEN,
        FIGURE_PIECE_ROOK,
        FIGURE_PIECE_KNIGHT,
        FIGURE_PIECE_BISHOP,
        FIGURE_PIECE_PAWN
    };

public:
    explicit Board(QObject *parent = 0);
    ~Board();

    int size() const;

    Q_INVOKABLE void clear();
    Q_INVOKABLE bool move(int fromX, int fromY, int toX, int toY);

    Q_INVOKABLE void previous();
    Q_INVOKABLE void next();

signals:

protected:
    int rowCount(const QModelIndex & parent) const override;
    QVariant data(const QModelIndex &modelIndex, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};
