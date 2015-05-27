-- Reset RNG seed to get consistent results across runs (i.e. XCode)
math.randomseed(12345)

local static = _ARGS[1] == 'static'
local action = premake.action.current()

if string.startswith(_ACTION, "vs") then
	-- We need debugging symbols for all configurations, but runtime library depends on official Symbols flag, so hack it
	function premake.vs200x_vcproj_symbols(cfg)
		return 3
	end

	if action then
		-- Disable solution generation
		function action.onsolution(sln)
			sln.vstudio_configs = premake.vstudio_buildconfigs(sln)
		end

		-- Rename output file
		function action.onproject(prj)
			premake.generate(prj, "%%_" .. _ACTION .. (static and "_static" or "") .. ".vcproj", premake.vs200x_vcproj)
		end
	end
elseif _ACTION == "codeblocks" then
	action.onsolution = nil

	function action.onproject(prj)
		premake.generate(prj, "%%_" .. _ACTION .. ".cbp", premake.codeblocks_cbp)
	end
elseif _ACTION == "codelite" then
	action.onsolution = nil

	function action.onproject(prj)
		premake.generate(prj, "%%_" .. _ACTION .. ".project", premake.codelite_project)
	end
end

solution "pugixml"
	objdir(_ACTION)
	targetdir(_ACTION)

if string.startswith(_ACTION, "vs") then
	if _ACTION ~= "vs2002" and _ACTION ~= "vs2003" then
		platforms { "x32", "x64" }

		configuration "x32" targetdir(_ACTION .. "/x32")
		configuration "x64" targetdir(_ACTION .. "/x64")
	end

	configurations { "Debug", "Release" }

    if static then
        configuration "Debug" targetsuffix "_sd"
        configuration "Release" targetsuffix "_s"
    else
        configuration "Debug" targetsuffix "_d"
    end
else
	if _ACTION == "xcode3" then
		platforms "universal"
	end

	configurations { "Debug", "Release" }

	configuration "Debug" targetsuffix "_d"
end

project "pugixml"
	kind "StaticLib"
	language "C++"
	files { "../src/pugixml.hpp", "../src/pugiconfig.hpp", "../src/pugixml.cpp" }
	flags { "NoPCH", "NoMinimalRebuild" }
	uuid "89A1E353-E2DC-495C-B403-742BE206ACED"

configuration "Debug"
	defines { "_DEBUG" }
	flags { "Symbols" }

configuration "Release"
	defines { "NDEBUG" }
	flags { "Optimize" }

if static then
    configuration "*"
        flags { "StaticRuntime" }
end
