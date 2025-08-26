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
      fro::Locator::get<fro::Logger>().info("created Sandbox!");

      auto& window{ fro::Locator::get<fro::Window>() };
      window.change_title("Sandbox");
      window.change_size({ 1280, 720 });
      window.change_resizability(true);
      window.change_visibility(true);

      auto& renderer{ fro::Locator::get<fro::Renderer>() };
      renderer.change_scaling_mode(fro::Renderer::ScalingMode::STRETCH);
      renderer.change_resolution({ 400, 240 });
      renderer.change_present_mode(fro::Renderer::PresentingMode::SINGLE_BUFFERED);

      fro::Scene const& scene{ fro::Locator::get<fro::SceneManager>().add("scene") };

      //

      fro::Entity& entity_a{ scene.create_entity() };

      auto& sprite{ entity_a.add_component<fro::Sprite>() };
      sprite.texture =
         fro::Reference{ renderer.upload_texture(fro::Surface{ "resources/shovel_knight.png" }) };
      sprite.texture->change_linear_filtering(false);
      sprite.source_rectangle = { 0.0, 0.0, 64.0, 64.0 };
      sprite.transform.change_scale({ 0.75, 1.2 });
      sprite.transform.translate({ -4.0, 2.0 });

      auto& transform_a{ entity_a.add_component<fro::Transform>() };
      transform_a.local_translate({ 100.0, 50.0 });
      transform_a.local_scale({ 1.5, 0.6 });

      polygon_ = fro::Reference{ entity_a.add_component<fro::RigidBody>() };
      polygon_->colliders.push_back({ fro::Polygon{ { { -8.0, -5.5 }, { 8.0, -7.5 }, { 12.0, 6.5 }, { -8.0, 4.5 } } } });
      // rigid_body_a.colliders.push_back({ fro::Circle{ 12.0 } });
      polygon_->inverse_mass = 1.0 / 1.5;
      polygon_->inverse_inertia = 1.0 / (0.5 * 1.5 * 12.0 * 12.0);

      //

      fro::Entity& entity_b{ scene.create_entity() };

      auto& transform_b{ entity_b.add_component<fro::Transform>() };
      transform_b.local_translate({ 110.0, 150.0 });
      transform_b.local_scale({ 1.2, 2.7 });

      auto& rigid_body_b{ entity_b.add_component<fro::RigidBody>() };
      // rigid_body_b.colliders.push_back({ fro::Polygon{ { { -4.0, -4.0 }, { 4.0, -4.0  }, { 4.0, 4.0 }, { -4.0, 4.0 } } } });
      rigid_body_b.colliders.push_back({ fro::Circle{ 8.0 } });
      rigid_body_b.velocity.y = -50.0;
      rigid_body_b.angular_velocity = -2.0;
      rigid_body_b.inverse_mass = 1.0;
      rigid_body_b.inverse_inertia = 1.0 / (0.25 * 1.0 * 8.0 * 8.0 * (1.2 * 1.2 + 2.7 * 2.7));

      //

      fro::Entity& entity_c{ scene.create_entity() };

      auto& transform_c{ entity_c.add_component<fro::Transform>() };
      transform_c.local_translate({ 50.0, 200.0 });

      auto& rigid_body_c{ entity_c.add_component<fro::RigidBody>() };
      // rigid_body_c.colliders.push_back({ fro::Polygon{ { { -4.0, -4.0 }, { 100.0, -16.0  }, { 4.0, 4.0 }, { -4.0, 4.0 } } }, a });
      rigid_body_c.colliders.push_back({ fro::Circle{ 16.0 } });
      rigid_body_c.velocity.y = -50.0;
      rigid_body_c.inverse_mass = 1.0 / 2.0;
      rigid_body_c.inverse_inertia = 1.0 / (0.5 * 2.0 * 16.0 * 16.0);

      //

      fro::Entity& entity_d{ scene.create_entity() };

      auto& transform_d{ entity_d.add_component<fro::Transform>() };
      transform_d.local_translate({ 220.0, 200.0 });

      auto& rigid_body_d{ entity_d.add_component<fro::RigidBody>() };
      // rigid_body_c.colliders.push_back({ fro::Polygon{ { { -4.0, -4.0 }, { 100.0, -16.0  }, { 4.0, 4.0 }, { -4.0, 4.0 } } }, a });
      rigid_body_d.colliders.push_back({ fro::Circle{ 24.0 } });
      // rigid_body_d.velocity.y = -55.0;
      rigid_body_d.inverse_mass = 0.0 / 3.0;
      rigid_body_d.inverse_inertia = 1.0 / (0.5 * 3.0 * 24.0 * 24.0);
   }

   Sandbox::~Sandbox()
   {
      fro::Locator::get<fro::Logger>().info("destroying Sandbox!");
   }

   void Sandbox::run()
   {
      auto& system_event_dispatcher{ fro::Locator::get<fro::SystemEventDispatcher>() };
      auto& scene_manager{ fro::Locator::get<fro::SceneManager>() };
      auto& renderer{ fro::Locator::get<fro::Renderer>() }; 
      auto const& editor_ui{ fro::Locator::get<fro::EditorUI>() };

      fro::UserInput const& user_input{ fro::Locator::get<fro::InputManager>().user_input(0) };
      user_input.bind_action("move", fro::VectorAction{
         .positive_x_inputs{ fro::Key::D, fro::GamepadAxis::LEFT_STICK_EAST },
         .negative_x_inputs{ fro::Key::A, fro::GamepadAxis::LEFT_STICK_WEST },
         .positive_y_inputs{ fro::Key::S, fro::GamepadAxis::LEFT_STICK_SOUTH },
         .negative_y_inputs{ fro::Key::W, fro::GamepadAxis::LEFT_STICK_NORTH },
      });

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

         fro::Vector2 const strength{ user_input.vector_action_strength("move") };
         polygon_->velocity += strength * 100.0 * delta_seconds;

         renderer.clear();
         scene_manager.render();
         editor_ui.begin_frame();
         editor_ui.show_demo_window();
         editor_ui.end_frame();
         renderer.present();

         scene_manager.execute_queued();
      }
   }
}