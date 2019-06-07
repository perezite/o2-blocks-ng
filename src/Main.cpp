#include "Window.h"
#include "Input.h"
#include "Stopwatch.h"
#include <iostream>

float getSeconds() {
	static sb::Stopwatch sw;
	return sw.getElapsedSeconds();
}

float getDeltaSeconds()
{
	static float lastElapsed = 0;
	float elapsed = getSeconds();
	float delta = elapsed - lastElapsed;
	lastElapsed = elapsed;
	return delta;
}

inline float getDesktopAspectRatio() { return 16.0f / 9.0f; }

void version() {
	#ifdef _DEBUG
		std::string configuration = "Debug";
	#else
		std::string configuration = "Release";
	#endif	
	SB_MESSAGE("Blocks (" << configuration << "): " << __DATE__ << ", " << __TIME__);
}

void demo0() {
	sb::Window window(400, int(400 * getDesktopAspectRatio()));

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
 
		window.clear(sb::Color(1, 1, 1, 1));
		window.display();
	}
}

int main() {
	version();

	demo0();

	return 0;
}
