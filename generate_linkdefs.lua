local input = io.popen("nm wimu.elf", "r")

local syms = {}
local defs = {}

local exclude = {
	linkdefs = true,
	__main = true,
	ps_quQuatScale = true
}

for line in input:lines() do
	local addr, type, name = line:match("(%x-)%s+(.-)%s+(.+)")
	
	if type:upper() == type and name:sub(1, 5) ~= "__cxa" and not exclude[name] then
		syms[#syms+1] = '{"'..name..'", (void*)&'..name..'},'
		defs[#defs+1] = 'extern void *'..name..';'
	end
end

print(table.concat(defs,"\n"))
print()

print("linkdef linkdefs[] = {")
print(table.concat(syms,"\n"))
print("{0,0}")
print("};")
