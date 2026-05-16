#include "PlaytestMenuWindowPlaytest.h"
#include "PlaytestMenuWindow.h"
#include "../../Core/Events.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include <iostream>
using namespace std;

namespace o2
{
	namespace my
	{
		static void simplePlaytest() { cout << "Simple playtest"; }
		static void miscPlaytest1() { cout << "Misc playtest 1"; }
		static void miscPlaytest2() { cout << "Misc playtest 2"; }

		void my::PlaytestMenuWindowPlaytest::showWindow()
		{
			PlaytestMenuWindow playtestMenuWindow;
			playtestMenuWindow.addPlaytest("Simple Playtests/Some simple Playtest", simplePlaytest);
			playtestMenuWindow.addPlaytest("Misc Playtests/Misc Playtest 1", miscPlaytest1);
			playtestMenuWindow.addPlaytest("Misc Playtests/Misc Playtest 2", miscPlaytest2);

			playtestMenuWindow.printTree();

			while (playtestMenuWindow.isOpen()) {
				Events::update();
				playtestMenuWindow.update();
				playtestMenuWindow.display();
			}
		}
	}
}