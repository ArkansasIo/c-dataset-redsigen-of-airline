#!/usr/bin/env lua
-- demo.lua
-- Demonstrates the RecordBase OO Lua library (lib/dbtree.lua).
-- Run with:  lua scripts/demo.lua

package.path = package.path .. ";./?.lua;" .. arg[0]:gsub("[^/\\]+$", "") .. "../?.lua;" ..
               (arg[0]:gsub("[^/\\]+$", "")) .. "../lib/?.lua"

local DBTree = require("dbtree")

-- Build the database -----------------------------------------------
local db = DBTree.new("RecordBase")

local t1 = db:add_transaction("T0001", "E0001", "PR101", 245.00)
local t2 = db:add_transaction("T0002", "E0002", "PR101", 245.00)
local t3 = db:add_transaction("T0003", "E0003", "PR205", 310.50)

-- Build the tree for T0001 ------------------------------------------
local root = t1:add_node("Main Tree", "ROOT", "idx_12A")
local bag  = root:add_child("Record Group", "SUBTREE")
local br   = bag:add_branch("Archived Records", "OPEN")
local sb   = br:add_sub_branch("Standard Record", "OPEN")
sb:add_module("retention_years", "7")
sb:add_module("storage_fee", "0.00")

local br2 = bag:add_branch("Active Records", "OPEN")
local sb2 = br2:add_sub_branch("Oversize Record", "OPEN")
sb2:add_module("retention_years", "10")
sb2:add_module("storage_fee", "75.00")

-- Build the tree for T0002 ------------------------------------------
local root2 = t2:add_node("Main Tree", "ROOT", "idx_12B")
root2:add_branch("Quick Lookup", "OPEN"):add_sub_branch("Temp Index", "OPEN")
    :add_module("ttl_days", "30")

-- Show everything ----------------------------------------------------
db:print_all()

-- A few API checks ---------------------------------------------------
assert(db:count() == 3)
assert(math.abs(db:total_amount() - 800.50) < 0.001)
assert(db:find_transaction("T0002") ~= nil)
assert(db:remove_transaction("T0003"))
assert(db:count() == 2)
assert(root2:toggle() == false)
assert(root2:toggle() == true)

print("All Lua OO library checks passed.")
