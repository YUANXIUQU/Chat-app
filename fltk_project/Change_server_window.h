#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <Fl/Fl_Box.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Input.H>
#include <string>
class ch_s_win :public Fl_Window
{
private:
	Fl_Flex m_content;
	Fl_Box t_port;
	Fl_Input input;
	Fl_Button send_button;
	int port;
	bool is_press_send;
public:
	ch_s_win();
	void static send_port(Fl_Widget* _widget, void* _userdata);
	int getport() { return port; }
	bool get_ispress() { return is_press_send; }
	void set_ispress(bool con) { is_press_send = con; }
};