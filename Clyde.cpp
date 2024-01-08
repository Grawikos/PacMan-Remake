#include "Clyde.h"

#include <QObject>
#include <QLabel>
#include <QtMath>


Clyde::Clyde(QObject *parent) : Ghost(parent){
	loadSprite("pacman_sprites/Clyde.png");
	homeX = 0;
	homeY = 33;
}

void Clyde::findTarget(int pX, int pY){
	float pacDst = qSqrt((x-pX) * (x-pX) + (y-pY) * (y-pY));
	if(pacDst > 8){
		targetX = pX;
		targetY = pY;
	}
	else{
		targetX = homeX;
		targetY = homeY;
	}
}



void Clyde::newChase(){
	mode = CHASE;
	loadSprite("pacman_sprites/Clyde.png");
}
