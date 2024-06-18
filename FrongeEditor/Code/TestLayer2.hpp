#if not defined TEST_LAYER2_HPP
#define TEST_LAYER2_HPP

#include "Fronge.hpp"

namespace fro
{
	class TestLayer2 final : public Layer
	{
	public:
		TestLayer2() = default;
		TestLayer2(TestLayer2 const&) = default;
		TestLayer2(TestLayer2&&) noexcept = default;

		virtual ~TestLayer2() override = default;

		TestLayer2& operator=(TestLayer2 const&) = default;
		TestLayer2& operator=(TestLayer2&&) noexcept = default;

		virtual void onAttach() override;
		virtual void onDetach() override;

		virtual void update() override;
		virtual void onEvent(Event& event) override;
	};
}

#endif