#include "FrongeEditor.hpp"

namespace fro
{
	std::unique_ptr<Application> createApplication()
	{
		fro::Logger::info("FrongeEditor created!");
		return std::make_unique<FrongeEditor>();
	}

	FrongeEditor::FrongeEditor()
	{
		mMainWindow.mWindowCloseEvent.addListener(mOnMainWindowCloseEvent);
		GlobalEventManager::mSystemEvent.addListener(mOnEvent);
	}

	void FrongeEditor::run()
	{
		while (mIsRunning)
			GlobalEventManager::pollEvents();
	}
}