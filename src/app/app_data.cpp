#include "app_data.hpp"

aco::app_data& aco::app_data::get_instance()
{
	static aco::app_data instance;
	return instance;
}
