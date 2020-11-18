local player = { }

function player:new()
	local o = {
		player = ex_player:new(),
		animation = ex_animation:new(64.0, 64.0),
		velocity = ex_velocity:new(ex_dir.none, 3.0),
		position = ex_position:new(),
		next_position = ex_next_position:new(),
		collider = ex_collider:new(20.0, 44.0, 24.0, 18.0),
		sprite = ex_sprite:new("./assets/textures/player_thief_01.png")
	}
	setmetatable(o, self);
	return o;
end

return player