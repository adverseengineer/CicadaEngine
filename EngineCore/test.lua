
log('these', 'are', 'args', 4, true, {})

Event.Register('OnStart', function()
	log('info text')
end)

Event.Register('OnStart', function()
	warn('warning text'..'wahoo?')
end)

local foo = function() 
	error('this one is a local!')
end
Event.Register('OnStart', foo)

local counter = 0
Event.Register('OnUpdate', function()
	counter = counter + 1
	if counter % 100 == 0 then
		log('another 100 frames! #'..counter)
	end
end)
