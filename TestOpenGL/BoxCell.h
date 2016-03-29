#pragma once
#include <memory>
#include <gl/glew.h>
#include <gl/GL.h>
#include "vmath.h"

struct BoxCellModel;

typedef std::shared_ptr<BoxCellModel> BoxCellModelPtr;
BoxCellModelPtr createBoxCellModel(const char* verShaderFile, const char* fragShaderFile);

class BoxCell
{
public:
	BoxCell();
	~BoxCell();

	void onRender(BoxCellModelPtr& model, const vmath::mat4& viewTransform, const vmath::mat4& projectTransform);
	void setTransform(const vmath::mat4& trans) { 
		_worldTransform = trans;
	}
private:
	vmath::mat4 _worldTransform;
};

