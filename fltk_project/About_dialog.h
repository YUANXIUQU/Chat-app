#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <Fl/Fl_Box.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Input.H>
#include <string>
class About_dialog:public Fl_Window
{
private:
	Fl_Flex m_content;
	Fl_Box version_info;
	Fl_Box Author;
public:
	About_dialog();
	void updatebox(std::string version);
};