#include "Components/BulletBouncer.hpp"	
#include "Components/EnemyMoveAI.hpp"
#include "Components/Navigator.hpp"
#include "Components/PlayerCanonShooter.hpp"
#include "Prefabs.hpp"

namespace tron
{
	namespace prefabs
	{
		fro::Entity level(std::size_t const which)
		{
			fro::Entity entity{};

			entity.attachComponent<fro::Transform>()->setLocalTranslation({ 240.0, 288.0 });

			fro::Sprite& sprite{ *entity.attachComponent<fro::Sprite>() };
			sprite.texture = fro::ResourceManager::find<fro::Texture>("level" + std::to_string(which));
			sprite.layer = 0;

			fro::Rigidbody& rigidbody{ *entity.attachComponent<fro::Rigidbody>() };
			rigidbody.setType(fro::Rigidbody::Type::STATIC);

			NavigationMesh const& navigationMesh{ *fro::ResourceManager::find<NavigationMesh>("level" + std::to_string(which) + "Mesh") };
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

			entity.attachComponent<Navigator>();

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

			entity.attachComponent<PlayerCanonShooter>();

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
			sprite.texture = fro::ResourceManager::find<fro::Texture>("bullet");
			sprite.layer = 2;

			fro::Rigidbody& rigidbody{ *entity.attachComponent<fro::Rigidbody>() };
			rigidbody.setType(fro::Rigidbody::Type::DYNAMIC);
			rigidbody.setFixedRotation(true);
			rigidbody.setLinearVelocity(direction * 256.0);

			fro::Collider& collider{ rigidbody.addCollider() };
			collider.setShape(fro::Circle<double>{ .radius{ 2.0 } });
			collider.setDensity(1.0);
			collider.setRestitution(1.0);
			collider.setGroupIndex(-1);

			entity.attachComponent<BulletBouncer>();

			return entity;
		}

		fro::Entity blueTankAI(fro::Vector2<double> const position, fro::Reference<fro::Transform const> const redTankTransform)
		{
			fro::Entity entity{};

			entity.attachComponent<fro::Transform>()->setLocalTranslation(position);

			fro::Sprite& sprite{ *entity.attachComponent<fro::Sprite>() };
			sprite.texture = fro::ResourceManager::find<fro::Texture>("blueTank");
			sprite.layer = 1;

			fro::Rigidbody& rigidbody{ *entity.attachComponent<fro::Rigidbody>() };
			rigidbody.setType(fro::Rigidbody::Type::KINEMATIC);

			fro::Collider& collider{ rigidbody.addCollider() };
			collider.setShape(fro::Rectangle<double>{ .width{ 32.0 }, .height{ 32.0 } });
			collider.setDensity(1.0);

			EnemyMoveAI& enemyMoveAI{ *entity.attachComponent<EnemyMoveAI>() };
			enemyMoveAI.target = redTankTransform;
			enemyMoveAI.targetPosition = redTankTransform->world().getTranslation();

			entity.attachComponent<Navigator>();

			return entity;
		}
	}
}