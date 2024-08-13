#if not defined NAVIGATION_MESH_HPP
#define NAVIGATION_MESH_HPP

#include "Fronge.hpp"

namespace tron
{
	class NavigationMesh final : public fro::Referencable
	{
	public:
		using NodeType = std::pair<fro::Vector2<double>, std::vector<std::size_t>>;

		NavigationMesh() = default;
		NavigationMesh(std::vector<fro::Polygon<double>> const& polygons);
		NavigationMesh(NavigationMesh const&) = default;
		NavigationMesh(NavigationMesh&&) noexcept = default;

		~NavigationMesh() = default;

		NavigationMesh& operator=(NavigationMesh const&) = default;
		NavigationMesh& operator=(NavigationMesh&&) noexcept = default;

		FRO_NODISCARD std::vector<NodeType> const& getNodes() const;

		FRO_NODISCARD void translate(fro::Vector2<double> const translation);

	private:
		FRO_NODISCARD static std::vector<NodeType> calculateNodes(std::vector<fro::Polygon<double>> const& polygons);

		FRO_NODISCARD static std::vector<fro::Vector2<double>> getPoints(std::vector<fro::Polygon<double>> const& polygons);

		FRO_NODISCARD static bool doLinesIntersect(fro::Vector2<double> const p1,
			fro::Vector2<double> const p2,
			fro::Vector2<double> const q1,
			fro::Vector2<double> const q2);

		FRO_NODISCARD static bool doesIntersectPolygon(fro::Vector2<double> const p1,
			fro::Vector2<double> const p2,
			fro::Polygon<double> const& polygon);

		FRO_NODISCARD static bool doesIntersectAnyPolygon(fro::Vector2<double> const p1,
			fro::Vector2<double> const p2,
			std::vector<fro::Polygon<double>> const& polygons);

		std::vector<NodeType> mNodes{};
	};
}

#endif