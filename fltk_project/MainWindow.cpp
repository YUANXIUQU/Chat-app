#include "MainWindow.h"

Mainwindow::Mainwindow(int _port,std::string _version, Settings& _settings): //server 
	Fl_Window(_settings.getwidth(), _settings.getheight(), "client connection"),
	Timer(1/30),
	maincol(0,0,w(), h()),
	portbox(0, 0, 0, 0),
	t_portbox(0, 0, 0, 0, "Port:"),
	input_port(0, 0, 0, 0),
	username(0, 0, 0, 0),
	t_username(0, 0, 0, 0, "Name:"),
	input_username(0, 0, 0, 0),
	spacer(0, 0, 0, 0),
	bottom_row(0,0,0,0),
	connect(0,0,0,0,"Connect"),
	server(_port,_version),
	port(_port),
	settings(_settings.get_path())
{
	add_resizable(maincol);
	maincol.add_resizable(portbox);
	maincol.add_resizable(username);
	maincol.add_resizable(bottom_row);

	maincol.fixed(portbox, 50);
	maincol.fixed(username, 50);
	maincol.fixed(bottom_row, 50);

	portbox.type(Fl_Flex::ROW);
	portbox.add_resizable(t_portbox);
	portbox.add_resizable(input_port);
	portbox.fixed(t_portbox, 100);
	portbox.fixed(input_port, 200);

	username.type(Fl_Flex::ROW);
	username.add_resizable(t_username);
	username.add_resizable(input_username);
	input_username.value(_settings.getname().c_str());
	username.fixed(t_username, 100);
	username.fixed(input_username, 200);
	
	
	bottom_row.type(Fl_Flex::ROW);
	bottom_row.add_resizable(spacer);
	bottom_row.add_resizable(connect);
	bottom_row.fixed(spacer, 200);
	bottom_row.fixed(connect, 100);

	connect.callback(TryConnect, this);

	end();
}



void Mainwindow::TryConnect(Fl_Widget* widget, void* _userdata)
{
	Mainwindow* window = (Mainwindow*)_userdata;
	std::string _port = window->input_port.value();
	window->name = window->input_username.value();
	window->settings.height(window->h());
	window->settings.width(window->w());
	try {
		int num_port = std::atoi(_port.c_str());
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
			std::cout << "Username contains invalid character\n";
			return;
		}
		else
		{
			//Chatwin* win = new Chatwin(num_port, window->name);
			//std::unique_ptr<Chatwin> win = std::make_unique<Chatwin>(num_port, window->name);
			Chatwin win(num_port, window->name);
			win.show();
			window->settings.setname(window->name);
		}
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "You have to enter number" << std::endl;
	}
	catch (const std::runtime_error& e)
	{
		std::cout << "Invalid port" << std::endl;
	}
	window->input_port.value("");
	window->input_username.value("");
}



void Mainwindow::on_tick()
{
	std::vector<std::pair<std::string, std::shared_ptr<ClientSocket>>> removed;
	try {
		std::shared_ptr<ClientSocket> client = server.accept();
		if (client)
		{
			std::string t_name;
			if(client->receive(t_name))
			{
				clients.push_back(std::make_pair(t_name, client));
				std::cout << "connected client!\n";
			}
			for (auto ci = clients.begin(); ci != clients.end(); ++ci) // check users in room
			{
				std::cout << "Users: " << ci->first << "\n";
			}
		}
	}
	catch (const std::runtime_error& e)
	{
		std::cout << "Runtime error: " << e.what();
	}
	for (auto ci = clients.begin();ci!=clients.end();++ci)
	{
		std::string message;
		while (ci->second->receive(message) && !message.empty())
		{
			std::cout<<"Message recived: n" + message + "\n";
			if (message == "ask version")
			{
				std::string ver = "\n" + server.get_version() + "\n";
				ci->second->send(ver);
			}
			else if (message.at(0) == '@') // manage private message
			{
				std::size_t pos = message.find(":");
				std::string s_name = message.substr(1, pos - 1); // get username
				std::string m = message.substr(pos + 1); // message
				int no_name = 0;
				for (auto i = clients.begin(); i != clients.end(); ++i)
				{
					if (i->first == s_name)
					{
						//only send to 1 specific person
						i->second->send(m);
					}
					else
					{
						no_name++;
					}
				}
				if (no_name == clients.size())
				{
					std::cout << "\nNo user found with that username\n";
					ci->second->send("*No user found with that username*");
				}
			}
			else if (message == "all") // respond to get all usernames request
			{
				std::string all_usernames = message;
				for (auto i = clients.begin(); i != clients.end(); ++i)
				{
					all_usernames += i->first;
					all_usernames += "\n";
				}
				ci->second->send(all_usernames);
			}

			else if (message.substr(0, 4) == "name") // manage the name change
			{
				std::string changed_name = message.substr(4);
				ci->first = changed_name;

			}

			else 
			{
				for (auto i = clients.begin(); i != clients.end(); ++i)
				{
					// broadcast new message to all clients except the one sending new message to server
					if (i != ci)
					{
						i->second->send(message);
					}
				}
			}

		}
		if (ci->second->closed()) {
			std::cout << "Client Disconnected\n";
			removed.push_back(*ci);
		}
	}

	// clean closed client sockets
	for (auto i = removed.begin(); i != removed.end(); ++i)
	{
		for (auto c = clients.begin(); c != clients.end(); ++c) {
			if (c->first == i->first) 
			{ 
				clients.erase(c);  
				break; 
			}
		}
	}
	removed.clear();
}

int Mainwindow::handle (int _event)
{
	if (_event == 15)
	{
		std::cout << "Window close event triggered!" << std::endl;
		hide();
		return 1;
	}
	return Fl_Window::handle(_event);
}

