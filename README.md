# EnTT Tower Defense Example
Other tower defense examples using EnTT exist. This one was only built for learning experience also for a talk I gave at work for a Lunch n Learn, also for TechSphere Ottawa.

## Repository structure
* `assets/` Game content loaded at runtime. CMake will copy this into your build folder.

* `contentProcessor/` Node.js script to generate data.h to be consumed by the game. Run this if you will to modify the map or waves data.

* `src/` Game source files.

* `thirdparty/` Various thirdparty libraries and submodules used by this project.

## The ruleset for this project
- Components are struct only. No complex type and no logic.
- Systems are single function only.
- Systems don't call other systems.
- Helper functions are used to create complex entity types (Factories) or to perform eventful actions.

## Reference
Overwatch Gameplay Architecture and Netcode. The rules applied to this project are greatly influenced by this talk. Go check it out.
https://www.youtube.com/watch?v=W3aieHjyNvw