local box = { }

function box:new()
	local o = {
		iob = ex_iob:new(),
		box = ex_box:new(),
		position = ex_position:new(),
		next_position = ex_next_position:new(),
		collider = ex_collider:new(0.0, 0.0, 23.0, 32.0),
		sprite = ex_sprite:new("./assets/textures/box_01.png")
	}
	setmetatable(o, self);
	return o;
end

return box