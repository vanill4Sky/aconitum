#pragma once

#include <memory>
#include <stack>

#include "state.hpp"

namespace aco
{

using state_ref = std::unique_ptr<aco::state>;

class state_manager
{
public:
	void replace_state(aco::state_ref new_state);
	void push_state(aco::state_ref new_state);
	void pop_state();
	void process_state_transition();

	aco::state_ref& get_active_state();

private:
	std::stack<aco::state_ref> m_states;
	aco::state_ref m_new_state;
	bool m_is_removing;
	bool m_is_adding;
};

}