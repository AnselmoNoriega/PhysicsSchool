#pragma once

#include "OpenGL/OpenGL.h"
#include "Visual.h"

namespace jm
{
	struct Rendering::Context;
}

class FrameBuffer
{
public:
	FrameBuffer(jm::Rendering::Context& renderer, int winWidth, int winHeight, const char* vertexShader, const char* fragmentShader);

	void Bind();

	void Update();

private:
	jm::Visual::ShaderProgram mProgram;
	jm::OpenGL::InputLayoutHandle mInputLayout;
	jm::OpenGL::InputBufferHandle mInputBuffer;

	int mWindowWidth;
	int mWindowHeight;

	unsigned int mFrameBufferID;
};
