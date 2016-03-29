#pragma once

#include "BoxCell.h"
#include "vmath.h"

static const int CUBE_RANK = 3;

enum FaceType { FRONT_FACE, BACK_FACE, UP_FACE, DOWN_FACE, LEFT_FACE, RIGHT_FACE, FACE_COUNT};
enum FaceColor {NULL_COLOR,RED_FACE, BLUE_FACE, YELLO_FACE, WHITE_FACE, GREEN_FACE, ORANGE_FACE};

struct CellInfo {
	FaceColor faceColor[FACE_COUNT];
	BoxCell renderCell;
	CellInfo() {
		for (auto i = 0; i < FACE_COUNT; i++) {
			faceColor[i] = NULL_COLOR;
		}
	}
};


class Cube
{
public:
	Cube(BoxCellModelPtr& cellMode);
	~Cube();

	void onRender(const vmath::mat4& viewMatrix, const vmath::mat4& projectMatrix);
private:	
	BoxCellModelPtr _modePtr;
	CellInfo _cells [CUBE_RANK][CUBE_RANK][CUBE_RANK];
};

