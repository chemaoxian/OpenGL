// TestOpenGL.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <Windows.h>
#include "LoadShaders.h"
#include "BoxCell.h"
#include "vmath.h"
#include "Screen.h"
#include "Camera.h"

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
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			double x = 0;
			double y = 0;
			glfwGetCursorPos(pWindow, &x, &y);
			gCamera.onMouseDown(x, y);
		}
		else {
			double x = 0;
			double y = 0;
			glfwGetCursorPos(pWindow, &x, &y);
			gCamera.onMouseUp(x, y);
		}
	}
	else {
		gCamera._ball.reset();
	}
}

void onMouseMoveEvent(GLFWwindow* pWindow, double x, double y) {
	gCamera.onMouseMove(x, y);
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
BoxCell* pBox = nullptr;

void init() {
	glClearColor(0.0f, 0.0, 0.0, 1);
	glEnable(GL_CULL_FACE); // 开启剪裁
	glCullFace(GL_BACK);
	
	gCamera.setCameraPos(vmath::vec3(0.0f, 0.0f, 0.0f), vmath::vec3(0.0f, 0.0f, -1.0f), vmath::vec3(0.0f, 1.0f, 0.0f));
	pBox = new BoxCell(createBoxCellModel("box.vert", "box.frag"));
	pBox->setTransform(vmath::translate(0.0f, 0.0f, -2.0f));
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	static float rote = 0.0f;
	
	if (pBox) {
		vmath::mat4 projectT = vmath::perspective(120.0f,  800.0f / 600.0f, 1.0f, 150.0f);

		rote +=  0.3f;
		
		pBox->setTransform( vmath::translate(0.0f, 0.0f, -5.0f)  * gCamera._ball.getPose());
		pBox->onRender(gCamera.getViewMatrix(), projectT);

		// render 2
		pBox->setTransform(vmath::translate(2.0f, 0.0f, -5.0f) * vmath::rotate(rote, vmath::vec3(1, 1, 1)));
		pBox->onRender(gCamera.getViewMatrix(), projectT);

		// render 3
		pBox->setTransform(vmath::translate(-2.0f, 0.0f, -5.0f) * vmath::rotate(rote, vmath::vec3(1, 1, 1)));
		pBox->onRender(gCamera.getViewMatrix(), projectT);

		Sleep(0);
	}

	//glFlush();
}