#include "Components/PlayerCanonShooter.hpp"
#include "Components/EnemyMoveAI.hpp"
#include "Systems/EnemyMoveAISystem.hpp"
#include "Systems/NavigatorSystem.hpp"
#include "Systems/PlayerCanonShooterSystem.hpp"
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

		InputManager::bindActionToInput("moveRight", Key::D);
		InputManager::bindActionToInput("moveLeft", Key::A);
		InputManager::bindActionToInput("moveUp", Key::W);
		InputManager::bindActionToInput("moveDown", Key::S);
		InputManager::bindActionToInput("lookRight", Key::RIGHT);
		InputManager::bindActionToInput("lookLeft", Key::LEFT);
		InputManager::bindActionToInput("lookUp", Key::UP);
		InputManager::bindActionToInput("lookDown", Key::DOWN);
		InputManager::bindActionToInput("shoot", Key::SPACE);

		PhysicsSystem::setGravity({});

		ResourceManager::store<Texture>("redTank", mRenderer, Surface{ "Data/Sprites/redTank.png" });
		ResourceManager::store<Texture>("blueTank", mRenderer, Surface{ "Data/Sprites/blueTank.png" });
		ResourceManager::store<Texture>("canon", mRenderer, Surface{ "Data/Sprites/canon.png" });
		ResourceManager::store<Texture>("bullet", mRenderer, Surface{ "Data/Sprites/bullet.png" });
		ResourceManager::store<Texture>("level1", mRenderer, Surface{ "Data/Sprites/level1.png" });
		ResourceManager::store<Texture>("level2", mRenderer, Surface{ "Data/Sprites/level2.png" });
		ResourceManager::store<Texture>("level3", mRenderer, Surface{ "Data/Sprites/level3.png" });

		ResourceManager::store<NavigationMesh>("level1Mesh", SVGParser::parse("Data/SVGs/level1.svg")).translate({ 0.0, 64.0 });
		ResourceManager::store<NavigationMesh>("level2Mesh", SVGParser::parse("Data/SVGs/level2.svg")).translate({ 0.0, 64.0 });
		ResourceManager::store<NavigationMesh>("level3Mesh", SVGParser::parse("Data/SVGs/level3.svg")).translate({ 0.0, 64.0 });

		mScene1->addEntity(prefabs::level(1));
		mScene2->addEntity(prefabs::level(2));
		mScene3->addEntity(prefabs::level(3));

		*mTank = prefabs::redTank({});

		Reference<Transform> const transform{ *mTank->findComponent<Transform>() };
		transform->setLocalTranslation({ 240, 256 });
		*mCanon = prefabs::canon(transform);

		mTank->findComponent<Navigator>()->setNavigationMesh(*ResourceManager::find<NavigationMesh>("level1Mesh"));

		mEnemies.emplace_back(mScene1->addEntity(prefabs::blueTankAI({ 0, 0 }, transform)))->
			findComponent<Navigator>()->setNavigationMesh(*ResourceManager::find<NavigationMesh>("level1Mesh"));
		
		mEnemies.emplace_back(mScene1->addEntity(prefabs::blueTankAI({ 600, 600 }, transform)))->
			findComponent<Navigator>()->setNavigationMesh(*ResourceManager::find<NavigationMesh>("level1Mesh"));
		
		mEnemies.emplace_back(mScene1->addEntity(prefabs::blueTankAI({ 100, 400 }, transform)))->
			findComponent<Navigator>()->setNavigationMesh(*ResourceManager::find<NavigationMesh>("level1Mesh"));

		SceneManager::setActiveScene(*mScene1);

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
				auto activeScene{ fro::SceneManager::getActiveScene() };

				mScene1->addEntity(activeScene->removeEntity(*mTank).value());
				mScene1->addEntity(activeScene->removeEntity(*mCanon).value());
				mTank->findComponent<Navigator>()->setNavigationMesh(*fro::ResourceManager::find<NavigationMesh>("level1Mesh"));

				fro::SceneManager::setActiveScene(*mScene1);
			}

			if (fro::InputManager::isInputJustPressed(fro::Key::NUMBER_2))
			{
				auto activeScene{ fro::SceneManager::getActiveScene() };

				mScene2->addEntity(activeScene->removeEntity(*mTank).value());
				mScene2->addEntity(activeScene->removeEntity(*mCanon).value());
				mTank->findComponent<Navigator>()->setNavigationMesh(*fro::ResourceManager::find<NavigationMesh>("level2Mesh"));

				fro::SceneManager::setActiveScene(*mScene2);
			}

			if (fro::InputManager::isInputJustPressed(fro::Key::NUMBER_3))
			{
				auto activeScene{ fro::SceneManager::getActiveScene() };

				mScene3->addEntity(activeScene->removeEntity(*mTank).value());
				mScene3->addEntity(activeScene->removeEntity(*mCanon).value());
				mTank->findComponent<Navigator>()->setNavigationMesh(*fro::ResourceManager::find<NavigationMesh>("level3Mesh"));

				fro::SceneManager::setActiveScene(*mScene3);
			}

			auto const moveDirection{ fro::InputManager::getActionStrengthAxis2D("moveRight", "moveLeft", "moveDown", "moveUp") };
			mTank->findComponent<Navigator>()->mDesiredDirection = moveDirection;

			auto lookDirection{ fro::InputManager::getActionStrengthAxis2D("lookRight", "lookLeft", "lookDown", "lookUp") };
			if (lookDirection.x or lookDirection.y)
			{
				auto const canonTransform{ mCanon->findComponent<fro::Transform>() };
				canonTransform->setWorldRotation(std::atan2(lookDirection.y, lookDirection.x));
			}

			if (fro::InputManager::isActionJustPressed("shoot"))
				mCanon->findComponent<PlayerCanonShooter>()->mFire = true;

			fixedUpdateAccumulator += deltaSeconds;
			while (fixedUpdateAccumulator >= fixedDeltaSeconds)
			{
				fixedUpdateAccumulator -= fixedDeltaSeconds;
				fro::PhysicsSystem::onFixedUpdate(fixedDeltaSeconds);
				NavigatorSystem::onFixedUpdate(deltaSeconds);
			}

			fro::SpriteAnimatorSystem::onUpdate(deltaSeconds);
			PlayerCanonShooterSystem::onUpdate(deltaSeconds);

			EnemyMoveAISystem::onUpdate(deltaSeconds);

			mRenderer.clear();
			fro::SpriteSystem::onRender(mRenderer);
			mRenderer.present();

			fro::SceneManager::doomAndAdd();
		}
	}
}