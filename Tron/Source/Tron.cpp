#include "Prefabs/Prefabs.hpp"
#include "Systems/AIMoveControllerSystem.hpp"
#include "Systems/AIShootControllerSystem.hpp"
#include "Systems/NavigatorSystem.hpp"
#include "Systems/PlayerCanonControllerSystem.hpp"
#include "Systems/PlayerMoveControllerSystem.hpp"
#include "Systems/PlayerShootControllerSystem.hpp"
#include "Tron.hpp"

#if defined FRO_DEBUG
#include <vld.h>
#endif

namespace fro
{
	std::unique_ptr<fro::Application> createApplication()
	{
		InputManager::bindActionToInput("moveRight1", Key::D);
		InputManager::bindActionToInput("moveLeft1", Key::A);
		InputManager::bindActionToInput("moveUp1", Key::W);
		InputManager::bindActionToInput("moveDown1", Key::S);
		InputManager::bindActionToInput("lookRight1", Key::RIGHT);
		InputManager::bindActionToInput("lookLeft1", Key::LEFT);
		InputManager::bindActionToInput("lookUp1", Key::UP);
		InputManager::bindActionToInput("lookDown1", Key::DOWN);
		InputManager::bindActionToInput("shoot1", Key::SPACE);

		PhysicsSystem::setGravity({});

		ResourceManager::store<tron::NavigationMesh>("world1Mesh", SVGParser::parse("Data/SVGs/world1.svg")).translate({ 0.0, 64.0 });
		ResourceManager::store<tron::NavigationMesh>("world2Mesh", SVGParser::parse("Data/SVGs/world2.svg")).translate({ 0.0, 64.0 });
		ResourceManager::store<tron::NavigationMesh>("world3Mesh", SVGParser::parse("Data/SVGs/world3.svg")).translate({ 0.0, 64.0 });

		return std::make_unique<tron::Tron>();
	}
}

namespace tron
{
	struct Test final
	{
		Test() = default;
		Test(Test const&) = delete;
		Test(Test&&) noexcept = delete;

		~Test() = default;

		Test& operator=(Test const&) = delete;
		Test& operator=(Test&&) noexcept = delete;
	};

	Tron::Tron()
	{
		using namespace fro;

		ResourceManager::store<Texture>("redTank", mRenderer, Surface{ "Data/Sprites/redTank.png" });
		ResourceManager::store<Texture>("greenTank", mRenderer, Surface{ "Data/Sprites/greenTank.png" });
		ResourceManager::store<Texture>("redCanon", mRenderer, Surface{ "Data/Sprites/redCanon.png" });
		ResourceManager::store<Texture>("greenCanon", mRenderer, Surface{ "Data/Sprites/greenCanon.png" });
		ResourceManager::store<Texture>("blueTank", mRenderer, Surface{ "Data/Sprites/blueTank.png" });
		ResourceManager::store<Texture>("recognizer", mRenderer, Surface{ "Data/Sprites/recognizer.png" });
		ResourceManager::store<Texture>("playerBullet", mRenderer, Surface{ "Data/Sprites/playerBullet.png" });
		ResourceManager::store<Texture>("enemyBullet", mRenderer, Surface{ "Data/Sprites/enemyBullet.png" });
		ResourceManager::store<Texture>("world1", mRenderer, Surface{ "Data/Sprites/world1.png" });
		ResourceManager::store<Texture>("world2", mRenderer, Surface{ "Data/Sprites/world2.png" });
		ResourceManager::store<Texture>("world3", mRenderer, Surface{ "Data/Sprites/world3.png" });

		Logger::info("created Tron!");
	}

	Tron::~Tron()
	{
		fro::Logger::info("destroyed Tron!");
	}

	void Tron::run()
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

			if (fro::InputManager::isInputJustPressed(fro::Key::NUMBER_1))
			{
				fro::Reference<fro::Scene const> const getActiveScene{ fro::SceneManager::getActiveScene() };
				if (getActiveScene.valid())
					fro::SceneManager::removeScene(*getActiveScene);
			
				fro::SceneManager::setActiveScene(fro::SceneManager::addScene(prefabs::level(1, Mode::SINGLE)));
			}
			
			if (fro::InputManager::isInputJustPressed(fro::Key::NUMBER_2))
			{
				fro::Reference<fro::Scene const> const getActiveScene{ fro::SceneManager::getActiveScene() };
				if (getActiveScene.valid())
					fro::SceneManager::removeScene(*getActiveScene);
			
				fro::SceneManager::setActiveScene(fro::SceneManager::addScene(prefabs::level(2, Mode::COOP)));
			}
			
			if (fro::InputManager::isInputJustPressed(fro::Key::NUMBER_3))
			{
				fro::Reference<fro::Scene const> const getActiveScene{ fro::SceneManager::getActiveScene() };
				if (getActiveScene.valid())
					fro::SceneManager::removeScene(*getActiveScene);
			
				fro::SceneManager::setActiveScene(fro::SceneManager::addScene(prefabs::level(3, Mode::VERSUS)));
			}

			fixedUpdateAccumulator += deltaSeconds;
			while (fixedUpdateAccumulator >= fixedDeltaSeconds)
			{
				fixedUpdateAccumulator -= fixedDeltaSeconds;
				fro::PhysicsSystem::onFixedUpdate(fixedDeltaSeconds);
				NavigatorSystem::onFixedUpdate(deltaSeconds);
			}

			PlayerMoveControllerSystem::onUpdate(deltaSeconds);
			PlayerCanonControllerSystem::onUpdate(deltaSeconds);
			PlayerShootControllerSystem::onUpdate(deltaSeconds);

			AIMoveControllerSystem::onUpdate(deltaSeconds);
			AIShootControllerSystem::onUpdate(deltaSeconds);

			mRenderer.clear();
			fro::SpriteSystem::onRender(mRenderer);
			mRenderer.present();

			fro::SceneManager::doomAndAdd();
		}
	}
}