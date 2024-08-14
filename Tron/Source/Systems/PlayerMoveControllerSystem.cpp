#include "PlayerMoveControllerSystem.hpp"

namespace tron
{
	void PlayerMoveControllerSystem::onUpdate(double const)
	{
		for (auto&& [entity, navigator, controller] : sGroup)
		{
			fro::Vector2<double> const moveDirection
			{
				fro::InputManager::getActionStrengthAxis2D(
					controller->moveRightAction,
					controller->moveLeftAction,
					controller->moveDownAction,
					controller->moveUpAction)
			};

			navigator->mDesiredDirection = moveDirection;
		}
	}

	fro::Group<Navigator, PlayerMoveController> PlayerMoveControllerSystem::sGroup{};
}