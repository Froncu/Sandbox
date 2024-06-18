#include "TestLayer.hpp"

namespace fro
{
	void TestLayer::onAttach()
	{
		Logger::info("attached!");
	}

	void TestLayer::onDetach()
	{
		Logger::info("detached!");
	}

	void TestLayer::update()
	{
		Logger::info("update!");
	}

	void TestLayer::onEvent(Event& event)
	{
		Logger::info("Test layer received: {}!", event.getLogString());
		event.markHandeled();
	}
}