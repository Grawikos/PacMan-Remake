#include "Pinky.h"

#include <QObject>
#include <QLabel>

Pinky::Pinky(QObject *parent) : Ghost(parent){
	loadSprite("pacman_sprites/Pinky.png");
	homeX = 2;
	homeY = 0;
}

void Pinky::findTarget(int pX, int pY){
	switch(PacDirection){
		case 'w':
			targetX = pX;
			targetY = pY - 4;
			return;
		case 'd':
			targetX = pX + 4;
			targetY = pY;
			return;
		case 's':
			targetX = pX;
			targetY = pY + 4;
			return;
		default:
			targetX = pX - 4;
			targetY = pY;
			return;
	}
}


void Pinky::newChase(){
	mode = CHASE;
	loadSprite("pacman_sprites/Pinky.png");
}



