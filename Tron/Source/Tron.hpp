#if not defined FRONGE_EDITOR_HPP
#define FRONGE_EDITOR_HPP

#include "Fronge.hpp"

#include "Prefabs/Prefabs.hpp"

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
		fro::Renderer mRenderer{ mMainWindow, { 480, 512 } };

		std::unique_ptr<fro::Gamepad> mGamePad{};

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
				[this](fro::GamepadConnectedEvent const& event)
				{
					using namespace fro;

					if (mGamePad.get())
						return false;

					mGamePad = std::make_unique<Gamepad>(event.deviceID);

					auto const ID{ mGamePad->getID() };
					InputManager::bindActionToInput("moveRight2", GamepadAxisInput{ ID, GamepadAxis::LEFT_STICK_RIGHT });
					InputManager::bindActionToInput("moveLeft2", GamepadAxisInput{ ID, GamepadAxis::LEFT_STICK_LEFT });
					InputManager::bindActionToInput("moveUp2", GamepadAxisInput{ ID, GamepadAxis::LEFT_STICK_UP });
					InputManager::bindActionToInput("moveDown2", GamepadAxisInput{ ID, GamepadAxis::LEFT_STICK_DOWN });
					InputManager::bindActionToInput("lookRight2", GamepadAxisInput{ ID, GamepadAxis::RIGHT_STICK_RIGHT });
					InputManager::bindActionToInput("lookLeft2", GamepadAxisInput{ ID, GamepadAxis::RIGHT_STICK_LEFT });
					InputManager::bindActionToInput("lookUp2", GamepadAxisInput{ ID, GamepadAxis::RIGHT_STICK_UP });
					InputManager::bindActionToInput("lookDown2", GamepadAxisInput{ ID, GamepadAxis::RIGHT_STICK_DOWN });
					InputManager::bindActionToInput("shoot2", GamepadAxisInput{ ID, GamepadAxis::RIGHT_TRIGGER });

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

		fro::Reference<fro::Entity> mPlayer1{};
		fro::Reference<fro::Entity> mPlayer2{};
		std::vector<fro::Reference<fro::Entity>> mEnemies{};
	};
}

#endif