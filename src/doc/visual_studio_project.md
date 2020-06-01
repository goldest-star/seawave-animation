# Compiling VCL using Visual Studio Project

This part describe the steps to load a Visual Studio Project from a CMakeLists.txt. Here CMake is used as an exernal tool to generate a .sln file used by Visual Studio.

* [Generate Visual Studio project from CMake](#Generate)
* [Load the project](#Load)
* [Setup Visual Studio](#Setup)
* [Compiling and running](#Run)
* [Note on language](#Language)


< [Back to Compilation in Windows](visual_studio.md)

<br>

<a name="Generate"></a>

## Generate Visual Studio project from CMake

* Start **CMake** (cmake-gui)
  * Fill "Where is the source code" with the root directory of VCL (= directory that contains CMakeList.txt)
  * Fill "Where to build the binaries" with "root_directory"`\build`
* Click **Configure**
  * Check that the generator is set to Visual Studio 2019
* Once the configuration is done, click **Generate**
  * Check that the generation is done
  * A `build` directory should be created and contain a file `project.sln`.

![](assets/visual_studio/project/00_cmake.gif)

<br>

<a name="Load"></a>

## Load the project

* Start Visual Studio, and use the option **Open a project or solution**. Open the file `project.sln` in the root directory.
* The project should be loaded, and the "Solution Explorer" should allows you to naviguate through the hierarchy of files.

![](assets/visual_studio/project/01_load_visual.gif)

<br>

<a name="Setup"></a>

## Setup Visual Studio

Project settings need to be adapted to export the executable in the correct place. By default Visual Studio will generate the executable file in a subdirectory like "root_directory"`\build\Debug\x64\pgm.exe`.
In this directory, pgm.exe will not have access to the data of the directory `scene\`. You will either need to copy scene in this output directory, or follow these settings

![](assets/visual_studio/project/02_visual_setup.gif)

* Right click on `pgm` (in the Solution Explorer)
  * Properties -> General
    * Change "Output Directory" to the root directory only (remove `\build\Debug`)
* Go to the Debug options (the options near the Green arrow to run the program)
  * ALL_BUILD Properties
    * General 
      * Change the Output Directory to `$(SolutionDir)..\` (will targets the root directory)
      * Set "Target Name" to `pgm`
    * Debugging
      * Change "Working Directory" to `$(TargetDir)`

<br>

<a name="Run"></a>

## Compiling and running

* The program can be compiled from **Build** -> **Build All** (or F7).
* Once compiled, the file `pgm.exe` should be generated in the root directory.
* You can either run the executable 
  * From the "Local Windows Debugger" within Visual Studio
  * In double-clicking directly on the file pgm.exe

![](assets/visual_studio/project/03_run.gif)

<br>

<a name="Language"></a>

## Note on language

It is recommended that you install the English version of Visual Studio (instead of the French one for instance) - or change the language once Visual Studio has been installed ([see these steps for instance](https://agirlamonggeeks.com/2019/03/10/how-to-change-language-in-visual-studio-2019-after-installation/)).
