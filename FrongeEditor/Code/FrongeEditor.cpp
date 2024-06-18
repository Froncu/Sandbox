#include "FrongeEditor.hpp"

#include "TestLayer.hpp"
#include "TestLayer2.hpp"

namespace fro
{
	std::unique_ptr<Application> createApplication()
	{
		fro::Logger::info("FrongeEditor created!");
		return std::make_unique<FrongeEditor>();
	}

	FrongeEditor::FrongeEditor()
	{
		GlobalEventManager::mSystemEvent.addListener(mOnEvent);
		mMainWindow.mWindowCloseEvent.addListener(mOnMainWindowCloseEvent);

		mLayerStack.pushLayer(std::make_unique<TestLayer>());
		mLayerStack.pushLayer(std::make_unique<TestLayer2>());
	}

	void FrongeEditor::run()
	{
		while (mIsRunning)
			GlobalEventManager::pollEvents();
	}
}