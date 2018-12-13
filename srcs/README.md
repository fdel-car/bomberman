# Objective
The purpose of this Readme is to give a good overall understanding on how the engine has been built and how it is supposed to run. In this way you should be able to easily create your own game using our engine.

### Engine logic overview
![Alt text](./diagram.png?raw=true "Diagram")

# Preamble
Normally you shouldn't have the need to modify the game engine resources, however you will have to include some of the engine includes (for example AGame.hpp and Entity.hpp) in order to be able to create the classes needed by the engine.

# The AGame class (a.k.a. Where to start)
In order to create your own game you will need to have a class which inherits from AGame. This class will contain all the infos needed by the engine to run properly, provided that you implemented all the virtual functions.

The engine will require the AGame to provide some entities (Camera and Light, but also a Entity list and a Skybox) for both the loading screen and the actual level that is currently playing.

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
- Then all basic Entity, if they are in the field of view of the Camera, are rendered based on their Model.
- The Skybox is then added.
- Finally, the GUI object is given to the Camera in order to add any gui.

The GameRenderer other roles are to tell the GameEngine if there are some user inputs (both from keyboard and mouse) and to adapt the window resolution to the requested one.

# The GUI class
The GUI class is a wrapper for the Nuklear library and will enable the end user to create all the GUI/HUD related stuff (if he overrides the "drawGUI()" function in his Camera object).

The most called functions will surely be "uiStartBlock()" and "uiEndBlock()" since they are the mandatory start and end functions to add even the most basic UI element. Between them you can then call functions like "uiText()" that will add a text to the current block.

Another useful GUI function is "setStyle()", which needs to be called if you want to change the different colors of your theme.

# The AudioManager class
The AudioManager is nothing more than what its name implies: it handles everything that is audio related using the SFML Audio library. It's usage is quite straightforward to, with functions like "playMusic()" or "updateSoundsVolume()" you will have the complete control of which audio source you are playing and how the output will be like.

The only thing that you will need to know it's that you can only play one music at the time, while for sounds you are capped at 254 simultaneous outputs.

# The Entity class
The Entity class is the cornerstone for each object of your game, therefore having a full comprehension on how you can change its attributes and use its functions is fundamental.

#### Constructing and Initializing
First of all you need to know that there is a clear difference between when an instance is created (with "new Entity(..)") and when the instance is retrieved and loaded by the GameEngine (the "initEntity()" function will then be called). The first will happen when the scene is loaded in a side thread while the second one will be called right before the first update is called on it.

After the entity has been initialized by the engine, it will have a pointer on the GameEngine object and you will have the possibility to use it at any time to call functions such as "entity->addNewEntity()".

#### Moving
The second most important point is that a Entity child has no direct acces to its 3D position. In order to move your entity you will have to set the "targetMovement" attribute to something different than the identity vector. The GameEngine will then move your entity of the desired amount in the given direction, handling the collisions with all others entities and triggering "onCollisionEnter()" and "onTriggerEnter()" if a Collider has been set.

#### Animations
Even though entities of the same class will have a pointer over the same Model, each single entity will be able to play (or not) it's own animation without being affected by the others. In order to do so, some attributes like "_currentAnimName" and "_loopAnim" are available to the end user of the class.

#### Scene Manager
When instantiating any entity you may give as a parameter a pointer over another entity (usually the Camera) and it will be stored in the "_sceneManager" attribute. This will enable your levels to have a reference object to which every other entity will report to every time they move or they die.

#### How to destroy
This bring us to the "_needToBeDestroyed" attribute, which is a boolean checked by the GameEngine at every frame and will tell whether the current entity is to be destroyed or not.

# The Collider class
As you may have guessed, an Entity can have a Collider attached to it. Colliders are used by the GameEngine to know if an Entity can perform the requested movement or if it's not possible due to a physical collision.

Beware that you can set a different "layerTag" attribute for each entity and, in your AGame instance, you will be able to define which layer collides with which. You can also change the layer of an entity at any moment during runtime, thus creating interesting changes in your gameplay.

# The Model class
We already mentioned this class in the GameRenderer but, now that we better understand how an Entity works, we will no longer delay the Model class presentation.
This class is the sum of one or more Mesh objects with a list of Joint objects, so it's the core object you need in order to have a visual representation of your game elements.

Thanks to the Assimp library, a Model may be created from both ".obj" and ".dae" files. Obviously only the latter will provide a skeleton, thus enabling the capability of animating the model.
To add additional animations (only one can be put in a ".dae") a function called "addAnimation()" is provided.

# The Mesh class
A Mesh object organises the vertices, materials and textures of a specific model's fragment. A Mesh is usually static but it can be deformed by the influence of its linked Joint objects.

# The Joint class
A Joint is a group of position, rotation and scale values that will cause a deformation to the Model thay are owned by. These deformations will depend on the time elapsed from the beginning of the animations since we lerp between two keyframes.

# The Camera class
The Camera class is a mandatory Entity for each level of your game since the GameRenderer will use its position and rotation to draw what is visible and what is not. Moreover it's also needed if you want to draw any UI.

Even the GameEngine uses the Camera in a unique way: it's this instance that will tell if the game is still running, or whether we have to update the other entities or if we need to load a different level.

# The Light class
The Light class is another mandatory Entity, but it's role is way more simple than the Camera. For instance, it's only used by the GameRenderer to know the direction of the main light and it's color.

# The Skybox class
A Skybox object is a child of the Entity class and it's quite easy to setup and, except if you need to define a special behaviour, you will never need to inherit from it.

To load your own skybox just give as a parameter the name of the folder containing your skybox ".png" files, just check that their names are "top", "bottom", "left", "right", "front" and "back".
