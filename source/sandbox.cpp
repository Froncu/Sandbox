#include "sandbox.hpp"

namespace fro
{
   std::unique_ptr<Application> create_application()
   {
      return std::make_unique<sbx::Sandbox>();
   }
}

namespace sbx
{
   Sandbox::Sandbox()
   {
      fro::Locator::set<fro::Logger, fro::ColouredLogger>();
      fro::Locator::set<fro::SystemEventDispatcher, fro::RegularSystemEventManager>();

      fro::Locator::get<fro::Logger>().info("created Sandbox!");
   }

   Sandbox::~Sandbox()
   {
      fro::Locator::reset<fro::SystemEventDispatcher>();

      fro::Locator::get<fro::Logger>().info("destroying Sandbox!");
   }

   void Sandbox::run()
   {
      fro::Scene scene{};
      auto& group{ scene.group<fro::TemplateParameterPack<int, float>, fro::TemplateParameterPack<char>>() };

      fro::Entity& entity_1{ scene.create_entity() };
      entity_1.add_component<int>(1);
      entity_1.add_component<float>(1.0f);
      entity_1.add_component<char>('1');

      fro::Entity& entity_2{ scene.create_entity() };
      entity_2.add_component<int>(2);
      entity_2.add_component<float>(2.0f);

      fro::Entity& entity_3{ scene.create_entity() };
      entity_3.add_component<int>(3);
      entity_3.add_component<float>(3.0f);
      entity_3.add_component<char>('3');

      fro::Entity& entity_4{ scene.create_entity() };
      entity_4.add_component<int>(4);
      entity_4.add_component<float>(4.0f);
      entity_4.add_component<char>('4');

      scene.execute_queued();

      for (auto&& [entity, integer, character, floating] : group)
         fro::Locator::get<fro::Logger>().info("integer: {}, character: {}, floating: {}",
            integer,
            character,
            floating);

      scene.destroy_entity(entity_1);

      scene.execute_queued();
      scene.execute_queued();

      for (auto&& [entity, integer, character, floating] : group)
         fro::Locator::get<fro::Logger>().info("integer: {}, character: {}, floating: {}",
            integer,
            character,
            floating);

      while (run_)
         fro::Locator::get<fro::SystemEventDispatcher>().poll_events();
   }
}