#include "Blinky.h"

#include <QObject>
#include <QLabel>
#define PACE 180

Blinky::Blinky(QObject *parent) : Ghost(parent){
	loadSprite("pacman_sprites/Blinky.png");
	homeX = 26;
	homeY = -2;
	sprite = "pacman_sprites/Blinky.png";
}

void Blinky::findTarget(int pX, int pY){
	targetX = pX;
	targetY = pY;
}


void Blinky::newChase(){
	mode = CHASE;
	loadSprite("pacman_sprites/Blinky.png");
	pace = PACE;
}

