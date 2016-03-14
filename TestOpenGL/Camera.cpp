#include "stdafx.h"
#include "Camera.h"
#include "Screen.h"

vmath::vec3 ArcBall::getBallPosFromScreenPos(int x, int y) {
	float posX = (x - (gScreen.screenWidth / 2.0f)) / gScreen.screenWidth / 2.0f;
	float posY = -(y - (gScreen.screenHeight / 2.0f)) / gScreen.screenHeight / 2.0f;

	float posZ = 0;
	float magic = posX * posX + posY * posY;
	
	if (magic > 1.0) {
		float scale = 1.0f / sqrtf(magic);
		posX *= scale;
		posY *= scale;
	} else {
		posZ = sqrtf(1 - magic);
	}

	return vmath::vec3(posX, posY, posZ);
}

vmath::mat4 getBallRote(vmath::vec3& startPos, vmath::vec3& endPos) {
	
	if (vmath::length(startPos - endPos) < 0.0001f) {
		return vmath::mat4::identity();
	}

	float angle = acos(vmath::dot(startPos, endPos)) * 180 / 3.1415;
	
	vmath::vec3 axis = vmath::cross(startPos, endPos);
	//printf("angle : %f, axis : (%f, %f, %f) \n", angle, axis[0], axis[1], axis[2]);
	return vmath::rotate(angle, axis).transpose();
}

void ArcBall::reset() {
	_ballMatrix = vmath::mat4::identity();
}

void ArcBall::drag(vmath::vec2& startPos, vmath::vec2& endPos) {
	vmath::vec3 startPos3D = ArcBall::getBallPosFromScreenPos(startPos[0], startPos[1]);
	vmath::vec3 endPos3D = ArcBall::getBallPosFromScreenPos(endPos[0], endPos[1]);

	_ballMatrix = getBallRote(startPos3D, endPos3D) * _ballMatrix;
}

const vmath::mat4& ArcBall::getPose() {
	return _ballMatrix; 
}

//////////////////////////////////////////////////////

Camera::Camera() :
	_bMouseDrag(false){
}


Camera::~Camera() {
}

void Camera::setCameraPos(vmath::vec3& eye, vmath::vec3& center, vmath::vec3& up) {
	_viewMatrix = vmath::lookat(eye, center, up);
}

void Camera::onMouseDown(int x, int y) {
	_bMouseDrag = true;
	_startPos = vmath::vec2(x, y);
}

void Camera::onMouseMove(int x, int y) {
	if (_bMouseDrag) {
		_ball.drag(_startPos, vmath::vec2(x, y));
		_startPos[0] = x;
		_startPos[1] = y;
	}
	
}

void Camera::onMouseUp(int x, int y) {
	_bMouseDrag = false;
}

Camera gCamera;