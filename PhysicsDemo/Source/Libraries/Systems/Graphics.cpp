#include "Graphics.h"

#include "Platform/WindowedApplication.h"
#include "Visual/DearImGui/ImGuiContext.h"

namespace jm::System
{
	Graphics::Graphics(Platform::Window& window, Entity_registry& registry)
		: mRenderer(window), mRegistry(registry), mProgram(R"(
		#Version 330 core
		layout (location = 0) in vec3 inPosition;
		layout (location = 1) in vec3 inColour;

		out vec3 outColor;

		void main()
		{
			gl_Position = vec4(inPosition, 1.0);
			outColour = inColour;
		}
		)", R"(
		#Version 330 core

		in vec3 outColor;

		out vec4 FragColor;

		void main()
		{
			FragColor = vec4(outColor, 1.0);
		}
		)")
	{
	}

	Graphics::~Graphics()
	{
	}

	Platform::MessageHandler* Graphics::GetMessageHandler()
	{
		return mRenderer.ImGuiContextPtr->GetMessageHandler();
	}

	void Graphics::Draw3D(math::vector3_f32 const& clearColour, std::function<void()>&& imguiFrame)
	{
		mRenderer.RasterizerImpl->PrepareRenderBuffer(clearColour);

		mRenderer.ImGuiContextPtr->RunFrame(std::move(imguiFrame));

		mRenderer.RasterizerImpl->UpdateRenderBuffer();
	}
}
