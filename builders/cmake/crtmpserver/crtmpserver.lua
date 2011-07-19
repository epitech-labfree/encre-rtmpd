-- Start of the configuration. This is the only node in the config file.
-- The rest of them are sub-nodes
configuration=
{
	-- if true, the server will run as a daemon.
	-- NOTE: all console appenders will be ignored if this is a daemon
	daemon=false,
	-- the OS's path separator. Used in composing paths
	pathSeparator="/",

	-- this is the place where all the logging facilities are setted up
	-- you can add/remove any number of locations
	logAppenders=
	{
		{
			-- name of the appender. Not too important, but is mandatory
			name="console appender",
			-- type of the appender. We can have the following values:
			-- console, coloredConsole and file
			-- NOTE: console appenders will be ignored if we run the server
			-- as a daemon
			type="coloredConsole",
			-- the level of logging. 6 is the FINEST message, 0 is FATAL message.
			-- The appender will "catch" all the messages below or equal to this level
			-- bigger the level, more messages are recorded
			level=6
		},
		{
			name="file appender",
			type="file",
			level=6,
			-- the file where the log messages are going to land
			fileName="/tmp/crtmpserver.log"
		}
	},

	-- this node holds all the RTMP applications
	applications=
	{
		-- this is the root directory of all applications
		-- usually this is relative to the binary execuable
		rootDirectory="applications",


		--this is where the applications array starts
		{
			-- The name of the application. It is mandatory and must be unique
			name="appselector",
			-- Short description of the application. Optional
			description="Application for selecting the rest of the applications",

			-- The type of the application. Possible values are:
			-- dynamiclinklibrary - the application is a shared library
			protocol="dynamiclinklibrary",
			-- the complete path to the library. This is optional. If not provided,
			-- the server will try to load the library from here
			-- <rootDirectory>/<name>/lib<name>.{so|dll|dylib}
			-- library="/some/path/to/some/shared/library.so"

			-- Tells the server to validate the clien's handshake before going further.
			-- It is optional, defaulted to true
			validateHandshake=true,
			-- this is the folder from where the current application gets it's content.
			-- It is optional. If not specified, it will be defaulted to:
			-- <rootDirectory>/<name>/mediaFolder
			-- mediaFolder="/some/directory/where/media/files/are/stored"
			-- the application will also be known by that names. It is optional
			--aliases=
			--{
			--	"simpleLive",
			--	"vod",
			--	"live",
			--},
			-- This flag designates the default application. The default application
			-- is responsable of analyzing the "connect" request and distribute
			-- the future connection to the correct application.
			default=true,
			acceptors =
			{
				{
					ip="0.0.0.0",
					port=1935,
					protocol="inboundRtmp"
				},
				{
					ip="0.0.0.0",
					port=8081,
					protocol="inboundRtmps",
					sslKey="server.key",
					sslCert="server.crt"
				},
				{
					ip="0.0.0.0",
					port=8080,
					protocol="inboundRtmpt"
                },
			}
		},
		{
			description="FLV Playback Sample",
			name="flvplayback",
			protocol="dynamiclinklibrary",
			aliases=
			{
				"simpleLive",
				"vod",
				"live",
				"WeeklyQuest",
				"SOSample",
				"oflaDemo",
			},
			acceptors =
			{
				{
					ip="0.0.0.0",
					port=1935,
					protocol="inboundRtmfp"
				},
				{
					ip="0.0.0.0",
					port=6666,
					protocol="inboundLiveFlv",
					waitForMetadata=true,
				},
				{
					ip="0.0.0.0",
					port=9999,
					protocol="inboundTcpTs"
				},
				{
					ip="0.0.0.0",
					port=4242,
					protocol="inboundEncreTS",
				},
				--[[{
					ip="0.0.0.0",
					port=7654,
					protocol="inboundRawHttpStream",
					crossDomainFile="/tmp/crossdomain.xml"
				},
				{
					ip="0.0.0.0",
					port=554,
					protocol="inboundRtsp"
				},]]--
			},
			externalStreams =
			{
				--[[{
					uri="mms://channels.webradio.antenne.de/chillout",
					localStreamName="myMms",
					enableAAC=false,
					enableMP3=true
				},
				{
					uri="rtsp://a1956.l1857055475.c18570.g.lq.akamaistream.net/D/1956/18570/v0001/reflector:55475",
					localStreamName="test_aaa",
				},
				{
					uri="rtsp://0.0.0.0:2323/test.sdp",
					localStreamName="stream-1-udp"
				},
				{
					uri="rtsp://stream01.qt.slamtv.true.nl/slamtv.sdp",
					localStreamName="stream0-udp"
				},
				{
                    uri="rtsp://stream01.qt.slamtv.true.nl/slamtv.sdp",
                    localStreamName="stream0-tcp",
					forceTcp=true
                },
				{
					uri="rtmp://10.0.1.15/vod/mp4:test_mp4",
					localStreamName="stream1"
				},
				{
                    uri="rtmp://10.0.1.15/vod/test_flv",
                    localStreamName="stream2"
                },
				{
					uri="rtsp://media-us-2.soundreach.net/slcn_lifestyle.sdp",
					localStreamName="stream3",
					forceTcp=true
				},
				{
					uri="rtsp://82.177.67.61/axis-media/media.amp",
					localStreamName="stream4",
					forceTcp=false
				},
				{
					uri="http://v13.nonxt6.c.youtube.com/videoplayback?ip=0.0.0.0&sparams=id%2Cexpire%2Cip%2Cipbits%2Citag%2Calgorithm%2Cburst%2Cfactor%2Coc%3AU0dXSlFSVl9FSkNNN19JTFpF&algorithm=throttle-factor&itag=34&ipbits=0&burst=40&sver=3&expire=1285178400&key=yt1&signature=548764ECB414E3CA08BDD414F4B633853F9051AC.D8FFF913C9B3BF1C339BD1A60A77C5D1790775EE&factor=1.25&id=bcdad173a3ba8daa&redirect_counter=1",
					localStreamName="stream5",
				}
				{
					uri="rtmp://edge01.fms.dutchview.nl/botr/bunny",
					localStreamName="stream6",
					emulateUserAgent="MAC 10,1,82,76",
				}
				{
 					uri="rtmp://edge01.fms.dutchview.nl/botr/bunny",
					localStreamName="stream6",
					swfUrl="http://www.example.com/example.swf";
					pageUrl="http://www.example.com/";
					emulateUserAgent="MAC 10,1,82,76",
 				}]]--
			},
			validateHandshake=true,
			keyframeSeek=true,
			seekGranularity=1.5, --in seconds, between 0.1 and 600
			clientSideBuffer=12, --in seconds, between 5 and 30
			--generateMetaFiles=true, --this will generate seek/meta files on application startup
			--renameBadFiles=false,
			--[[authentication=
			{
				type="adobe",
				encoderAgents=
				{
					"FMLE/3.0 (compatible; FMSc/1.0)",
					"my 3rd party encoder",
					"some other encoder",
				},
				usersFile="users.lua",
			}]]--
		},
		{
			name="samplefactory",
			description="asdsadasdsa",
			protocol="dynamiclinklibrary",
			aliases=
			{
				"httpOutboundTest"
			},
			acceptors =
			{
				{
					ip="0.0.0.0",
					port=8989,
					protocol="httpEchoProtocol"
				},
				{
					ip="0.0.0.0",
					port=8988,
					protocol="echoProtocol"
				}
			}
			--validateHandshake=true,
			--default=true,
		},
		{
			-- The name of the application. It is mandatory and must be unique 
			name="encre2",
			-- Short description of the application. Optional
			description="Application for Encre2",
			
			-- The type of the application. Possible values are:
			-- dynamiclinklibrary - the application is a shared library
			protocol="dynamiclinklibrary",
			-- the complete path to the library. This is optional. If not provided, 
			-- the server will try to load the library from here
			-- <rootDirectory>/<name>/lib<name>.{so|dll|dylib}
			-- library="/some/path/to/some/shared/library.so"
			
			-- Tells the server to validate the clien's handshake before going further. 
			-- It is optional, defaulted to true
			validateHandshake=true,
			-- this is the folder from where the current application gets it's content.
			-- It is optional. If not specified, it will be defaulted to:
			-- <rootDirectory>/<name>/mediaFolder
			-- mediaFolder="/some/directory/where/media/files/are/stored"
			-- the application will also be known by that names. It is optional
			--aliases=
			--{
			--	"simpleLive",
			--	"vod",
			--	"live",
			--},
			-- This flag designates the default application. The default application
			-- is responsable of analyzing the "connect" request and distribute 
			-- the future connection to the correct application.
			default=true,
			acceptors = 
			{
				{
					ip="0.0.0.0",
					port=4243,
					protocol="inboundEncreTS",
				},
			}
		},
		{
			name="proxypublish",
			description="Application for forwarding streams to another RTMP server",
			protocol="dynamiclinklibrary",
			acceptors =
			{
				{	
					ip="0.0.0.0",
					port=6665,
					protocol="inboundLiveFlv"
				},
			},
			abortOnConnectError=true,
			targetServers = 
			{
				--[[{
					targetUri="rtmp://x.xxxxxxx.fme.ustream.tv/ustreamVideo/xxxxxxx",
					targetStreamName="xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
					localStreamName="gigi",
					emulateUserAgent="FMLE/3.0 (compatible; FMSc/1.0 http://www.rtmpd.com)"
				}]]--,
				{
					targetUri="rtmp://localhost/vod",
					targetStreamType="live", -- (live, record or append)
					emulateUserAgent="My user agent",
					localStreamName="stream1"
				},
			},
			--[[externalStreams = 
			{
				{
					uri="rtsp://82.177.67.61/axis-media/media.amp",
					localStreamName="stream4",
					forceTcp=false
                },
				{
					uri="rtmp://edge01.fms.dutchview.nl/botr/bunny",
					localStreamName="stream1"
                },
			},]]--
			--validateHandshake=true,
			--default=true,
		},
		{
			name="stresstest",
			description="Application for stressing a streaming server",
			protocol="dynamiclinklibrary",
			targetServer="localhost",
			targetApp="vod",
			active=false,
			--[[streams = 
			{
				"lg00","lg01","lg02","lg03","lg04","lg05","lg06","lg07","lg08",
				"lg09","lg10","lg11","lg12","lg13","lg14","lg15","lg16","lg17",
				"lg18","lg19","lg20","lg21","lg22","lg23","lg24","lg25","lg26",
				"lg27","lg28","lg29","lg30","lg31","lg32","lg33","lg34","lg35",
				"lg36","lg37","lg38","lg39","lg40","lg41","lg42","lg43","lg44",
				"lg45","lg46","lg47","lg48","lg49"
			},]]--
			streams = 
			{
				"mp4:lg.mp4"
			},
			numberOfConnections=10,
			randomAccessStreams=false
		},
		{
			name="applestreamingclient",
			description="Apple Streaming Client",
			protocol="dynamiclinklibrary",
			--[[acceptors = 
			{
				{
					ip="0.0.0.0",
					port=5544,
					protocol="inboundRtsp"
				}
			},]]--
			aliases=
			{
				"asc",
			},
			--validateHandshake=true,
			--default=true,
		},
		--[[{
			name="vmapp",
			description="An application demonstrating the use of virtual machines",
			protocol="dynamiclinklibrary",
			vmType="lua",
			script="flvplayback.lua",
			aliases=
			{
				"flvplayback1",
				"vod1"
			},
			acceptors=
			{
				{
					ip="0.0.0.0",
					port=6544,
					protocol="inboundTcpTs"
				}
			}
		},]]--
		--#INSERTION_MARKER# DO NOT REMOVE THIS. USED BY appscaffold SCRIPT.
	}
}

