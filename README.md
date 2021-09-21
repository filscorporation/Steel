# Steel Engine

Steel is a 2D OpenSource C++ Engine with C# scripting support.

## Current features
* Windows and Linux support
* Fast 2D sprites batch rendering (using OpenGL)
* Aseprite integration
* 2D physics support (using Box2D)
* Basic sounds (using OpenAL)
* Full built-in ECS created from scratch
* Simple sprites animation system
* Fast built-in UI library powered by engine's rendering system
* Full support of C# scripting with documented API (using Mono)

## Project structure
* application - contains base C++ executable project that builds application powered by Steel Engine
* editor - contains engine editor project (WIP)
* engine - contains engine sources that can be compiled into library
* examples - contains example projects
* scripting - contains projects to compile scripts dll. SteelCore is used to expose engine API to custom scripts, SteelCustom is a custom scripts project template
* tests - contains autotests for various engine systems

## Installation
#### Requirements
Project requires Mono to be installed (https://www.mono-project.com/docs/getting-started/install/)
#### Supported platforms
Windows and Linux are currently supported. On windows for now it can only be compiled with VS compiler.
#### Building project
* Build SteelCore C# project (scripting/SteelCore)
* Build SteelCustom C# project (scripting/SteelCustom)
* Build Application executable (application)
* Run Application
#### Distribution build
There is currently an option in CMakeLists to enable distribution build. It will disable console and debugging functionality and use local paths for dependencies. This option is not finished and for now it is required to collect project files by hands (resources, dependencies dlls, scripting dlls). Later this option will help to build an all-in-one final application.
#### Release configuration
Engine performance will drastically increase if built in release configuration.

## Developing with Steel Engine
Scripting API is very similar to Unity. Some basic examples can be found in scripting/SteelCustom/GameManager.cs and projects examples.
To create your own application logic expand SteelCustom project: create ScriptingComponents inheritors, override event functions and initialize starting scene members in GameManager.EntryPoint() method. Application resources loaded with ResourcesManager will be searched in application/resources folder.
