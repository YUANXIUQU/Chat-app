#include "Change_name_window.h"
#include <iostream>
ch_name_win::ch_name_win() :Fl_Window(300, 50, "Change server"),
m_content(0, 0, w(), h()),
t_port(0, 0, 0, 0, "New Name:"),
input(0, 0, 0, 0),
send_button(0, 0, 0, 0, "Send"),
is_press_send(false)
{
	add_resizable(m_content);
	m_content.type(Fl_Flex::ROW);
	m_content.add_resizable(t_port);
	m_content.add_resizable(input);
	m_content.add_resizable(send_button);
	m_content.fixed(t_port, 100);
	m_content.fixed(input, 150);
	m_content.fixed(send_button, 50);
	send_button.callback(send_name, this);
	end();
}

void ch_name_win::send_name(Fl_Widget* _widget, void* _userdata)
{
	ch_name_win* window = (ch_name_win*)_userdata;
	window->name = window->input.value();
	if (window->name.empty())
	{
		std::cout << "\nUsername should not be empty\n";
	}
	else if (window->name.front() == ':')
	{
		std::cout << "Username can not start with : \n";
	}
	else if (!(window->name.front() >= -1 && window->name.front() <= 255))
	{
		std::cout << "Username can not start with this that character\n";
		window->is_press_send = false;
	}
	else
	{
		window->is_press_send = true;
	}
	window->input.value("");
}

