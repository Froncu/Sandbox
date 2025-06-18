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

      fro::Texture const& texture{ render_context.upload_texture(fro::Surface{ "resources/shovel_knight.png" }) };
      texture.change_linear_filtering(false);

      fro::Scene& scene{ fro::Locator::get<fro::SceneManager>().add("scene") };
      fro::Entity& entity{ scene.create_entity() };
      entity.add_component<fro::Transform>();
      entity.add_component<fro::Sprite>(fro::Reference{ texture }, fro::Rectangle<double>{ 0, 0, 80, 51 });
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