#include "Components/AIMoveController.hpp"
#include "Components/AIShootController.hpp"
#include "Components/EnemyBulletCollider.hpp"
#include "Components/EnemyCollider.hpp"
#include "Components/Navigator.hpp"
#include "Components/PlayerBulletCollider.hpp"	
#include "Components/PlayerCanonController.hpp"
#include "Components/PlayerCollider.hpp"
#include "Components/PlayerMoveController.hpp"
#include "Components/PlayerShootController.hpp"
#include "Prefabs.hpp"

namespace tron
{
	namespace prefabs
	{
		fro::Entity world(std::size_t const which)
		{
			fro::Entity entity{};

			entity.attachComponent<fro::Transform>()->setLocalTranslation({ 240.0, 288.0 });

			fro::Sprite& sprite{ *entity.attachComponent<fro::Sprite>() };
			sprite.texture = fro::ResourceManager::find<fro::Texture>("world" + std::to_string(which));
			sprite.layer = 0;

			fro::Rigidbody& rigidbody{ *entity.attachComponent<fro::Rigidbody>() };
			rigidbody.setType(fro::Rigidbody::Type::STATIC);

			NavigationMesh const& navigationMesh{ *fro::ResourceManager::find<NavigationMesh>("world" + std::to_string(which) + "Mesh") };
			for (fro::Polygon<double> polygon : navigationMesh.getPolygons())
			{
				for (fro::Vector2<double>& vertex : polygon.vertices)
				{
					vertex.x -= 240;
					vertex.y -= 288;
				}

				fro::Collider& collider{ rigidbody.addCollider() };
				collider.setShape(polygon);
				collider.setRestitution(1.0);
				collider.setFriction(0.0);
				collider.setCategoryBits(fro::createBitfield(0ull));
				collider.setMaskBits(fro::createBitfield(1ull, 2ull, 5ull));
			};

			return entity;
		}

		fro::Entity redTank(fro::Vector2<double> const position)
		{
			fro::Entity entity{};

			entity.attachComponent<fro::Transform>()->setLocalTranslation(position);

			fro::Sprite& sprite{ *entity.attachComponent<fro::Sprite>() };
			sprite.texture = fro::ResourceManager::find<fro::Texture>("redTank");
			sprite.layer = 1;

			fro::Rigidbody& rigidbody{ *entity.attachComponent<fro::Rigidbody>() };
			rigidbody.setType(fro::Rigidbody::Type::DYNAMIC);

			fro::Collider& collider{ rigidbody.addCollider() };
			collider.setShape(fro::Circle<double>{ { -3.0, 0.0 }, 16.0 });
			collider.setDensity(1.0);
			collider.setSensor(true);
			collider.setCategoryBits(fro::createBitfield(3ull));
			collider.setMaskBits(fro::createBitfield(1ull, 2ull, 4ull, 5ull));

			entity.attachComponent<Navigator>();
			entity.attachComponent<PlayerCollider>();
			entity.attachComponent<PlayerMoveController>();

			return entity;
		}

		fro::Entity canon(fro::Reference<fro::Transform> const redTankTransform)
		{
			fro::Entity entity{};

			fro::Transform& transform{ *entity.attachComponent<fro::Transform>() };
			transform.setParent(&*redTankTransform, false);
			transform.setLocalTranslation({ -3.0, 0.0 });

			fro::Sprite& sprite{ *entity.attachComponent<fro::Sprite>() };
			sprite.texture = fro::ResourceManager::find<fro::Texture>("canon");
			sprite.layer = 3;

			entity.attachComponent<PlayerCanonController>();

			return entity;
		}

