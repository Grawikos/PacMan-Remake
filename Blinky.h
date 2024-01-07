#include "Ghost.h"

#ifndef BLINKY_H
#define BLINKY_H

#include <QObject>
#include <QLabel>

class Blinky : virtual public Ghost {
    Q_OBJECT

public:
	Blinky(QObject *parent = nullptr);
	void findTarget(int pX, int pY) override;    
};

#endif // BLINKY_H

