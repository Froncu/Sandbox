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

		InputManager::bindActionToInput("rotateLeft1", Key::Q);
		InputManager::bindActionToInput("rotateRight1", Key::E);

		InputManager::bindActionToInput("rotateLeft2", Key::U);
		InputManager::bindActionToInput("rotateRight2", Key::O);

		InputManager::bindActionToInput("right1", Key::D);
		InputManager::bindActionToInput("left1", Key::A);
		InputManager::bindActionToInput("up1", Key::W);
		InputManager::bindActionToInput("down1", Key::S);

		InputManager::bindActionToInput("right2", Key::L);
		InputManager::bindActionToInput("left2", Key::J);
		InputManager::bindActionToInput("up2", Key::I);
		InputManager::bindActionToInput("down2", Key::K);

		Logger::info("created Tron!");

		auto transform{ mEntity1.attachComponent<Transform>() };
		transform->localTranslate({ 100, 100 });
		transform->localScale({ 2.6, 1.4 });

		auto sprite{ mEntity1.attachComponent<Sprite>() };
		sprite->texture = ResourceManager::storeTexture("test", mRenderer, Surface{ "test.png" });

		transform = mEntity2.attachComponent<Transform>();
		transform->localTranslate({ 200, 100 });
		transform->setParent(mEntity1.findComponent<Transform>());

		sprite = mEntity2.attachComponent<Sprite>();
		sprite->texture = ResourceManager::storeTexture("test", mRenderer, Surface{ "test.png" });
		sprite->sourceRectangle.width = 16;
		sprite->sourceRectangle.height = 16;
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

			auto moveStrength{ InputManager::getActionStrengthAxis2D("right1", "left1", "down1", "up1") };
			mEntity1.getComponent<Transform>()->localTranslate(moveStrength * deltaSeconds * 64);

			moveStrength = InputManager::getActionStrengthAxis2D("right2", "left2", "down2", "up2");
			mEntity2.getComponent<Transform>()->localTranslate(moveStrength * deltaSeconds * 64);

			auto rotateStrength{ InputManager::getActionStrengthAxis1D("rotateRight1", "rotateLeft1") };
			mEntity1.getComponent<Transform>()->localRotate(rotateStrength * deltaSeconds * 8);

			rotateStrength = InputManager::getActionStrengthAxis1D("rotateRight2", "rotateLeft2");
			mEntity2.getComponent<Transform>()->localRotate(rotateStrength * deltaSeconds * 8);

			mRenderer.clear();
			SpriteSystem::onRender(mRenderer);
			mRenderer.present();
		}
	}
}