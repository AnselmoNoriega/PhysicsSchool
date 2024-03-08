#include "Graphics.h"
#include "Components.h"

#include "Visual/DearImGui/ImGuiContext.h"
#include "Visual/VisualGeometry.h"

#include "DearImGui/imgui.h"

#include "Platform/WindowedApplication.h"

namespace jm::System
{
	Graphics::Data2D::Data2D(cstring vertexSource, cstring fragmentSource)
		: Program(vertexSource, fragmentSource)
	{
	}

	Graphics::Data3D::Data3D(cstring vertexSource, cstring fragmentSource)
		: Program(vertexSource, fragmentSource)
	{
	}

	cstring pixelShader = R"(
			#version 330 core
			
			out vec4 FragColor;
			
			in vec3 outColour;
			
			void main()
			{
			    FragColor = vec4(outColour, 1.0f);
			}
			)";

	Graphics::Graphics(Platform::Window& window, Entity_registry& registry, math::vector3_f32 const& clearColour)
		: Renderer(window)
		, EntityRegistry(registry)
		, TwoDimensional(R"(
			#version 330 core
			layout (location = 0) in vec2 inPosition;
			layout (location = 1) in vec3 inColour;
			  
			out vec3 outColour;
			
			uniform mat3 view;
			uniform mat3 model;
			
			void main()
			{
				vec3 worldPosition = view * model * vec3(inPosition, -1.0);
			    gl_Position = vec4(worldPosition, 1.0);
				outColour = inColour;
			}
			)", pixelShader)
		, ThreeDimensional(R"(
			#version 330 core
			layout (location = 0) in vec3 inPosition;
			layout (location = 1) in vec3 inColour;
			  
			out vec3 outColour;
			
			uniform mat4 projectionView;
			uniform mat4 model;
			
			void main()
			{
			    gl_Position = projectionView * model * vec4(inPosition, 1.0);
				outColour = inColour;
			}
			)", pixelShader)
		, FB(Renderer, (int)window.GetArea().Width, (int)window.GetArea().Height,R"(
		    #version 330 core
		    
		    layout(location = 0) in vec2 _pos;
		    layout(location = 1) in vec2 _texCoords;
		    
		    out vec2 textureCoord;
		    
		    void main()
		    {
		        gl_Position = vec4(_pos.x, _pos.y, 0.0, 1.0);
		        textureCoord = _texCoords;
		    };
		    )", R"(
			#version 330 core 
			
			out vec4 FragColor;
			
			in vec2 textureCoord;
			
			uniform sampler2D screenTexture;
			
			const float offset_x = 1.0f / 800.0f;
			const float offset_y = 1.0f / 800.0f;
			
			vec2 offsets[9] = vec2[]
			(
			    vec2(-offset_x, offset_y), vec2(0.0f,  offset_y), vec2(offset_x, offset_y),
			    vec2(-offset_x,     0.0f), vec2(0.0f,      0.0f), vec2(offset_x,     0.0f),
			    vec2(-offset_x, offset_y), vec2(0.0f, -offset_y), vec2(offset_x, -offset_y)
			);
			
			float kernel[9] = float[]
			(
			     2,  2,  2,
			     1, -8,  1,
			     1,  0,  0
			);
			
			void main()
			{
			    vec3 color = vec3(0.0f);
			    for (int i = 0; i < 9; i++)
			    {
			        color += vec3(texture(screenTexture, textureCoord.st + offsets[i])) * kernel[i];
			        FragColor = vec4(color, 1.0f);
			    }
			
			})")
		, ClearColour(clearColour)
	{
		{
			Visual::InputLayout layout{ {2, 3 } };

			byte_list inputVertexData;
			{
				auto boxVertexData = Visual::GenerateBox(layout);
				inputVertexData.insert(inputVertexData.end(), boxVertexData.data.begin(), boxVertexData.data.end());
				TwoDimensional.squareVertices = (GLsizei)boxVertexData.size;
			}
			{
				auto diskVertexData = Visual::GenerateDisk(layout);
				inputVertexData.insert(inputVertexData.end(), diskVertexData.data.begin(), diskVertexData.data.end());
				TwoDimensional.diskVertices = (GLsizei)diskVertexData.size;
			}
			{
				auto axesVertexData = Visual::GenerateCoordinateAxes2(layout);
				inputVertexData.insert(inputVertexData.end(), axesVertexData.data.begin(), axesVertexData.data.end());
				TwoDimensional.axesVertices = (GLsizei)axesVertexData.size;
			}

			TwoDimensional.Program.MakeActive();
			TwoDimensional.inputLayoutHandle = Renderer.RasterizerMemory->createInputLayout(layout);
			TwoDimensional.inputBufferHandle = Renderer.RasterizerMemory->createInputBuffer(TwoDimensional.inputLayoutHandle, inputVertexData);
		}
		{
			Visual::InputLayout layout{ { 3, 3 } };

			byte_list inputVertexData;
			{
				auto cubeVertexData = Visual::GenerateCube(layout);
				inputVertexData.insert(inputVertexData.end(), cubeVertexData.data.begin(), cubeVertexData.data.end());
				ThreeDimensional.cubeVertices = (GLsizei)cubeVertexData.size;
			}
			{
				auto sphereVertexData = Visual::GenerateSphere(layout);
				inputVertexData.insert(inputVertexData.end(), sphereVertexData.data.begin(), sphereVertexData.data.end());
				ThreeDimensional.sphereVertices = (GLsizei)sphereVertexData.size;
			}
			{
				auto axesVertexData = Visual::GenerateCoordinateAxes3(layout);
				inputVertexData.insert(inputVertexData.end(), axesVertexData.data.begin(), axesVertexData.data.end());
				ThreeDimensional.axesVertices = (GLsizei)axesVertexData.size;
			}

			ThreeDimensional.Program.MakeActive();
			ThreeDimensional.inputLayoutHandle = Renderer.RasterizerMemory->createInputLayout(layout);
			ThreeDimensional.inputBufferHandle = Renderer.RasterizerMemory->createInputBuffer(ThreeDimensional.inputLayoutHandle, inputVertexData);
		}

		glEnable(GL_DEPTH_TEST);
	}

	Graphics::~Graphics()
	{
		Renderer.RasterizerMemory->destroyInputBuffer(TwoDimensional.inputLayoutHandle, TwoDimensional.inputBufferHandle);
		Renderer.RasterizerMemory->destroyInputLayout(TwoDimensional.inputLayoutHandle);

		Renderer.RasterizerMemory->destroyInputBuffer(ThreeDimensional.inputLayoutHandle, ThreeDimensional.inputBufferHandle);
		Renderer.RasterizerMemory->destroyInputLayout(ThreeDimensional.inputLayoutHandle);
	}

	Platform::MessageHandler* Graphics::GetMessageHandler()
	{
		return Renderer.ImGuiContextPtr->GetMessageHandler();
	}

	template <typename T>
	auto&& get_shapes2(Entity_registry& registry)
	{
		auto shape_entity_view = registry.view<const T, const spatial2_component>();
		return shape_entity_view.each();
	}

	template <typename T>
	auto&& get_shapes3(Entity_registry& registry)
	{
		auto shape_entity_view = registry.view<const T, const spatial3_component>();
		return shape_entity_view.each();
	}

	void Graphics::Draw(math::camera3<f32> const& camera, std::function<void()>&& imguiFrame)
	{
		std::vector<math::matrix33_f32> diskInstances;
		std::vector<math::matrix33_f32> squareInstances;
		std::vector<math::matrix44_f32> sphereInstances;
		std::vector<math::matrix44_f32> cubeInstances;
		{
			auto disk_entity_view = EntityRegistry.view<const disk_shape_component, const spatial2_component>();
			diskInstances.reserve(disk_entity_view.size_hint());
			for (auto&& [entity, shape, spatial] : disk_entity_view.each())
			{
				diskInstances.push_back(math::isometry_matrix2(spatial.position, spatial.orientation) * math::scale_matrix2(shape.radius));
			}
		}
		{
			auto rectangle_entity_view = EntityRegistry.view<const rectangle_shape_component, const spatial2_component>();
			squareInstances.reserve(rectangle_entity_view.size_hint());
			for (auto&& [entity, shape, spatial] : rectangle_entity_view.each())
			{
				squareInstances.push_back(math::isometry_matrix2(spatial.position, spatial.orientation) * math::scale_matrix2(shape.extents));
			}
		}
		{
			auto sphere_entity_view = EntityRegistry.view<const sphere_shape_component, const spatial3_component>();
			sphereInstances.reserve(sphere_entity_view.size_hint());
			for (auto&& [entity, shape, spatial] : sphere_entity_view.each())
			{
				sphereInstances.push_back(math::isometry_matrix3(spatial.position, spatial.orientation) * math::scale_matrix3(shape.radius));
			}
		}
		{
			auto box_entity_view = EntityRegistry.view<const box_shape_component, const spatial3_component>();
			cubeInstances.reserve(box_entity_view.size_hint());
			for (auto&& [entity, shape, spatial] : box_entity_view.each())
			{
				cubeInstances.push_back(math::isometry_matrix3(spatial.position, spatial.orientation) * math::scale_matrix3(shape.extents));
			}
		}

		FB.Bind();

		Renderer.RasterizerImpl->PrepareRenderBuffer(ClearColour);

		{
			ThreeDimensional.Program.MakeActive();

			glBindVertexArray(static_cast<GLuint>(ThreeDimensional.inputLayoutHandle));
			ThreeDimensional.Program.SetUniform("projectionView", camera.get_perspective_transform() * camera.get_view_transform());

			GLsizei start = 0;
			for (auto& instance : cubeInstances)
			{
				ThreeDimensional.Program.SetUniform("model", instance);
				glDrawArrays(GL_TRIANGLES, start, ThreeDimensional.cubeVertices);
			}
			start += ThreeDimensional.cubeVertices;

			for (auto& instance : sphereInstances)
			{
				ThreeDimensional.Program.SetUniform("model", instance);
				glDrawArrays(GL_TRIANGLES, start, ThreeDimensional.sphereVertices);
			}
			start += ThreeDimensional.sphereVertices;

			if (Debug3D)
			{
				ThreeDimensional.Program.SetUniform("model", math::identity4);
				glDrawArrays(GL_LINES, start, ThreeDimensional.axesVertices);
			}
		}


		{

			TwoDimensional.Program.MakeActive();
			glBindVertexArray(static_cast<GLuint>(TwoDimensional.inputLayoutHandle));
			TwoDimensional.Program.SetUniform("view", math::scale_matrix2(0.1f) * math::matrix33_f32(camera.get_orthogonal_transform()));

			GLsizei start = 0;
			for (auto& instance : squareInstances)
			{
				TwoDimensional.Program.SetUniform("model", instance);
				glDrawArrays(GL_TRIANGLES, start, TwoDimensional.squareVertices);
			}
			OpenGL::CheckError();
			start += TwoDimensional.squareVertices;
			for (auto& instance : diskInstances)
			{
				TwoDimensional.Program.SetUniform("model", instance);
				glDrawArrays(GL_TRIANGLES, start, TwoDimensional.diskVertices);
			}
			start += TwoDimensional.diskVertices;

			if (Debug2D)
			{
				TwoDimensional.Program.SetUniform("model", math::identity3);
				glDrawArrays(GL_LINES, start, TwoDimensional.axesVertices);
			}
		}

		FB.Update();

		Renderer.ImGuiContextPtr->RunFrame(std::move(imguiFrame));
		
		Renderer.RasterizerImpl->UpdateRenderBuffer();
	}

	void Graphics::ImGuiDebug()
	{
		ImGui::Text("Graphics");
		ImGui::ColorEdit3("BG Colour", reinterpret_cast<f32*>(&ClearColour));
		ImGui::Checkbox("Debug 2D", &Debug2D);
		ImGui::Checkbox("Debug 3D", &Debug3D);
	}
}
