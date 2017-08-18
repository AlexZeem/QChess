#include <QStringList>
#include "history.h"

#include <QDebug>

struct History::Impl
{
    QString xToFile(int x);
    int fileToX(QString file);
    QString yToRank(int y);
    int rankToY(QString rank);
    QString pieceToName(int piece);

    QStringList history;
    int historyPosition;
};

QString History::Impl::xToFile(int x)
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

int History::Impl::fileToX(QString file)
{
    qDebug() << file.toInt();
//    switch (file) {
//    case "a": return 0;
//    case "b": return 1;
//    case "c": return 2;
//    case "d": return 3;
//    case "e": return 4;
//    case "f": return 5;
//    case "g": return 6;
//    case "h": return 7;
//    default: return -1;
//    }

    return -1;
}

QString History::Impl::yToRank(int y)
{
    return QString::number(y + 1);
}

int History::Impl::rankToY(QString rank)
{
    return rank.toInt() - 1;
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

History::History(QObject *parent)
    : QObject(parent)
    , impl(new Impl())
{ }

History::~History()
{ }

void History::update(int fromX, int fromY, int toX, int toY, int piece)
{
    impl->history << impl->pieceToName(piece) + impl->xToFile(fromX) + impl->yToRank(fromY) + "-" + impl->xToFile(toX) + impl->yToRank(toY);
    qDebug() << impl->history;
    ++impl->historyPosition;
}

void History::previous()
{
    if (impl->history.size() < 2 || impl->historyPosition - 1 < 0) {
        return;
    }

    qDebug() << "prev: " << impl->history.at(--impl->historyPosition);
}

void History::next()
{
    if (impl->historyPosition + 1 > impl->history.size() - 1) {
        return;
    }

    qDebug() << "next: " << impl->history.at(++impl->historyPosition);
}
