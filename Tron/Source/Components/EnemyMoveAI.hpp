#if not defined ENEMY_MOVE_AI_HPP
#define ENEMY_MOVE_AI_HPP

#include "Fronge.hpp"

namespace tron
{
	struct EnemyMoveAI final : public fro::Component
	{
	public:
		fro::Reference<fro::Transform> target{};
		fro::Vector2<double> targetPosition{};
		double elapsedSeconds{};
		double maxElapsedSeconds{ 10.0 };
	};
}

#endif