#include "Sandbox.hpp"

#if defined FRO_DEBUG
#include <vld.h>
#endif

namespace fro
{
	std::unique_ptr<fro::Application> createApplication()
	{
		return std::make_unique<sbx::Sandbox>();
	}
}

namespace sbx
{
	Sandbox::Sandbox()
	{
		fro::Logger::info("created Sandbox!");
	}

	Sandbox::~Sandbox()
	{
		fro::Logger::info("destroyed Sandbox!");
	}

	void Sandbox::run()
	{
		auto constexpr fixedDeltaSeconds{ 1.0 / 60 };
		double fixedUpdateAccumulator{};

		auto t1{ std::chrono::steady_clock::now() };
		while (mIsRunning)
		{
			auto const t2{ std::chrono::steady_clock::now() };
			double const deltaSeconds{ std::chrono::duration<double>(t2 - t1).count() };
			t1 = t2;
			
			fro::InputManager::processInputContinous();
			fro::SystemEventManager::pollEvents();

			fixedUpdateAccumulator += deltaSeconds;
			while (fixedUpdateAccumulator >= fixedDeltaSeconds)
			{
				fixedUpdateAccumulator -= fixedDeltaSeconds;
				fro::PhysicsSystem::onFixedUpdate(fixedDeltaSeconds);
			}

			mRenderer.clear();
			fro::SpriteSystem::onRender(mRenderer);
			mRenderer.present();

			fro::SceneManager::doomAndAdd();
		}
	}
}