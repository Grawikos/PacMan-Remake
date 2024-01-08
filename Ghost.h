#ifndef GHOST_H
#define GHOST_H

#include <QObject>
#include <QLabel>
#define CHASE 0
#define SCATTER 1
#define FRIGHTENED 2


class Ghost : public QObject {
    Q_OBJECT

public:
	explicit Ghost(QObject *parent = nullptr);
	virtual ~Ghost() = default;
    QLabel* getLabel();
	int mode;
	int x, y;
	int targetX, targetY;	
	int homeX, homeY;
	char direction; //w,s,a,d 
	void setPosition(int _x, int _y);
	void loadSprite(QString s);
	void animation(int newX, int newY, float modifier = 1);
	virtual void findTarget(int pX, int pY) = 0;
	void move();
	void runAway();
	void eaten();
	void getFrightened();
	virtual void newChase() = 0;
	void nextMove();
	bool isEaten;
private:
	char reverse(char direction);
    QVector<QVector<int>> map;
    QLabel *label;
    
};

#endif // GHOST_H
