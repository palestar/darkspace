enableAlerts(0);  --don't change this
------------------------------------
----- message.lua v1.0 ----
-------------------------------

-- Start the script and leave it running. It will send server wide messages from the included list at random intervals.

if isServer() then 

------------------------------------
-- Main Script Variables ----
------------------------------------

local scripter = "[Myth]Drafell"; -- Insert scripter ID here, not strictly needed but included as a failsafe for default script options.
local message_target = scripter;

-- Set color keys, format BBGGRR.
local lightred = "<color;6666ff>";
local salmon = "<color;9999ff>";
local red = "<color;3333ff>";
local teal = "<color;ffff66>";
local blue = "<color;ff0000>";
local white = "<color;ffffff>";
local lilac = "<color;ffccff>"; 
local green = "<color;00ff00>";
local lightgreen = "<color;99ff66>";
local black = "<color;000000>"; --why on earth would you use this?
local ec = "</color>";

function newTimer()
	startTimer(randomNumber(60, 4800), "checkPlayers()");
end

function checkPlayers()
	local player_check = getName(getShip(randomNumber(0, shipCount()-1)));
	if shipCount()==0 then	-- Check to see if there are any ships, restart timer if none found
		startTimer(300, "checkPlayers()");
	elseif isAIControlled(player_check)==1 and shipCount()==1 then 	-- Check to see if selected ship is AI controlled
		startTimer(300, "checkPlayers()");
	elseif isAIControlled(player_check)==1 and shipCount() > 1 then 	-- Check to see if selected ship is AI controlled
		startTimer(10, "checkPlayers()");
	else
		msgTarget(randomNumber(1, 25)); -- The total number ofmessages currently included, If you include more be sure to change the second value here.
	end
end

