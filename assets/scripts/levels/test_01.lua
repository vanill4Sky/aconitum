package.path = "C:/Users/nightshade/Desktop/aconitum/assets/scripts/levels/?.lua;" .. package.path
package.path = "./assets/scripts/levels/?.lua;" .. package.path
package.path = "C:/Users/nightshade/Desktop/aconitum/assets/scripts/?.lua;" .. package.path
package.path = "./assets/scripts/?.lua;" .. package.path

local helpers = require("helpers")
-- tablica z definicjami wszystkich szablonów
local templates = helpers.reloading_require("entities")
-- tablica z namiastkami jednoste
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
ex_connect_torch(entities.lever_switch_0, entities.torch_0)
ex_connect_torch(entities.lever_switch_0, entities.torch_1)
ex_connect_torch(entities.lever_switch_0, entities.torch_2)
ex_connect_torch(entities.lever_switch_1, entities.torch_0)
ex_connect_torch(entities.lever_switch_1, entities.torch_3)
ex_connect_torch(entities.lever_switch_2, entities.torch_0)

function door_open()
	return (ex_has_active_state(entities.torch_0) 
		and ex_has_active_state(entities.torch_1)
		and ex_has_active_state(entities.torch_2)
		and ex_has_active_state(entities.torch_3)
	)
end
