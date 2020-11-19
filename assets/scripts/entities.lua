package.path = "C:/Users/nightshade/Desktop/aconitum/assets/scripts/entities/?.lua;" .. package.path
package.path = "./assets/scripts/entities/?.lua;" .. package.path
package.path = "C:/Users/nightshade/Desktop/aconitum/assets/scripts/?.lua;" .. package.path
package.path = "./assets/scripts/?.lua;" .. package.path

package.loaded.helpers = nil
local helpers = require("helpers")

local box = helpers.reloading_require("box")
local player = helpers.reloading_require("player")
local stalker = helpers.reloading_require("stalker")
local metal_box = helpers.reloading_require("metal_box")

entities_templates = {
	box = box:new(),
	player = player:new(),
	stalker = stalker:new(),
	metal_box = metal_box:new()
}

local entities = {
	box = box,
	player = player,
	stalker = stalker,
	metal_box = metal_box
}

return entities