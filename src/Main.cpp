#include "Demo.h"
#include "Logger.h"
#include <iostream>

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
	
	demo();

	return 0;
}
