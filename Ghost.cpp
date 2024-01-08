#include "Ghost.h"
#include <QPropertyAnimation>
#include <QDebug>
#include <QMap>
#include <QRandomGenerator>
#include <QVector>

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
#define WALL 1


Ghost::Ghost(QObject *parent) : QObject(parent) {
    label = new QLabel();
    label->setScaledContents(false);
    label->setContentsMargins(0, 0, 0, 0);
    mode = CHASE;
    isEaten = false;
    direction = 'd';
    map = { // 0 - empty 1 - wall, 2 - food, 3 - powerup
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
	{1,3,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,3,1},
	{1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
	{1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
	{1,2,2,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,2,2,1},
	{1,1,1,1,1,1,2,1,1,1,1,1,0,1,1,0,1,1,1,1,1,2,1,1,1,1,1,1},
	{1,1,1,1,1,1,2,1,1,1,1,1,0,1,1,0,1,1,1,1,1,2,1,1,1,1,1,1},
	{0,0,0,0,1,1,2,1,1,0,0,0,0,0,0,0,0,0,0,1,1,2,1,1,0,0,0,0},
	{1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1},
	{1,1,1,1,1,1,2,1,1,0,1,1,0,0,0,0,1,1,0,1,1,2,1,1,1,1,1,1},
	{0,0,0,0,0,0,2,0,0,0,1,1,0,0,0,0,1,1,0,0,0,2,0,0,0,0,0,0},
	{1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1},
	{1,0,0,0,0,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,0,0,0,0,1},
	{0,0,0,0,0,1,2,1,1,0,0,0,0,0,0,0,0,0,0,1,1,2,1,0,0,0,0,0},
	{0,0,0,0,0,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,0,0,0,0,0},
	{1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
	{1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
	{1,3,2,2,1,1,2,2,2,2,2,2,2,0,0,2,2,2,2,2,2,2,1,1,2,2,3,1},
	{1,1,1,2,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,2,1,1,1},
	{1,1,1,2,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,2,1,1,1},
	{1,2,2,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,2,2,1},
	{1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1},
	{1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};
}

void Ghost::animation(int newX, int newY, float modifier){
	QPropertyAnimation* animation = new QPropertyAnimation(label, "geometry");
    animation->setStartValue(label->geometry());
    animation->setEndValue(QRect(newX * TILE_SIZE - PADDING, newY * TILE_SIZE - PADDING + SPACE_SCORE, SPRITE_SIZE, SPRITE_SIZE));
    animation->setDuration(PACE * modifier);  // ms
    animation->start();
}

char Ghost::reverse(char direction){
	if(direction == 'w')
		return 's';
	if(direction == 'd')
		return 'a';
	if(direction == 's')
		return 'w';
	if(direction == 'a'){
		return 'd';
	}
	return 'x';
}

void Ghost::setPosition(int _x, int _y){
	label->move(_x * TILE_SIZE - PADDING, _y * TILE_SIZE - PADDING + SPACE_SCORE);
	x = _x;
	y = _y;
}


void Ghost::getFrightened(){
	mode = FRIGHTENED;
	direction = reverse(direction);
	loadSprite("pacman_sprites/ScaredGhost.png");
}


void Ghost::eaten(){
	if(x == 13 && y == 11){
		isEaten = false;
		mode = CHASE;
	}else{
		isEaten = true;
		targetX = 13;
		targetY = 11;
		direction = 'x';
	}
}

void Ghost::nextMove(){	
	if(isEaten){
		eaten();
		return;
	}
	switch(mode){
		case CHASE:
			move();
			break;
		case SCATTER:
			break;
		case FRIGHTENED:
			runAway();
	}
}

void Ghost::runAway(){
	QVector<char>* dirVec = new QVector<char>;
	if(map[y-1][x] != WALL && 'w' != reverse(direction))
		dirVec->push_back('w');
	if(map[y][x+1] != WALL && 'd' != reverse(direction))
		dirVec->push_back('d');
	if(map[y+1][x] != WALL && 's' != reverse(direction))
		dirVec->push_back('s');
	if(map[y][x-1] != WALL && 'a' != reverse(direction))
		dirVec->push_back('a');
	int rand = QRandomGenerator::global()->bounded(0, dirVec->size());
	direction = dirVec->at(rand);
	int newX = x, newY = y;
	switch(direction){
		case  'w':
            newY--;
            break;
        case 's':
            newY++;
            break;
        case 'a':
            newX--;
            break;
        case 'd':
            newX++;
            break;
    }
    delete dirVec;
    if (x == 27 && y == 14 && newX == 28) {
		setPosition(0, 14);
		newX = 0;
		return;
	}
	else if (x == 0 && y == 14 && newX == -1) {
		setPosition(27, 14);
		newX = 27;
		return;
	}
    x = newX;
    y = newY;
    animation(newX, newY, 1.5);
}

void Ghost::move(){
	QMap<char, float>* dirDst = new QMap<char, float>;
	if(map[y-1][x] != WALL)
		dirDst->insert('w', (x - targetX) * (x - targetX) + (y-1 - targetY) * (y-1 - targetY));
	if(map[y][x+1] != WALL)
		dirDst->insert('d', (x+1 - targetX) * (x+1 - targetX) + (y - targetY) * (y - targetY));
	if(map[y+1][x] != WALL)
		dirDst->insert('s', (x - targetX) * (x - targetX) + (y+1 - targetY) * (y+1 - targetY));
	if(map[y][x-1] != WALL)
		dirDst->insert('a', (x-1 - targetX) * (x-1 - targetX) + (y - targetY) * (y - targetY));
	dirDst->remove(reverse(direction));
	float minDst = 999999;
	char newDirection = 'x';
	for(auto i = dirDst->cbegin(), end = dirDst->cend(); i != end; i++){
		if(i.value() < minDst){
			newDirection = i.key();
			minDst = i.value();
		}
	}
	direction = newDirection;
	int newX = x, newY = y;
	switch(direction){
		case  'w':
            newY--;
            break;
        case 's':
            newY++;
            break;
        case 'a':
            newX--;
            break;
        case 'd':
            newX++;
            break;
    }
    delete dirDst;
    if (x == 27 && y == 14 && newX == 28) {
		setPosition(0, 14);
		newX = 0;
		return;
	}
	else if (x == 0 && y == 14 && newX == -1) {
		setPosition(27, 14);
		newX = 27;
		return;
	}
    x = newX;
    y = newY;
    animation(newX, newY);
}



void Ghost::loadSprite(QString s){
	label->setPixmap(QPixmap(s).scaled(SPRITE_SIZE, SPRITE_SIZE));
	setPosition(x, y);
}


QLabel* Ghost::getLabel() {
    return label;
}
