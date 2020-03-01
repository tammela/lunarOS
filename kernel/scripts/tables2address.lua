#!/usr/bin/lua5.3

function error()
   print("Input is malformed")
   os.exit(-1)
end

function help()
   print("No arguments")
end

if not arg[1] or type(arg[1]) ~= "string" then
   help()
   os.exit(-1)
end

local address = 0
local t = load(arg[1], "argument chunk", "t")()
address = address | (t.pml4 << 39)
address = address | (t.pdpt << 30)
address = address | (t.pd << 21)
address = address | (t.pt << 12)

if t.pml4 > 255 then -- higher half
   address = address | (0xFFFF << 48)
end

print(string.format("%x", address))
