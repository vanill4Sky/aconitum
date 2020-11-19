local helpers = {}

function helpers.reloading_require(modname)
	package.loaded[modname] = nil
	return require(modname)
end

return helpers