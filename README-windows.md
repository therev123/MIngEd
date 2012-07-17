Developing with Maratis/MIngEd under Windows
==========================================

I've provided my environment scripts to help developing with MIngEd.

To setup the environment, you should just be able to run the `env/linux/setup.bat` and it should take care of everything for you.

It defaults to installing scripts to `%HOMEPATH%/bin`, creating a symlink at `%HOMEPATH%/MIngEd` to the `pkg/Maratis` directory in the development tree and adding environment variables to the system.

To change these defaults, just run `setup.bat` as follows:

`setup.bat [installdir]`

eg. `setup.bat C:\`

Would set MIngEd up on the root of the C: drive. the script will ask for administrator access for permission to create the links required.

NOTE: The environment comes with an `MSDK` scripts which is used for switching between the stock Maratis SDK and Maratis/MIngEd. To use this script, first it expects the standard Maratis distribution to be present alongside where the setup script has put the symlink to MIngEd, then you can select which SDK to use with the following:

`MSDK minged` Selects MIngEd as the current SDK.
`MSDK` Selects the original Maratis distribution.

NOTE:
-----
`pkg/Maratis` is a tree structure created to match the standard Maratis distribution. This can be created, once the project is compiled, by running `build/MTool scripts/MPackager.lua` or, once the project has been packaged for the first time, simply `MPackager`