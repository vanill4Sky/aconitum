package.path = "C:/Users/nightshade/Desktop/aconitum/assets/scripts/levels/?.lua;" .. package.path
package.path = "./assets/scripts/levels/?.lua;" .. package.path
package.path = "C:/Users/nightshade/Desktop/aconitum/assets/scripts/?.lua;" .. package.path
package.path = "./assets/scripts/?.lua;" .. package.path

local helpers = require("helpers")
-- tablica z definicjami wszystkich szablonów
local templates = helpers.reloading_require("entities")
-- tablica z namiastkami jednoste
local get_test_perf_stubs = helpers.reloading_require("test_perf_stubs")

local entities = {}
local stubs = get_test_perf_stubs()
for id, stub in pairs(stubs) do
	local template = templates[stub.name]:new() 
	local position = stub.position
	local idx = stub.idx
	
	template["position"] = position
	template["next_position"] = position
	
	local entity = ex_create_entity(template, position, idx)
	
	entities[id] = entity
end
for i = 0, 47, 1 do
    ex_set_target(entities["stalker_" .. i], entities.player_0)
end

function door_open()
	return (true)
end
