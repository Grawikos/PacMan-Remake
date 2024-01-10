#include "Inky.h"

#include <QObject>
#include <QLabel>

#define PACE 180

Inky::Inky(QObject *parent) : Ghost(parent){
	loadSprite("pacman_sprites/Inky.png");
	homeX = 28;
	homeY = 33;
	sprite = "pacman_sprites/Inky.png";
}

void Inky::findTarget(int pX, int pY){
	int facingX = 0, facingY = 0;
	facingX = pX + PacDirection[0] * 2;
	facingY = pY + PacDirection[1] * 2;
	int vectorX = facingX - BlinkyX;
	int vectorY = facingY - BlinkyY;
	targetX = facingX + vectorX;
	targetY = facingY + vectorY;
	
}



void Inky::newChase(){
	mode = CHASE;
	loadSprite("pacman_sprites/Inky.png");
	pace = PACE;
}



