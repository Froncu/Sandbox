#include "BulletBouncer.hpp"
#include "PlayerCanonShooter.hpp"

namespace tron
{
	fro::Reference<fro::Entity> PlayerCanonShooter::getParentingEntity() const
	{
		return mParentingEntity;
	}
}