#define _CRT_SECURE_NO_WARNINGS
#include <FL/Fl.H>
#include "MainWindow.h"
#include "Chatwin.h"
#include <iostream>
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "Wsa.h"
#include "Settings.h"


int main(int argc, char* argv[])
{
	Wsa wsa;
	char* homeenv = getenv("USERPROFILE");
	if (!homeenv)
	{
		throw std::runtime_error("Failed to obtain home directory");
	}
	std::string homedir = homeenv;

	Fl::scheme("gtk+");

	Settings setting("store.xml");
	Mainwindow win(2500, "1.0", setting);

	Settings setting2("second.xml");
	Mainwindow win2(3000, "2.0", setting2);
	win.show();
	win2.show();
	
	return Fl::run();
}

