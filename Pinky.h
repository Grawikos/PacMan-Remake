#include "Ghost.h"

#ifndef PINKY_H
#define PINKY_H

#include <QObject>
#include <QLabel>

class Pinky : virtual public Ghost {
    Q_OBJECT

public:
	char PacDirection;
	Pinky(QObject *parent = nullptr);
	void findTarget(int pX, int pY) override;    
	void newChase() override;
};

#endif // PINKY_H

