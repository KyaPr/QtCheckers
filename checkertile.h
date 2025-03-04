#pragma once
#include "checkersboard.h"
#include <QGraphicsRectItem>
#include <QObject>

class CheckerPiece;

class CheckerTile : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    CheckerTile(int row, int col, QGraphicsItem* parent = nullptr);

    void resetColor();
    void highlight();
    bool hasPiece() const;
    CheckerPiece* piece() const;
    void setPiece(CheckerPiece* piece);

    const int row;
    const int col;

signals:
    void clicked(CheckerTile* tile);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    CheckerPiece* currentPiece = nullptr;
    QColor originalColor;
};
