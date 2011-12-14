var $j = jQuery.noConflict();
var $u = uce.createClient("http://localhost:5280");

var login_submit = function() {
  $u.auth("toto", "123456", function(err, result) {
      $j("#login_box").fadeOut(1000);
      var root_meeting = $u.meeting("");
      var meeting = $u.meeting("test-meeting");
      meeting.join();
      root_meeting.on(function(ev){ ev_log_handler('root', ev); });
      meeting.on(function(ev){ ev_log_handler('test-meeting', ev); });
      root_meeting.startLoop();
      meeting.startLoop();
  });

  return false;
};

  $j('#event_log').prepend("<p> Logged in ...</p>");

  var ev_log_handler = function(meeting, event) {
      $j('#event_log').prepend('<li><span class="log_meeting">'
        + meeting + ':</span> <span class="log_json">'
        + JSON.stringify(event) + "</span></li>");
  };

$j(document).ready(function()
{
});
