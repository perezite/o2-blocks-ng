#include "TestMenuWindowPlaytests.h"
#include "../TestMenuWindow.h"
#include "../../Core/Events.h"
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

namespace o2
{
	namespace my
	{
		static void simplePlaytest() { 
			cout << "Simple playtest" << endl; 
		}
		static void miscPlaytest1() { cout << "Misc playtest 1" << endl; }
		static void miscPlaytest2() { cout << "Misc playtest 2" << endl; }
		static void writeAllTextTest() { cout << "writeAllTextTest" << endl; }
		static void writeAllTextTransactionalTest() { cout << "writeAllTextTransactionalTest" << endl; }
		static void miscAutotest1() { cout << "writeAllTextTransactionalTest" << endl; }
		static void failingTest() { 
			cout << "This test will fail" << endl; 
			throw runtime_error("fail");
		}
		static void slowTest() {
			cout << "This test takes 500 ms.." << endl;
			this_thread::sleep_for(chrono::milliseconds(500));
			cout << "Finished!" << endl;
		}

		void my::TestMenuWindowPlaytests::showWindow()
		{
			TestMenuWindow testMenuWindow;
			testMenuWindow.addPlaytest("Simple Playtests/Some simple Playtest", simplePlaytest);
			testMenuWindow.addPlaytest("Misc Playtests/Misc Playtest 1", miscPlaytest1);
			testMenuWindow.addPlaytest("Misc Playtests/Misc Playtest 2", miscPlaytest2);

			testMenuWindow.addAutotest("FileHelper/writeAllText", writeAllTextTest);
			testMenuWindow.addAutotest("FileHelper/writeAllTextTransactional", writeAllTextTransactionalTest);
			testMenuWindow.addAutotest("FileHelper/fail", failingTest);

			testMenuWindow.addAutotest("Misc Autotests/miscAutotest1", miscAutotest1);
			testMenuWindow.addAutotest("Misc Autotests/slowTest", slowTest);

			while (testMenuWindow.isOpen()) {
				Events::update();
				testMenuWindow.update();
				testMenuWindow.display();
			}
		}
	}
}