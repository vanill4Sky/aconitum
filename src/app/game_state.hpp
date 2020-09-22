#pragma once

#include "../core/state.hpp"

namespace aco
{

class game_state : public state
{
public:
	game_state();

	void init();
	void handle_input();
	void update(float dt);
	void draw();
};

}