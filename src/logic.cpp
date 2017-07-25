#include "logic.h"
#include <QList>
#include <QByteArray>
#include <QHash>
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

  void initPosition();
  int findByPosition(int x, int y);
  bool canMove(int index, int toX, int toY);
};

void Logic::Impl::initPosition()
{
    figures << Figure { FIGURE_WHITE, FIGURE_ROOK, 0, 7 };
    figures << Figure { FIGURE_WHITE, FIGURE_KNIGHT, 1, 7 };
    figures << Figure { FIGURE_WHITE, FIGURE_BISHOP, 2, 7 };
    figures << Figure { FIGURE_WHITE, FIGURE_QUEEN, 3, 7 };
    figures << Figure { FIGURE_WHITE, FIGURE_KING, 4, 7 };
    figures << Figure { FIGURE_WHITE, FIGURE_BISHOP, 5, 7 };
    figures << Figure { FIGURE_WHITE, FIGURE_KNIGHT, 6, 7 };
    figures << Figure { FIGURE_WHITE, FIGURE_ROOK, 7, 7 };

    for (int i = 0; i < 8; ++i) {
        figures << Figure { FIGURE_WHITE, FIGURE_PAWN, i, 6 };
    }

    figures << Figure { FIGURE_BLACK, FIGURE_ROOK, 0, 0 };
    figures << Figure { FIGURE_BLACK, FIGURE_KNIGHT, 1, 0 };
    figures << Figure { FIGURE_BLACK, FIGURE_BISHOP, 2, 0 };
    figures << Figure { FIGURE_BLACK, FIGURE_QUEEN, 3, 0 };
    figures << Figure { FIGURE_BLACK, FIGURE_KING, 4, 0 };
    figures << Figure { FIGURE_BLACK, FIGURE_BISHOP, 5, 0 };
    figures << Figure { FIGURE_BLACK, FIGURE_KNIGHT, 6, 0 };
    figures << Figure { FIGURE_BLACK, FIGURE_ROOK, 7, 0 };

    for (int i = 0; i < 8; ++i) {
        figures << Figure { FIGURE_BLACK, FIGURE_PAWN, i, 1 };
    }
}

int Logic::Impl::findByPosition(int x, int y) {
  for (int i(0); i<figures.size(); ++i) {
    if (figures[i].x != x || figures[i].y != y ) { 
      continue; 
    }
    return i;    
  }
  return -1;
}

bool Logic::Impl::canMove(int index, int toX, int toY)
{
    Figure figure = figures[index];
    qDebug() << "canMove" << figure.type << figure.piece << index << toX << toY;

    if (figure.type == FIGURE_WHITE && figure.piece == FIGURE_PAWN) {
        if (figure.y - toY > (figure.y == 6 ? 2 : 1) || figure.y - toY <= 0) {
            return false;
        } else {
            return true;
        }
    }

    if (figure.type == FIGURE_BLACK && figure.piece == FIGURE_PAWN) {
        if (toY - figure.y > (figure.y == 1 ? 2 : 1) || toY - figure.y <= 0) {
            return false;
        } else {
            return true;
        }
    }

    return false;
}

Logic::Logic(QObject *parent)
    : QAbstractListModel(parent)
    , impl(new Impl())
{
    impl->initPosition();
}

Logic::~Logic() {
}

int Logic::boardSize() const {
  return BOARD_SIZE;
}

int Logic::rowCount(const QModelIndex & ) const {
  return impl->figures.size(); 
}

QHash<int, QByteArray> Logic::roleNames() const { 
  QHash<int, QByteArray> names;
  names.insert(Roles::Type      , "type");
  names.insert(Roles::Piece     , "piece");
  names.insert(Roles::PositionX , "positionX");
  names.insert(Roles::PositionY , "positionY");
  return names;
}

QVariant Logic::data(const QModelIndex & modelIndex, int role) const { 
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
  impl->initPosition();
  endResetModel();
}

bool Logic::move(int fromX, int fromY, int toX, int toY) {
  int index = impl->findByPosition(fromX, fromY);
  qDebug() << "move" << index << fromX << fromY << toX << toY;

  if (index < 0) return false;
  
  if (toX < 0 || toX >= BOARD_SIZE || toY < 0 || toY >= BOARD_SIZE) {
    return false;
  }

  if (impl->findByPosition(toX, toY) >= 0) {
      qDebug() << "fight!";
      return false;
  }

  if (!impl->canMove(index, toX, toY)) {
      return false;
  }

  impl->figures[index].x = toX;
  impl->figures[index].y = toY;
  QModelIndex topLeft = createIndex(index, 0);
  QModelIndex bottomRight = createIndex(index, 0);
  emit dataChanged(topLeft, bottomRight);
  return true;
}
