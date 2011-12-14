
window.$u = uce.createClient('http://localhost:5280');

ev_log_handler = (meeting, event) ->
        str = '<li><span class="log_meeting">'
        str += meeting + ':</span> <span class="log_json">'
        str += JSON.stringify(event) + "</span></li>"
        $('#event_log').prepend str

window.meeting_init = (name, handler, should_join) ->
        tmp = $u.meeting name
        tmp.join() if should_join
        tmp.on (ev) =>
                handler(name, ev)
        tmp.startLoop()

window.login_submit = () ->
        $u.auth 'toto', '123456', (err, res) =>
                $('#event_log').prepend "<p> Logged in ...</p>"
                $('#login_box').fadeOut 1000
                meeting_init '', ev_log_handler, false
                meeting_init 'test-meeting', ev_log_handler, true
        return false

