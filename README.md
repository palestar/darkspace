# Darkspace
A Tactical starship combat MMO

# Requirements:
* Windows 7/8/10
* Visual Studio 2005

# Building:
* Clone the following git repositories into your local drive in the following directory structure:
* /DarkSpace (https://github.com/palestar/darkspace.git)
* /Medusa (https://github.com/palestar/medusa.git)
* /GameCQ (https://github.com/palestar/gamecq.git)

# Running the Client:
* Open the solution, DarkSpace/DarkSpace.sln, compile the solution.
* Set the working directory for debugging to /DarkSpace (the same directory containing the Data/) directory.
* Add "-login [user] [pass]" to the command line options and provide your username/password from the gamecq server.
* Add "-connect [host] [port]" to specify the server to connect into. 

# Running a Server:
* Open the solution, DarkSpace/Server.sln, compile the solution if needed.
* Set the working directory to /DarkSpace
* Set the DarkSpaceServer project as the start project, the command line options will specify the server config file to load.


