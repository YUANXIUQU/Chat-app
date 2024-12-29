#include "About_dialog.h"
#include <iostream>
About_dialog::About_dialog():
	Fl_Window(200,100,"About"),
	m_content(0,0,w(),h()),
	version_info(0,0,0,0,"version"),
	Author(0,0,0,0,"Developed by: Yuanxiu Qu")
{
	add_resizable(m_content);
	m_content.add_resizable(version_info);
	m_content.add_resizable(Author);
	m_content.fixed(version_info, 50);
	m_content.fixed(Author, 50);
}


void About_dialog::updatebox(std::string version)
{
	version_info.copy_label(version.c_str());
	version_info.redraw();
}