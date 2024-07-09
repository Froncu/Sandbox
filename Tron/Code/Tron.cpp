#include "Tron.hpp"

#include <numeric>

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
		auto t1{ std::chrono::steady_clock::now() };
		std::vector<double> frameTimes{};
		double elapsedSeconds{};
		while (mIsRunning)
		{
			auto const t2{ std::chrono::steady_clock::now() };
			double const deltaSeconds{ std::chrono::duration<double>(t2 - t1).count() };
			frameTimes.push_back(deltaSeconds);
			t1 = t2;

			elapsedSeconds += deltaSeconds;
			if (elapsedSeconds >= 1.0)
			{
				elapsedSeconds = 0;
				Logger::info(1.0 / (std::reduce(frameTimes.begin(), frameTimes.end()) / frameTimes.size()));
				frameTimes.clear();
			}

			GlobalEventManager::pollEvents();
			mRenderer.clear(0.4f, 0.4f, 0.4f);
			mRenderer.renderTexture(mTexture, math::createTranslator(32.0, 32.0), { 8, 0, 0, 16 });
			mRenderer.present();
		}
	}
}