MIngEd
======

Maratis3D Ingame Editor

This requires premake4, git and svn to build

Update 24.06.12 - Now compiles and links under Windows and Linux. Added libnui which definitely compiles under Linux. Yet to check Windows. Still need to integrate libnui properly with minged

Update 20.06.12 - After reworking the premake build system, it compiles! It doesn't link yet (on Windows) but it's a start

---

TODO
----

- Build dependencies for libnui
- Replace components of libnui with Maratis wrappers (renderer, file loader etc)
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