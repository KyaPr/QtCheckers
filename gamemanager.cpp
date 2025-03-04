#include "gamemanager.h"
#include <QPropertyAnimation>

GameManager::GameManager(CheckersBoard* board, QObject* parent)
    : QObject(parent), board(board) {
    for(auto& row : board->tiles) {
        for(auto tile : row) {
            connect(tile, &CheckerTile::clicked, this, &GameManager::handleTileClick);
            if(tile->hasPiece()) {
                connect(tile->piece(), &CheckerPiece::clicked, this, &GameManager::handlePieceClick);
            }
        }
    }
}

void GameManager::handlePieceClick(CheckerPiece* piece) {
    if(!isCurrentPlayerPiece(piece)) return;
    clearHighlights();
    selectedPiece = piece;
    calculateValidMoves(piece);
}

void GameManager::handleTileClick(CheckerTile* tile) {
    if (!selectedPiece) return;
    if (validMoves.contains(tile)) {
        movePiece(selectedPiece, tile);
        switchTurn();
    }
    clearHighlights();
    selectedPiece = nullptr;
}

void GameManager::calculateValidMoves(CheckerPiece* piece) {
    validMoves.clear();
    int direction = (piece->color() == CheckerPiece::White) ? 1 : -1;
    int newRow = piece->currentTile()->row + direction;

    for (int colOffset : {-1, 1}) {
        int newCol = piece->currentTile()->col + colOffset;
        if (newRow < 0 || newRow >= CheckersBoard::BOARD_SIZE || newCol < 0 || newCol >= CheckersBoard::BOARD_SIZE)
            continue;
        CheckerTile* targetTile = board->tiles[newRow][newCol];
        if (!targetTile->hasPiece()) {
            validMoves.append(targetTile);
            targetTile->highlight();
        }
    }
}

void GameManager::performMove(CheckerTile* to) {
    selectedPiece->moveToTile(to);
    checkForPromotion(selectedPiece);
    switchTurn();
}

void GameManager::checkForPromotion(CheckerPiece* piece) {
    if((piece->color() == CheckerPiece::White && piece->currentTile()->row == 7) ||
       (piece->color() == CheckerPiece::Black && piece->currentTile()->row == 0)) {
        piece->promote();
    }
}

void GameManager::clearHighlights() {
    for(auto tile : validMoves) {
        tile->resetColor();
    }
    validMoves.clear();
}

bool GameManager::isCurrentPlayerPiece(CheckerPiece* piece) const {
    return (isWhiteTurn && piece->color() == CheckerPiece::White) ||
           (!isWhiteTurn && piece->color() == CheckerPiece::Black);
}

void GameManager::checkDiagonal(CheckerTile* from, int targetRow, int targetCol) {
    Q_UNUSED(from);
    if(targetRow < 0 || targetRow >= CheckersBoard::BOARD_SIZE ||
       targetCol < 0 || targetCol >= CheckersBoard::BOARD_SIZE)
        return;
    CheckerTile* targetTile = board->tiles[targetRow][targetCol];
    if(!targetTile->hasPiece()) {
        validMoves.append(targetTile);
    }
}

void GameManager::checkCapture(CheckerTile* from, int targetRow, int targetCol,
                              int jumpRow, int jumpCol) {
    Q_UNUSED(from);
    if(targetRow < 0 || targetRow >= CheckersBoard::BOARD_SIZE ||
       targetCol < 0 || targetCol >= CheckersBoard::BOARD_SIZE)
        return;
    CheckerTile* jumpTile = board->tiles[jumpRow][jumpCol];
    CheckerTile* targetTile = board->tiles[targetRow][targetCol];
    if(jumpTile->hasPiece() &&
       jumpTile->piece()->color() != selectedPiece->color() &&
       !targetTile->hasPiece()) {
        validCaptures.append(targetTile);
    }
}

void GameManager::checkDiagonalKing(CheckerTile* from, int targetRow, int targetCol) {
    int rowStep = (targetRow > from->row) ? 1 : -1;
    int colStep = (targetCol > from->col) ? 1 : -1;
    int currentRow = from->row + rowStep;
    int currentCol = from->col + colStep;
    int piecesCount = 0;

    while(currentRow != targetRow && currentCol != targetCol) {
        CheckerTile* tile = board->tiles[currentRow][currentCol];
        if(tile->hasPiece()) {
            if(tile->piece()->color() == selectedPiece->color() || piecesCount > 0)
                return;
            piecesCount++;
        }
        currentRow += rowStep;
        currentCol += colStep;
    }

    if(piecesCount <= 1) {
        validMoves.append(board->tiles[targetRow][targetCol]);
    }
}

void GameManager::performCapture(CheckerTile* to) {
    int midRow = (selectedTile->row + to->row) / 2;
    int midCol = (selectedTile->col + to->col) / 2;
    CheckerTile* jumpedTile = board->tiles[midRow][midCol];
    if(jumpedTile->hasPiece()) {
        delete jumpedTile->piece();
        jumpedTile->setPiece(nullptr);
    }
    selectedPiece->moveToTile(to);
    checkForPromotion(selectedPiece);
    calculateValidMoves(selectedPiece);
    if(validCaptures.isEmpty()) {
        switchTurn();
    } else {
        clearHighlights();
        for(auto capture : validCaptures) capture->highlight();
    }
}

void GameManager::movePiece(CheckerPiece* piece, CheckerTile* targetTile) {
    CheckerTile* currentTile = piece->currentTile();
    currentTile->setPiece(nullptr);
    piece->setParentItem(targetTile);

    QPointF targetPos = QPointF(
        (CheckersBoard::TILE_SIZE - piece->rect().width()) / 2,
        (CheckersBoard::TILE_SIZE - piece->rect().height()) / 2
    );

    QPropertyAnimation* anim = new QPropertyAnimation(piece, "pos");
    anim->setDuration(200);
    anim->setStartValue(piece->pos());
    anim->setEndValue(targetPos);
    anim->start(QAbstractAnimation::DeleteWhenStopped);

    targetTile->setPiece(piece);
    piece->setTile(targetTile);

    connect(anim, &QPropertyAnimation::finished, [this, piece, targetPos]() {
        piece->setPos(targetPos);
        board->update();
    });
}

void GameManager::switchTurn() {
    isWhiteTurn = !isWhiteTurn;
}
