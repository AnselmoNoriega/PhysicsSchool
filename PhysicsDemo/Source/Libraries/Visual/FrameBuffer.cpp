#include "FrameBuffer.h"

#include "Visual/RenderingContext.h"

FrameBuffer::FrameBuffer(jm::Rendering::Context& renderer, int winWidth, int winHeight, const char* vertexShader, const char* fragmentShader)
	: mProgram(vertexShader, fragmentShader)
{
	using namespace jm;
	float rectangleVertices[] =
	{
		 1.0f, -1.0f,    1.0f, 0.0f,
		-1.0f, -1.0f,    0.0f, 0.0f,
		-1.0f,  1.0f,    0.0f, 1.0f,

		 1.0f,  1.0f,	1.0f, 1.0f,
		 1.0f, -1.0f,	1.0f, 0.0f,
		-1.0f,  1.0f,	0.0f, 1.0f
	};


	std::vector<std::byte> frameRec((std::byte*)&rectangleVertices, (std::byte*)&rectangleVertices + sizeof(rectangleVertices));
	Visual::RawBuffer vertexBufferData{ frameRec , 6 };

	mProgram.MakeActive();
	mProgram.SetUniform("uTexture", 0);
	jm::Visual::InputLayout layout{ {2, 2 } };
	mInputLayout = renderer.RasterizerMemory->createInputLayout(layout);
	mInputBuffer = renderer.RasterizerMemory->createInputBuffer(mInputLayout, vertexBufferData.data);

	glGenFramebuffers(1, &mFrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
	// vao static_cast<GLuint>(mInputLayout)
	unsigned int frameTexture;
	glGenTextures(1, &frameTexture);
	glBindTexture(GL_TEXTURE_2D, frameTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, frameTexture, 0);
}

void FrameBuffer::Bind()
{
}

void FrameBuffer::Update()
{
}
