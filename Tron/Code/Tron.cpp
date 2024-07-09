#include "Tron.hpp"

#if defined FRO_DEBUG
	#include <vld.h>
#endif

namespace fro
{
	std::unique_ptr<Application> createApplication()
	{
		fro::Logger::info("Tron created!");

		Audio::playMusic({ "test.mp3" });

		return std::make_unique<Tron>();
	}

	Tron::Tron()
	{
		mMainWindow.mWindowCloseEvent.addListener(mOnMainWindowCloseEvent);
	}

	void Tron::run()
	{
		while (mIsRunning)
		{
			GlobalEventManager::pollEvents();
			mRenderer.clear(0.4f, 0.4f, 0.4f);
			mRenderer.renderTexture(mTexture, math::createTranslator(32.0, 32.0), { 8, 0, 0, 16 });
			mRenderer.present();
		}
	}
}