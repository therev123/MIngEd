MIngEd
======

Maratis3D Ingame Editor

Overview
--------

This project is an extension of the Maratis3D game engine, using the existing plugin system to allow simple enhancements to be made easily and painlessly by anyone. The concept is that, rather than a separate editor, to access editor functionality, a game would be loaded with the minged plugin, which would be inactive during normal operation, but when a hotkey is pressed, would pause the game clock and display customisable editor overlays. This would also allow editor functionality to be defined by the game, and library plugins, most usually in the form of lua script extentions.

Requirements
------------

This requires premake4, git and svn to build

Any questions about compiling or using this project, please feel free to contact me at <philipp@geyer.co.uk>

Updates
-------

Updates moved to CHANGELOG.md

Notes
-----

There are some perculiarities with now the repository, and the project are organised.

First of all, a quite note about the build system. I've chosen to use premake4 as I find it the least painful method of maintaining cross platform projects. In order to create the project/workspace/solution, I have included a copy of premake4 in tools/{os-type} and also some scripts in the root of the project. Probably all you need to do is run these scripts, but I've made them (for now) to aid my own development and I make no assurances that they would be set up for anyone else.
_See: Premake notes_

Secondly. The repository doesn't actually contain all the code you need. Normally what people seem to do is include all prerequisites in a dependency directory. I have chosen not to do this purposefully as I intend to try to allow for easy upgrade of dependencies. At some point I also would like to extend the build system to allow you to decide between compiling everything yourself, using prebuilt libraries included in the repository, or trying to find matching system libraries to link against. So yes, you need svn and git to be able to get the appropriate code. Eventually, I would like to be able to include this in with the build system (rather than calling os.execute from lua) but for now, it's dirty


TODO
----

- Write a script to generate the entire patch tree
- Finally begin work on the editor framework
- Simple editor panel with selected entity and transform information
- Hotkey to display/hide editor panel (F-key?)
- 3D picking to choose entity in panel
- Integrate lua, move editor functionality to lua script (note, should have standard Maratis lua API, plus minged module)
- Allow for multiple editor scripts to be running simultaneously
- Add more editor functionality, such as behaviour adding/editing
- Extend MPackageManager to support multiple npk package files
- Maybe embed an npk into minged lib with things such as images and layout
- Themes? (hey, if I get this far, themes hardly seem the most difficult thing to implement, right?)
- Extend premake4. It's fine as it is right now, but it would be nice to not have to use
        os.execute("svn checkout ...")
    > things to add to premake: svn, git (hg? for kicks?), patch.
    > this probably means having a crazy bootstrap build process.
- Allow support for precompiled 3rd party libs and linking to stock Maratis core components.
    > This probably means more editing of premake, adding support for dialogue boxes and the like - maybe GUI ones?
- Maybe add a console with direct lua interpretation?
- Create another plugin and/or script to extend minged
- While I'm making distant goals, why not create a plugin repository...