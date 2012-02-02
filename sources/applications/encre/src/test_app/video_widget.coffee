
EncreVideo =
        _create: ->
                @meeting = @options.meeting
                @element.append "Flash widget for "+@options.stream+" (publish="+@options.publish+")"
                #@flash = $('<embed>').appendTo(@element)
                @flash = $('<embed>').attr(@_getFlashAttr()).appendTo(@element);
        _init: ->
                return
        _getFlashSrc: ->
                src = @options.flash_url + '/'
                src += @options.flash_publish if @options.publish
                src += @options.flash_reveice if !@options.publish
                src += ".swf"
        _getFlashVar: ->
                params =
                        wmode      : 'opaque'
                        stream     : @options.stream
                        server     : @options.url + '' + @options.meeting.name
                        streamtype : if @options.publish and @options.record then "record" else "live"
                        uid        : @options.uid
                        sid        : @options.token
                        width      : @options.width - 40
                        height     : @options.height - 20
                $.param params
        _getFlashAttr: ->
                src       : @_getFlashSrc(),
                flashvars : @_getFlashVar(),
                quality   : 75,
                width     : @options.width,
                height    : @options.height,
                type      : 'application/x-shockwave-flash'
        options:
                uid: null
                token: null
                publish: false
                record: false
                stream: null
                url: "rtmp://localhost/encrev1/"
                meeting: window.$um
                flash_url: "flash"
                flash_publish: "publish_video"
                flash_reveice: "receive_video"
                width: 320
                height: 240

$.widget "ui.encre_video", EncreVideo

