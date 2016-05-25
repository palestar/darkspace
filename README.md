# Darkspace
* A Tactical starship combat MMO
* Visit http://www.darkspace.net for more information about the game.

# Requirements:
* Windows 7/8/10
* Visual Studio 2005

# Cloning:
* Clone the following git repositories into the same directory, the directory names should be named as below since the projects/solutions span accross all 3 directories.
* /DarkSpace/ should be cloned from https://github.com/palestar/darkspace.git
* /Medusa/ should be cloned from https://github.com/palestar/medusa.git
* /GameCQ/ should be cloned from https://github.com/palestar/gamecq.git

# Running the Client:
* Open the solution, DarkSpace/DarkSpace.sln, compile the solution.
* Set the working directory for debugging to /DarkSpace (the same directory containing the Data/) directory.
* Add "-login [user] [pass]" to the command line options and provide your username/password from the gamecq server.
* Add "-connect [host] [port]" to specify the server to connect into. 

# Running a Server:
* Open the solution, DarkSpace/Server.sln, compile the solution if needed.
* Set the working directory to /DarkSpace
* Set the DarkSpaceServer project as the start project, the command line options will specify the server config file to load.

# Contributing:
* master is the main release branch, only HOTFIX's will be pushed into this branch directly.
* develop is the main development branch, this branch is compiled nightly and pushed to the DarkSpace beta servers. 
* All pull requests should be made against the develop branch. Once a pull request is made, a administrator will review the code/data changes and make notes if changes are needed. 
* Follow the coding standards of the code base. Pull requests that don't follow the coding standard will not be pulled.
* Use the issues to communicate between all developers. 
