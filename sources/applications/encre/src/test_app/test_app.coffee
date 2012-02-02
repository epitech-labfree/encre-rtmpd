
$ ->
        window.$u = uce.createClient('http://localhost:5280')
        $(".column").sortable({connectWith: ".column"})
        $(".portlet").addClass("ui-widget ui-widget-content ui-helper-clearfix ui-corner-all")
                .find( ".portlet-header" )
                        .addClass( "ui-widget-header ui-corner-all" )
                        .prepend( "<span class='ui-icon ui-icon-minusthick'></span>")
                        .end()
                .find( ".portlet-content" )
        $( ".column" ).disableSelection()


ev_log_handler = (meeting, event) ->
        str = '<li><span class="log_meeting">'
        str += meeting + ':</span> <span class="log_json">'
        str += JSON.stringify(event) + "</span></li>"
        $('#event_log').prepend str

ev_stream_started_handler = (event) ->
        uid = event.metadata.user_uid
        token = event.metadata.token
        name = event.metadata.name
        ne = $('<div id="#video_receive_'+name+'">').appendTo('#video_column_right').addClass('portlet')
        ne.encre_video({uid: $uid, token: $token, meeting: $um, stream: event.metadata.name})

ev_video_handler = (event) ->
        uid = event.to
        window.$uid = uid
        token = event.metadata.token
        window.$token = token
        ne = $('<div id=#video_publish>').appendTo('#video_column_left').addClass('portlet')
        ne.encre_video({uid: uid, token: token, publish: true, meeting: $um, stream: uid})

ev_stream_stopped_handler = (event) ->
        #alert JSON.stringify(event)
        uid = event.metadata.user_uid
        name = event.metadata.name
        cid = '#video_receive_'+name
        alert cid
        $(cid).remove()


window.meeting_init = (name, handler, should_join) ->
        tmp = $u.meeting name
        tmp.join() if should_join
        tmp.on (ev) => handler(name, ev)
        $u.time.get (err, res) => tmp.startLoop(res)
        return tmp

window.login_submit = () ->
        login_time = new Date
        $u.auth $('input[name=uid]').val(), '123456', (err, res) =>
                window.$um = $u.meeting 'test-meeting'
                $um.join () =>
                        $um.on (ev) => ev_log_handler('test-meeting', ev)
                        $um.on 'ev_token', (ev) => ev_video_handler(ev)
                        $um.on 'ev_stream_started', (ev) => ev_stream_started_handler(ev)
                        $um.on 'ev_stream_stopped', (ev) => ev_stream_stopped_handler(ev)
                        $u.time.get (err, res) =>
                                startloop_time = new Date
                                diff = startloop_time.getTime() - login_time.getTime()
                                $um.startLoop(res-diff)
                $('#event_log').prepend "<p> Logged in ...</p>"
                $('#login_box').fadeOut 500
                $('#user_box').delay(500).fadeIn 500
                #root_meeting = meeting_init '', ev_log_handler, false
                #window.$um = meeting_init 'test-meeting', ev_log_handler, true
                $('#user_box').userlist()

        return false


UCEUserList =
        _roster_add: (ev) ->
                @updateRoster()
        _roster_del: (ev) ->
                @updateRoster()
        _roster_list: (err, roster) ->
                @list.empty()
                @list.append "<li>" + guy.name + "</li>" for guy in roster
        updateRoster: ->
                @meeting.getRoster (err, roster) =>
                        @_roster_list(err, roster)
        _create: ->
                @meeting = $um
                @meeting.on 'internal.roster.add', (ev) =>
                        @_roster_add(ev)
                @meeting.on 'internal.roster.delete', (ev) =>
                        @_roster_del(ev)
                #$u.time.get (err, res) => @meeting.startLoop(res)
                @updateRoster()
                @element.append "<p class=userlist_title>Roster of <span class='userlist_meeting'>" + @meeting.name + "</span></p>"
                @list = $( "<ul>" ).appendTo @element
        _init: ->
                @list.clear

$.widget "ui.userlist", UCEUserList

