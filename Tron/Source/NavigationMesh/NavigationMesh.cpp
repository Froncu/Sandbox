#include "NavigationMesh.hpp"

namespace tron
{
	//std::vector<NavigationMesh::EdgeType> NavigationMesh::getEdges(
	//	std::vector<fro::Vector2<double>> const& points,
	//	std::vector<fro::Polygon<double>> const& polygons)
	//{
	//	using namespace fro;

	//	std::vector<EdgeType> edges{};

	//	if (points.empty())
	//		return edges;

	//	auto element{ points.begin() };
	//	while (true)
	//	{
	//		Vector2<double> const point1{ *element++ };

	//		if (element == points.end())
	//			break;

	//		Vector2<double> const point2{ *element };

	//		if (point1.x not_eq point2.x)
	//			continue;

	//		if (not doesIntersectAnyPolygon(point1, point2, polygons))
	//			edges.emplace_back(point1, point2);
	//	}

	//	element = points.begin();
	//	while (element not_eq points.end())
	//	{
	//		Vector2<double> const point1{ *element };

	//		element = std::find_if(element + 1, points.end(),
	//			[point1](Vector2<double> const point2)
	//			{
	//				return point1.y == point2.y;
	//			});

	//		if (element == points.end())
	//		{
	//			element = points.begin();
	//			auto closestNextElement{ points.end() };
	//			while (element not_eq points.end())
	//			{
	//				if (point1.y < element->y and (closestNextElement == points.end() or closestNextElement->y > element->y))
	//					closestNextElement = element;

	//				++element;
	//			}

	//			element = closestNextElement;
	//			continue;
	//		}

	//		Vector2<double> const point2{ *element };

	//		if (not doesIntersectAnyPolygon(point1, point2, polygons))
	//			edges.emplace_back(point1, point2);
	//	}

	//	return edges;
	//}

	std::vector<fro::Vector2<double>> NavigationMesh::getPoints(std::vector<fro::Polygon<double>> const& polygons)
	{
		using namespace fro;

		static auto constexpr vectorLess
		{
			[](Vector2<double> const v1, Vector2<double> const v2)
			{
				if (v1.x == v2.x)
					return v1.y < v2.y;

				return v1.x < v2.x;
			}
		};

		std::set<Vector2<double>, decltype(vectorLess)> points{};

		for (Polygon<double> const& polygon : polygons)
		{
			std::size_t const vertexCount{ polygon.vertices.size() };
			for (std::size_t index{}; index < vertexCount; ++index)
			{
				Vector2<double> const previousVertex{ polygon.vertices[(index == 0 ? vertexCount : index) - 1] };
				Vector2<double> const currentVertex{ polygon.vertices[index] };
				Vector2<double> const nextVertex{ polygon.vertices[(index + 1) % vertexCount] };

				Vector2<double> point;

				if (Vector2<double> previousDirection{ (currentVertex - previousVertex).getNormalized() };
					previousDirection.x and previousDirection.y)
				{
					previousDirection.x = previousDirection.x > 0 ? std::ceil(previousDirection.x) : std::floor(previousDirection.x);
					previousDirection.y = previousDirection.y > 0 ? std::ceil(previousDirection.y) : std::floor(previousDirection.y);

					point = currentVertex + previousDirection * 16;
				}
				else if (Vector2<double> currentDirection{ (nextVertex - currentVertex).getNormalized() };
					currentDirection.x and currentDirection.y)
				{
					currentDirection.x = currentDirection.x > 0 ? std::ceil(currentDirection.x) : std::floor(currentDirection.x);
					currentDirection.y = currentDirection.y > 0 ? std::ceil(currentDirection.y) : std::floor(currentDirection.y);

					point = currentVertex - currentDirection * 16;
				}
				else
				{
					Vector2<double> const perpendicular{ currentDirection.y, -currentDirection.x };
					point = currentVertex + (perpendicular - currentDirection) * 16;
				}

				point.x = std::round(point.x);
				point.y = std::round(point.y);

				if (point.x <= 16 or point.x >= 464 or
					point.y <= 16 or point.y >= 432)
					continue;

				points.insert(point);
			}
		}

		return { points.begin(), points.end() };
	}

