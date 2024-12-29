#include "Chatwin.h"
#include <string>
#include <iostream>
#include <cctype>
Chatwin::Chatwin(int _port, std::string _name):
	client("localhost", _port),
	Timer(1 / 30),
	Fl_Window(500,350, "chat app"),
	text_buffer(),
	text_display(0, 0, 0, 0),
	m_col(0, 0, w(), h()),
	bottom_row(0,0,0,0),
	menu(0,0,0,0),
	name(_name),
	name_box(0,0,0,0,name.c_str()),
	spacer(0,0,0,0),
	t_message(0,0,0,0,"Message:"),
	input(0,0,0,0),
	send(0,0,0,0,"Send"),
	server_change_win(),
	name_change_win(),
	port(_port),
	about_win(),
	version_num("1.0"),
	first_inroom(true)
{
	text_display.color(fl_rgb_color(200, 200, 255));
	client.send(name);
	add_resizable(m_col);
	m_col.add_resizable(menu);
	m_col.add_resizable(name_box);
	m_col.add_resizable(text_display);//
	m_col.add_resizable(spacer);
	m_col.add_resizable(bottom_row);
	m_col.fixed(menu, 30);
	m_col.fixed(name_box, 30);
	m_col.fixed(spacer, 20);
	m_col.fixed(bottom_row, 50);

	//menu setting
	menu.add("&Options/&Change Server", NULL, change_server, this);
	menu.add("&Options/&Change name", NULL, change_name, this);
	menu.add("&Options/&Get server version", NULL, show_version_server,this);
	menu.add("&About", NULL, show_about_win,this);
	menu.add("&Mode/&Dark mode", NULL, dark_mode, this);
	menu.add("&Mode/&Day mode", NULL, day_mode, this);
	menu.add("&Mode/&Blue mode", NULL, blue, this);
	menu.add("&Users", NULL,show_usernames,this );

	bottom_row.type(Fl_Flex::ROW);
	bottom_row.add_resizable(t_message);
	bottom_row.add_resizable(input);
	bottom_row.add_resizable(send);
	bottom_row.fixed(t_message, 100);
	bottom_row.fixed(send, 100);
	text_display.buffer(text_buffer);
	send.callback(send_message, this);
	std::string wellcome = " ---Welcome to chatroom (^-^) , " + name + "---\n";
	text_buffer.append(wellcome.c_str());
	end();
	//show();
}



void Chatwin::send_message(Fl_Widget* widget, void* _userdata)
{
	Chatwin* window = (Chatwin*)_userdata;
	std::string you = "\nYou : ";
	std::string m = window->input.value();
	std::string output_message = you + m + "\n"; //shows at own screen
	std::string new_message = window->name + " : " + m; // message to all users

	if (!m.empty()&&m.at(0) == '@') //send to specific person.
	{
		try {
			std::size_t pos = m.find(":");
			std::string s_name = m.substr(0, pos);
			std::string em = window->name + m.substr(pos);
			window->encrypt_rot13(em);
			new_message = s_name + ":" + em;
			window->client.send(new_message.c_str());
		}
		catch (const std::out_of_range& e)
		{
			window->text_buffer.append("\n *Wrong format to send message, use @Username:Message* \n");
		}
		catch (const std::runtime_error& e) {
			std::cout << "Server connection failed: " << "\n";
		}
		window->text_buffer.append(output_message.c_str());
	}
	else if(!m.empty())
	{
			window->encrypt_rot13(new_message);
			new_message += "\n";
			window->client.send(new_message.c_str()); 
			window->text_buffer.append(output_message.c_str());
	}
	window->input.value("");
	
}

