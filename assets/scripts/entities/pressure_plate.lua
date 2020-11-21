local pressure_plate = { }

function pressure_plate:new()
	local o = {
        iob = ex_iob:new(),
        pressure_plate = ex_pressure_plate:new(),
		animation = ex_animation:new(23.0, 21.0, 2),
		position = ex_position:new(),
		next_position = ex_next_position:new(),
		collider = ex_collider:new(2.0, 2.0, 19.0, 17.0),
		trigger = ex_trigger:new(),
		sprite = ex_sprite:new("./assets/textures/pressure_plate_01.png", true),
	}
	setmetatable(o, self);
	return o;
end

return pressure_plate