#include <QStringList>
#include "history.h"

#include <QDebug>

struct History::Impl
{
    QStringList history;

    QString xToFiles(int x);
    QString yToRank(int y);
    QString pieceToName(int piece);

    void update(int fromX, int fromY, int toX, int toY, int piece, bool nextTurn);
};

QString History::Impl::xToFiles(int x)
{
    switch (x) {
    case 0: return "a";
    case 1: return "b";
    case 2: return "c";
    case 3: return "d";
    case 4: return "e";
    case 5: return "f";
    case 6: return "g";
    case 7: return "h";
    default: return "";
    }

    return "";
}

QString History::Impl::yToRank(int y)
{
    return QString::number(y + 1);
}

QString History::Impl::pieceToName(int piece)
{
    switch (piece) {
    case 1: return "K";
    case 2: return "Q";
    case 3: return "R";
    case 4: return "N";
    case 5: return "B";
    case 6:
    default: return "";
    }

    return "";
}

void History::Impl::update(int fromX, int fromY, int toX, int toY, int piece, bool nextTurn)
{
    QString turn = nextTurn || history.isEmpty() ? "" : history.last() + " ";
    turn += pieceToName(piece) + xToFiles(fromX) + yToRank(fromY) + "-" + xToFiles(toX) + yToRank(toY);

    if (!nextTurn && !history.isEmpty()) {
        history.pop_back();
    }
    history << turn;

    qDebug() << history;
}

History::History(QObject *parent)
    : QObject(parent)
    , impl(new Impl())
{ }

History::~History()
{ }

void History::update(int fromX, int fromY, int toX, int toY, int piece, bool nextTurn)
{
    impl->update(fromX, fromY, toX, toY, piece, nextTurn);
}
