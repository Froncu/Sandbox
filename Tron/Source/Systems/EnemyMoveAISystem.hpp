#if not defined ENEMY_MOVE_AI_SYSTEM_HPP
#define ENEMY_MOVE_AI_SYSTEM_HPP

#include "Fronge.hpp"

#include "Components/Navigator.hpp"
#include "Components/EnemyMoveAI.hpp"

namespace tron
{
	class EnemyMoveAISystem final
	{
	public:
		static void onUpdate(double const deltaSeconds);

	private:
		static fro::Group<fro::Transform, Navigator, EnemyMoveAI> sGroup;

		EnemyMoveAISystem() = delete;
		EnemyMoveAISystem(EnemyMoveAISystem const&) = delete;
		EnemyMoveAISystem(EnemyMoveAISystem&&) noexcept = delete;

		~EnemyMoveAISystem() = delete;

		EnemyMoveAISystem& operator=(EnemyMoveAISystem const&) = delete;
		EnemyMoveAISystem& operator=(EnemyMoveAISystem&&) noexcept = delete;
	};
}

#endif