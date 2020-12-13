local torch = { }

function torch:new()
	local o = {
        iob = ex_iob:new(),
        torch = ex_torch:new(),
		animation = ex_animation:new(32.0, 32.0, 2),
		position = ex_position:new(),
		next_position = ex_next_position:new(),
		sprite = ex_sprite:new("./assets/textures/torch.png", true),
	}
	setmetatable(o, self);
	return o;
end

return torch