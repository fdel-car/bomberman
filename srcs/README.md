# Objective
The purpose of this Readme is to give a good overall understanding on how the engine has been built and how it is supposed to run. In this way you should be able to easily create your own game using our engine.

### Engine logic overview
![Alt text](./diagram.png?raw=true "Diagram")

# Preamble
Normally you shouldn't have the need to modify the game engine resources, however you will have to include some of the engine includes (for example AGame.hpp and Entity.hpp) in order to be able to create the classes needed by the engine.

# The AGame class (a.k.a. Where to start)
In order to create your own game you will need to have a class which inherits from AGame. This class will contain all the infos needed by the engine to run properly, provided that you implemented all the virtual functions.

The engine will require the AGame to provide some entities (Camera and Light, but also Entity list and a Skybox) for both the loading screen and the actual level that is currently playing.

How you handle a level switch and the change of in-use entities/assets is up to you, just bear in mind that the engine will always put the loading screen between each switch and will launch a thread to let you load all your new assets without freezing the game.

# The GameEngine class
The GameEngine class is the core class of the engine.
Its main role is to run the game loop and ensuring that every logical step is done at the right time:
- First of all it checks if a scene is loading, has finished loading, is running or has finished running.
- Then it retrieves all inputs from the GameRenderer and updates the deltaTime.
- The first entity to be update is the Camera object. It's the camera that will say if the level is to be ended/changed and if all other entities are to be updated or not.
- After the camera is updated, it's the light that will be updated.
- Last but not least, the update function is called on all the other entities.
- After all updates are done the engine will add the new requested entities to the entities list.
- All triggers and collisions will then be tested and triggered (if necessary), it's also at this time that the entities will be moved (if their "targetMovement" vector is set).
- All entities that need to be destroyed are then destroyed.
- The loop ends with a call to the refreshWindow function of the GameRenderer object, this will draw everything to the screen until the next frame.

# The GameRenderer class
The GameRender object is a wrapper for a GLFW context and its main duty is to display in a OpenGL window all the entities that are given by the GameEngine.
The order of rendering is as follow:
- The Light is added first and the shadows are pre-calculated.
- Then all basic Entity are rendered if they are in the field of view of the Camera.
- The Skybox is then added.
- Finally, the GUI object is given to the Camera in order to add any gui.


The GameRenderer other roles are to tell the GameEngine if there are some user inputs (both from keyboard and mouse) and to adapt the window resolution to the requested one.

# The GUI class

# The AudioManager class

# The Entity class

# The Camera class

# The Light class

# The Skybox class
