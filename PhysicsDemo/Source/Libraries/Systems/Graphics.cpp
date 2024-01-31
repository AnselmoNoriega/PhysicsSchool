#include "Graphics.h"

#include "Platform/WindowedApplication.h"
#include "Visual/DearImGui/ImGuiContext.h"
#include "Visual/VisualGeometry.h"
//#include "Systems/"

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

		uniform mat4 model;
		uniform mat4 projectionView;

		void main()
		{
			gl_Position = projectionView * model * vec4(inPosition, 1.0);
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
		{
			auto axesData = Visual::GenerateCoordinateAxes3(layout);
			inputVertexData.insert(inputVertexData.end(), axesData.data.begin(), axesData.data.end());
			axesVertices = static_cast<GLsizei>(axesData.size);
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

	void Graphics::Draw3D(math::camera3<f32> const& camera, math::vector3_f32 const& clearColour, std::function<void()>&& imguiFrame)
	{
		auto spatial_shape_view = mRegistry.view<const spatial3_component>();

		std::vector<math::matrix44_f32> CubeInstances;
		for (auto&& [entity, spatial] : spatial_shape_view.each())
		{
			CubeInstances.push_back(math::translation_matrix3(spatial.position));
		}

		mRenderer.RasterizerImpl->PrepareRenderBuffer(clearColour);

		mProgram.SetUniform("projectionView", camera.get_perspective_transform() * camera.get_view_transform());
		{
			GLsizei start = 0;
			glBindVertexArray(static_cast<GLuint>(inputLayoutHandler));

			for (math::matrix44_f32& instance : CubeInstances)
			{
				mProgram.SetUniform("model", instance);
				glDrawArrays(GL_TRIANGLES, start, cubeVertices);
			}
			start += cubeVertices;

			mProgram.SetUniform("model", math::identity4);
			glDrawArrays(GL_LINES, start, axesVertices);
		}

		mRenderer.ImGuiContextPtr->RunFrame(std::move(imguiFrame));

		mRenderer.RasterizerImpl->UpdateRenderBuffer();
	}
}
