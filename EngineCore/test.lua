
Event.Register('OnStart', function()
	print('wahoo')
end)

Event.Register('OnStart', function()
	print('SUCK ME !!!!!')
end)

Event.Register('OnStart', function() 
	print('bing bong')
end)

local counter = 0
Event.Register('OnUpdate', function()
	counter = counter + 1
	if counter % 100 then
		print('another 100 frames!'..counter)
	end
end)
