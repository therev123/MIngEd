TODO
----

I'll start with a fairly unorganised list of goals. As I think of more things, I will add them to the bottom of the list (this doesn't mean that they are any less realistic, or have a lower priority) and when I complete one, I'll move it to a separate DONE section, along with a short note.

- Finally begin work on the editor framework
- Simple editor panel with selected entity and transform information
- 3D picking to choose entity in panel
- Integrate lua, move editor functionality to lua script (note, should have standard Maratis lua API, plus minged module)
- Allow for multiple editor scripts to be running simultaneously
- Add more editor functionality, such as behaviour adding/editing
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
- Design lua interface

DONE
----

- Write a script to generate the entire patch tree
    > This shouldn't be needed. Just using one patch file for the whole svn checkout now.
- Hotkey to display/hide editor panel (F-key?)
    > Ok, the editor panel is currently just a white square, but it toggles when you press F1
- Extend MPackageManager to support multiple npk package files
    > I changed a #define from 1->10. I think that should do it.