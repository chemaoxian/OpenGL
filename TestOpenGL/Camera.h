#pragma once

#include "vmath.h"

class ArcBall {
public:
	ArcBall(){
		_ballMatrix = vmath::mat4::identity();
	}
	~ArcBall() {}
	
	void reset();
	void drag(vmath::vec2& startP1, vmath::vec2& startP2);
	const vmath::mat4& getPose();
private:
	static vmath::vec3 getBallPosFromScreenPos(int x, int y);
	vmath::mat4 _ballMatrix;
};

class Camera {
public:
	Camera();
	~Camera();

	void setCameraPos(vmath::vec3& eye, vmath::vec3& center, vmath::vec3& up);

	void onMouseDown(int x, int y);
	void onMouseMove(int x, int y);
	void onMouseUp(int x, int y);
	const vmath::mat4& getViewMatrix() { return _viewMatrix; }

	ArcBall _ball;
private:
	vmath::mat4 _viewMatrix;
	vmath::vec2 _startPos;
	bool        _bMouseDrag;
};


extern Camera gCamera;
