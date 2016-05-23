-- Script by Backslash *Jack* (@74393)
-- Thanks to Shigernafy (@29)
-- Advanced monster spawn script
-- Last update 25 July 2006

enableAlerts(0);

if isServer() then

checkvar = "1"; -- allows the script to loop if faction 0 is found

for i=1, checkvar do

-- //////////////////////////////////////// DECLARE VARIABLES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


target = getName( getShip( randomNumber( 1, shipCount() ))); -- gets spawn point (player)

ugtofactionid = "1"; -- selects ugto faction ID for faction check

iccfactionid = "2"; -- selects icc faction ID for faction check

kluthfactionid = "3"; -- selects kluth faction ID for faction check

num = "5"; -- number of primary monsters to spawn

num2 = "1"; -- number of secondary monsters to spawn

secondmonster = "1"; -- Choose to spawn a second monster (0 = NO, 1 = YES)

invadename = "Mir"; -- Selects what name to broadcast to the faction (ie, 'Mir' detected near...)



-- //////////////////////////////////////// FIRST MONSTER \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

monsterspawn = "ships\\mi\\cruiser\\legion\\sc_legion.prt"; -- primary monster to spawn (ie Mir Cruiser)

monstername = "Mir Cruiser"; -- name of monster to spawn (ie Mir Cruiser)



-- //////////////////////////////////////// SECOND MONSTER \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

monsterspawn2 = "ships\\mi\\dreadnought\\longhead\\sc_dread.prt"; -- secondary monster to spawn (ie Mir Dread)

monstername2 = "Mir Dread"; -- name of monster to spawn (ie Mir Dread)



-- //////////////////////////////////////// END OF DECLARE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


if isFaction( target, 0 ) then

if checkvar == "1" then
checkvar = checkvar + 1;
end

end


-- //////////////////////////////////////// CHECK FOR UGTO \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


if isFaction( target, ugtofactionid ) then

checkvar = "0";

sendChat( target, 3, "<color;ffcc00> UGTO High Command: '" ..invadename.. " detected near " ..target.. ".  Destroy infestation immediately!'" );


for i=1, num do

spawnShip( monsterspawn, target, monstername, -1 );

end -- End's for i=1



if secondmonster == "1" then

for i=1, num2 do

spawnShip( monsterspawn2, target, monstername2, -1 );

end -- End's for i=1

end -- End's if for second spawn



end -- End's if isfaction UGTO

-- //////////////////////////////////////// END CHECK FOR UGTO \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\



-- //////////////////////////////////////// CHECK FOR ICC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


if isFaction( target, iccfactionid ) then

checkvar = "0";

sendChat( target, 3, "<color;ffcc00> ICC Central Command: '" ..invadename.. " detected near " ..target.. ".  Destroy infestation immediately!'" );


for i=1, num do

spawnShip( monsterspawn, target, monstername, -1 );

end -- End's for i=1



if secondmonster == "1" then

for i=1, num2 do

spawnShip( monsterspawn2, target, monstername2, -1 );

end -- End's for i=1

end -- End's if for second spawn



end -- End's if isfaction ICC

-- //////////////////////////////////////// END CHECK FOR ICC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


-- //////////////////////////////////////// CHECK FOR KLUTH \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


if isFaction( target, kluthfactionid ) then

checkvar = "0";

sendChat( target, 3, "<color;ffcc00> K'luth Hive Mind: '" ..invadename.. " detected near " ..target.. ".  Destroy infestation immediately!'" );


for i=1, num do

spawnShip( monsterspawn, target, monstername, -1 );

end -- End's for i=1



if secondmonster == "1" then

for i=1, num2 do

spawnShip( monsterspawn2, target, monstername2, -1 );

end -- End's for i=1

end -- End's if for second spawn



end -- End's if isfaction Kluth

-- //////////////////////////////////////// END CHECK FOR KLUTH \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

end -- End's loop


end -- End's if isServer