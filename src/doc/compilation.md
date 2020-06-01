# Compiling VCL


<a name="command_line"></a>
## Command lines to compile in Linux/MacOS


In Linux/MacOS, the following commands should compile and execute the code (assuming your command line is open in the root directory of the library).


__Using Make__
```shell
make
./pgm
```


__Using CMake__
```shell
mkdir build
cd build
cmake ..
make
cd ..
build/pgm
```


Note that the executable should be run from the root directory. 


_More precisely, the data (shaders and assets) defined in the scenes/ directory should be accessible from the running directory as files are read from this relative path._



# Detailed tutorials to set up your system and compile VCL


<a name="Ubuntu"></a>
## Linux/Ubuntu


In the case you need to set up a personal computer on Linux/Ubuntu from scratch, you may follow these steps


```shell
# Basic development tools (g++, make, etc)
sudo apt-get install build-essential


# CMake 
sudo apt-get install cmake 


# GLFW
sudo apt-get install glfw3-dev


# [Optionnal] QtCreator to edit files
sudo apt-get install qtcreator
```


Then follow 
* [Command lines to compile the code](#command_line)
* (optional) [Use an IDE to edit the code](qtcreator.md)



<a name="MacOS"></a>


## MacOS


In the case you need to set up a personal computer on MacOS from scratch, you may follow these steps


Check first that g++/clang++ is installed in typing in command line


```shell
g++ -v
```


(Follow the installation instruction if the OS proposes to install it.)


For the other dependencies, the easiest way is to use the package manager [Homebrew](https://brew.sh/). Follow the instructions of the website to install it.


Then install the necessary software and libraries


```shell
# The Library GLFW
brew install glfw


# CMake tool to compile
brew install cmake


# [Optionnal] QtCreator to edit files
brew cask install qt-creator
```


Then follow 
* [Command lines to compile the code](#command_line)
* (optional) [Use an IDE to edit the code](qtcreator.md)


## Windows

[Compilation with Visual Studio](visual_studio.md)
