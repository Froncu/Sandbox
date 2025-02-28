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
      fro::Locator::set<fro::SystemEventDispatcher, fro::RegularSystemEventManager>();
      fro::Logger::info("created Sandbox!");
   }

   Sandbox::~Sandbox()
   {
      fro::Locator::reset<fro::SystemEventDispatcher>();
      fro::Logger::info("destroyed Sandbox!");
   }

   void Sandbox::run()
   {
      while (run_)
         fro::Locator::get<fro::SystemEventDispatcher>().poll_events();
   }
}