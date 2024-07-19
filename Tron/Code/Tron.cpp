#include "Tron.hpp"

#if defined FRO_DEBUG
	#include <vld.h>
#endif

namespace fro
{
	std::unique_ptr<fro::Application> createApplication()
	{
		return std::make_unique<tron::Tron>();
	}
}

namespace tron
{
	Tron::Tron()
	{
		using namespace fro;

		mMainWindow.mCloseEvent.addListener(mOnMainWindowCloseEvent);
		SystemEventManager::mInputEvent.addListener(mOnInputEvent);

		InputManager::bindActionToInput("up", Key::S);
		InputManager::bindActionToInput("down", Key::W);
		InputManager::bindActionToInput("left", Key::A);
		InputManager::bindActionToInput("right", Key::D);

		Logger::info("created Tron!");
	}

	Tron::~Tron()
	{
		fro::Logger::info("destroyed Tron!");
	}

	void Tron::run()
	{
		using namespace fro;

		static auto t1{ std::chrono::steady_clock::now() };
		while (mIsRunning)
		{
			auto const t2{ std::chrono::steady_clock::now() };
			double const deltaSeconds{ std::chrono::duration<double>(t2 - t1).count() };
			t1 = t2;

			InputManager::processInputContinous();
			SystemEventManager::pollEvents();

			Vector2<double> strength{ InputManager::getActionStrengthAxis2D("right", "left", "up", "down") };
			mTextureTransformation = mTextureTransformation * math::createTranslator(strength * deltaSeconds * 64);

			mRenderer.clear();
			mRenderer.renderTexture(mTexture, mTextureTransformation);
			mRenderer.present();
		}
	}
}