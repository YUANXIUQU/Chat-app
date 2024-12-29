#pragma once

#include <iostream>
#include <stdexcept>
#include "pugixml.hpp"
#include <sstream>
#include <string>
#include <cstdlib>
struct Settings
{
	Settings(){}
	Settings(const std::string& _path)
	{
		std::cout << "\ncon" << "\n";
		path = _path;
		pugi::xml_parse_result res = m_doc.load_file(path.c_str());
		if (res.status)
		{
			std::cout << "test\n";
			root = m_doc.child("new_setting");
			if (!root)
			{
				root = m_doc.append_child("new_setting");
			}
			
			setting = root.child("setting");
			if (!setting)
			{
				setting = root.append_child("setting");
				setting.append_attribute("w").set_value("300");
				setting.append_attribute("h").set_value("150");
				setting.append_attribute("name").set_value("hello");
			}
			
		}
		else
		{
			root = m_doc.child("new_setting");
			setting = root.child("setting");
		}
		m_doc.save_file(path.c_str());
	}
	~Settings()
	{
	}
	int getwidth()
	{
		std::string w = setting.attribute("w").value();
		return std::atoi(w.c_str());
		
	}
	void width(int _width)
	{
		setting.attribute("w").set_value(_width);
		m_doc.save_file(path.c_str());
	}

	int getheight()
	{
		std::string h = setting.attribute("h").value();
		return std::atoi(h.c_str());
	}
	void height(int _height)
	{
		setting.attribute("h").set_value(_height);
		m_doc.save_file(path.c_str());
	}

	std::string getname()
	{
		std::string name = setting.attribute("name").value();
		return name;
	}
	void setname(const std::string& _name)
	{
		m_doc.child("new_setting").child("setting").attribute("name").set_value(_name.c_str());
		m_doc.save_file(path.c_str());
	}
	void save_file()
	{
		m_doc.save_file(path.c_str());
	}
	std::string get_path()
	{
		return path;
	}

private:
	pugi::xml_document m_doc;
	std::string path;
	pugi::xml_node root;
	pugi::xml_node setting;
};