		fro::Entity redTankBullet(fro::Reference<fro::Transform const> const canonTransform)
		{
			fro::Entity entity{};

			fro::Vector2<double> const canonTranslation{ canonTransform->world().getTranslation() };
			double const canonRotation{ canonTransform->world().getRotation() };
			fro::Vector2<double> const direction{ std::cos(canonRotation), std::sin(canonRotation) };

			entity.attachComponent<fro::Transform>()->setLocalTranslation(canonTranslation + direction * 20.0);

			fro::Sprite& sprite{ *entity.attachComponent<fro::Sprite>() };
			sprite.texture = fro::ResourceManager::find<fro::Texture>("playerBullet");
			sprite.layer = 2;

			fro::Rigidbody& rigidbody{ *entity.attachComponent<fro::Rigidbody>() };
			rigidbody.setType(fro::Rigidbody::Type::DYNAMIC);
			rigidbody.setFixedRotation(true);
			rigidbody.setLinearVelocity(direction * 256.0);

			fro::Collider& collider{ rigidbody.addCollider() };
			collider.setShape(fro::Circle<double>{.radius{ 2.0 } });
			collider.setDensity(1.0);
			collider.setRestitution(1.0);
			collider.setCategoryBits(fro::createBitfield(1ull));
			collider.setMaskBits(fro::createBitfield(0ull, 3ull, 4ull));

			entity.attachComponent<PlayerBulletCollider>();

			return entity;
		}

		fro::Entity blueTank(fro::Vector2<double> const position)
		{
			fro::Entity entity{};

			entity.attachComponent<fro::Transform>()->setLocalTranslation(position);

			fro::Sprite& sprite{ *entity.attachComponent<fro::Sprite>() };
			sprite.texture = fro::ResourceManager::find<fro::Texture>("blueTank");
			sprite.layer = 1;

			fro::Rigidbody& rigidbody{ *entity.attachComponent<fro::Rigidbody>() };
			rigidbody.setType(fro::Rigidbody::Type::KINEMATIC);

			fro::Collider& collider{ rigidbody.addCollider() };
			collider.setShape(fro::Rectangle<double>{.width{ 32.0 }, .height{ 32.0 } });
			collider.setDensity(1.0);
			collider.setCategoryBits(fro::createBitfield(4ull));
			collider.setMaskBits(fro::createBitfield(1ull, 3ull));

			entity.attachComponent<Navigator>();
			entity.attachComponent<EnemyCollider>();

			return entity;
		}

		fro::Entity blueTankAI(fro::Vector2<double> const position, fro::Reference<fro::Transform const> const redTankTransform)
		{
			fro::Entity entity{ blueTank(position) };

			AIMoveController& controller{ *entity.attachComponent<AIMoveController>() };
			controller.target = redTankTransform;
			controller.targetPosition = redTankTransform->world().getTranslation();

			entity.attachComponent<AIShootController>();

			return entity;
		}

		fro::Entity blueTankPlayer(fro::Vector2<double> const position)
		{
			fro::Entity entity{ blueTank(position) };

			PlayerMoveController& controller{ *entity.attachComponent<PlayerMoveController>() };
			controller.moveRightAction = "moveRight2";
			controller.moveLeftAction = "moveLeft2";
			controller.moveUpAction = "moveUp2";
			controller.moveDownAction = "moveDown2";

			entity.attachComponent<PlayerShootController>()->shootAction = "shoot2";

			return entity;
		}

		fro::Entity blueTankBullet(fro::Reference<fro::Transform const> const blueTankTransform)
		{
			fro::Entity entity{};

			fro::Vector2<double> const tankTranslation{ blueTankTransform->world().getTranslation() };
			double const tankRotation{ blueTankTransform->world().getRotation() };
			fro::Vector2<double> const direction{ std::cos(tankRotation), std::sin(tankRotation) };

			fro::Transform& transform{ *entity.attachComponent<fro::Transform>() };
			transform.setLocalTranslation(tankTranslation + direction * 20.0);
			transform.setLocalRotation(tankRotation);

			fro::Sprite& sprite{ *entity.attachComponent<fro::Sprite>() };
			sprite.texture = fro::ResourceManager::find<fro::Texture>("enemyBullet");
			sprite.layer = 2;

			fro::Rigidbody& rigidbody{ *entity.attachComponent<fro::Rigidbody>() };
			rigidbody.setType(fro::Rigidbody::Type::DYNAMIC);
			rigidbody.setFixedRotation(true);
			rigidbody.setLinearVelocity(direction * 192.0);

			fro::Collider& collider{ rigidbody.addCollider() };
			collider.setShape(fro::Circle<double>{.radius{ 3.0 } });
			collider.setDensity(1.0);
			collider.setCategoryBits(fro::createBitfield(5ull));
			collider.setMaskBits(fro::createBitfield(0ull, 3ull));
			collider.setSensor(true);

			entity.attachComponent<EnemyBulletCollider>();

			return entity;
		}

