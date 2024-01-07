#include "Blinky.h"

#include <QObject>
#include <QLabel>

Blinky::Blinky(QObject *parent) : Ghost(parent){
	loadSprite("pacman_sprites/Blinky.png");
}

void Blinky::findTarget(int pX, int pY){
	targetX = pX;
	targetY = pY;
}