	std::vector<NavigationMesh::NodeType> NavigationMesh::calculateNodes(std::vector<fro::Polygon<double>> const& polygons)
	{
		using namespace fro;

		std::vector<Vector2<double>> points{ getPoints(polygons) };

		if (points.empty())
			return{};

		std::vector<NodeType> nodes{};
		nodes.reserve(points.size());

		auto const getClosestInDirection
		{
			[&points, &polygons](Vector2<double> const point1, Vector2<double> const direction)
			{
				std::size_t closestPointIndex{ points.size() };
				double smallestMagnitude{ std::numeric_limits<double>().infinity() };

				for (std::size_t index{}; index < points.size(); ++index)
				{
					Vector2<double> const point2{ points[index] };

					if (point1 == point2)
						continue;

					Vector2<double> const point1Point2{ point2 - point1 };
					double const magnitude{ point1Point2.getMagnitude() };

					if (magnitude >= smallestMagnitude)
						continue;

					double dot{ direction * (point1Point2 / magnitude) };
					if (std::abs(dot - 1.0) > std::numeric_limits<double>().epsilon())
						continue;

					if (doesIntersectAnyPolygon(point1, point2, polygons))
						continue;

					closestPointIndex = index;
					smallestMagnitude = magnitude;
				}

				return closestPointIndex;
			}
		};

		for (Vector2<double> const point : points)
		{
			std::vector<std::size_t> directions{};

			std::size_t availableIndex{ getClosestInDirection(point, { 1.0, 0.0 }) };
			if (availableIndex not_eq points.size())
				directions.push_back(availableIndex);

			availableIndex = getClosestInDirection(point, { 0.0, 1.0 });
			if (availableIndex not_eq points.size())
				directions.push_back(availableIndex);

			availableIndex = getClosestInDirection(point, { -1.0, 0.0 });
			if (availableIndex not_eq points.size())
				directions.push_back(availableIndex);

			availableIndex = getClosestInDirection(point, { 0.0, -1.0 });
			if (availableIndex not_eq points.size())
				directions.push_back(availableIndex);


			nodes.emplace_back(point, std::move(directions));
		}

		return nodes;
	}

	bool NavigationMesh::doLinesIntersect(fro::Vector2<double> const p1,
		fro::Vector2<double> const p2,
		fro::Vector2<double> const q1,
		fro::Vector2<double> const q2)
	{
		// NOTE: I completely forgot how line intersection detection works,
		// so I have no idea what's happening here no time left to dig into it,
		// thanks ChatGPT!

		using namespace fro;

		Vector2<double> const p2p1{ p1 - p2 };
		Vector2<double> const q2q1{ q1 - q2 };
		Vector2<double> const p1q1{ q1 - p1 };

		double const cross1{ p2p1.getCross(q2q1) };
		double const cross2{ p2p1.getCross(p1q1) };
		double const cross3{ q2q1.getCross(p1q1) };

		double constexpr epsilon{ std::numeric_limits<double>::epsilon() };

		if (std::abs(cross1) < epsilon)
		{
			auto const isBetween
			{
				[](double const a, double const b, double const c)
				{
					return std::min(a, b) <= c and c <= std::max(a, b);
				}
			};

			return
				(isBetween(p1.x, p2.x, q1.x) and isBetween(p1.y, p2.y, q1.y)) or
				(isBetween(p1.x, p2.x, q2.x) and isBetween(p1.y, p2.y, q2.y)) or
				(isBetween(q1.x, q2.x, p1.x) and isBetween(q1.y, q2.y, p1.y)) or
				(isBetween(q1.x, q2.x, p2.x) and isBetween(q1.y, q2.y, p2.y));
		}
		else
		{
			double const inverseCross1{ 1.0 / cross1 };
			double const t{ cross3 * inverseCross1 };
			double const u{ cross2 * inverseCross1 };

			return
				t >= 0.0 and
				t <= 1.0 and
				u >= 0.0 and
				u <= 1.0;
		}
	}

	bool NavigationMesh::doesIntersectPolygon(fro::Vector2<double> const p1,
		fro::Vector2<double> const p2,
		fro::Polygon<double> const& polygon)
	{
		std::size_t const vertexCount{ polygon.vertices.size() };

		for (std::size_t index{}; index < vertexCount; ++index)
		{
			using namespace fro;

			if (doLinesIntersect(p1,
				p2,
				polygon.vertices[index],
				polygon.vertices[(index + 1) % vertexCount]))
				return true;
		}

		return false;
	}

	bool NavigationMesh::doesIntersectAnyPolygon(fro::Vector2<double> const p1,
		fro::Vector2<double> const p2,
		std::vector<fro::Polygon<double>> const& polygons)
	{
		using namespace fro;

		for (Polygon<double> const& polygon : polygons)
			if (doesIntersectPolygon(p1, p2, polygon))
				return true;

		return false;
	}


	NavigationMesh::NavigationMesh(std::vector<fro::Polygon<double>> const& polygons)
		: mNodes{ calculateNodes(polygons) }
	{
	}

	std::vector<NavigationMesh::NodeType> const& NavigationMesh::getNodes() const
	{
		return mNodes;
	}

	void NavigationMesh::translate(fro::Vector2<double> const translation)
	{
		for (auto&& [point, availableIndices] : mNodes)
			point += translation;
	}
}