#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QLabel>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QDebug>
#include <QTimer>
#include <QVector>




class Player : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    explicit Player(QObject *parent = nullptr);
    void setPosition(int x, int y);
    void setSpritePos(float x, float y);
    
    void animate(int newX, int newY);
    void move(int newX, int newY);
	int x, y;
	int frame;
	bool isPoweredUP;
	void jump(int toX);
	QVector<int> direction; //x, y
	QTimer* animTimer;
	void reset();
};

#endif // PLAYER_H
