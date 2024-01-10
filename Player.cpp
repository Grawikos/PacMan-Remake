#include "Player.h"
#include <QPropertyAnimation>
#include <QDebug>


#define SIZE_MODIFIER 2
#define SPRITE_SIZE 14 * SIZE_MODIFIER
#define PADDING 4
#define SIZE_MODIFIER 2
#define TILE_SIZE 8 * SIZE_MODIFIER
#define ROWS 28
#define COLUMNS 31
#define PACE 180
#define JUMPPACE PACE/2
#define SPACE_SCORE 50

Player::Player(QObject *parent) : QObject(parent) {
    label = new QLabel();
    label->setPixmap(QPixmap("pacman_sprites/PacMan.png").scaled(SPRITE_SIZE, SPRITE_SIZE));
    label->setScaledContents(false);
    label->setContentsMargins(0, 0, 0, 0);
    isPoweredUP = false;
}

void Player::setPosition(int _x, int _y) {
    x = _x;
    y = _y;
    label->move(x * TILE_SIZE - PADDING, y * TILE_SIZE - PADDING + SPACE_SCORE);
}

void Player::move(int newX, int newY) {
    QPropertyAnimation* animation = new QPropertyAnimation(label, "geometry");
    animation->setStartValue(label->geometry());
    animation->setEndValue(QRect(newX * TILE_SIZE - PADDING, newY * TILE_SIZE - PADDING + SPACE_SCORE, SPRITE_SIZE, SPRITE_SIZE));
    animation->setDuration(PACE);  // ms
    animation->start();

    connect(animation, &QPropertyAnimation::finished, [=]() {
        setPosition(newX, newY);
        animation->deleteLater();  // Clean up the animation object
    });
}

void Player::jump(int toX){
	QPropertyAnimation* animation = new QPropertyAnimation(label, "geometry");
    animation->setStartValue(label->geometry());
    float newX, jumpTo;
    if(toX == 0){
		newX = 27.5;
		jumpTo = -0.5;
	}
	else{
		newX = -0.5;
		jumpTo = 27.5;
	}
    animation->setEndValue(QRect(newX * TILE_SIZE - PADDING, y * TILE_SIZE - PADDING + SPACE_SCORE, SPRITE_SIZE, SPRITE_SIZE));
    animation->setDuration(JUMPPACE);  // ms
    animation->start();

    connect(animation, &QPropertyAnimation::finished, [=]() {
		label->move(jumpTo * TILE_SIZE - PADDING, y * TILE_SIZE - PADDING + SPACE_SCORE);
        secondAnimation(toX);
        animation->deleteLater();  // Clean up the animation object
    });    
}

void Player::secondAnimation(int newX){
	QPropertyAnimation* animation = new QPropertyAnimation(label, "geometry");
    animation->setStartValue(label->geometry());
    animation->setEndValue(QRect(newX * TILE_SIZE - PADDING, y * TILE_SIZE - PADDING + SPACE_SCORE, SPRITE_SIZE, SPRITE_SIZE));
    animation->setDuration(JUMPPACE);  // ms
    animation->start();

    connect(animation, &QPropertyAnimation::finished, [=]() {
        setPosition(newX, y);
        animation->deleteLater();  // Clean up the animation object
    });  
}



QLabel* Player::getLabel() {
    return label;
}
