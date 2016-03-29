// TestOpenGL.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "LoadShaders.h"
#include "BoxCell.h"
#include "vmath.h"
#include "Screen.h"
#include "Camera.h"
#include "Cube.h"

void printErrorAndExitFileLine(const char* file, int line, const char* fmt, ...) {

	char buff[1024] = { 0 };
	
	va_list args;
	va_start(args, args);
	vsprintf(buff, fmt, args);
	va_end(args);
	printf("%s:%d : %s\n", file, line, buff);
	exit(-1);
}

#define FILE_AND_LINE __FILE__, __LINE__

#define PRINT_ERROR_AND_EXIT(fmt, ...) \
	printErrorAndExitFileLine(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

void error(int err, const char* errMsg) {
	printf("%d:%s\n", err, errMsg);
} 

bool isPressed = false;
void onMouseEven(GLFWwindow* pWindow, int button, int action, int modify) {
	if (action == GLFW_PRESS) {
		double x = 0;
		double y = 0;
		glfwGetCursorPos(pWindow, &x, &y);
		gCamera.onMouseDown(button, (int)x, (int)y);
	}
	else if (action == GLFW_RELEASE){
		double x = 0;
		double y = 0;
		glfwGetCursorPos(pWindow, &x, &y);
		gCamera.onMouseUp(button, (int)x, (int)y);
	}
}

void onMouseMoveEvent(GLFWwindow* pWindow, double x, double y) {
	gCamera.onMouseMove((int)x, (int)y);
}

const static float step = 0.1f;
const static float step_angle = 0.5f;
void onKeyEvent(GLFWwindow*, int key, int scancode, int action, int modify) {
	switch (key) {
	case 'W':
		gCamera.fly(step);
		break;
	case 'A':
		gCamera.strafe(-step);
		break;
	case 'D':
		gCamera.strafe(step);
		break;
	case 'S':
		gCamera.fly(-step);
		break;
	case GLFW_KEY_KP_8:
		gCamera.walk(step);
		break;
	case GLFW_KEY_KP_5:
		gCamera.walk(-step);
		break;
	case GLFW_KEY_KP_4:
		gCamera.roll(step_angle);
		break;
	case GLFW_KEY_KP_6:
		gCamera.roll(-step_angle);
		break;
	case GLFW_KEY_KP_7:
		gCamera.yaw(step_angle);
		break;
	case GLFW_KEY_KP_9:
		gCamera.yaw(-step_angle);
		break;
	case GLFW_KEY_KP_1:
		gCamera.pitch(step_angle);
		break;
	case GLFW_KEY_KP_3:
		gCamera.pitch(-step_angle);
		break;
	}
}

void init();
void display();

int _tmain(int argc, _TCHAR* argv[])
{
	if (!glfwInit()) {
		PRINT_ERROR_AND_EXIT("init glfw failed");
	}
	
	gScreen.screenWidth = 800;
	gScreen.screenHeight = 600;

	GLFWwindow* pWindow = nullptr;
	pWindow = glfwCreateWindow(gScreen.screenWidth, gScreen.screenHeight, "TestOpenGL", nullptr, nullptr);
	if (!pWindow) {
		PRINT_ERROR_AND_EXIT("create create opengl window failed");
	}

	glfwMakeContextCurrent(pWindow);
	glfwSetErrorCallback(error);
	glfwSetMouseButtonCallback(pWindow, onMouseEven);
	glfwSetCursorPosCallback(pWindow, onMouseMoveEvent);
	glfwSetKeyCallback(pWindow, onKeyEvent);

	if (glewInit() != GLEW_OK) {
		PRINT_ERROR_AND_EXIT("init glew falied");
	}

	init();
	while (!glfwWindowShouldClose(pWindow)) {
		display();
		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

GLuint boxShader = 0;
BoxCellModelPtr pBoxCellModel = nullptr;
std::shared_ptr<Cube> pCube = nullptr;

void init() {
	glClearColor(1.0f, 0.5, 0.2, 1);
	glEnable(GL_CULL_FACE); // 开启剪裁
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, 800, 600);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	gCamera.lookat(vmath::vec3(0.0f, 0.0f, 5.0f), vmath::vec3(0.0f, 0.0f, -1.0f), vmath::vec3(0.0f, 1.0f, 0.0f));
	pBoxCellModel = createBoxCellModel("box.vert", "box.frag");

	pCube = std::make_shared<Cube>(pBoxCellModel);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (pBoxCellModel) {
		vmath::mat4 projectT = vmath::perspective(90.0f,  800.0f / 600.0f, 2.0f, 15000.0f);
		pCube->onRender(gCamera.getViewMatrix(), projectT);

		Sleep(0);
	}

	//glFlush();
}