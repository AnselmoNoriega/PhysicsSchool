#pragma once
#include "Visual/RenderingContext.h"
#include "Visual/Visual.h"
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

		void Draw3D(math::vector3_f32 const& clearColour, std::function<void()>&& imguiFrame);

	private:
		Visual::ShaderProgram mProgram;

		Rendering::Context mRenderer;
		Entity_registry& mRegistry;
	};
}