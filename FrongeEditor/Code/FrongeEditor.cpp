#include "FrongeEditor.hpp"

namespace fro
{
	std::unique_ptr<Application> createApplication()
	{
		fro::Logger::info("FrongeEditor created!");
		return std::make_unique<FrongeEditor>();
	}

	FrongeEditor::FrongeEditor()
	{
		SystemEventManager::mOnWindowClose.addListener(mOnWindowClose);
	}

	void FrongeEditor::run()
	{
		while (mIsRunning)
			SystemEventManager::pollEvents();
	}
}