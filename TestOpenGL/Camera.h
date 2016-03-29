#pragma once

#include <functional>
#include <list>
#include <mutex>
#include "vmath.h"
#include "../common/include/GLFW/glfw3.h"


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


struct MouseEvent {
	GLFWwindow* targetWindow;
	int event;
	int button;
	int modify;
	int x;
	int y;
};

struct KeyboardEvent {
	GLFWwindow* targetWindow;
	int event;
	int keyboard;
};

template <typename T>
class Singleton {
	static T* _ins;
public:
	Singleton(const Singleton&) = delete;
	Singleton& operator = (const Singleton&) = delete;

	static T* getInstanse() {
		static std::once_flag flag;
		std::call_once(flag, [&_ins] {_ins = new T()});

		return _ins;
	}

private:
	
};

template <typename T>
T* Singleton<T>::_ins = nullptr;

class InputManager : public Singleton<InputManager>{
public:
	typedef std::function<void(const KeyboardEvent&)> KeyboardEventListener;
	typedef std::function<void(const MouseEvent&)> MouseEventListener;

	InputManager(const InputManager&) = delete;
	InputManager& operator = (const InputManager&) = delete;

public:
	void addKeyboardEventListener(KeyboardEventListener& f);
	void addMouseEventListener(MouseEventListener& f);

	void dispatchMouseEvent(const MouseEvent& e);
	void dispatchKeyboardEvent(const KeyboardEvent& e);

private:
	std::list<KeyboardEventListener> _keyboardListenerList;
	std::list<MouseEventListener> _mouseEventListenerList;
};

class Camera {
public:
	Camera();
	~Camera();

	void setCameraPos(vmath::vec3& eye, vmath::vec3& center, vmath::vec3& up);

	void onMouseDown(int key, int x, int y);
	void onMouseMove(int x, int y);
	void onMouseUp(int key, int x, int y);
	vmath::mat4 getViewMatrix();

	void lookat(const vmath::vec3& eye, const vmath::vec3& center, vmath::vec3 up);
	void walk(float distance);		// move front or back
	void fly(float distance);		// move up or down
	void strafe(float distance);	// move left or right

	void pitch(float angle);	// rote on right axie
	void yaw(float angle);		// rote on up axie
	void roll(float angle);		// rote on front axie

	vmath::mat4 _viewMatrix;
private:
	
	vmath::vec2 _startPos;
	ArcBall		_cameraBall;
	bool        _bMouseDrag;
	bool		_bDragCamera;
	
	vmath::vec3 _pos;
	vmath::vec3 _up;
	vmath::vec3 _front;
	vmath::vec3 _right;
};


extern Camera gCamera;
