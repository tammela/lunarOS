#!/usr/bin/lua5.3

function error()
   print("Input is malformed")
   os.exit(-1)
end

function help()
   print("No arguments")
end

if not arg[1] then
   help()
   os.exit(-1)
end

local address = tonumber(arg[1]) or (tonumber(string.format("0x%s", arg[1])) or error())
address = address >> 12 -- get rid of the offset
local pte = address & 0x1FF
address = address >> 9
local pde = address & 0x1FF
address = address >> 9
local pdpte = address & 0x1FF
address = address >> 9
local pml4 = address & 0x1FF

print("pml4", pml4)
print("pdpte", pdpte)
print("pde", pde)
print("pte", pte)
