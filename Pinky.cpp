#include "Pinky.h"

#include <QObject>
#include <QLabel>

#define PACE 180

Pinky::Pinky(QObject *parent) : Ghost(parent){
	loadSprite("pacman_sprites/Pinky.png");
	homeX = 2;
	homeY = 0;
	sprite = "pacman_sprites/Pinky.png";
}

void Pinky::findTarget(int pX, int pY){
	targetX = pX + PacDirection[0] * 4;
	targetY = pY + PacDirection[1] * 4;
}


void Pinky::newChase(){
	mode = CHASE;
	loadSprite("pacman_sprites/Pinky.png");
	pace = PACE;
}



