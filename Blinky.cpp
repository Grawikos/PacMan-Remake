#include "Blinky.h"

#include <QObject>
#include <QLabel>

Blinky::Blinky(QObject *parent) : Ghost(parent){
	loadSprite("pacman_sprites/Blinky.png");
	homeX = 26;
	homeY = 0;
}

void Blinky::findTarget(int pX, int pY){
	targetX = pX;
	targetY = pY;
}


void Blinky::newChase(){
	mode = CHASE;
	loadSprite("pacman_sprites/Blinky.png");
}

