#ifndef SANDBOX_HPP
#define SANDBOX_HPP

#include "fronge/fronge.hpp"

namespace sbx
{
   class Sandbox final : public fro::Application
   {
      public:
         Sandbox();
         Sandbox(Sandbox const&) = delete;
         Sandbox(Sandbox&&) noexcept = delete;

         virtual ~Sandbox() override;

         Sandbox& operator=(Sandbox const&) = delete;
         Sandbox& operator=(Sandbox&&) noexcept = delete;

         [[nodiscard]] virtual bool tick() override;

      private:
         fro::EventListener<> on_window_close_
         {
            [this]
            {
               run_ = false;
               return true;
            },
            fro::Locator::get<fro::Window>().close_event
         };

         bool run_{ true };

         fro::Reference<fro::RigidBody> polygon_{};

         std::chrono::high_resolution_clock::time_point last_time_{ std::chrono::high_resolution_clock::now() };
         double const fixed_delta_seconds_{ 1.0 / 60.0 };
         double accumulator_{};
   };
}

#endif