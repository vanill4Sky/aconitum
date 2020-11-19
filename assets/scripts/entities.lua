package.path = "C:/Users/nightshade/Desktop/aconitum/assets/scripts/entities/?.lua;" .. package.path
package.path = "./assets/scripts/entities/?.lua;" .. package.path
package.path = "C:/Users/nightshade/Desktop/aconitum/assets/scripts/?.lua;" .. package.path
package.path = "./assets/scripts/?.lua;" .. package.path

package.loaded.helpers = nil
local helpers = require("helpers")

local entities = {
	box = helpers.reloading_require("box"),
	player = helpers.reloading_require("player"),
	stalker = helpers.reloading_require("stalker"),
	metal_box = helpers.reloading_require("metal_box"),
	doors = helpers.reloading_require("doors")
}

entities_templates = {
	box = entities.box:new(),
	player = entities.player:new(),
	stalker = entities.stalker:new(),
	metal_box = entities.metal_box:new(),
	doors = entities.doors:new()
}

return entities