local lever_switch = { }

function lever_switch:new()
	local o = {
        iob = ex_iob:new(),
        lever_switch = ex_lever_switch:new(),
		animation = ex_animation:new(16.0, 16.0, 2),
		position = ex_position:new(),
		next_position = ex_next_position:new(),
		sprite = ex_sprite:new("./assets/textures/lever_switch.png", true),
	}
	setmetatable(o, self);
	return o;
end

return lever_switch