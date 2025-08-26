#ifndef SANDBOX_HPP
#define SANDBOX_HPP

#include "fronge.hpp"

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

         virtual void run() override;

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
   };
}

#endif