#include "FrongeEditor.hpp"

#if defined FRO_DEBUG
	#include <vld.h>
#endif

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
		{
			GlobalEventManager::pollEvents();
			mRenderer.clear(0.0f, 0.0f, 0.0f);
			mRenderer.renderTexture(mTexture, math::createTranslator(32.0f, 32.0f) * math::createRotator(math::radians(30)));
			mRenderer.present();
		}
	}
}