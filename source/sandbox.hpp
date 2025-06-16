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

         Sandbox& operator=(Sandbox const&)= delete;
         Sandbox& operator=(Sandbox&&) noexcept= delete;

         virtual void run() override;

      private:
         fro::EventListener<> on_render_context_close_
         {
            [this]
            {
               run_ = false;
               return true;
            },
            fro::Locator::set<fro::RenderContext, fro::RenderContext>("Sandbox", fro::Vector2{ 1280, 720 }).close_event
         };

         bool run_{ true };
   };
}

#endif