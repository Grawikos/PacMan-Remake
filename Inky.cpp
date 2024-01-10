#include "Inky.h"

#include <QObject>
#include <QLabel>

Inky::Inky(QObject *parent) : Ghost(parent){
	loadSprite("pacman_sprites/Inky.png");
	homeX = 28;
	homeY = 33;
}

void Inky::findTarget(int pX, int pY){
	int facingX = 0, facingY = 0;
	switch(PacDirection){
		case 'w':
			facingX = pX;
			facingY = pY - 2;
			break;
		case 'd':
			facingX = pX + 2;
			facingY = pY;
			break;
		case 's':
			facingX = pX;
			facingY = pY + 2;
			break;
		default:
			facingX = pX - 2;
			facingY = pY;
			break;
	}
	int vectorX = facingX - BlinkyX;
	int vectorY = facingY - BlinkyY;
	targetX = facingX + vectorX;
	targetY = facingY + vectorY;
	
}



void Inky::newChase(){
	mode = CHASE;
	loadSprite("pacman_sprites/Inky.png");
}



