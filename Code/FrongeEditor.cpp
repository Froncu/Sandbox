#include "FrongeEditor.hpp"

namespace fro
{
	std::unique_ptr<Application> createApplication()
	{
		fro::Logger::info("FrongeEditor created!");
		return std::make_unique<FrongeEditor>();
	}

	void FrongeEditor::run()
	{
		fro::Logger::info("FrongeEditor started running!");
		while (true)
			mWindow.update();
	}
}