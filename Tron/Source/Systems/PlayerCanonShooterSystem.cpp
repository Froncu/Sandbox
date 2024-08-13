#include "Components/BulletBouncer.hpp"
#include "PlayerCanonShooterSystem.hpp"

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
					return;

				++playerCanonShooter->mConsecutiveShotsCount;
			}
			else
				playerCanonShooter->mConsecutiveShotsCount = 1;

			playerCanonShooter->mElapsedSecondsSinceLastShot = 0.0;

			fro::Entity bullet{};

			auto const canonTransform{ playerCanonShooter->getParentingEntity()->findComponent<fro::Transform>()};
			auto const canonTranslation{ canonTransform->world().getTranslation() };
			auto const canonRotation{ canonTransform->world().getRotation() };
			fro::Vector2<double> const lookDirection{ std::cos(canonRotation), std::sin(canonRotation) };

			auto const spawnTranslation{ canonTranslation + lookDirection * 20.0 };

			bullet.attachComponent<BulletBouncer>();
			bullet.attachComponent<fro::Transform>()->setLocalTranslation(spawnTranslation);
			bullet.attachComponent<fro::Sprite>()->texture = fro::ResourceManager::find<fro::Texture>("bullet");
			bullet.findComponent<fro::Sprite>()->layer = 2;
			auto const rigidbody{ bullet.attachComponent<fro::Rigidbody>() };
			rigidbody->setType(fro::Rigidbody::Type::DYNAMIC);
			rigidbody->setFixedRotation(true);
			rigidbody->setLinearVelocity(lookDirection * 256);
			auto const collider{ &rigidbody->addCollider() };
			collider->setShape(fro::Circle<double>{ {}, 2.0 });
			collider->setDensity(1.0);
			collider->setRestitution(1.0);
			collider->setGroupIndex(-1);

			playerCanonShooter->getParentingEntity()->getParentingScene()->addEntity(std::move(bullet));
		}
	}


	fro::Group<PlayerCanonShooter> PlayerCanonShooterSystem::sGroup{};
}