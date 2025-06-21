# CubeShooter
A small Unreal project where player has to shoot cubes which are spawned using a JSON file.


## Pre-Requisites:-
Unreal Engine 5.6 installed on your system.

## How to run the project:- 
1. Clone the repository on your machine.
2. Open the .uproject file which will launch Unreal Engine 5.6.
3. Hit the play button in Unreal Editor or press Alt + P.

## Video:-
https://www.youtube.com/watch?v=t452CdIqMUc

### Logic Implemented
1. Extracting and Parsing JSON file:-
Using Unreal's HTTP and JSON modules, the data for cubes like color, health, score and spawn transform was extracted from JSON file. After the connection to source was successfull, the data was deserialised into an object. The above modules has many utility funtions to parse the data in this object. The parsed data is then stored in two custom structs, with one struct containing spawn transform and the other containing the rest of the properties. Using these two structs, the cubes were then made to spawn with appropriate properties passed to each instance.

2. Custom Dynamic Material for cubes:- 
A custom material was created which has a "Color Control" node. This node changes the base color of the material instance depending on the RGB values fed to it. By using Unreal's Material Interface and MaterialInstanceDynamic class, the RGB values of the Color Control node can be changed dynamically at runtime through C++.

3. Gun Architecture:- 
A scalable gun architecture in which the abstract gun class defines the basic properties common to all weapons like Ammo count, Reload Speed, Firing Rate, Damage and Range etc. The child classes inheriting this class must override the Fire function and can provide default values to the basic properties. Right now, only Hitscan Gun class inherits from the this class. The Hitscan guns do a line trace when fired and instantly apply damage to the target. A projectile gun class can also inherit the abstract gun class and override the fire method for weapons that have bullets affected by gravity and takes time to travel to target.

4. Pickups:- 
A Pickup Component which when attached to an object, makes that object pick-able by the player. Right now the player can only pickup guns. If the player already has a gun, then the ammo of the pick-able gun will be added to the player's gun. Functionality to pick up ammo boxes and other items can be implemented without the need to extend the class thanks to Enums.

5. UI implementation using Unreal Motion Graphics (UMG):- 
Two classes were inherited from UMGs User Widget class in C++. The first class called Player HUD Widget class manages the Player's Heads Up Display and is responsible for showing crosshair, player's current score, ammo count and a victory screen congratulating the player for destroying all the cubes. The second class called Cube Health Widget class is reponsible for displaying the current health of each cube.


### Challenges Faced
1. Parsing JSON file:-
Having never touched web dev in my life, it was a challenge figuring out how to send a HTTP request, parse and extract the JSON data, especially with the scarcity of good tutorials achieving the functionality 'without' using any external plugin. So I had to figure this out on my own. Fortunately, I was guided by someone on Discord to look at Http.h and Json.h header files located in HTTP and JSON module resp. After hours of digging into the source code and trial and error, I was finally able to parse the JSON file.

2. Extracting JSON data and spawning cubes based on it:-
Having figured out parsing the JSON file, next step was to design an architecture which will correctly store the extracted data and spawn the cubes having desired properties. This was achieved by creating two structs:- FCubeDescription and FCubeSpawnInfo. The FCubeDescription will store properties common to each type of cube, like name, color, health and score. The FCubeSpawnInfo will store information regarding what type of cube to spawn and at what location, with desired rotation and scale. With the help of a TMap of <key, value> pair as <FString, FCubeDescription> and a TArray of type FCubeSpawnInfo, all the cubes with desired properties were spawned.
