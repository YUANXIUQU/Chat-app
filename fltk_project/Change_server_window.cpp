#include "Change_server_window.h"
#include <cctype>
#include <string>
#include <iostream>
ch_s_win::ch_s_win():Fl_Window(300,50,"Change server"),
	m_content(0,0,w(),h()),
	t_port(0,0,0,0, "New Port:"),
	input(0,0,0,0),
	send_button(0,0,0,0,"Send"),
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
	send_button.callback(send_port, this);
	end();
}

void ch_s_win::send_port(Fl_Widget* _widget, void* _userdata)
{
	ch_s_win* window = (ch_s_win*)_userdata;
	std::string _port = window->input.value();
	int num_digit=0;                             //check if input is all number
	for (int i = 0; i < _port.size(); i++)
	{
		if (!std::isdigit(_port.at(i)))
		{
			window->is_press_send = false;
		}
		else
		{
			num_digit++;
		}
	}
	if (num_digit == _port.size())
	{
		window->is_press_send = true;
		window->port = std::stoi(_port);
	}
	else
	{
		std::cout << "\nyou have to enter number" << std::endl;
	}
	window->input.value("");
}
