#pragma once
#include "checkertile.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainterPath>
#include <QObject>

class CheckerTile;

class CheckerPiece : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    enum Color { White, Black };

    CheckerPiece(Color color, CheckerTile* tile, QGraphicsItem* parent = nullptr);

    Color color() const { return pieceColor; }
    bool isKing() const { return king; }
    void promote();
    void setTile(CheckerTile* tile) { this->tile = tile; }
    CheckerTile* currentTile() const { return tile; }
    void moveToTile(CheckerTile* newTile);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    Color pieceColor;
    bool king = false;
    CheckerTile* tile;

    void setupAppearance();

signals:
    void clicked(CheckerPiece* piece);
};
