package.path = "C:/Users/nightshade/Desktop/aconitum/assets/scripts/entities/?.lua;" .. package.path
package.path = "./assets/scripts/entities/?.lua;" .. package.path

local box = require("box")
local player = require("player")

entities_templates = {
	box = box:new(),
	player = player:new()
}

local entities = {
	box = box,
	player = player
}

return entities