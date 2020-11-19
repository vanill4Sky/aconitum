local doors  = { }

function doors:new()
	local o = {
		iob = ex_iob:new(),
		-- animation = ex_animation:new(23.0, 31.0, 5),
		position = ex_position:new(),
		next_position = ex_next_position:new(),
		collider = ex_collider:new(0.0, 0.0, 64.0, 25.0),
		sprite = ex_sprite:new("./assets/textures/doors_01.png"),
	}
	setmetatable(o, self);
	return o;
end

return doors