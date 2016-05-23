//check for current browser version
function Is() {
	var agent = navigator.userAgent.toLowerCase();
	this.major = parseInt(navigator.appVersion);
	this.minor = parseFloat(navigator.appVersion);
	this.ns  = ((agent.indexOf('mozilla')!=-1) && ((agent.indexOf('spoofer')==-1) && (agent.indexOf('compatible') == -1)));
	this.ns2 = (this.ns && (this.major == 2));
	this.ns3 = (this.ns && (this.major == 3));
	this.ns4 = (this.ns && (this.major == 4));
	this.ns6 = (this.ns && (this.major >= 5));
	this.ie   = (agent.indexOf("msie") != -1);
	this.ie3  = (this.ie && (this.major == 2));
	this.ie4  = (this.ie && (this.major >= 4));
	this.op3 = (agent.indexOf("opera") != -1);
}

var is = new Is()

//prepare variables
if(is.ns4 || is.ns6) {
	doc = "document";
	sty = "";
	htm = ".document";
} else if(is.ie4) {
	doc = "document.all";
	sty = ".style";
	htm = "";
}


//change/swap image
function ImgChange(ImgNr,ImgElement) {
	window.document.images[ImgNr].src = ImgElement.src;
}

// open new window
function newWindow(mypage,myname,w,h,features) 
{
  var settings = 'height=' + h + ',';
  settings += 'width=' + w + ',';
  settings += features;

  win = window.open(mypage,myname,settings);
  if ( win )
	  win.window.focus();
}

function openURL(sURL) 
{
    opener.document.location = sURL;
}

function openLoginWindow()
{
	var loginURL = "/login.htm";
	newWindow( loginURL,"loginWindow",450,250,"resizable=yes");
}

function openNewLoginWindow()
{
	var newloginURL = "/newlogin.htm";
	newWindow( newloginURL,"newLoginWindow", 600, 550, "resizeable=yes,scrollbars=yes" );
}

function openForgotPW()
{
	var forgotURL = "/forgotpw.htm";
	newWindow( forgotURL, "forgotPW", 450,250,"resizable=yes");
}

function openVoteWindow(cand_id, flag)
{
    var voteURL = "/vsystem/vote.php?cand_id=";
    voteURL += cand_id;
    voteURL += "&flag=";
    voteURL += flag;
    
    if (flag == 0) {
    newWindow( voteURL, "Vote", 450,300, "resizable=yes");
    }
    
    if (flag == 1) {
    newWindow( voteURL, "Change", 450,300, "resizable=yes");
    }    
    
}

function openSignupWindow( sessionId )
{
	var signupURL = "/mod_signup.htm";
	if ( sessionId > 0 ) {
		signupURL += "?sid=";
		signupURL += sessionId;
	}
	newWindow ( signupURL , "Signup", 400, 450, "resizable=yes");

}


function openSubscribeWindow( sessionId )
{
	var subscribeURL = "/subscribe_now.htm";
	if ( sessionId > 0 )
	{
		subscribeURL += "?sid=";
		subscribeURL += sessionId;
	}
	
	newWindow( subscribeURL,"subscribe",600,600,"status=yes,resizable=yes,scrollbars=yes");
}

function openCouponWindow( sessionId )
{
	var couponURL = "/coupon.htm";
	if ( sessionId )
	{
		couponURL += "?sid=";
		couponURL += sessionId;
	}
	newWindow( couponURL, "coupon", 500,150,"");
}

function editProfile( sessionId )
{
	var profileURL = "/profile_edit.htm";
	profileURL += "?sid=";
	profileURL += sessionId;
	
	newWindow( profileURL, "profile", 700,450, "resizable=yes,scrollbars=yes" );
}

function popUp(src,extra)
{
	var url = "/shots/thumbnail.php?image="+src+"&width=" +screen.width + "&height="+ screen.height + "&extra=" + escape(extra);
	
	newWindow( url, "ScreenShot", 50, 50, "top=40,left=40" );
	//open(url,"","width=800,height=600");
}