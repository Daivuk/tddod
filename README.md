# EnTT Tower Defense Example
Other tower defense examples using EnTT exist. This one was only built for learning experience also for a talk I gave at work for a Lunch n Learn, also for TechSphere Ottawa.

## Repository structure
* `assets/` Game content loaded at runtime. CMake will copy this into your build folder.

* `contentProcessor/` Node.js script to generate data.h to be consumed by the game. Run this if you will to modify the map or waves data.

* `src/` Game source files.

* `thirdparty/` Various thirdparty libraries and submodules used by this project.

* `visualizer/` Tool to visualize Tags/Systems/Components and see which uses which.

## The ruleset for this project
- Components are struct only. No complex type and no logic.
- Systems are single function only.
- Helper functions are used to create complex entity types (Factories) or to perform eventful actions.

## References
* Typical C++ Bullshit https://macton.smugmug.com/Other/2008-07-15-by-Eye-Fi/n-xmKDH/i-BrHWXdJ
* CppCon 2014: Mike Acton "Data-Oriented Design and C++"https://www.youtube.com/watch?v=rX0ItVEVjHc
* EnTT -  a fast and reliable entity-component system https://github.com/skypjack/entt
* Overwatch Gameplay Architecture and Netcode https://www.youtube.com/watch?v=W3aieHjyNvw
* Data Oriented Design Resources https://github.com/dbartolini/data-oriented-design
