#pragma once

#include <string_view>

#include "app_data.hpp"

namespace aco
{

class app
{
public:
	app(std::string_view window_title);

private:
	void run() const;

	app_data& m_app_data;
};

}