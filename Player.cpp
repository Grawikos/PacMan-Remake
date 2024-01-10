#include "Player.h"


#define SIZE_MODIFIER 2
#define SPRITE_SIZE 14 * SIZE_MODIFIER
#define PADDING 12
#define SIZE_MODIFIER 2
#define TILE_SIZE 8 * SIZE_MODIFIER
#define ROWS 28
#define COLUMNS 31
#define PACE 180
#define JUMPPACE PACE/2
#define SPACE_SCORE 50
#define FRAMES 4


Player::Player(QObject *parent) : QObject(parent), QGraphicsPixmapItem() {
    setPixmap(QPixmap("pacman_sprites/PacMan.png").scaled(SPRITE_SIZE, SPRITE_SIZE));
    setOffset(-SPRITE_SIZE / 2, -SPRITE_SIZE / 2);  // offset
    isPoweredUP = false;
	animTimer = new QTimer(this);
	direction.append(0);
	direction.append(0);
}

void Player::setPosition(int _x, int _y) {
    x = _x;
    y = _y;
    setSpritePos(x, y);
}

void Player::setSpritePos(float _x, float _y) {
    setPos(_x * TILE_SIZE + PADDING, _y * TILE_SIZE + PADDING);
}


void Player::move(int newX, int newY) {
    animate(newX, newY);       

    QTimer::singleShot(PACE, this, [=]() {
        setPosition(newX, newY);
        animTimer->stop();
    });
}

void Player::animate(int toX, int toY){
	
	animTimer->start(PACE/FRAMES);
	float dx = x - toX;
	float dy = y - toY;
	frame = 0;
	disconnect(animTimer, &QTimer::timeout, this, nullptr);
	connect(animTimer, &QTimer::timeout, this, [=]() {
        setSpritePos(x - frame*dx/FRAMES, y - frame*dy/FRAMES);
        frame++;
        if(frame >= 5){
			animTimer->stop();
		}
    });
	
}

void Player::jump(int toX){
	QTimer::singleShot(PACE, this, [=]() {
        setPosition(toX, 14);
        animTimer->stop();
    });   
}

void Player::reset(){
	setPosition(14, 23);
	frame = 10;
	isPoweredUP = false;
	direction = {0, 0};
	animTimer->stop();
}