function msgTarget(msg_no)
	message_target = getName(getShip(randomNumber(0, shipCount()-1)));
	local spawn_target = getName(getRandomPlanet());
	if isAIControlled(message_target) then -- Finds new message target is ship is AI controlled
		msgTarget(msg_no);
	elseif msg_no==1 then -- Unique message ID,. Increment by one for each new message, and remember to change the value in the checkPlayers() function when adding more.
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."There has been cheese shortage reported on "..ec..spawn_target..lightgreen..". 'Cartel Verde de Queso' is suspected to be involved."..ec);
		newTimer();
	elseif msg_no==2 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."DarkSpace: The Phantom Gerbil - coming soon to a holo-dome near you."..ec);
		newTimer();
	elseif msg_no==3 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."You now get a FREE and exclusive 'Feral Hug Me' t-shirt whenever you purchase a mega-cookie from the Bifrost Cafe. Offer only available while stocks last."..ec);
		newTimer();
	elseif msg_no==4 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."*whipcrack* Mod's getting you down? Now you can get proven fast and effective Mod relief with the new ModWhip 3000! Guaranteed to make your Mod woes disappear in the time it takes to type /ban n00b!"..ec);
		newTimer();
	elseif msg_no==5 then
		sendChat(message_target, 1, "SERVER: If you suspect someone of spying, sabotage, greifing other players or otherwise exploiting the game system, you can report them to the game Staff using the "..teal.."'/report'"..ec.." function. Check the game forums for more information on this feature.");
		newTimer();
	elseif msg_no==6 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."The 'Mods Gone Wild 2275' Calendar is now available from a store near you!"..ec);
		newTimer();
	elseif msg_no==7 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."The "..spawn_target.." Planetarey Council has just passed a motion to increase the cheesecake rations for all registered citizens. The people have responded with a spntaneous celebration."..ec);
		newTimer();
	elseif msg_no==8 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."A notorious cheese bandit has been aprehended near "..spawn_target..". Representatives of 'Cartel Verde de Queso' deny any links with this individual."..ec);
		newTimer();
	elseif msg_no==9 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."A serial fluxxer on "..spawn_target.." has finally been captured. Court proceedings are expected to begin early next week."..ec);
		newTimer();
	elseif msg_no==10 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."MegaSoft has released a new patch for their widely used ShipNav OAP starship navigational interface software. Patch 0mgb33pz0rs features increased sensor latency and an overall reduction in helm response, which should help to enhance the unique experience of piloting any OAP based starship."..ec);
		newTimer();
	elseif msg_no==11 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."The elections for the new Planetary Governor of "..spawn_target.." are due to start in two days time. This follows the shocking death of the old governor when he was murdered by a clown in a public restroom. Detectives are still hunting for the suspect, so please be on the lookout for anyone who has a pale complexion, red nose, wears pink baggy pants and goes by the name of Bito."..ec);
		newTimer();
	elseif msg_no==12 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."Och, the power grids wil'nae tak' it, cap'n! Explodin' in 10 seconds..."..ec);
		newTimer();
	elseif msg_no==13 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."The famed exotic dancer, Sausage Fingers, is now on tour near "..spawn_target.." and will be performing at nearby planets for the next 7 days."..ec);
		newTimer();
	elseif msg_no==14 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."Giant mutant space-gerbils have been sighted near "..spawn_target..". Scarey..."..ec);
		newTimer();
	elseif msg_no==15 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."A giant sandstorm on "..spawn_target.." has caused "..spawn_target.." Idol 2275 to be cancelled. The Human Quadrant breaks out in universal celebration."..ec);
		newTimer();
	elseif msg_no==16 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."ICC cheese stocks have dipped havily this quarter, resulting in widespread panic throughout the quadrant. It is supsected that black market trade in the extremely narcotic and highly illegal Ocotatian Green Cheese may be having a serious negative impact on the quadrant-wide market place. Meanwhile the outlawed Cartel Verde de Queso has issued a press release stating 'This shows that our efforts to bring cheese to the massses, free of unfair and excessive government taxation shall not falter. This is a great acheivement for us and the entire cheese loving community. You can tak' oor lives, but ye cannae tak' our CHEESEDOMMMMM!!!!!!'"..ec);
		newTimer();
	elseif msg_no==17 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."Nine out of ten dentists agree. K’Luth cloaking technology hides tooth stains the best."..ec);
		newTimer();
	elseif msg_no==18 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."Unidentified hostiles have been detected near...\nNo... wait...\nIt’s just a coffee can. Stop littering in the jump lanes!"..ec);
		newTimer();
	elseif msg_no==19 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."Pirates have invaded your ship! We don’t know their intensions yet, but they brought a flag…"..ec);
		newTimer();
	elseif msg_no==20 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."NEWS ALERT: Wormhole Remnant has moved to an ultra-secret location! You will never find it now!"..ec);
		newTimer();
	elseif msg_no==21 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."NEWS ALERT: The Cookie Sensei knows that a cookie never crumbles the same way twice."..ec);
		newTimer();
	elseif msg_no==22 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."NEWS ALERT: How many UGTO does it take to change a lightbulb?\nWait..\nIt's 2275 and NO-ONE has invented a replacement for the lightbulb yet? You have got to be kidding!!!"..ec);
		newTimer();
	elseif msg_no==23 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."NEWS ALERT: The venerable Nactho Nacho Hamasamwitcha will be touring and making speeches on the Quadrant Cheese Crisis near "..spawn_target..". We advise you stay away from this location as mob-narcolepsy is rumored to break out during his lectures, although no-one has so far been able to confirm this as they all fell asleep."..ec);
		newTimer();
	elseif msg_no==24 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."NEWS ALERT: EEK!!!\nIt's a mirver, mirver, mirver, mirver, mirver, mirver, mirver, mirver, mirver, mirver, mirver, mirver, pulse-wave, pulse-WAVE!! A mirver, mivrer, mirver, mirver...*static*\n*sounds of gerbils being electructed*\n*tzzztttzzzzzaaaAAAPPPP!*\n OK... which fool let idiot near the damned microphone???"..ec);
		newTimer();
	elseif msg_no==25 then
		sendChat(message_target, 1, "NEWS ALERT: "..lightgreen.."NEWS ALERT: Who let the gaifen out? Who? Who? Who?"..ec);
		newTimer();
	end
end

newTimer();

end
