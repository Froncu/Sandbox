#include "Sandbox.hpp"

#if defined FRO_DEBUG
#include <vld.h>
#endif

namespace fro
{
   std::unique_ptr<Application> createApplication()
   {
      return std::make_unique<sbx::Sandbox>();
   }
}

namespace sbx
{
   Sandbox::Sandbox()
   {
      mSystemEventManager.mInputEvent.addListener(mInputManager.mOnInputEvent);
      mSystemEventManager.mWindowEvent.addListener(mMainWindow.mOnWindowEvent);
      fro::Logger::info("created Sandbox!");
   }

   Sandbox::~Sandbox()
   {
      fro::Logger::info("destroyed Sandbox!");
   }

   void Sandbox::run()
   {
      auto constexpr fixedDeltaSeconds{ 1.0 / 60 };
      double fixedUpdateAccumulator{};

      auto t1{ std::chrono::steady_clock::now() };
      while (mIsRunning)
      {
         auto const t2{ std::chrono::steady_clock::now() };
         double const deltaSeconds{ std::chrono::duration<double>(t2 - t1).count() };
         t1 = t2;

         mInputManager.processInputContinous();
         mSystemEventManager.pollEvents();

         fixedUpdateAccumulator += deltaSeconds;
         while (fixedUpdateAccumulator >= fixedDeltaSeconds)
         {
            fixedUpdateAccumulator -= fixedDeltaSeconds;
            // TODO: add physics code
         }

         mRenderer.clear();
         fro::SpriteSystem::onRender(mRenderer);
         mRenderer.present();

         fro::SceneManager::doomAndAdd();
      }
   }
}