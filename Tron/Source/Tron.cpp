#include "Components/PlayerCanonShooter.hpp"
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

		ResourceManager::store<Texture>("redTank", mRenderer, Surface{ "Data/redTank.png" });
		ResourceManager::store<Texture>("blueTank", mRenderer, Surface{ "Data/blueTank.png" });
		ResourceManager::store<Texture>("canon", mRenderer, Surface{ "Data/canon.png" });
		ResourceManager::store<Texture>("bullet", mRenderer, Surface{ "Data/bullet.png" });
		ResourceManager::store<Texture>("level1", mRenderer, Surface{ "Data/level1.png" });
		ResourceManager::store<Texture>("level2", mRenderer, Surface{ "Data/level2.png" });
		ResourceManager::store<Texture>("level3", mRenderer, Surface{ "Data/level3.png" });

		SceneManager::setActiveScene(*mScene1);

		{
			auto worldBorder{ &mScene1->addEntity() };
			worldBorder->attachComponent<Sprite>()->texture = ResourceManager::find<Texture>("level1");
			worldBorder->findComponent<Sprite>()->layer = 0;
			worldBorder->attachComponent<Transform>()->setLocalTranslation({ mRenderer.getResolution().x / 2.0, mRenderer.getResolution().y / 2.0 + 32 });
			auto rigidbody{ worldBorder->attachComponent<Rigidbody>() };
			rigidbody->setType(Rigidbody::Type::STATIC);

			auto worldColliders{ SVGParser::parse("Data/level1.svg") };
			ResourceManager::store<NavigationMesh>("level1Mesh", worldColliders).translate({ 0.0, 64.0 });

			for (Polygon<double>& polygon : worldColliders)
			{
				auto collider{ &rigidbody->addCollider() };
				for (Vector2<double>& vertex : polygon.vertices)
					vertex -= Vector2<double>{ 240, 224 };

				collider->setShape(polygon);
				collider->setRestitution(1.0);
				collider->setFriction(0.0);
			};

			worldBorder = &mScene2->addEntity();
			worldBorder->attachComponent<Sprite>()->texture = ResourceManager::find<Texture>("level2");
			worldBorder->findComponent<Sprite>()->layer = 0;
			worldBorder->attachComponent<Transform>()->setLocalTranslation({ mRenderer.getResolution().x / 2.0, mRenderer.getResolution().y / 2.0 + 32 });
			rigidbody = worldBorder->attachComponent<Rigidbody>();
			rigidbody->setType(Rigidbody::Type::STATIC);

			worldColliders = SVGParser::parse("Data/level2.svg");
			ResourceManager::store<NavigationMesh>("level2Mesh", worldColliders).translate({ 0.0, 64.0 });

			for (Polygon<double>& polygon : worldColliders)
			{
				auto collider{ &rigidbody->addCollider() };
				for (Vector2<double>& vertex : polygon.vertices)
					vertex -= Vector2<double>{ 240, 224 };

				collider->setShape(polygon);
				collider->setRestitution(1.0);
				collider->setFriction(0.0);
			};

			worldBorder = &mScene3->addEntity();
			worldBorder->attachComponent<Sprite>()->texture = ResourceManager::find<Texture>("level3");
			worldBorder->findComponent<Sprite>()->layer = 0;
			worldBorder->attachComponent<Transform>()->setLocalTranslation({ mRenderer.getResolution().x / 2.0, mRenderer.getResolution().y / 2.0 + 32 });
			rigidbody = worldBorder->attachComponent<Rigidbody>();
			rigidbody->setType(Rigidbody::Type::STATIC);

			worldColliders = SVGParser::parse("Data/level3.svg");
			ResourceManager::store<NavigationMesh>("level3Mesh", worldColliders).translate({ 0.0, 64.0 });

			for (Polygon<double>& polygon : worldColliders)
			{
				auto collider{ &rigidbody->addCollider() };
				for (Vector2<double>& vertex : polygon.vertices)
					vertex -= Vector2<double>{ 240, 224 };

				collider->setShape(polygon);
				collider->setRestitution(1.0);
				collider->setFriction(0.0);
			};
		}

		mTank->attachComponent<Navigator>()->setNavigationMesh(*ResourceManager::find<NavigationMesh>("level1Mesh"));
		mTank->attachComponent<Transform>()->setLocalTranslation({ mRenderer.getResolution().x / 2.0, mRenderer.getResolution().y / 2.0 });
		mTank->attachComponent<Sprite>()->texture = ResourceManager::find<Texture>("redTank");
		mTank->findComponent<Sprite>()->layer = 1;
		auto rigidbody{ mTank->attachComponent<Rigidbody>() };
		rigidbody->setType(Rigidbody::Type::DYNAMIC);
		auto collider{ &rigidbody->addCollider() };
		collider->setShape(Circle<double>{ { -3.0, 0.0 }, 16.0 });
		collider->setDensity(1.0);
		collider->setSensor(true);

		mCanon->attachComponent<Transform>()->setParent(mTank->findComponent<Transform>(), false);
		mCanon->findComponent<Transform>()->setLocalTranslation({ -3.0, 0.0 });
		mCanon->attachComponent<Sprite>()->texture = ResourceManager::find<Texture>("canon");
		mCanon->findComponent<Sprite>()->layer = 3;
		mCanon->attachComponent<PlayerCanonShooter>();

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
			mTank->findComponent<Navigator>()->desiredDirection = moveDirection;

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

			mRenderer.clear();
			fro::SpriteSystem::onRender(mRenderer);
			mRenderer.present();

			fro::SceneManager::doomAndAdd();
		}
	}
}