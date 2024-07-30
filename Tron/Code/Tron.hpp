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
		fro::Window mMainWindow{ "Tron", { 1280, 720 } };
		fro::Renderer mRenderer{ mMainWindow, { 512, 480 } };

		std::unique_ptr<fro::Gamepad> mGamePad{};

		fro::Entity mEntity1{};
		fro::Entity mEntity2{};

		bool mIsRunning{ true };

		fro::EventListener<> mOnMainWindowCloseEvent
		{
			[this]()
			{
				mIsRunning = false;
				return true;
			}, mMainWindow.mCloseEvent
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

					case Key::NUMBER_9:
						mMainWindow.setFullscreen(true);
						return true;

					case Key::NUMBER_0:
						mMainWindow.setFullscreen(false);
						return true;
					}

					return false;
				},

				[this](fro::GamepadConnectedEvent const& event)
				{
					using namespace fro;
					mGamePad = std::make_unique<Gamepad>(event.deviceID);

					auto const ID{ mGamePad->getID() };
					InputManager::bindActionToInput("right", GamepadAxisInput{ ID, GamepadAxis::LEFT_STICK_RIGHT });
					InputManager::bindActionToInput("left", GamepadAxisInput{ ID, GamepadAxis::LEFT_STICK_LEFT });
					InputManager::bindActionToInput("up", GamepadAxisInput{ ID, GamepadAxis::LEFT_STICK_UP });
					InputManager::bindActionToInput("down", GamepadAxisInput{ ID, GamepadAxis::LEFT_STICK_DOWN });

					InputManager::bindActionToInput("trigger", GamepadAxisInput{ ID, GamepadAxis::LEFT_TRIGGER });
					InputManager::bindActionToInput("trigger", GamepadAxisInput{ ID, GamepadAxis::RIGHT_TRIGGER });

					return true;
				},

				[this](fro::GamepadDisconnectedEvent const& event)
				{
					if (not mGamePad.get())
						return false;

					if (event.ID not_eq mGamePad->getID())
						return false;

					mGamePad.reset();
					return true;
				},

				[](auto&&)
				{
					return false;
				}
			}, fro::SystemEventManager::mInputEvent
		};
	};
}

#endif