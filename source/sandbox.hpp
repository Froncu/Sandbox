#ifndef SANDBOX_HPP
#define SANDBOX_HPP

#include "Fronge.hpp"

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
         fro::SystemEventManager mSystemEventManager{};
         fro::InputManager mInputManager{};

         fro::Window mMainWindow{ "Sandbox", { 1280, 720 } };
         fro::Renderer mRenderer{ mMainWindow, { 480, 512 } };

         fro::EventListener<> mOnMainWindowCloseEvent
         {
            [this]()
            {
               mIsRunning = false;
               return true;
            },
            mMainWindow.mCloseEvent
         };

         fro::EventListener<fro::InputEvent const> mOnInputEvent
         {
            fro::VariantVisitor
            {
               [this](fro::KeyDownEvent const& event)
               {
                  if (event.key not_eq fro::Key::ESCAPE)
                     return false;

                  mIsRunning = false;
                  return true;
               },

               [](auto&&)
               {
                  return false;
               }
            },
            mSystemEventManager.mInputEvent
         };

         bool mIsRunning{ true };
   };
}

#endif