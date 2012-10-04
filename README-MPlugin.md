MPlugin, or, How to extend Maratis.
===================================

Creating a plugin for Maratis is easy, just check the example in examples/plugin to see how simple.
As you probably don't want to make the wrong move and recompile the whole of Maratis each time, I would suggest using the structure within the example folder, rather than attempting to compile it within the Maratis/MIngEd source tree. However, if you wish to do the latter, the code is identical. Just modify the build scripts to allow for the header/library locations

Setting up a plugin
-------------------

In order to create a plugin project, the code needed is quite minimal. Take a look at premake4.lua in examples/plugin for correct compilation setup. For further information on the premake setup within Maratis/MIngEd, please check README-premake.md and the [premake documentation](http://industriousone.com/premake/userguide).

Each plugin needs a minimum of two files, the plugin header (example.h) and the plugin implemantation (example.cpp). Within the header, all that needs to be done is:

`#include <MEngine.h>
 #include <MPlugin.h>

 MPLUGIN_DECLARE(Example);`

In the implementation file, the following will create the basis for the plugin.

`MPLUGIN_START_IMPLEMENT(Example)
 {
 }

 MPLUGIN_END_IMPLEMENT(Example)
 {
 }`

-- See examples/plugin

Setting up an editor plugin
---------------------------

To get a plugin loading into MIngEd automatically, a few changes have to be made to the above description. The header should be as follows:

`#include <MMIngEdPlugin.h>

MINGED_PLUGIN_DECLARE(ExampleEditor);`

With the following in the source file:

`#include "ExampleEditor.h"
#include "ExampleEditor_npk.h"

MINGED_PLUGIN_START_IMPLEMENT(ExampleEditor)
{
}

MINGED_PLUGIN_END_IMPLEMENT(ExampleEditor)
{
}`

The main difference here (apart from further wrapping in MINGED_PLUGIN_ rather than MPLUGIN_) is that we have to include another file. This file has to exist, and it contains all of the editor scripts and data. To create this file, the easiest way is to use Mnpk tool to package a directory into an npk package, then use MEmbedder to create an embedded file to compile.

`$ Mnpk Example.npk data/`
$ MEmbedder Example.npk Example_npk.h Example_npk`

It is often useful to split editor and gameplay functionality into two separate plugins. In this case, the best option is to manually load the runtime plugin within the editor plugin. It will still have to be manually loaded from within a game, when not run through MIngEd, however this will ensure the correct loading of definitions for the editor functionality.

`MINGED_PLUGIN_START_IMPLEMENT(ExampleEditor)
{
	MEngine::getInstance()->loadPlugin("Example");
}`

-- See examples/editorPlugin

Compiling a plugin
------------------

Please take a look at the `premake4.lua` found within `examples/plugin` for the correct setup for compiling a Maratis/MIngEd plugin. This script assumes that Maratis (or MIngEd) is installed at `$MSDKDIR`. If compiling Maratis/MIngEd from source, running the relevant `setup` script in `env/${OS}/` should create all environment variables and links required.

For instructions on how to compile a premake4 project, please see the [premake documentation]((http://industriousone.com/premake/userguide).

Installing a plugin
-------------------

Assuming you have $MSDKDIR/Tools in your PATH environment variable. The following would install the plugin Example.so to ~/.Maratis/Plugins in Linux

`MPlugin install Example.so`

And likewise, on Windows, to %APPDATA%/Maratis/Plugins

`MPlugin install Example.dll`

And finally, on Mac OS X, to ~/.Maratis/Plugins

`MPlugin install Example.dylib`

For an editor plugin, it is recommended that they get installed to the Maratis system directory. This can simply be done by replacing `install` with `install-sys`

Using a plugin
--------------

To load a plugin into Maratis, there are two ways to do this. Firstly, it is possible to explicitely declare which plugin to load from the command line in the following way:

`Maratis --preload=Example`

This is a useful way to check that plugins are linking correctly, but isn't normally recommended for development.

Instead, from within the game plugin (which is automatically loaded) you can do the following:

`MEngine::getInstance()->loadPlugin("Example");`

Publishing a plugin
-------------------

-- Really need to decide how to specify which plugins to pull in for publishing. Auto generated XML file?

Notes
-----

A plugin support compatability header is provided at `examples/PluginCompatability.h`. This will allow the same plugin code to be compiled without MIngEd support in the engine.

Plugin loading is done by performing a search of plugin path directories. This is as follows:
`./Plugins/` (Usually within the game project root)
`{HOME}/.Maratis/Plugins`
`{MSDKDIR}/Plugins`
This means that it is possible to have a standard plugin which is loaded at a higher level (eg from an editor plugin) and is then replaced within the project for a custom version.

TODO
----

* Cleanup plugin checking - possibly allow cross platform checking?
* Split plugins into directories for each OS/platform
* Test static plugins
* Save referenced plugins for publishing - xml?
* Plugin dependencies and interdependencies