Developing with Maratis/MIngEd under Linux
==========================================

I've provided my environment scripts to help developing with MIngEd.

To setup the environment, you should just be able to run the `env/linux/setup.sh` and it should take care of everything for you.

It defaults to installing scripts to `$(HOME)/bin`, creating a symlink at `$(HOME)/MIngEd` to the `pkg/Maratis` directory in the development tree and adding environment variables to `~/.bashrc`

To change these defaults, just run `setup.sh` as follows:

`setup.sh [installdir [profile]]`

eg. `sudo ./setup.sh / /etc/profile`
Would set MIngEd up on the root of the filesystem, and put the environment variables in the global profie. Obviously, you need write access to wherever you're wanting to set this up.

NOTE: The environment comes with an `MSDK` scripts which is used for switching between the stock Maratis SDK and Maratis/MIngEd. To use this script, first it expects the standard Maratis distribution to be present alongside where the setup script has put the symlink to MIngEd, then you can select which SDK to use with the following:

`MSDK minged` Selects MIngEd as the current SDK.
`MSDK` Selects the original Maratis distribution.

NOTE:
-----
`pkg/Maratis` is a tree structure created to match the standard Maratis distribution. This can be created, once the project is compiled, by running `build/MTool scripts/MPackager.lua` or, once the project has been packaged for the first time, simply `MPackager`