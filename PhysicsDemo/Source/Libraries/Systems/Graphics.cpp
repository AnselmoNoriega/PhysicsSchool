#include "Graphics.h"

#include "Platform/WindowedApplication.h"
#include "Visual/DearImGui/ImGuiContext.h"
#include "Visual/VisualGeometry.h"

namespace jm::System
{
	Graphics::Graphics(Platform::Window& window, Entity_registry& registry)
		: mRenderer(window),
		mRegistry(registry),
		mProgram(R"(
		#version 330 core
		layout (location = 0) in vec3 inPosition;
		layout (location = 1) in vec3 inColour;

		out vec3 outColour;

		void main()
		{
			gl_Position = vec4(inPosition, 1.0);
			outColour = inColour;
		}
		)", R"(
		#version 330 core

		in vec3 outColour;

		out vec4 FragColor;

		void main()
		{
			FragColor = vec4(outColour, 1.0);
		}
		)")
	{
		Visual::InputLayout layout{ {3, 3} };

		byte_list inputVertexData;
		{
			auto cubeVertexData = Visual::GenerateCube(layout);
			inputVertexData.insert(inputVertexData.end(), cubeVertexData.data.begin(), cubeVertexData.data.end());
			cubeVertices = static_cast<GLsizei>(cubeVertexData.size);
		} 

		inputLayoutHandler = mRenderer.RasterizerMemory->createInputLayout(layout);
		inputBufferHandler = mRenderer.RasterizerMemory->createInputBuffer(inputLayoutHandler, inputVertexData);

		glEnable(GL_DEPTH_TEST);
	}

	Graphics::~Graphics()
	{
		mRenderer.RasterizerMemory->destroyInputBuffer(inputLayoutHandler, inputBufferHandler);
		mRenderer.RasterizerMemory->destroyInputLayout(inputLayoutHandler);
	}

	Platform::MessageHandler* Graphics::GetMessageHandler()
	{
		return mRenderer.ImGuiContextPtr->GetMessageHandler();
	}

	void Graphics::Draw3D(math::vector3_f32 const& clearColour, std::function<void()>&& imguiFrame)
	{
		mRenderer.RasterizerImpl->PrepareRenderBuffer(clearColour);

		{
			glBindVertexArray(static_cast<GLuint>(inputLayoutHandler));
			GLsizei start = 0;

			glDrawArrays(GL_TRIANGLES, start, cubeVertices);
		}

		mRenderer.ImGuiContextPtr->RunFrame(std::move(imguiFrame));

		mRenderer.RasterizerImpl->UpdateRenderBuffer();
	}
}
