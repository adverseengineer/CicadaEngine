
local testMath = function()

	log(math.pi)
	log(w)
end

log(math.pi)

Event.Register('OnStart', testMath)

local counter = 0
local interval = 1500
Event.Register('OnUpdate', function()
	counter = counter + 1
	if counter % interval == 0 then
		log('another '..interval..' frames! #'..counter)
	end
end)
