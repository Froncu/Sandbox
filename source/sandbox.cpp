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
      fro::Locator::get<fro::Logger>().info("created Sandbox!");
   }

   Sandbox::~Sandbox()
   {
      fro::Locator::get<fro::Logger>().info("destroying Sandbox!");
   }

   void Sandbox::run()
   {
      auto& render_context{ fro::Locator::get<fro::RenderContext>() };
      render_context.change_resizability(true);
      render_context.change_scaling_mode(fro::RenderContext::ScalingMode::STRETCH);
      render_context.change_resolution({ 1280, 720 });

      fro::Texture const& texture{ render_context.upload_texture(fro::Surface{ "resources/shovel_knight.png" }) };
      texture.change_linear_filtering(false);

      fro::Scene scene{};
      auto& group{ scene.group<fro::Pack<fro::Transform, fro::Sprite>, fro::Pack<>>() };

      fro::Entity& entity{ scene.create_entity() };
      entity.add_component<fro::Transform>().local_scale({ 4, 4 });
      entity.add_component<fro::Sprite>(fro::Reference{ texture });

      scene.execute_queued();

      while (run_)
      {
         fro::Locator::get<fro::SystemEventDispatcher>().poll_events();

         render_context.clear();
         for (auto&& [_, transform, sprite] : group)
         {
            transform.local_translate({ 0.001, 0.001 });
            render_context.render(*sprite.texture, transform.world(), sprite.source_rectangle);
         }
         render_context.present();
      }
   }
}