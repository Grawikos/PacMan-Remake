#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QLabel>

class Player : public QObject {
    Q_OBJECT

public:
    explicit Player(QObject *parent = nullptr);
    void setPosition(int x, int y);
    void move(int newX, int newY);
    QLabel* getLabel();
	int x, y;
	bool isPoweredUP;
	void jump(int toX);
	void secondAnimation(int toX);
	char direction; //w,s,a,d and x = stop
private:
    QLabel *label;
    
};

#endif // PLAYER_H
