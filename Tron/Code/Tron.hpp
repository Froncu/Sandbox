#if not defined FRONGE_EDITOR_HPP
#define FRONGE_EDITOR_HPP

#include "Fronge.hpp"

namespace fro
{
	class Tron final : public Application
	{
	public:
		Tron();
		Tron(Tron const&) = default;
		Tron(Tron&&) noexcept = default;

		virtual ~Tron() override = default;

		Tron& operator=(Tron const&) = default;
		Tron& operator=(Tron&&) noexcept = default;

		virtual void run() override;

	private:
		EventListener<> mOnMainWindowCloseEvent
		{
			[this]()
			{
				mIsRunning = false;
				return false;
			}
		};

		Window mMainWindow{ "Tron", 1280, 720 };
		Renderer mRenderer{ mMainWindow, {}, Renderer::ScalingMode::aspectRatio };
		Texture mTexture{ mRenderer, { "test.ttf", 128 }, "wassup" };

		bool mIsRunning{ true };
	};
}

#endif