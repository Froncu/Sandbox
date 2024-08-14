#if not defined PREFABS_HPP
#define PREFAB_HPP

#include "Fronge.hpp"

namespace tron
{
	class NavigationMesh;

	namespace prefabs
	{
		FRO_NODISCARD fro::Entity world(std::size_t const which);

		FRO_NODISCARD fro::Entity redTank(fro::Vector2<double> const position);
		FRO_NODISCARD fro::Entity canon(fro::Reference<fro::Transform> const redTankTransform);
		FRO_NODISCARD fro::Entity redTankBullet(fro::Reference<fro::Transform const> const canonTransform);

		FRO_NODISCARD fro::Entity blueTank(fro::Vector2<double> const position);
		FRO_NODISCARD fro::Entity blueTankAI(fro::Vector2<double> const position, fro::Reference<fro::Transform const> const redTankTransform);
		FRO_NODISCARD fro::Entity blueTankPlayer(fro::Vector2<double> const position);
		FRO_NODISCARD fro::Entity blueTankBullet(fro::Reference<fro::Transform const> const blueTankTransform);

		FRO_NODISCARD fro::Entity recognizer(fro::Vector2<double> const position, fro::Reference<fro::Transform const> const redTankTransform);

		FRO_NODISCARD fro::Scene level(std::size_t const which);
	}
}

#endif