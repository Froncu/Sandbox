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
      fro::Locator::set<fro::Logger, fro::ColouredLogger>().info("created Sandbox!");

      auto& render_context{ fro::Locator::get<fro::RenderContext>() };
      render_context.change_resizability(true);
      render_context.change_scaling_mode(fro::RenderContext::ScalingMode::STRETCH);
      render_context.change_resolution({ 400, 240 });

      fro::Scene const& scene{ fro::Locator::get<fro::SceneManager>().add("scene") };

      //

      fro::Entity& entity_a{ scene.create_entity() };

      auto& transform_a{ entity_a.add_component<fro::Transform>() };
      transform_a.local_translate({ 100.0, 50.0 });
      transform_a.local_scale({ 1.3, 2.2 });

      auto& rigid_body_a{ entity_a.add_component<fro::RigidBody>() };
      rigid_body_a.colliders.push_back({ fro::Circle{ 16.0 } });
      rigid_body_a.inverse_mass = 1.0;

      //

      fro::Entity& entity_b{ scene.create_entity() };

      auto& transform_b{ entity_b.add_component<fro::Transform>() };
      transform_b.local_translate({ 110.0, 150.0 });
      transform_b.local_scale({ 2.6, 4.4 });

      auto& rigid_body_b{ entity_b.add_component<fro::RigidBody>() };
      rigid_body_b.colliders.push_back({ fro::Circle{ 8.0 } });
      rigid_body_b.inverse_mass = 1.0;
      rigid_body_b.velocity.y = -50.0;

      //

      fro::Entity& entity_c{ scene.create_entity() };

      auto& transform_c{ entity_c.add_component<fro::Transform>() };
      transform_c.local_translate({ 50.0, 200.0 });
      transform_c.local_scale({ 0.65, 2.1 });

      auto& rigid_body_c{ entity_c.add_component<fro::RigidBody>() };
      rigid_body_c.colliders.push_back({ fro::Circle{ 12.0 } });
      rigid_body_c.inverse_mass = 1.0;
      rigid_body_c.velocity.y = -60.0;
   }

   Sandbox::~Sandbox()
   {
      fro::Locator::get<fro::Logger>().info("destroying Sandbox!");
   }

   void Sandbox::run()
   {
      auto& system_event_dispatcher{ fro::Locator::get<fro::SystemEventDispatcher>() };
      auto& scene_manager{ fro::Locator::get<fro::SceneManager>() };
      auto& render_context{ fro::Locator::get<fro::RenderContext>() };

      auto last_time{ std::chrono::high_resolution_clock::now() };
      constexpr double fixed_delta_seconds{ 1.0 / 60.0 };
      double accumulator{};
      while (run_)
      {
         auto const current_time{ std::chrono::high_resolution_clock::now() };
         double const delta_seconds{ std::chrono::duration<double>{ current_time - last_time }.count() };
         last_time = current_time;

         system_event_dispatcher.poll_events();

         accumulator += delta_seconds;
         while (accumulator >= fixed_delta_seconds)
         {
            scene_manager.fixed_update(fixed_delta_seconds);
            accumulator -= fixed_delta_seconds;
         }

         scene_manager.update(delta_seconds);

         render_context.clear();
         scene_manager.render();
         render_context.present();

         scene_manager.execute_queued();
      }
   }
}