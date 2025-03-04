#pragma once
#include "checkerpiece.h"
#include "checkertile.h"
#include "checkersboard.h"
#include <QObject>
#include <QVector>

class CheckersBoard;
class CheckerTile;
class CheckerPiece;

class GameManager : public QObject {
    Q_OBJECT
public:
    explicit GameManager(CheckersBoard* board, QObject* parent = nullptr);

    void switchTurn();
    bool isCurrentPlayerPiece(CheckerPiece* piece) const;

public slots:
    void handleTileClick(CheckerTile* tile);
    void handlePieceClick(CheckerPiece* piece);

signals:
    void turnChanged(bool isWhiteTurn);

private:
    CheckersBoard* board;
    CheckerPiece* selectedPiece = nullptr;
    CheckerTile* selectedTile = nullptr;
    bool isWhiteTurn = true;

    QVector<CheckerTile*> validMoves;
    QVector<CheckerTile*> validCaptures;

    void clearHighlights();
    void calculateValidMoves(CheckerPiece* piece);
    bool isValidMove(CheckerTile* from, CheckerTile* to) const;
    bool isCapturePossible() const;
    void performMove(CheckerTile* to);
    void performCapture(CheckerTile* to);
    void checkForPromotion(CheckerPiece* piece);
    void checkDiagonal(CheckerTile* from, int targetRow, int targetCol);
    void checkCapture(CheckerTile* from, int targetRow, int targetCol, int jumpRow, int jumpCol);
    void checkDiagonalKing(CheckerTile* from, int targetRow, int targetCol);
    void movePiece(CheckerPiece* piece, CheckerTile* targetTile);
};
