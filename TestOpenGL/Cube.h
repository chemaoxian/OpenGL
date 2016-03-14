#pragma once

#include "BoxCell.h"
#include "vmath.h"

class Cube
{
public:
	Cube();
	~Cube();

private:	
	BoxCellModelPtr _modePtr;
};

