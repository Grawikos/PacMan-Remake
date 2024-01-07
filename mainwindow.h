// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Player.h"
#include "Blinky.h"
#include "Pinky.h"
#include "Inky.h"
#include <QMainWindow>
#include <QKeyEvent>
#include <QPixmap>
#include <QLabel>
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QPropertyAnimation>
#include <QLabel>
#include <QPoint>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

public slots:
	void continuousMove(); 
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;



private:
	void ghostMove();
	QLabel* highScoreLabel;
	QLabel* scoreLabel;
	void directionalMove();
	Player *pacman;
	QTimer *ghostMoveTimer;
	QTimer* continuousMoveTimer;
    void updatePlayerPosition();
	void handleAnimation(int X, int Y);
    QWidget *centralWidget;
    int tileSize;
    QVector<QVector<int>> map;
    QPixmap food;
    QPixmap bigFood;
    Blinky *blinky;
    Pinky *pinky;
    Inky *inky;
    QPixmap backgroundImage;
    int lastKeyPressed;
    int score;
    int highScore;
    void updateScore();
    void updateHighScore();
};

#endif // MAINWINDOW_H
