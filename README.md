# CrashpadHandlerCPP
A simple executable designed to wait for a process to close.
The program would then tell the user the program crashed and ask to enable crash dumps.

## Usage
To use the program in your project, you must first obtain the executable. See "Compiling From Source" for more information on compiling the project.
You can also obtain binaries from the releases if you don't want to compile the source code.

Your program will have to start the executable programmatically with the PID of the program you want it to watch as a parameter.
On windows you will have to specify a parameter to a .reg file for enabling crash dumps.

You will also have to make your program close the crashpadhandler process when your program exits normally.
This will make the crash dialog only appear when the program closes unexpectedly.

## Compiling From Source
1. Install cmake, cmake-gui, and a compiler. (*MingW for Windows | GCC for Linux*)
2. Select a preset. (*Either Linux_x64 or Windows_x64 **Note: Cross compilation from Windows to Linux not tested***)
3. Press the "Configure" and "Generate" button located at the bottom of the interface.
4. Open a terminal and cd into the build directory (For Windows : {PROJECT ROOT}/bin/Windows/) (For Linux : {PROJECT ROOT}/bin/Linux/)
5. Run the "make" command.
6. The compiled executable should appear in the same directory.
