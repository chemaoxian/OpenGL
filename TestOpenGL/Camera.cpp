#include "stdafx.h"
#include "Camera.h"
#include "Screen.h"
#include "../common/include/GLFW/glfw3.h"


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
	_bMouseDrag(false),
	_bDragCamera(false){
}

Camera::~Camera() {
}

void Camera::setCameraPos(vmath::vec3& eye, vmath::vec3& center, vmath::vec3& up) {
	_viewMatrix = vmath::lookat(eye, center, up);
}

void Camera::onMouseDown(int key, int x, int y) {
	if (key == GLFW_MOUSE_BUTTON_LEFT) {
		_bMouseDrag = true;
		_startPos = vmath::vec2(x, y);
	}
	else if (key == GLFW_MOUSE_BUTTON_RIGHT) {
		_bDragCamera = true;
		_startPos = vmath::vec2(x, y);
	}
}

void Camera::onMouseMove(int x, int y) {
	if (_bDragCamera) {
		_cameraBall.drag(_startPos, vmath::vec2(x, y));
		_startPos[0] = x;
		_startPos[1] = y;

		_viewMatrix = _cameraBall.getPose() * _viewMatrix;
		_cameraBall.reset();
	}

}

void Camera::onMouseUp(int key, int x, int y) {
	_bMouseDrag = false;
	_bDragCamera = false;
}

void Camera::lookat(const vmath::vec3& eye, const vmath::vec3& center, vmath::vec3 up) {
	_pos = eye;
	_up = vmath::normalize(up);
	_front = -vmath::normalize(center - eye);
	
	assert(vmath::dot(_up, _front) == 0.0f);

	_right = vmath::normalize(vmath::cross(_up, _front));
	//_up = vmath::normalize(vmath::cross(_right, _front));
}

void Camera::walk(float distance) {
	_pos = _pos + _front * distance;
}

void Camera::fly(float distance) {
	_pos = _pos + _up * distance;
}

void Camera::strafe(float distance) {
	_pos = _pos + _right * distance;
}

void Camera::pitch(float angle) {
	vmath::mat4 roteMatrix = vmath::rotate(angle, _right);
	vmath::vec4 up = (vmath::vec4(_up, 0.0f) * roteMatrix);
	vmath::vec4 front = vmath::vec4(_front, 0.0f) *roteMatrix;
	_front = vmath::normalize(vmath::vec3(front[0], front[1], front[2]));
	_up = vmath::normalize(vmath::vec3(up[0], up[1], up[2]));
}

void Camera::yaw(float angle) {
	vmath::mat4 roteMatrix = vmath::rotate(angle, _up);
	vmath::vec4 right = (vmath::vec4(_right, 0.0f) * roteMatrix);
	vmath::vec4 front = vmath::vec4(_front, 0.0f) *roteMatrix;
	_front = vmath::normalize(vmath::vec3(front[0], front[1], front[2]));
	_right = vmath::normalize(vmath::vec3(right[0], right[1], right[2]));
}

void Camera::roll(float angle) {
	vmath::mat4 roteMatrix = vmath::rotate(angle, _front);
	vmath::vec4 right = (vmath::vec4(_right, 0.0f) * roteMatrix);
	vmath::vec4 up = vmath::vec4(_up, 0.0f) *roteMatrix;
	_up = vmath::normalize(vmath::vec3(up[0], up[1], up[2]));
	_right = vmath::normalize(vmath::vec3(right[0], right[1], right[2]));
}

vmath::mat4 Camera::getViewMatrix() {
	float x = -vmath::dot(_pos, _right);
	float y = -vmath::dot(_pos, _up);
	float z = -vmath::dot(_pos, _front);

	return vmath::mat4(vmath::vec4(_right[0], _up[0], _front[0], 0.0f),
		vmath::vec4(_right[1], _up[1], _front[1], 0.0f),
		vmath::vec4(_right[2], _up[2], _front[2], 0.0f),
		vmath::vec4(x, y, z, 1.0f));
}

Camera gCamera;