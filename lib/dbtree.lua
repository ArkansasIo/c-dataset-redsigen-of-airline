-- dbtree.lua
-- Object-oriented record-database tree library (Lua 5.4).
-- Mirrors the C++ structures in config/reservation_edit.h and the
-- MySQL schema in database/schema.sql:
--
--   transaction (tree root)
--     -> tree_node       (sub-node / subtree)
--        -> tree_branch  (branch)
--           -> tree_sub_branch (sub-branch)
--              -> tree_module (modular leaf)
--
-- Usage:
--   local DB = require("lib.dbtree")
--   local db = DB.new("RecordBase")
--   local t  = db:add_transaction("T0001", "E0001", "PR101", 245.00)
--   t:add_node("Main Tree", "ROOT", "idx_12A")
--   db:print_all()

local DBTree = {}

------------------------------------------------------------
-- helpers
------------------------------------------------------------
local function assert_str(v, what)
    if type(v) ~= "string" or #v == 0 then
        error(what .. " must be a non-empty string", 3)
    end
    return v
end

local function table_find(list, pred)
    for _, item in ipairs(list) do
        if pred(item) then return item end
    end
    return nil
end

------------------------------------------------------------
-- TreeModule (modular leaf)
------------------------------------------------------------
local TreeModule = {}
TreeModule.__index = TreeModule

function TreeModule.new(name, value)
    local self = setmetatable({}, TreeModule)
    self.name  = assert_str(name, "module name")
    self.value = value or ""
    return self
end

function TreeModule:to_string(indent)
    indent = indent or ""
    return string.format("%s[module] %s = %s", indent, self.name, self.value)
end

------------------------------------------------------------
-- TreeSubBranch
------------------------------------------------------------
local TreeSubBranch = {}
TreeSubBranch.__index = TreeSubBranch

function TreeSubBranch.new(name, status)
    local self = setmetatable({}, TreeSubBranch)
    self.name = assert_str(name, "sub-branch name")
    self.status = status or "OPEN"
    self.modules = {}
    return self
end

function TreeSubBranch:add_module(name, value)
    local m = TreeModule.new(name, value)
    table.insert(self.modules, m)
    return m
end

function TreeSubBranch:remove_module(name)
    for i, m in ipairs(self.modules) do
        if m.name == name then
            table.remove(self.modules, i)
            return true
        end
    end
    return false
end

function TreeSubBranch:find_module(name)
    return table_find(self.modules, function(m) return m.name == name end)
end

function TreeSubBranch:to_string(indent)
    indent = indent or ""
    local out = string.format("%s[sub-branch] %s (%s)\n",
        indent, self.name, self.status)
    for _, m in ipairs(self.modules) do
        out = out .. m:to_string(indent .. "    ") .. "\n"
    end
    return out
end

------------------------------------------------------------
-- TreeBranch
------------------------------------------------------------
local TreeBranch = {}
TreeBranch.__index = TreeBranch

function TreeBranch.new(name, status)
    local self = setmetatable({}, TreeBranch)
    self.name = assert_str(name, "branch name")
    self.status = status or "OPEN"
    self.sub_branches = {}
    return self
end

function TreeBranch:add_sub_branch(name, status)
    local sb = TreeSubBranch.new(name, status)
    table.insert(self.sub_branches, sb)
    return sb
end

function TreeBranch:find_sub_branch(name)
    return table_find(self.sub_branches, function(sb) return sb.name == name end)
end

function TreeBranch:to_string(indent)
    indent = indent or ""
    local out = string.format("%s[branch] %s (%s)\n", indent, self.name, self.status)
    for _, sb in ipairs(self.sub_branches) do
        out = out .. sb:to_string(indent .. "    ")
    end
    return out
end

------------------------------------------------------------
-- TreeNode (sub-node / subtree)
------------------------------------------------------------
local TreeNode = {}
TreeNode.__index = TreeNode

