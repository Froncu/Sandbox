#if not defined PREFABS_HPP
#define PREFAB_HPP

#include "Fronge.hpp"

namespace tron
{
	class NavigationMesh;

	namespace prefabs
	{
		FRO_NODISCARD fro::Entity level(std::size_t const which);

		FRO_NODISCARD fro::Entity redTank(fro::Vector2<double> const position);
		FRO_NODISCARD fro::Entity canon(fro::Reference<fro::Transform> redTankTransform);

		FRO_NODISCARD fro::Entity blueTankAI(fro::Vector2<double> const position, fro::Reference<fro::Transform> const redTankTransform);
	}
}

#endif