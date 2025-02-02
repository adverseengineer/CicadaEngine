
log('these', 'are', 'args', 4, true, {})

for k,_ in pairs(Key) do
	log(k)
end

Event.Register('OnStart', function()
	log('info text')
end)

Event.Register('OnStart', function()
	warn('warning text'..'EAT MY PANTS')
end)

local foo = function() 
	error('this one is a local!')
end
Event.Register('OnStart', foo)

local counter = 0
local interval = 1500
Event.Register('OnUpdate', function()
	counter = counter + 1
	if counter % interval == 0 then
		log('another '..interval..' frames! #'..counter)
	end
end)
