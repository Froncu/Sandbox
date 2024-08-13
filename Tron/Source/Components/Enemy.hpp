#if not defined ENEMY_HPP
#define ENEMY_HPP

#include "Fronge.hpp"

namespace tron
{
	class Enemy final : public fro::Component
	{
	public:
		Enemy() = default;
		Enemy(Enemy const&) = default;
		Enemy(Enemy&&) noexcept = default;

		virtual ~Enemy() override = default;

		Enemy& operator=(Enemy const&) = default;
		Enemy& operator=(Enemy&&) noexcept = default;

	private:
		fro::EventListener<fro::Entity, fro::Component, std::type_index const> mOnComponentAttachEvent
		{
			[smartThis = fro::Reference{ this }](fro::Entity& entity, fro::Component& component, std::type_index const& typeIndex)
			{
				auto& parentingEntity{ smartThis->mParentingEntity };

				if (&*smartThis == &component)
				{
					parentingEntity = entity;
					auto const rigidbody{ parentingEntity->findComponent<fro::Rigidbody>() };
					if (not rigidbody)
						return false;

					rigidbody->mBeginContactEvent.addListener(smartThis->mOnContactBeginEvent);
					return true;
				}

				if (parentingEntity.valid() and &*parentingEntity == &entity and typeIndex == typeid(fro::Rigidbody))
				{
					fro::Rigidbody& rigidbody{ static_cast<fro::Rigidbody&>(component) };
					rigidbody.mBeginContactEvent.addListener(smartThis->mOnContactBeginEvent);
					return true;
				}

				return false;
			}, fro::EntityManager::getComponentAttachEvent()
		};

		fro::EventListener<fro::Entity, fro::Component, std::type_index const> mOnComponentDetachEvent
		{
			[smartThis = fro::Reference{ this }](fro::Entity& entity, fro::Component& component, std::type_index const& typeIndex)
			{
				auto& parentingEntity{ smartThis->mParentingEntity };

				if (&*smartThis == &component)
				{
					auto const rigidbody{ parentingEntity->findComponent<fro::Rigidbody>() };
					if (rigidbody)
						rigidbody->mBeginContactEvent.removeListener(smartThis->mOnContactBeginEvent);

					parentingEntity.reset();
					return true;
				}

				if (parentingEntity.valid() and &*parentingEntity == &entity and typeIndex == typeid(fro::Rigidbody))
				{
					fro::Rigidbody& rigidbody{ static_cast<fro::Rigidbody&>(component) };
					rigidbody.mBeginContactEvent.removeListener(smartThis->mOnContactBeginEvent);
					return true;
				}

				return false;
			}, fro::EntityManager::getComponentDetachEvent()
		};

		fro::EventListener<fro::Collider, fro::Rigidbody, fro::Collider> mOnContactBeginEvent
		{
			[smartThis = fro::Reference{ this }](fro::Collider const&, fro::Rigidbody const&, fro::Collider const& collider)
			{
				if (not (collider.getCategoryBits() & fro::createBitfield(1ull)))
					return false;

				--smartThis->mHealth;
				if (smartThis->mHealth == 0)
					smartThis->mParentingEntity->markDoomed();

				return true;
			}
		};

		fro::Reference<fro::Entity> mParentingEntity{};
		std::size_t mHealth{ 3 };
	};
}

#endif