# Physics-Simulation-Assignment
3rd Year assignment utilising C++, OpenGL and Physx to create a sandbox game for the manipluation of objects and joints. Both rendered models and physic meshes can be visualised either seperately or simulatanously providing an intuative way to explore how Physx works within the sandbox. Objects can be manipulated and combined through the use of fixed, spring and pivot joints.

# Dependancies
+ git
+ libsdl2-dev
+ libsdl2-image-dev
+ libglew-dev
+ libassimp-dev
+ libPhysX3
+ libPhysX3Common
+ libPhysX3Extensions
+ libPhysX3CharacterKinematic
+ libPhysXVisualDebuggerSDK
+ libPhysX3Cooking
+ libPhysX3GPU
+ libPxTask

# Building
## Clone repository
```bash
git clone https://github.com/sangwe11/Physics-Simulation-Assignment.git
```
## Linux
```bash
cd Physics-Simulation-Assignment
premake4 gmake
make
./bin/Debug/PhysicsAssignment
./bin/Release/PhysicsAssignment
```

## Windows
+ Place includes and dependancies in correct folders
+ Run visual_studio.bat
+ Open solution file
+ Build and run