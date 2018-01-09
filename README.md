# Darkspace
* A Tactical starship combat MMO
* Visit http://www.darkspace.net for more information about the game.

# Requirements:
* Windows 7/8/10
* Visual Studio 2017

# Cloning:
* Clone the following git repositories into the same directory, the directory names should be named as below since the projects/solutions span across all 3 directories.
* /DarkSpace/ should be cloned from https://github.com/palestar/darkspace.git
* /Medusa/ should be cloned from https://github.com/palestar/medusa.git
* /GameCQ/ should be cloned from https://github.com/palestar/gamecq.git

# Running the Client:
* Open the solution, DarkSpace/DarkSpace.sln, compile the solution.
* Set DarkSpaceClient as the StartUp Project.
* Open the properties for DarkSpaceClient and go to Configuration Properties > Debugging.
* The following entries should be populated:
```
Command			$(TargetDir)\$(TargetName).exe
Command Arguments	-connect ip port -login username password
Working Directory	..
```
#### Notes:  
1. If you're running under Debug profile, Command should be changed to $(TargetDir)\$(TargetName)D.exe  
2. If you're running the server locally, the default ip should be localhost and port 9020.  
3. The username and password required are the same used to login to the GCQL client or DarkSpace website.  

# Running a Server:
* Open the solution, DarkSpace/Server.sln, compile the solution if needed.
* Set DarkSpaceServer as the StartUp Project.
* Open the properties for DarkSpaceServer and go to Configuration Properties > Debugging.
* The following entries should be populated:
```
Command			$(TargetDir)\$(TargetName).exe
Command Arguments	Config\Debug.ini
Working Directory	..
```
#### Notes:  
1. If you're running under Debug profile, Command should be changed to $(TargetDir)\$(TargetName)D.exe  
2. It's a good idea to make a copy the Debug config file and use that one so if something goes wrong you can swap out easily.  
3. Do not populate the user and password fields in the config file for the server, these are reserved for persistent prestige enabled servers and enabling them will result in the meta-server refusing access.  

# Contributing:
* master is the main release branch, only HOTFIX's will be pushed into this branch directly.
* develop is the main development branch, this branch is compiled nightly and pushed to the DarkSpace beta servers. 
* All pull requests should be made against the develop branch. Once a pull request is made, a administrator will review the code/data changes and make notes if changes are needed. 
* Follow the coding standards of the code base. Pull requests that don't follow the coding standard will not be pulled.
* Use the issues to communicate between all developers.