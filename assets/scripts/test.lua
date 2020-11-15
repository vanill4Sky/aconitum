package.path = "C:/Users/nightshade/Desktop/aconitum/assets/scripts/entities/?.lua;" .. package.path
package.path = "./assets/scripts/?.lua;" .. package.path
local entities = require("entities")

local box_1 = entities.box:new();
local box_2 = entities.box:new();
box_1.position.pos.x = 20.0;
print(box_2.position.pos.x)
print(box_1, box_2)
