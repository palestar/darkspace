enableAlerts( 1 )

local aKey = getKey("[PS]Faustus");
scriptAlert( aKey );
scriptAlert( isNoun( aKey ) );
scriptAlert( isBody( aKey ) );
scriptAlert( isAsteroid( aKey ) );
scriptAlert( isStar( aKey ) );
scriptAlert( isNebula( aKey ) );

local t = {name="lua", version="4.0"};
x = gsub("$name - $version", "%$(%w+)", function (v) return %t[v] end);
--> x="lua - 4.0"
scriptAlert(x or "error");
