#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <Fl/Fl_Box.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl.H>
#include <Fl/Fl_Text_Display.H>
#include <FL/Fl_Color_Chooser.H>
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "Timer.h"
#include "MainWindow.h"
#include "Change_server_window.h"
#include "Change_name_window.h"
#include "About_dialog.h"
#include "Settings.h"
class Chatwin: public Fl_Window,Timer
{
public:
	Chatwin(int _port,std::string _name);
	static void send_message(Fl_Widget* widget, void* _userdata);
	static void change_server(Fl_Widget* widget, void* _userdata);
	static void show_version_server(Fl_Widget* widget, void* _userdata);
	static void change_name(Fl_Widget* widget, void* _userdata);
	static void show_about_win(Fl_Widget* widget, void* _userdata);
	static void show_usernames(Fl_Widget* widget, void* _userdata);
	static void day_mode(Fl_Widget* widget, void* _userdata);
	static void dark_mode(Fl_Widget* widget, void* _userdata);
	static void blue(Fl_Widget* widget, void* _userdata);

	void encrypt_rot13(std::string& _data);
	void inroom_m();
	void on_tick();

	int handle(int _event);
private:
	Fl_Flex m_col;
	Fl_Menu_Bar menu;
	Fl_Box name_box;

	Fl_Text_Buffer text_buffer;
	Fl_Text_Display text_display;

	Fl_Box spacer;
	Fl_Flex bottom_row;
	Fl_Box t_message;
	Fl_Input input;
	Fl_Button send;
	
	ClientSocket client;
	ch_s_win server_change_win;
	ch_name_win name_change_win;
	std::string name;
	std::string version_num;
	int port;
	bool first_inroom;
	About_dialog about_win;
};