#include "stdafx.h"
#include "Cube.h"


Cube::Cube(BoxCellModelPtr& cellMode) : _modePtr(cellMode) {
	for (auto layer = 0; layer < CUBE_RANK; layer++) {
		for (auto row = 0; row < CUBE_RANK; row++) {
			for (auto col = 0; col < CUBE_RANK; col++) {
				if (layer == 0) {
					_cells[layer][row][col].faceColor[FRONT_FACE] = BLUE_FACE;
				} 
				else if (col == 0) {
					_cells[layer][row][col].faceColor[LEFT_FACE] = WHITE_FACE;
					
				}
				else if (row == 0) {
					_cells[layer][row][col].faceColor[UP_FACE] = YELLO_FACE;
				}
				else if (col == 3) {
					_cells[layer][row][col].faceColor[RIGHT_FACE] = GREEN_FACE;
				}
				else if (row == 3) {
					_cells[layer][row][col].faceColor[DOWN_FACE] = ORANGE_FACE;
				}
				else if (layer == 3) {
					_cells[layer][row][col].faceColor[BACK_FACE] = RED_FACE;
				}

				_cells[layer][row][col].renderCell.setTransform(vmath::translate(vmath::vec3(row - 1, -(col - 1), -(layer - 1))));
			}
		}
	}
}


Cube::~Cube() {

}

void Cube::onRender(const vmath::mat4& viewMatrix, const vmath::mat4& projectMatrix) {
	for (auto layer = 0; layer < CUBE_RANK; layer++) {
		for (auto row = 0; row < CUBE_RANK; row++) {
			for (auto col = 0; col < CUBE_RANK; col++) {
				_cells[layer][row][col].renderCell.onRender(_modePtr, viewMatrix, projectMatrix);
			}
		}
	}
}

