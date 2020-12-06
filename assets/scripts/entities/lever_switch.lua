local lever_switch = { }

function lever_switch:new()
	local o = {
        iob = ex_iob:new(),
        lever_switch = ex_lever_switch:new(20.0),
		animation = ex_animation:new(52.0, 29.0, 2),
		position = ex_position:new(),
		next_position = ex_next_position:new(),
		sprite = ex_sprite:new("./assets/textures/lever_switch_52x29.png", true),
	}
	setmetatable(o, self);
	return o;
end

return lever_switch