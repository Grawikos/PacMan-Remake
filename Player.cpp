#include "Player.h"
#include "defines.h"


Player::Player(QObject *parent) : QObject(parent), QGraphicsPixmapItem() {
    spriteSheet = QPixmap("pacman_sprites/Pacman_full.png");
    currentFrame = 0;
    
    setPixmap(spriteSheet.copy(0, 0, RAW_SIZE, RAW_SIZE).scaled(SPRITE_SIZE, SPRITE_SIZE));
    setOffset(-PADDING, -PADDING);  // offset
    isPoweredUP = false;
	animTimer = new QTimer(this);
	eatAnimTimer = new QTimer(this);
    connect(eatAnimTimer, &QTimer::timeout, this, &Player::eatingAnimation);
    eatAnimTimer->start(PACE/2);
	direction.append(0);
	direction.append(0);
	
	
}

void Player::setPosition(int _x, int _y) {
    x = _x;
    y = _y;
    setSpritePos(x, y);
}

void Player::setSpritePos(float _x, float _y) {
    setPos(_x * TILE_SIZE, _y * TILE_SIZE);
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

void Player::eatingAnimation() {
    int frameX = currentFrame % 3 * (RAW_SIZE + 2);
    int tmpDir = direction[0]*2 + direction[1];
    int frameY;

    switch(tmpDir){
		case 2:
			frameY = 0;
			break;
		case -2:
			frameY = RAW_SIZE + 2;
			break;
		case -1:
			frameY = 2*RAW_SIZE + 4;
			break;
		case 1:
			frameY = 3*RAW_SIZE + 6;
			break;
		default:
			frameY = 0;
	}
    
    
    setPixmap(spriteSheet.copy(frameX, frameY, RAW_SIZE, RAW_SIZE).scaled(SPRITE_SIZE, SPRITE_SIZE));
    currentFrame = (currentFrame + 1) % 3;

}


void Player::reset(){
	setPosition(14, 23);
	frame = 10;
	isPoweredUP = false;
	direction = {0, 0};
	animTimer->stop();
}
