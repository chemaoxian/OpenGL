#include "stdafx.h"
#include "BoxCell.h"
#include <gl/glew.h>
#include "LoadShaders.h"
#include "targa.h"

struct BoxCellModel {
	GLuint vertextId;
	GLuint bufferId;
	GLuint indexId;
	GLuint shaderId;
	GLuint textId;

	GLint worldMatrixLoc;
	GLint viewMatrixLoc;
	GLint projectMatrixLoc;
	GLint texLoc;

	BoxCellModel(const char* verfile, const char* fragfile)
	:vertextId(0),
	 bufferId(0),
	 shaderId(0),
	 indexId(0){

		GLfloat pos[24] = {
			 0.5,  0.5,  0.5,
			 0.5,  0.5, -0.5,
			-0.5,  0.5, -0.5,
			-0.5,  0.5,  0.5,  // up face point
			 0.5, -0.5,  0.5,
			 0.5, -0.5, -0.5,
			-0.5, -0.5, -0.5,
			-0.5, -0.5,  0.5  // back face point
		};

		GLfloat colors[24] = {
			 0.5,  0.5,  0.5,
			 0.5,  0.5, -0.5,
			-0.5,  0.5, -0.5,
			-0.5,  0.5,  0.5,
			 0.5, -0.5,  0.5,
			 0.5, -0.5, -0.5,
			-0.5, -0.5, -0.5,
			-0.5, -0.5,  0.5
		};

		GLfloat normals[24] = {
			0
		};

		GLfloat uvs[16] = {
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 1.0f,
		};

		GLushort indexs[36] = {
			0, 1, 2,
			2, 3, 0, // up
			4, 6, 5,
			4, 7, 6, // down
			1, 5, 6,
			6, 2, 1, // front
			0, 3, 7,
			7, 4, 0, // back
			3, 2, 6, 
			6, 7, 3, // left
			0, 4, 5,
			5, 1, 0  // right
		};

		//init VAO
		glGenVertexArrays(1, &vertextId);
		glBindVertexArray(vertextId);

		// init IBO
		glGenBuffers(1, &indexId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs,GL_STATIC_DRAW);

		// init VBO
		glGenBuffers(1, &bufferId);
		glBindBuffer(GL_ARRAY_BUFFER, bufferId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pos) + sizeof(colors) + sizeof(normals) + sizeof(uvs), 0, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), pos);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(colors), colors);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos) + sizeof(colors), sizeof(normals), normals);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos) + sizeof(colors) + sizeof(normals), sizeof(uvs), uvs);

		glEnable(GL_TEXTURE_2D);
		
		glGenTextures(1, &textId);
		glBindTexture(GL_TEXTURE_2D, textId);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   // Linear Filtered  
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width = 0;
		int height = 0;
		GLenum format = 0;
		unsigned char* data = vtarga::load_targa("vase_plant.tga", format, width, height);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

		// bind vectext attribute
		glEnableVertexArrayAttrib(vertextId, 0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexArrayAttrib(vertextId, 1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(pos));
		glEnableVertexArrayAttrib(vertextId, 2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(pos) + sizeof(colors)));
		glEnableVertexArrayAttrib(vertextId, 3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(pos) + sizeof(colors) + sizeof(normals)));

		

		ShaderInfo infos[] = {
			{ GL_VERTEX_SHADER, verfile },
			{ GL_FRAGMENT_SHADER, fragfile },
			{ GL_NONE, nullptr }
		};

		shaderId =  LoadShaders(infos);

		worldMatrixLoc = glGetUniformLocation(shaderId, "worldMatrix");
		viewMatrixLoc = glGetUniformLocation(shaderId, "viewMatrix");
		projectMatrixLoc = glGetUniformLocation(shaderId, "projectMatrix");
		texLoc = glGetUniformLocation(shaderId, "tex");
	}

	bool uninitilize() {
		glDeleteBuffers(1, &bufferId);
		glDeleteBuffers(1, &indexId);
		glDeleteVertexArrays(1, &vertextId);
		return true;
	}

	void draw(const GLfloat* worldMatrix, const GLfloat* viewMatrix, const GLfloat* projectMatrix) {
		
		glBindVertexArray(vertextId);

		glUseProgram(shaderId);

		glUniformMatrix4fv(worldMatrixLoc, 1, GL_FALSE, worldMatrix);
		
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, viewMatrix);
	
		glUniformMatrix4fv(projectMatrixLoc, 1, GL_FALSE, projectMatrix);
		
		glBindBuffer(GL_ARRAY_BUFFER, bufferId);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textId);
		
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	
	}
};

BoxCellModelPtr createBoxCellModel(const char* verShaderFile, const char* fragShaderFile) {
	BoxCellModelPtr model = std::make_shared<BoxCellModel>(verShaderFile, fragShaderFile);
	return model;
}

BoxCell::BoxCell(BoxCellModelPtr& ptr) :
	_model(ptr){
}

BoxCell::~BoxCell() {
}

void BoxCell::onRender(const vmath::mat4& viewTransform, const vmath::mat4& projectTransform) {
	_model->draw ((const GLfloat*)_worldTransform, (const GLfloat*)viewTransform, (const GLfloat*)projectTransform);
}

