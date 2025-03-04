#include "checkersboard.h"
#include "checkertile.h"
#include "checkerpiece.h"
#include "gamemanager.h"

CheckersBoard::CheckersBoard(QObject *parent) : QGraphicsScene(parent) {
    // enable event handling
    setStickyFocus(true);
    setFocus();

    // create board elements
    createTiles();
}

void CheckersBoard::createTiles() {
    tiles.resize(BOARD_SIZE);
    for(int row = 0; row < BOARD_SIZE; ++row) {
        tiles[row].resize(BOARD_SIZE);
        for(int col = 0; col < BOARD_SIZE; ++col) {
            CheckerTile* tile = new CheckerTile(row, col);
            addItem(tile);
            tiles[row][col] = tile;
        }
    }
}

void CheckersBoard::setGameManager(GameManager* manager) {
    gameManager = manager;
}

void CheckersBoard::placePieces() {
    for(int row = 0; row < BOARD_SIZE; ++row) {
        for(int col = 0; col < BOARD_SIZE; ++col) {
            if((row + col) % 2 == 1) {
                CheckerPiece* piece = nullptr;
                if(row < 3) {
                    piece = new CheckerPiece(CheckerPiece::White, tiles[row][col]);
                } else if(row > 4) {
                    piece = new CheckerPiece(CheckerPiece::Black, tiles[row][col]);
                }

                if(piece && gameManager) {
                    connect(piece, &CheckerPiece::clicked, gameManager, &GameManager::handlePieceClick);
                }
            }
        }
    }
}
