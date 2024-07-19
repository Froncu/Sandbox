#if not defined FRONGE_EDITOR_HPP
#define FRONGE_EDITOR_HPP

#include "Fronge.hpp"

namespace tron
{
	class Tron final : public fro::Application
	{
	public:
		Tron();
		Tron(Tron const&) = default;
		Tron(Tron&&) noexcept = default;

		virtual ~Tron() override;

		Tron& operator=(Tron const&) = default;
		Tron& operator=(Tron&&) noexcept = default;

		virtual void run() override;

	private:
		fro::EventListener<> mOnMainWindowCloseEvent
		{
			[this]()
			{
				mIsRunning = false;
				return true;
			}
		};

		fro::EventListener<fro::InputEvent const> mOnInputEvent
		{
			fro::VariantVisitor
			{
				[this](fro::KeyDownEvent const& event)
				{
					using namespace fro;

					switch (event.key)
					{
					case Key::NUMBER_1:
						mRenderer.setScalingMode(Renderer::ScalingMode::NONE);
						return true;

					case Key::NUMBER_2:
						mRenderer.setScalingMode(Renderer::ScalingMode::FILL);
						return true;

					case Key::NUMBER_3:
						mRenderer.setScalingMode(Renderer::ScalingMode::ASPECT_RATIO);
						return true;

					case Key::NUMBER_8:
						mMainWindow.setMode(Window::Mode::WINDOWED);
						return true;

					case Key::NUMBER_9:
						mMainWindow.setMode(Window::Mode::BORDERLESS);
						return true;

					case Key::NUMBER_0:
						mMainWindow.setMode(Window::Mode::FULLSCREEN);
						return true;
					}

					return false;
				},

				[](auto&&)
				{
					return false;
				}
			}
		};

		fro::Window mMainWindow{ "Tron", { 1280, 720 } };
		fro::Renderer mRenderer{ mMainWindow, { 512, 480 } };

		fro::Matrix3x3<double> mTextureTransformation{ fro::math::createTranslator(mRenderer.getResolution() / 2.0) };

		fro::Texture mTexture{ mRenderer, { "test.png" } };
		
		bool mIsRunning{ true };
	};
}

#endif