void Chatwin::on_tick()
{
	//if press the button to change server,then call changeserver func in clientsocket
	if (first_inroom)
	{
		inroom_m();
		first_inroom = false;
	}
	if (server_change_win.get_ispress()) 
	{
		int _port = server_change_win.getport();
		try {
			if (port != _port) {
				client.chageserver("localhost", _port);
				text_buffer.append("\n--you change to a new chat room--\n");
			}
			client.send(name);
			port = _port;
		}
		catch (const std::runtime_error& e)
		{
			text_buffer.append("\nInvalid port\n");
			client.chageserver("localhost", port); // if enter character, still stay at original room
			client.send(name);
			std::cout << "\n" << port << "\n";
			std::cerr << "Runtime error: " << e.what() << std::endl;
		}
	
		server_change_win.set_ispress(false);
		server_change_win.hide();
	}

	std::string message; //client receive messages
	if (client.receive(message))
	{
		if (message.substr(0, 3) == "all") // print users in room
		{
			message = message.substr(3);
			std::string output_m = "All users in room:\n" + message;
			text_buffer.append(output_m.c_str());
		}
		else if (message == "*No user found with that username*")
		{
			text_buffer.append(message.c_str());
		}
		else //normal messages
		{
			encrypt_rot13(message); // decrpt
			std::string m = "\n" + message + "\n";
			text_buffer.append(m.c_str());
		}
	}
	
	if (name_change_win.get_ispress()) // change name
	{
		std::string _name = name_change_win.getname();
		std::string old_name = name;
		name = _name;
		name_change_win.set_ispress(false);
		name_change_win.hide();
		name_box.copy_label(_name.c_str()); // box 's label will be changed as well
		name_box.redraw();
		client.send("name" + name); //send to server message one username been changed
	}
	
}


void Chatwin::change_server(Fl_Widget* widget, void* _userdata) // callback function of send button to change server
{
	Chatwin* window = (Chatwin*)_userdata;
	window->server_change_win.show();
}


void Chatwin::show_version_server(Fl_Widget* widget, void* _userdata) //callback function of Get server version button
{
	Chatwin* window = (Chatwin*)_userdata;
	std::string re_version = "ask version";
	window->client.send(re_version);
}


void Chatwin::change_name(Fl_Widget* widget, void* _userdata) //callback function of send button to change name
{
	Chatwin* window = (Chatwin*)_userdata;
	window->name_change_win.show();
}


void Chatwin::show_about_win(Fl_Widget* widget, void* _userdata) // show window of about dialog
{
	Chatwin* window = (Chatwin*)_userdata;
	std::string v = "Client version : " + window->version_num;
	window->about_win.updatebox(v);
	window->about_win.show();
}


void Chatwin::encrypt_rot13(std::string& _data) // applying encrypt and decrypt function
{
	for (int i = 0; i < _data.size(); i++)
	{
		if (isalpha(_data.at(i))) 
		{
			char c = islower(_data.at(i)) ? 'a' : 'A';
			_data.at(i) = (_data.at(i) - c + 13) % 26 + c;
		}
	}
}

// color style
void Chatwin::dark_mode(Fl_Widget* widget, void* _userdata)
{
	Chatwin* window = (Chatwin*)_userdata;
	window->text_display.textcolor(FL_WHITE);
	window->text_display.color(fl_rgb_color(0, 0, 0));
	window->redraw();
}

void Chatwin::day_mode(Fl_Widget* widget, void* _userdata)
{
	Chatwin* window = (Chatwin*)_userdata;
	window->text_display.textcolor(FL_BLACK);
	window->text_display.color(fl_rgb_color(200, 200, 255));
	window->redraw();
}

void Chatwin::blue(Fl_Widget* widget, void* _userdata)
{
	Chatwin* window = (Chatwin*)_userdata;
	window->text_display.textcolor(FL_DARK_YELLOW);
	window->text_display.color(fl_rgb_color(178, 223, 255));
	window->redraw();
}

void Chatwin::inroom_m()
{
	std::string m = "\n---" + name + " enters in chat room !---\n";
	encrypt_rot13(m);
	client.send(m);
}



void Chatwin::show_usernames(Fl_Widget* widget, void* _userdata)
{
	Chatwin* window = (Chatwin*)_userdata;
	window->client.send("all");
}


int Chatwin::handle(int _event)
{
	if (_event == 15) // when closing chatwindow,close the socket as well.
	{
		std::string message = "\n---" + name + " leaves the room---\n"; //send message to server this user leaves the room then broadcast to others
		encrypt_rot13(message);
		client.send(message);
		std::cout << "Window close event triggered!" << std::endl;
		server_change_win.hide();
		name_change_win.hide();
		client.close_socket();
		return 1;
	}
	return Fl_Window::handle(_event);
}