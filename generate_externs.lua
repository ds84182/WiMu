local lfs = require "lfs"

local defs = {}

for file in lfs.dir(".") do
	if file:sub(-2,-1) == ".o" then
		--build a list of global symbols that we NEED to keep
		for line in io.popen("nm "..file, "r"):lines() do
			local addr, type, name = line:match("(%x-)%s+(.-)%s+(.+)")
			
			if type == "G" or type == "T" or type == "V" then
				defs[#defs+1] = "EXTERN("..name..");"
			end
		end
	end
end

print(table.concat(defs, "\n"))
