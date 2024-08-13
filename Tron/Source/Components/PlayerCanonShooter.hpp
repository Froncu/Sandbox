#if not defined PLAYER_CANON_SHOOTER_HPP
#define PLAYER_CANON_SHOOTER_HPP

#include "Fronge.hpp"

namespace tron
{
	class PlayerCanonShooter final : public fro::Component
	{
	public:
		PlayerCanonShooter() = default;
		PlayerCanonShooter(PlayerCanonShooter const&) = default;
		PlayerCanonShooter(PlayerCanonShooter&&) = default;

		virtual ~PlayerCanonShooter() override = default;

		PlayerCanonShooter& operator=(PlayerCanonShooter const&) = default;
		PlayerCanonShooter& operator=(PlayerCanonShooter&&) = default;

		fro::Reference<fro::Entity> getParentingEntity() const;

		double mCooldown{ 1.0 };
		double mElapsedSecondsSinceLastShot{};
		int mConsecutiveShotsCount{};
		bool mFire{};

	private:
		fro::EventListener<fro::Entity, fro::Component, std::type_index const> mOnComponentAttachEvent
		{
			[smartThis = fro::Reference{ this }](fro::Entity& entity, fro::Component const& component, std::type_index const&)
			{
				if (&*smartThis not_eq &component)
					return false;

				smartThis->mParentingEntity = entity;
				return true;
			}, fro::EntityManager::getComponentAttachEvent()
		};

		fro::EventListener<fro::Entity, fro::Component, std::type_index const> mOnComponentDetachEvent
		{
			[smartThis = fro::Reference{ this }](fro::Entity const&, fro::Component const& component, std::type_index const&)
			{
				if (&*smartThis not_eq &component)
					return false;

				smartThis->mParentingEntity.reset();
				return true;
			}, fro::EntityManager::getComponentDetachEvent()
		};

		fro::Reference<fro::Entity> mParentingEntity{};
	};
}

#endif