		fro::Entity recognizer(fro::Vector2<double> const position, fro::Reference<fro::Transform const> const redTankTransform)
		{
			fro::Entity entity{};

			entity.attachComponent<fro::Transform>()->setLocalTranslation(position);

			fro::Sprite& sprite{ *entity.attachComponent<fro::Sprite>() };
			sprite.texture = fro::ResourceManager::find<fro::Texture>("recognizer");
			sprite.layer = 1;

			fro::Rigidbody& rigidbody{ *entity.attachComponent<fro::Rigidbody>() };
			rigidbody.setType(fro::Rigidbody::Type::KINEMATIC);

			fro::Collider& collider{ rigidbody.addCollider() };
			collider.setShape(fro::Rectangle<double>{ .x{ -3.0 }, .width{ 26.0 }, .height{ 32.0 } });
			collider.setDensity(1.0);
			collider.setCategoryBits(fro::createBitfield(4ull));
			collider.setMaskBits(fro::createBitfield(1ull, 3ull));

			entity.attachComponent<Navigator>()->mVelocity *= 2.0;
			entity.attachComponent<EnemyCollider>();

			AIMoveController& controller{ *entity.attachComponent<AIMoveController>() };
			controller.target = redTankTransform;
			controller.targetPosition = redTankTransform->world().getTranslation();

			return entity;
		}

		fro::Scene level(std::size_t const which)
		{
			fro::Scene scene{};

			fro::Reference<fro::Entity const> const worldEntity{ scene.addEntity(world(which)) };

			NavigationMesh const& navigationMesh{ *fro::ResourceManager::find<NavigationMesh>("world" + std::to_string(which) + "Mesh") };
			auto const& nodes{ navigationMesh.getNodes() };
			std::size_t const redTankNodeIndex{ std::rand() % nodes.size() };
			fro::Reference<fro::Entity const> const redTankEntity{ scene.addEntity(redTank(nodes[redTankNodeIndex].first)) };

			Navigator* navigator{ redTankEntity->findComponent<Navigator>() };
			navigator->setNavigationMesh(navigationMesh);

			scene.addEntity(canon(redTankEntity->findComponent<fro::Transform>()));

			fro::Reference<fro::Transform const> redTankTransform{ redTankEntity->findComponent<fro::Transform>() };

			std::unordered_set<std::size_t> takenIndices{};
			for (std::size_t i{}; i < 4 - which; ++i)
			{
				std::size_t nodeIndex;
				do
				{
					nodeIndex = std::rand() % nodes.size();
				} while (takenIndices.contains(nodeIndex) or
					(nodes[nodeIndex].first - nodes[redTankNodeIndex].first).getMagnitude() < 256.0);

				takenIndices.insert(nodeIndex);
				navigator = scene.addEntity(prefabs::blueTankAI(nodes[nodeIndex].first, redTankTransform)).findComponent<Navigator>();
				navigator->setNavigationMesh(navigationMesh);
			}

			for (std::size_t i{}; i < which; ++i)
			{
				std::size_t nodeIndex;
				do
				{
					nodeIndex = std::rand() % nodes.size();
				} while (takenIndices.contains(nodeIndex) or
					(nodes[nodeIndex].first - nodes[redTankNodeIndex].first).getMagnitude() < 256.0);

				takenIndices.insert(nodeIndex);
				navigator = scene.addEntity(prefabs::recognizer(nodes[nodeIndex].first, redTankTransform)).findComponent<Navigator>();
				navigator->setNavigationMesh(navigationMesh);
			}

			return scene;
		}
	}
}