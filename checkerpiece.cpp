#include "checkerpiece.h"
#include "checkertile.h"
#include "checkersboard.h"
#include <QPropertyAnimation>
#include <QPainterPath>
#include <QPainter>

CheckerPiece::CheckerPiece(Color color, CheckerTile* tile, QGraphicsItem* parent)
    : QGraphicsEllipseItem(tile), pieceColor(color), tile(tile) {
    Q_UNUSED(parent);
    const int size = CheckersBoard::TILE_SIZE * 0.8;
    setRect(0, 0, size, size);

    // center the piece in tile
    setPos((CheckersBoard::TILE_SIZE - size) / 2, (CheckersBoard::TILE_SIZE - size) / 2);

    setupAppearance();
    setZValue(2);

    // enable event handling
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
}

void CheckerPiece::setupAppearance() {
    // main color
    QColor baseColor = (pieceColor == White) ? Qt::white : Qt::black;

    // gradient for 3d effect
    QRadialGradient gradient(rect().center(), rect().width() / 2);
    gradient.setColorAt(0, baseColor.lighter(150));
    gradient.setColorAt(1, baseColor.darker(150));

    setBrush(gradient);
    setPen(QPen(Qt::gray, 1));

    // if piece is king, add crown
    if (king) {
        QPainterPath crown;
        const qreal crownSize = rect().width() * 0.4;
        crown.addEllipse(rect().center().x() - crownSize / 2, rect().center().y() - crownSize / 2, crownSize, crownSize);
        QGraphicsPathItem* crownItem = new QGraphicsPathItem(crown, this);
        crownItem->setBrush(QColor(255, 215, 0)); // gold color
        crownItem->setPen(Qt::NoPen);
    }
}

void CheckerPiece::moveToTile(CheckerTile* newTile) {
    if (tile) tile->setPiece(nullptr);
    tile = newTile;
    newTile->setPiece(this);

    // use fixed size for animation
    const int tileCenter = CheckersBoard::TILE_SIZE / 2;
    QPropertyAnimation* anim = new QPropertyAnimation(this, "pos");
    anim->setDuration(200);
    anim->setEndValue(newTile->pos() + QPointF(tileCenter, tileCenter));
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void CheckerPiece::promote() {
    king = true;

    // add crown
    QPainterPath crown;
    const qreal size = rect().width() * 0.4;
    crown.addEllipse(-size / 2, -size / 2, size, size);
    QGraphicsPathItem* crownItem = new QGraphicsPathItem(crown, this);
    crownItem->setBrush(QColor(255, 215, 0));
    crownItem->setPen(Qt::NoPen);
}

void CheckerPiece::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsEllipseItem::mousePressEvent(event);
    emit clicked(this);
    event->accept();
}
