#if not defined TEST_LAYER_HPP
#define TEST_LAYER_HPP

#include "Fronge.hpp"

namespace fro
{
	class TestLayer final : public Layer
	{
	public:
		TestLayer() = default;
		TestLayer(TestLayer const&) = default;
		TestLayer(TestLayer&&) noexcept = default;

		virtual ~TestLayer() override = default;

		TestLayer& operator=(TestLayer const&) = default;
		TestLayer& operator=(TestLayer&&) noexcept = default;

		virtual void onAttach() override;
		virtual void onDetach() override;

		virtual void update() override;
		virtual void onEvent(Event& event) override;
	};
}

#endif