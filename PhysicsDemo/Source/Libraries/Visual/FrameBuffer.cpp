#include "FrameBuffer.h"
#include <iostream>

FrameBuffer::FrameBuffer(jm::Rendering::Context& renderer, int winWidth, int winHeight, const char* vertexShader, const char* fragmentShader)
	: mProgram(vertexShader, fragmentShader)
{
	using namespace jm;
	float rectangleVertices[] =
	{
		-1.0f,  1.0f,	 0.0f, 1.0f,
		 1.0f, -1.0f,	 1.0f, 0.0f,
		 1.0f,  1.0f,	 1.0f, 1.0f,

		-1.0f,  1.0f,	 0.0f, 1.0f,
		-1.0f, -1.0f,    0.0f, 0.0f,
		 1.0f, -1.0f,    1.0f, 0.0f
	};

	std::vector<std::byte> frameRec((std::byte*)&rectangleVertices, (std::byte*)&rectangleVertices + sizeof(rectangleVertices));
	Visual::RawBuffer vertexBufferData{ frameRec , 6 };

	mProgram.MakeActive();
	mProgram.SetUniform("screenTexture", 0);

	jm::Visual::InputLayout layout{ {2, 2 } };
	mInputLayout = renderer.RasterizerMemory->createInputLayout(layout);
	mInputBuffer = renderer.RasterizerMemory->createInputBuffer(mInputLayout, vertexBufferData.data);
	glBindVertexArray(static_cast<GLuint>(mInputLayout));

	glGenFramebuffers(1, &mFrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, winWidth, winHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureID, 0);

	unsigned int RB;
	glGenRenderbuffers(1, &RB);
	glBindRenderbuffer(GL_RENDERBUFFER, RB);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, winWidth, winHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RB);

	auto FbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (FbStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "FrameBuffer ERROR = " << FbStatus << std::endl;
	}
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
}

void FrameBuffer::Update()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	mProgram.MakeActive();
	glBindVertexArray(static_cast<GLuint>(mInputLayout));

	glDisable(GL_DEPTH_TEST);

	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
