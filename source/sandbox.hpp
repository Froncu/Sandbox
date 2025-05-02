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
         fro::RenderContext render_context_{ "Sandbox", { 1280, 720 } };

         fro::EventListener<> on_render_context_close_
         {
            [this]
            {
               run_ = false;
               return true;
            },
            render_context_.close_event
         };

         bool run_{ true };
   };
}

#endif