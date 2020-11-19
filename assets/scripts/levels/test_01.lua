package.path = "C:/Users/nightshade/Desktop/aconitum/assets/scripts/levels/?.lua;" .. package.path
package.path = "./assets/scripts/levels/?.lua;" .. package.path
package.path = "C:/Users/nightshade/Desktop/aconitum/assets/scripts/?.lua;" .. package.path
package.path = "./assets/scripts/?.lua;" .. package.path

local helpers = require("helpers")
local templates = helpers.reloading_require("entities")
local get_test_01_stubs = helpers.reloading_require("test_01_stubs")

local entities = {}
local stubs = get_test_01_stubs()
for id, stub in pairs(stubs) do
	local template = templates[stub.name]:new() 
	local position = stub.position
	local idx = stub.idx
	
	template["position"] = position
	template["next_position"] = position
	
	local entity = ex_create_entity(template, position, idx)
	
	entities[id] = entity
end

ex_set_target(entities.stalker_0, entities.player_0)

-- for id, entity in pairs(entities) do
-- 	print(id .. " = " .. entity)
-- end