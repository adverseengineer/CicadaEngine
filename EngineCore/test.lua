
local v1 = vec2.new(2.2, 3.3)

Event.Register('OnStart', function()
	log.info('info text')
end)

Event.Register('OnStart', function()
	log.warn('warning text'..'wahoo?')
end)

local foo = function() 
	log.error('this one is a local!')
end
Event.Register('OnStart', foo)

local counter = 0
Event.Register('OnUpdate', function()
	counter = counter + 1
	if counter % 100 == 0 then
		--log.info('another 100 frames! #'..counter)
	end
end)
