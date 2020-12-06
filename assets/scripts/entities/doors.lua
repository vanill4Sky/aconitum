local doors  = { }

function doors:new()
	local o = {
		iob = ex_iob:new(),
		door = ex_door:new(),
		animation = ex_animation:new(64.0, 43.0, 2),
		position = ex_position:new(),
		next_position = ex_next_position:new(),
		collider = ex_collider:new(0.0, 0.0, 64.0, 25.0),
		kinematic = ex_kinematic:new(),
		sprite = ex_sprite:new("./assets/textures/doors_01.png"),
	}
	setmetatable(o, self);
	return o;
end

return doors