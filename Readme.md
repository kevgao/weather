# Weather System #
A simulation of different weathers using OpenGl.


## Dependencies: ##

### glfw3 ###
    * macos 
    I would recommend installing glfw3 with homebrew:
    ```bash
    brew install glfw
    ```
    
    * linux
    You may use the following  command to install glfw3.
    ```bash
    sudo apt-get install libglfw3
    sudo apt-get install libglfw3-dev
    ```
    

## Compile and Run ##
    1. Install glfw
       (You might need to modify the glfw directory in makefile if compile on macos)
    2. make
    3. ./project

## Interaction ##
    * W/A/S/D to navigate through the landscape
    * Mouse motion to look around
    * Menu to switch between different weathers

The generated scene would show a landscape with mountains and sky. There are options on the right pannel to modify current time and current weather. 

To voyage inside the scence, you can use key W/A/S/D to slightly navigate through the mountains, and the mouse would automatically control your view. 


## How it works ##

The following techniques are used:
* Shaders
* Cubemap texture (skybox)
* Terrain drawn from heightmap
* Particles
* Instancing





