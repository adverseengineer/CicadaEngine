
--gameobject = require('gameobject')
--util = require('util')

Events.OnLoad(function()
	--util.log('hello world!')
	
	--local gobj = gameobject.new(0, 0, 0)
	--gobj:enable() --turn on the object
	--gobj:disable() --turn off the object
	--gobj:destroy() --deconstruct the object, freeing the memory --TODO: figure out how to manage the lifetime of lua userdatums
	
	--if(gobj.mesh)
		--util.log("the new object has a mesh")
end)

Events.Register("OnUpdate", function()
	--util.log('we are in the game loop!')
end)

Events.OnUpdate(OnUpdate, function()
	--util.log('this is also in the game loop!')
end)

thirdUpdateFunc = function()
	--util.log('you can also register events like this to give the function a descriptive name!')
end

Events.Register("OnUpdate", thirdUpdateFunc)
