#pragma once
#include <QGraphicsScene>
#include <QVector>

class CheckerTile;
class CheckerPiece;
class GameManager;

class CheckersBoard : public QGraphicsScene {
    Q_OBJECT
public:
    QVector<QVector<CheckerTile*>> tiles;
    explicit CheckersBoard(QObject *parent = nullptr);

    void setupBoard();
    void setGameManager(GameManager* manager);
    void placePieces();
    void createTiles();

    // board size
    static const int TILE_SIZE = 100;
    static const int BOARD_SIZE = 8;

private:
    GameManager* gameManager = nullptr;
};
