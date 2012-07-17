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

Using a plugin
--------------

To load a plugin into Maratis, there are two ways to do this. Firstly, it is possible to explicitely declare which plugin to load from the command line in the following way:

`Maratis --preload=Example`

This is a useful way to check that plugins are linking correctly, but isn't normally recommended for development.

Instead, from within the game plugin (which is automatically loaded) you can do the following:

`MPlugin examplePlugin;
 examplePlugin.load("Example");`

Publishing a plugin
-------------------

-- Really need to decide how to specify which plugins to pull in for publishing. Auto generated XML file?

TODO
----

* Cleanup plugin checking - possibly allow cross platform checking?
* Split plugins into directories for each OS/platform
* Test static plugins
* Save referenced plugins for publishing - xml?
* Plugin dependencies and interdependencies