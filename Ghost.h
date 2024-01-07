#ifndef GHOST_H
#define GHOST_H

#include <QObject>
#include <QLabel>

class Ghost : public QObject {
    Q_OBJECT

public:
	explicit Ghost(QObject *parent = nullptr);
	virtual ~Ghost() = default;
    QLabel* getLabel();
	int x, y;
	int targetX, targetY;
	char direction; //w,s,a,d 
	void setPosition(int _x, int _y);
	void loadSprite(QString s);
	void animation(int newX, int newY);
	virtual void findTarget(int pX, int pY) = 0;
	void move();
	bool isScared;
private:
	char reverse(char direction);
    QVector<QVector<int>> map;
    QLabel *label;
    
};

#endif // GHOST_H
