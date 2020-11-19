#include "app.hpp"

#include <algorithm>
#include <SFML/System/Clock.hpp>
#include <spdlog/spdlog.h>

#include "editor_state.hpp"
#include "game_state.hpp"
#include "lua_binding.hpp"

aco::app::app(std::string_view window_title)
	: m_app_data{ aco::app_data::get_instance() }
{
	m_app_data.lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
	register_user_types(m_app_data.lua);

	m_app_data.window.create(sf::VideoMode{ 1500, 1000 }, window_title.data());
	//m_app_data.window.setFramerateLimit(60);
	m_app_data.state_manager.push_state(std::make_unique<aco::game_state>(m_app_data));

	m_app_data.window.setPosition(sf::Vector2i(1920 + (1920 - m_app_data.window.getSize().x) / 2, 0));
	spdlog::set_level(spdlog::level::debug);

	run();
}

void aco::app::run() const
{
	constexpr float dt{ 1.0f / 60.0f };

	sf::Clock clock;
	float new_time;
	float frame_time;
	float current_time{ clock.getElapsedTime().asSeconds() };
	float accumulator{ dt };

	while (m_app_data.window.isOpen())
	{
		m_app_data.state_manager.process_state_transition();

		new_time = clock.getElapsedTime().asSeconds();
		frame_time = new_time - current_time;
		frame_time = std::min(frame_time, 0.2f);
		current_time = new_time;

		accumulator += frame_time;

		while (accumulator >= dt)
		{
			m_app_data.state_manager.get_active_state()->handle_input();
			m_app_data.state_manager.get_active_state()->update(dt);
			accumulator -= dt;
		}

		m_app_data.state_manager.get_active_state()->draw();
	}
}
