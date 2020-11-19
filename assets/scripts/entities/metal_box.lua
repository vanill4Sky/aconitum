local metal_box = { }

function metal_box:new()
	local o = {
		iob = ex_iob:new(),
		box = ex_box:new(),
		animation = ex_animation:new(23.0, 31.0, 5),
		position = ex_position:new(),
		next_position = ex_next_position:new(),
		collider = ex_collider:new(1.0, 5.0, 21.0, 26.0),
		sprite = ex_sprite:new("./assets/textures/metal_box_01.png"),
	}
	setmetatable(o, self);
	return o;
end

return metal_box