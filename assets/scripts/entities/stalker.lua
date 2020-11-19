local stalker = { }

function stalker:new()
	local o = {
		mob = ex_mob:new(),
		stalker = ex_stalker:new(),
		animation = ex_animation:new(64.0, 64.0, 9),
		velocity = ex_velocity:new(ex_dir.none, 1.5),
		position = ex_position:new(),
		next_position = ex_next_position:new(),
		collider = ex_collider:new(20.0, 44.0, 24.0, 18.0),
		sprite = ex_sprite:new("./assets/textures/stalker_01.png"),
		target = ex_target:new(200.0, 25.0)
	}
	setmetatable(o, self);
	return o;
end

return stalker