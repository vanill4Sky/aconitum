package.path = "C:/Users/nightshade/Desktop/aconitum/assets/scripts/levels/?.lua;" .. package.path
package.path = "./assets/scripts/levels/?.lua;" .. package.path
package.path = "C:/Users/nightshade/Desktop/aconitum/assets/scripts/?.lua;" .. package.path
package.path = "./assets/scripts/?.lua;" .. package.path

package.loaded.helpers = nil
local helpers = require("helpers")
local templates = helpers.reloading_require("entities")
local get_test_02_stubs = helpers.reloading_require("test_02_stubs")

local entities = {}
local stubs = get_test_02_stubs()
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
ex_set_target(entities.stalker_1, entities.player_0)
ex_set_target(entities.stalker_2, entities.player_0)
ex_set_target(entities.stalker_3, entities.player_0)
ex_set_target(entities.stalker_4, entities.player_0)
ex_set_target(entities.stalker_5, entities.player_0)
ex_set_target(entities.stalker_6, entities.player_0)
ex_set_target(entities.stalker_7, entities.player_0)
ex_set_target(entities.stalker_8, entities.player_0)
ex_set_target(entities.stalker_9, entities.player_0)