function TreeNode.new(name, node_type, key_name)
    local self = setmetatable({}, TreeNode)
    self.name = assert_str(name, "node name")
    self.node_type = node_type or "SUBTREE"
    self.key_name = key_name          -- index key (was seat assignment)
    self.available = true
    self.branches = {}
    self.children = {}                -- nested subtrees
    return self
end

function TreeNode:add_branch(name, status)
    local b = TreeBranch.new(name, status)
    table.insert(self.branches, b)
    return b
end

function TreeNode:add_child(name, node_type, key_name)
    local c = TreeNode.new(name, node_type, key_name)
    table.insert(self.children, c)
    return c
end

function TreeNode:toggle()
    self.available = not self.available
    return self.available
end

function TreeNode:to_string(indent)
    indent = indent or ""
    local avail = self.available and "available" or "unavailable"
    local out = string.format("%s[node] %s (%s, %s, key=%s)\n",
        indent, self.name, self.node_type, avail, self.key_name or "none")
    for _, b in ipairs(self.branches) do
        out = out .. b:to_string(indent .. "    ")
    end
    for _, c in ipairs(self.children) do
        out = out .. c:to_string(indent .. "    ")
    end
    return out
end

------------------------------------------------------------
-- Transaction (tree root)
------------------------------------------------------------
local Transaction = {}
Transaction.__index = Transaction

function Transaction.new(id, entry_id, dataset_id, amount)
    local self = setmetatable({}, Transaction)
    self.id = assert_str(id, "transaction id")
    self.entry_id = assert_str(entry_id, "entry id")
    self.dataset_id = assert_str(dataset_id, "dataset id")
    self.amount = tonumber(amount) or 0.0
    self.nodes = {}
    return self
end

function Transaction:add_node(name, node_type, key_name, parent)
    if parent then
        return parent:add_child(name, node_type, key_name)
    end
    local n = TreeNode.new(name, node_type, key_name)
    table.insert(self.nodes, n)
    return n
end

function Transaction:total_amount()
    return self.amount
end

function Transaction:to_string(indent)
    indent = indent or ""
    local out = string.format("%s[transaction] %s  entry=%s dataset=%s amount=%.2f\n",
        indent, self.id, self.entry_id, self.dataset_id, self.amount)
    for _, n in ipairs(self.nodes) do
        out = out .. n:to_string(indent .. "    ")
    end
    return out
end

------------------------------------------------------------
-- Database (top-level container)
------------------------------------------------------------
local Database = {}
Database.__index = Database

function DBTree.new(name)
    local self = setmetatable({}, Database)
    self.name = name or "RecordBase"
    self.transactions = {}
    return self
end

function Database:add_transaction(id, entry_id, dataset_id, amount)
    if self:find_transaction(id) then
        error("duplicate transaction id: " .. id, 2)
    end
    local t = Transaction.new(id, entry_id, dataset_id, amount)
    table.insert(self.transactions, t)
    return t
end

function Database:find_transaction(id)
    return table_find(self.transactions, function(t) return t.id == id end)
end

function Database:remove_transaction(id)
    for i, t in ipairs(self.transactions) do
        if t.id == id then
            table.remove(self.transactions, i)
            return true
        end
    end
    return false
end

function Database:count()
    return #self.transactions
end

function Database:total_amount()
    local sum = 0.0
    for _, t in ipairs(self.transactions) do
        sum = sum + t.amount
    end
    return sum
end

function Database:print_all()
    print("=== " .. self.name .. " database ===")
    for _, t in ipairs(self.transactions) do
        io.write(t:to_string())
    end
    print(string.format("=== %d transaction(s), total %.2f ===",
        self:count(), self:total_amount()))
end

-- expose classes for extension
DBTree.Transaction   = Transaction
DBTree.TreeNode      = TreeNode
DBTree.TreeBranch    = TreeBranch
DBTree.TreeSubBranch = TreeSubBranch
DBTree.TreeModule    = TreeModule

return DBTree
