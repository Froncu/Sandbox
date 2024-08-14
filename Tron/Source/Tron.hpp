#if not defined FRONGE_EDITOR_HPP
#define FRONGE_EDITOR_HPP

#include "Fronge.hpp"

namespace tron
{
	class Tron final : public fro::Application
	{
	public:
		enum class Mode
		{
			SINGLE,
			COOP,
			VERSUS
		};

		enum class State
		{
			MENU,
			LOADING,
			PLAYING,
			LOST,
			WON,
			END
		};

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

		std::unique_ptr<fro::Gamepad> mGamepad1{};
		std::unique_ptr<fro::Gamepad> mGamepad2{};

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

					auto& gamepadToSet{ mGamepad1.get() ? mGamepad2 : mGamepad1 };
					if (gamepadToSet.get())
						return false;

					gamepadToSet = std::make_unique<Gamepad>(event.deviceID);

					auto const ID{ gamepadToSet->getID() };

					std::string const number{ std::to_string(&gamepadToSet == &mGamepad1 ? 2 : 1) };
					InputManager::bindActionToInput("moveRight" + number, GamepadAxisInput{ ID, GamepadAxis::LEFT_STICK_RIGHT });
					InputManager::bindActionToInput("moveLeft" + number, GamepadAxisInput{ ID, GamepadAxis::LEFT_STICK_LEFT });
					InputManager::bindActionToInput("moveUp" + number, GamepadAxisInput{ ID, GamepadAxis::LEFT_STICK_UP });
					InputManager::bindActionToInput("moveDown" + number, GamepadAxisInput{ ID, GamepadAxis::LEFT_STICK_DOWN });
					InputManager::bindActionToInput("lookRight" + number, GamepadAxisInput{ ID, GamepadAxis::RIGHT_STICK_RIGHT });
					InputManager::bindActionToInput("lookLeft" + number, GamepadAxisInput{ ID, GamepadAxis::RIGHT_STICK_LEFT });
					InputManager::bindActionToInput("lookUp" + number, GamepadAxisInput{ ID, GamepadAxis::RIGHT_STICK_UP });
					InputManager::bindActionToInput("lookDown" + number, GamepadAxisInput{ ID, GamepadAxis::RIGHT_STICK_DOWN });
					InputManager::bindActionToInput("shoot" + number, GamepadAxisInput{ ID, GamepadAxis::RIGHT_TRIGGER });

					return true;
				},

				[this](fro::GamepadDisconnectedEvent const& event)
				{
					if (mGamepad1.get() and event.ID == mGamepad1->getID())
						mGamepad1.reset();

					else if (mGamepad2.get() and event.ID == mGamepad2->getID())
						mGamepad2.reset();

					else
						return false;

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