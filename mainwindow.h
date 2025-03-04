#pragma once
#include "checkersboard.h"
#include "gamemanager.h"
#include <QMainWindow>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QEvent>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private:
    GameManager *gameManager;
    QGraphicsView* view;
    CheckersBoard* board;

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
};
