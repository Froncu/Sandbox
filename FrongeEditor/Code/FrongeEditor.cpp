#include "FrongeEditor.hpp"

namespace fro
{
	std::unique_ptr<Application> createApplication()
	{
		fro::Logger::info("FrongeEditor created!");

		Audio::playMusic({ "test.mp3" });

		return std::make_unique<FrongeEditor>();
	}

	FrongeEditor::FrongeEditor()
	{
		mMainWindow.mWindowCloseEvent.addListener(mOnMainWindowCloseEvent);
	}

	void FrongeEditor::run()
	{
		while (mIsRunning)
			GlobalEventManager::pollEvents();
	}
}