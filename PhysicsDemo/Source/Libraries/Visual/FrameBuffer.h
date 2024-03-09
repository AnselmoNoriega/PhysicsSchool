#pragma once

#include "OpenGL/OpenGL.h"
#include "Visual.h"

#include "Visual/RenderingContext.h"


class FrameBuffer
{
public:
	FrameBuffer(jm::Rendering::Context& renderer, int winWidth, int winHeight, const char* vertexShader, const char* fragmentShader);

	void Bind();

	void Update();

	float& GetBlurStrength() { return blurStrength; };
	int& GetEffectType() { return effectType; };
	const char** GetEffectNames() { return EffectNames; };

private:
	jm::Visual::ShaderProgram mProgram;
	jm::OpenGL::InputLayoutHandle mInputLayout;
	jm::OpenGL::InputBufferHandle mInputBuffer;

	unsigned int mFrameBufferID;
	unsigned int mTextureID;

	float blurStrength = 1000;

	int effectType = 0;

	const char* EffectNames[2] = {
		"None",
		"Box blur"
	};
};

