local input = io.popen("nm wimu.elf", "r")

local syms = {}

for line in input:lines() do
	local addr, type, name = line:match("(%x-)%s+(.-)%s+(.+)")
	
	if type:upper() == type and name:sub(1, 5) ~= "__cxa" then
		syms[#syms+1] = name
	end
end

print("EXTERN("..table.concat(syms,",")..")")
