#include "mainwindow.h"
#include <QGraphicsRectItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    view = new QGraphicsView;
    board = new CheckersBoard;
    gameManager = new GameManager(board, this);
    board->setGameManager(gameManager);
    board->placePieces();

    view->setMouseTracking(true);
    view->setInteractive(true);
    view->setScene(board);
    view->setFixedSize(802, 802);
    view->setSceneRect(0, 0, 800, 800);
    view->setRenderHint(QPainter::Antialiasing);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    board->setStickyFocus(true);
    view->viewport()->installEventFilter(this);
    setCentralWidget(view);
    setWindowTitle("International Checkers");
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    if(event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
    }
    return QMainWindow::eventFilter(obj, event);
}
