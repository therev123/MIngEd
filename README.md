MIngEd
======

Maratis3D Ingame Editor

Overview
--------

This project is an extension of the Maratis3D game engine, using the existing plugin system to allow simple enhancements to be made easily and painlessly by anyone. The concept is that, rather than a separate editor, to access editor functionality, a game would be loaded with the minged plugin, which would be inactive during normal operation, but when a hotkey is pressed, would pause the game clock and display customisable editor overlays. This would also allow editor functionality to be defined by the game, and library plugins, most usually in the form of lua script extentions.

Requirements
------------

This requires premake4 to build, a version is provided in 3rdparty/premake4

Any questions about compiling or using this project, please feel free to contact me at <philipp@geyer.co.uk>

Updates
-------

Updates moved to CHANGELOG.md

Notes
-----

There are some perculiarities with now the repository, and the project are organised.

First of all, a quite note about the build system. I've chosen to use premake4 as I find it the least painful method of maintaining cross platform projects. In order to create the project/workspace/solution, I have included a copy of premake4 in tools/{os-type} and also some scripts in the root of the project. Probably all you need to do is run these scripts, but I've made them (for now) to aid my own development and I make no assurances that they would be set up for anyone else.
_See: Premake notes_

TODO
----

Todo moved to TODO.md