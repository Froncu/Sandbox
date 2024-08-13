#include "Navigator.hpp"

namespace tron
{
	void Navigator::setNavigationMesh(NavigationMesh const& navigationMesh)
	{
		mNavigationMesh = navigationMesh;
		mNodeIndex1 = INVALID_NODE_INDEX;
		mNodeIndex2 = INVALID_NODE_INDEX;
	}

	fro::Reference<NavigationMesh const> Navigator::getNavigationMesh() const
	{
		return mNavigationMesh;
	}

	std::size_t Navigator::getClosestNodeIndex() const
	{
		return mNodeIndex1;
	}
}