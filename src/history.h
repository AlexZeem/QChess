#pragma once
#include <memory>
#include <QObject>

class History : public QObject
{
    Q_OBJECT
public:
    explicit History(QObject *parent = 0);
    ~History();

   void update(int fromX, int fromY, int toX, int toY, int piece, bool nextTurn = true);

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};
