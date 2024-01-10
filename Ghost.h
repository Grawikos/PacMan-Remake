#ifndef GHOST_H
#define GHOST_H

#include <QObject>
#include <QLabel>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QVector>
#include <QString>



#define CHASE 0
#define SCATTER 1
#define FRIGHTENED 2


class Ghost : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
	explicit Ghost(QObject *parent = nullptr);
	virtual ~Ghost() = default;
	int mode;
	int frame;
	int pace;
	int x, y;
	int targetX, targetY;	
	int homeX, homeY;
	QVector<int> direction; //x, y
	void setPosition(int _x, int _y);
	void loadSprite(QString s);
	virtual void findTarget(int pX, int pY) = 0;
	void move();
	QString sprite;
	void runAway();
	void eaten();
	void getFrightened();
	virtual void newChase() = 0;
	void nextMove();
	bool isEaten;
	bool eatenLoop;
	void animate(int, int);
	void setSpritePos(float _x, float _y);
	void backHome();
	void reset();
private:
	void chooseTargetDirection();
	void chooseRandomDirection();
	QVector<QVector<int>> map;
	QTimer* animTimer;
};

#endif // GHOST_H
