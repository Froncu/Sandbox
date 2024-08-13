#if not defined PLAYER_CANON_SHOOTER_SYSTEM_HPP
#define PLAYER_CANON_SHOOTER_SYSTEM_HPP

#include "Fronge.hpp"
#include "Components/PlayerCanonShooter.hpp"

namespace tron
{
	class PlayerCanonShooterSystem final
	{
	public:
		static void onUpdate(double const deltaSeconds);

	private:
		static fro::Group<PlayerCanonShooter> sGroup;

		PlayerCanonShooterSystem() = delete;
		PlayerCanonShooterSystem(PlayerCanonShooterSystem const&) = delete;
		PlayerCanonShooterSystem(PlayerCanonShooterSystem&&) noexcept = delete;

		~PlayerCanonShooterSystem() = delete;

		PlayerCanonShooterSystem& operator=(PlayerCanonShooterSystem const&) = delete;
		PlayerCanonShooterSystem& operator=(PlayerCanonShooterSystem&&) noexcept = delete;
	};
}

#endif