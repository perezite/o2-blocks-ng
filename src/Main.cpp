#include "Game.h"
#include "Demo.h"
#include "Logger.h"

void version() {
	#ifdef _DEBUG
		std::string configuration = "Debug";	
	#else
		std::string configuration = "Release";
	#endif

	SB_MESSAGE("Blocks (" << configuration << "): " << __DATE__ << ", " << __TIME__);
}

int main() {
	version();
	
	game::game();
	
	//demo();
	
	return 0;
}
