#include "state_manager.hpp"

#include <cassert>

void aco::state_manager::replace_state(aco::state_ref new_state)
{
	m_new_state = std::move(new_state);
	m_is_removing = true;
	m_is_adding = true;
}

void aco::state_manager::push_state(aco::state_ref new_state)
{
	m_new_state = std::move(new_state);
	m_is_adding = true;
}

void aco::state_manager::pop_state()
{
	m_is_removing = true;
}

void aco::state_manager::process_state_transition()
{
	if (m_is_removing && !m_states.empty())
	{
		m_states.pop();
		m_is_removing = false;

		if (!m_is_adding && !m_states.empty())
		{
			m_states.top()->resume();
		}
	}

	if (m_is_adding)
	{
		if (!m_states.empty())
		{
			m_states.top()->pause();
		}

		m_states.push(std::move(m_new_state));
		m_states.top()->init();
		m_is_adding = false;
	}
}

aco::state_ref& aco::state_manager::get_active_state()
{
	assert(!m_states.empty());

	return m_states.top();
}
