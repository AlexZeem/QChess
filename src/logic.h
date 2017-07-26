#pragma once
#include <memory>
#include <QAbstractListModel>
#include <QVariantList>

class Logic: public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(FigureType FigurePiece)

    Q_PROPERTY(int boardSize READ boardSize CONSTANT)
    Q_PROPERTY(QVariantList availableMoves READ availableMoves NOTIFY availableMovesChanged)

public:
    enum FigureType
    {
        FIGURE_NONE = 0,
        FIGURE_WHITE,
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


    int boardSize() const;
    QVariantList availableMoves();

    Q_INVOKABLE void clear();
    Q_INVOKABLE bool move(int fromX, int fromY, int toX, int toY);
    Q_INVOKABLE void calculateAvailableMoves(int fromX, int fromY);

signals:
    void availableMovesChanged();

protected:
    int rowCount(const QModelIndex & parent) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};
