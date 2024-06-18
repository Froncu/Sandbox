#include "TestLayer2.hpp"

namespace fro
{
	void TestLayer2::onAttach()
	{
		Logger::info("attached!");
	}

	void TestLayer2::onDetach()
	{
		Logger::info("detached!");
	}

	void TestLayer2::update()
	{
		Logger::info("update!");
	}

	void TestLayer2::onEvent(Event& event)
	{
		Logger::info("Test layer 2 received: {}!", event.getLogString());
	}
}