#include "TestMenuWindowPlaytest.h"
#include "../TestMenuWindow.h"
#include "../../Core/Events.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include <iostream>
using namespace std;

namespace o2
{
	namespace my
	{
		static void simplePlaytest() { cout << "Simple playtest" << endl; }
		static void miscPlaytest1() { cout << "Misc playtest 1" << endl; }
		static void miscPlaytest2() { cout << "Misc playtest 2" << endl; }
		static void writeAllTextTest() { cout << "writeAllTextTest" << endl; }
		static void writeAllTextTransactionalTest() { cout << "writeAllTextTransactionalTest" << endl; }
		static void miscAutotest1() { cout << "writeAllTextTransactionalTest" << endl; }

		void my::TestMenuWindowPlaytest::showWindow()
		{
			TestMenuWindow testMenuWindow;
			testMenuWindow.addPlaytest("Simple Playtests/Some simple Playtest", simplePlaytest);
			testMenuWindow.addPlaytest("Misc Playtests/Misc Playtest 1", miscPlaytest1);
			testMenuWindow.addPlaytest("Misc Playtests/Misc Playtest 2", miscPlaytest2);

			testMenuWindow.addAutotest("FileHelper/writeAllText", writeAllTextTest);
			testMenuWindow.addAutotest("FileHelper/writeAllTextTransactional", writeAllTextTransactionalTest);
			testMenuWindow.addAutotest("Misc Autotests/miscAutotest1", miscAutotest1);

			// testMenuWindow.printTree();

			while (testMenuWindow.isOpen()) {
				Events::update();
				testMenuWindow.update();
				testMenuWindow.display();
			}
		}
	}
}