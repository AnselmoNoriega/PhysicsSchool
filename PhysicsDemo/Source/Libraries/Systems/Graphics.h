#pragma once

#include "Visual/RenderingContext.h"
#include "Visual/Visual.h"
#include "Math/Camera.h"

#include "Entity.h"

namespace jm
{
	namespace Platform
	{
		class MessageHandler;
	}
}

namespace jm::System
{
	class Graphics
	{
	public:
		Graphics(Platform::Window& window, Entity_registry& registry);
		~Graphics();

		Platform::MessageHandler* GetMessageHandler();
		void ImGuiDebug();

		void Draw3D(math::camera3<f32> const& camera, std::function<void()>&& imguiFrame);

	private:
		Rendering::Context mRenderer;
		Entity_registry& mRegistry;

		Visual::ShaderProgram mProgram;
		OpenGL::InputLayoutHandle inputLayoutHandler;
		OpenGL::InputBufferHandle inputBufferHandler;
		GLsizei cubeVertices;
		GLsizei sphereVertices;
		GLsizei axesVertices;

		math::vector3_f32 mClearColour;
		bool mDebug3D = true;
		bool mDebug2D = false;
	};
}