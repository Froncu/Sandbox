#include "EnemyMoveAISystem.hpp"

namespace tron
{
	void EnemyMoveAISystem::onUpdate(double const deltaSeconds)
	{
		for (auto&& [entity, transform, navigator, enemyMoveAI] : sGroup)
		{
			auto const navigationMesh{ navigator->getNavigationMesh() };
			if (not navigationMesh.valid())
				continue;

			auto const& nodes{ navigationMesh->getNodes() };
			if (nodes.empty())
				continue;

			double& elapsedSeconds{ enemyMoveAI->elapsedSeconds };
			elapsedSeconds += deltaSeconds;

			double const maxElapsedSeconds{ enemyMoveAI->maxElapsedSeconds };

			fro::Vector2<double> const translation{ transform->world().getTranslation() };
			fro::Vector2<double>& targetPosition{ enemyMoveAI->targetPosition };
			auto const& target{ enemyMoveAI->target };

			if (double constexpr epsilon{ 16.0 };
				elapsedSeconds >= maxElapsedSeconds or 
				((targetPosition - translation).getMagnitude() < epsilon and target.valid()))
			{
				elapsedSeconds = 0.0;
				targetPosition = target->world().getTranslation();
			}

			if (navigator->isOnNode())
			{
				auto const& currentNode{ nodes[navigator->getClosestNodeIndex()] };
				auto const& availableIndices{ currentNode.second };
				fro::Vector2<double> const direction{ (targetPosition - translation).normalize() };

				std::size_t const bestIndex
				{
					*std::max_element(availableIndices.begin(), availableIndices.end(),
						[&nodes, currentNode, direction](std::size_t const index1, std::size_t const index2)
						{
							return
								(nodes[index1].first - currentNode.first).normalize() * direction <
								(nodes[index2].first - currentNode.first).normalize() * direction;
						})
				};

				navigator->mDesiredDirection =
					(nodes[bestIndex].first - translation).normalize();
			}
		}
	}

	fro::Group<fro::Transform, Navigator, EnemyMoveAI> EnemyMoveAISystem::sGroup{};

}