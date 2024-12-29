#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <Fl/Fl_Box.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Input.H>
#include <FL/Fl.H>
#include "ClientSocket.h"
#include "ServerSocket.h"
#include "Chatwin.h"
#include "Timer.h"
#include <memory>
#include <vector>
#include "Settings.h"
#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <map>
struct Mainwindow : public Fl_Window,Timer
{
public:
	Mainwindow(int _port,std::string _version, Settings& _settings);
	~Mainwindow() { std::cout << "end"; }
	void on_tick();
	static void TryConnect(Fl_Widget* _widget, void* _userdata);
	int handle(int _event);
private:
	ServerSocket server;
	std::vector<std::pair<std::string, std::shared_ptr<ClientSocket>>> clients;
	std::string name;
	Settings settings;
	int port;

	Fl_Flex maincol;
	Fl_Flex portbox;
	Fl_Flex username;
	Fl_Flex bottom_row;

	Fl_Box t_portbox;
	Fl_Input input_port;

	
	Fl_Box t_username;
	Fl_Input input_username;


	Fl_Box spacer;
	Fl_Button connect;
	
};

	