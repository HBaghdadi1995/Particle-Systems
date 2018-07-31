# Particle Systems

## AIM OF SOFTWARE
To Compare to diffirent implementations of Hardware Accelertaed Particle Systems

## SKILLS DEMONSTRATED:
* Particle Systems Shader Design
### OpenGl Implementation
* Basic OpenGL Skills
* Transform Feedback (used to implement Particle Systems)
### Vulkan Implementation
* Basic Vulkan Skills
* Compute Shader used to (used to implement Particle Systems)

## Insturctions
Before you can run this you must install Vulkan 1.1.73.0  to C:\VulkanSDK\1.1.73.0 you can download it [here](https://vulkan.lunarg.com/sdk/home#sdk/downloadConfirm/1.1.73.0/windows/VulkanSDK-1.1.73.0-Installer.exe "VulkanSDK-1.1.73.0-Installer.exe").

### Running the project
Visual Studio as well as an OpenGL 3.1 and Vulkan 1.1.73.0 enabled GPU are required to generate the executables, to generate them, select "Particle Systems" if you want the option to run either implementation or "OpenGL standalone Scene"/"Vulkan Standalone Scene" as the startup project then select any configuration and x32 as the platform, finally click on build or the local windows debugger.

### Getting Started
If on Particle Systems, you will be asked to choose which implementation to run. Next you will be asked how many sets of 1024 you would want to generate and simulate. _(this will be the first step on the stand alone scenes)._

### Getting The Results of Efficency

Results will be stored in a results.txt file with lines listing the number of particles generated and the length of a frame in milliseconds

### Controls

#### OpenGL Controls

* __WASD:__ Move Camera

* __up and down arrow keys:__ camera height.

* __Mouse Movement:__ Change Pitch and Yaw

* __ESC:__ Quit

#### Vulkan Controls


* __WASD:__ Move Camera

* __Mouse Movement:__ Change Pitch and Yaw

## Citations
* OpenGL code based on [Newcastle University Graphics Framework](https://research.ncl.ac.uk/game/mastersdegree/graphicsforgames/)
* Vulkan code based on [Vulkan Tutorial](https://vulkan-tutorial.com/)
