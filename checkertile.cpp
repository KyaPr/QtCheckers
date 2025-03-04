#include "checkertile.h"
#include "checkersboard.h"
#include "checkerpiece.h"
#include <QBrush>

CheckerTile::CheckerTile(int row, int col, QGraphicsItem* parent)
    : QGraphicsRectItem(parent), row(row), col(col) {
    setRect(0, 0, CheckersBoard::TILE_SIZE, CheckersBoard::TILE_SIZE);
    setPos(col * CheckersBoard::TILE_SIZE, row * CheckersBoard::TILE_SIZE);

    const bool isDark = (row + col) % 2 == 1;
    originalColor = isDark ? QColor(101, 67, 33) : QColor(241, 218, 171);
    setBrush(originalColor);

    // enable event handling
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
}

void CheckerTile::resetColor() {
    setBrush(originalColor);
}

void CheckerTile::highlight() {
    setBrush(QColor(255, 255, 0)); // bright yellow
    update(); // force refresh
}

bool CheckerTile::hasPiece() const {
    return currentPiece != nullptr;
}

CheckerPiece* CheckerTile::piece() const {
    return currentPiece;
}

void CheckerTile::setPiece(CheckerPiece* piece) {
    currentPiece = piece;
    if (piece) piece->setPos(x() + rect().width() / 2, y() + rect().height() / 2);
}

void CheckerTile::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsRectItem::mousePressEvent(event);
    emit clicked(this);
    event->accept();
}

