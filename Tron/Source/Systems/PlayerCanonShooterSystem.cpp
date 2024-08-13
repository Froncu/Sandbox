#include "Components/BulletBouncer.hpp"
#include "PlayerCanonShooterSystem.hpp"
#include "Prefabs/Prefabs.hpp"

namespace tron
{
	void PlayerCanonShooterSystem::onUpdate(double const deltaSeconds)
	{
		for (auto&& [entity, playerCanonShooter] : sGroup)
		{
			playerCanonShooter->mElapsedSecondsSinceLastShot += deltaSeconds;

			if (not playerCanonShooter->mFire)
				continue;

			playerCanonShooter->mFire = false;

			if (playerCanonShooter->mElapsedSecondsSinceLastShot < playerCanonShooter->mCooldown)
			{
				if (playerCanonShooter->mConsecutiveShotsCount == 3)
					continue;

				++playerCanonShooter->mConsecutiveShotsCount;
			}
			else
				playerCanonShooter->mConsecutiveShotsCount = 1;

			playerCanonShooter->mElapsedSecondsSinceLastShot = 0.0;

			fro::Transform* const canonTransform{ playerCanonShooter->getParentingEntity()->findComponent<fro::Transform>() };
			playerCanonShooter->getParentingEntity()->getParentingScene()->addEntity(prefabs::redTankBullet(canonTransform));
		}
	}


	fro::Group<PlayerCanonShooter> PlayerCanonShooterSystem::sGroup{};
}