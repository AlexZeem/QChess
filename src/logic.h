#pragma once
#include <memory>
#include <QAbstractListModel>

class Logic: public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(FigureType FigurePiece)

public:
    enum FigureType
    {
        FIGURE_WHITE = 0,
        FIGURE_BLACK
    };

    enum FigurePiece
    {
        FIGURE_KING = 0,
        FIGURE_QUEEN,
        FIGURE_ROOK,
        FIGURE_KNIGHT,
        FIGURE_BISHOP,
        FIGURE_PAWN
    };

    enum GlobalConstants {
        BOARD_SIZE = 8
    };

    enum Roles {
        Type = Qt::UserRole,
        Piece,
        PositionX,
        PositionY,
    };
    
public:
    explicit Logic(QObject *parent = 0);
    ~Logic();

    Q_PROPERTY(int boardSize READ boardSize CONSTANT);
    int boardSize() const;

    Q_INVOKABLE void clear();
    Q_INVOKABLE bool move(int fromX, int fromY, int toX, int toY);

protected:
    int rowCount(const QModelIndex & parent) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};
