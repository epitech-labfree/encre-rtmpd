/**
 * U.C.Engine library
 * http://ucengine.org/
 * (c) 2011 af83
 */

(function(g) {
    var VERSION = "0.5";

    function UCEngine(baseUrl) {

        function getCollection(url, params, callback) {
            get(url, params, function(err, result, xhr) {
                if (!err) {
                    callback(err, result.result, xhr);
                } else {
                    callback(err, result, xhr);
                }
            });
        }

        function uce_api_call(method, url, data, callback) {
            var call_back = callback || jQuery.noop;
            url = baseUrl +'/api/' + VERSION + url;
            return jQuery.ajax({
                type     : method,
                dataType : "json",
                url      : url,
                data     : data,
                complete : function(xhr, textStatus) {
                    var response = jQuery.parseJSON(xhr.responseText);
                    if (xhr.status >= 400) {
                        call_back(xhr.status, response, xhr);
                    } else {
                        call_back(null, response, xhr);
                    }
                }
            });
        }

        function get(url, data, callback) {
            return uce_api_call("get", url, data, callback);
        }

        function post(url, data, callback) {
            return uce_api_call("post", url, data, callback);
        }

        function put(url, data, callback) {
            data = jQuery.extend({"_method": "put"}, data);
            return uce_api_call("post", url, data, callback);
        }

        function del(url, data, callback) {
            data = jQuery.extend({"_method": "delete"}, data);
            return uce_api_call("post", url, data, callback);
        }

        function UCEMeeting(client, meetingname, presence) {
            this.handlers = [];
            this.client = client;
            this.name = meetingname;
            this.uid = (presence || {}).user;
            this.sid = (presence || {}).id;
            this.params = {
                merge: function() {
                    var args = Array.prototype.slice.call(arguments);
                    args.unshift({}, {'uid': presence.user,
                                      'sid': presence.id});
                    return jQuery.extend.apply(jQuery, args);
                }
            };
        }

        UCEMeeting.prototype = {
            get: function(callback) {
                get("/meeting/all/" + this.name, this.params.merge(),
                    function(err, result, xhr) {
                        if (!err) {
                            callback(err, result.result, xhr);
                        } else {
                            callback(err, result, xhr);
                        }
                    });
                return this;
            },
            update: function(start, end, metadata, callback) {
                var params = this.params.merge({'metadata': metadata});
                if (start) {
                    params.start = start;
                }
                if (end && end != "never") {
                    params.end = end;
                }
                put("/meeting/all/" + this.name, params,
                    function(err, result, xhr) {
                        if (!callback) {
                            return;
                        }
                        callback(err, result, xhr);
                    });
            },
            join: function(callback) {
                post("/meeting/all/" + this.name + "/roster/",
                     this.params.merge(), callback);
                return this;
            },
            leave: function(callback) {
                del("/meeting/all/" + this.name + "/roster/" + this.uid,
                    this.params.merge(),
                    callback);
                return this;
            },
            getRoster: function(callback) {
                get("/meeting/all/" + this.name + "/roster",
                    this.params.merge(),
                    function (err, result, xhr) {
                        if (!callback) {
                            return;
                        }
                        var roster = result.result;
                        callback(err, roster, xhr);
                    });
            },

            /**
             * Push event
             */
            _push: function(params, callback) {
                post("/event/" + this.name,
                     this.params.merge(params),
                     callback);
                return this;
            },
            push: function(type, metadata, callback) {
                this._push({'type': type,
                            'metadata': metadata},
                           callback);
                return this;
            },
            pushTo: function(to, type, metadata, callback) {
                this._push({'type': type,
                            'to': to,
                            'metadata': metadata},
                           callback);
                return this;
            },

            /**
             * Get file upload url for this meeting
             */
            getFileUploadUrl: function() {
                return baseUrl +"/api/"+ VERSION +"/file/"+this.name+"?uid="+this.uid+"&sid="+ this.sid;
            },

            /**
             * Get file download url
             * @param String filename
             */
            getFileDownloadUrl: function(filename) {
                return baseUrl +"/api/"+ VERSION +"/file/"+this.name+"/"+ filename +"?uid="+this.uid+"&sid="+this.sid;
            },

            /**
             * List files
             */
            listFiles: function(callback) {
                get('/file/'+ this.name, this.params.merge(), function(err, result, xhr) {
                    callback(err, result.result, xhr);
                });
            },

            /**
             * @param String id
             * @param Function callback
             */
            delFile: function(id, callback) {
                del("/file/" + this.name + "/" + id,
                    this.params.merge(),
                    function (err, result, xhr) {
                        if (!callback) {
                            return;
                        }
                        callback (err, result, xhr);
                    });
            },

            /**
             * @param Object params
             *    search
             *    start you can use uce.time() (mandatory)
             *    type
             *    from
             * @param Function callback
             * @param Boolean one_shot
             * @return Object with a stop() method
             */
            waitEvents: function(params, callback, one_shot) {
                var that = this;
                function startLongPolling(p, callback) {
                    var getParams = that.params.merge({'_async': 'lp'}, p);
                    return get("/event/" + that.name,
                               getParams,
                               callback);
                }
                var longPolling = {
                    aborted : false,
                    _start : function(p, callback) {
                        var that = this;
                        this.xhr = startLongPolling(p, function(err, result, xhr) {
                            try {
                                var events = result.result;
                                jQuery.each(events, function(index, event) {
                                    try {
                                        callback(err, event, xhr);
                                    } catch (e) {
                                        // naive but it's better than nothing
                                        if (window.console) console.error(e);
                                    }
                                });
                                if (events.length > 0) {
                                    p.start = parseInt(events[events.length - 1].datetime, 10) + 1;
                                }
                            } catch (e) {
                                // do nothing
                            }
                            if (that.aborted === false && one_shot !== true) {
                                that._start(p, callback);
                            }
                        });
                    },
                    stop: function() {
                        this.aborted = true;
                        this.xhr.abort();
                    }
                };
                longPolling._start(params, callback);
                return longPolling;
            },

            /**
             * @param Object params
             *    search
             *    start
             *    end
             *    type
             *    from
             *    count
             *    page
             *    order
             * @param Function callback
             * @param Boolen onEachEvent
             */
            getEvents: function(params, callback, onEachEvent) {
                var that = this;
                params = this.params.merge(params);
                get("/event/" + this.name,
                    params,
                    function(err, result, xhr) {
                        if (!callback) {
                            return;
                        }
                        var events = result.result;
                        if (!onEachEvent) {
                            callback(err, events, xhr);
                        } else {
                            jQuery.each(events, function(index, event) {
                                callback(err, event, xhr);
                            });
                        }
                    });
                return this;
            },

            /**
             * Trigger event on the internal queue
             * @param Object event
             *  - type
             */
            trigger: function(event) {
                jQuery.each(this.handlers, function(i, item) {
                    if (!item.type) {
                        item.callback(event);
                    } else {
                        if (item.type == event.type)
                            item.callback(event);
                    }
                });
            },

            /**
             * Start main loop event
             * [@param Integer start]
             */
            startLoop: function(start) {
                var that = this;
                return this.waitEvents({start: start || 0}, function(err, result, xhr) {
                    that.trigger(result);
                });
            },

            /**
             * Start replay loop event
             * @param Integer start offset
             * @param Array events
             */
            startReplay: function(start, events, index) {
                this._replay_current_time = start;
                if (!index) {
                    this._replay_events = events;
                    index = 0;
                }
                var next = null;
                while (next = events[index]) {
                    if (next && start > next.datetime) {
                        this.trigger(next);
                        index++;
                    } else {
                        break;
                    }
                }
                if (next) {
                    this._replay_next_index = index;
                    var that = this;
                    var offset = 100; // each 100 milisecond
                    this._replay_temporized = setTimeout(function() {
                        that.startReplay(start + offset, events, index);
                    }, offset);
                }
            },

            getCurrentReplay: function() {
                return this._replay_current_time;
            },

            /**
             * Jump to a specific datetime
             */
            jumpToReplay: function(datetime) {
                this.stopReplay();
                if (datetime > this._replay_current_time) {
                    this.startReplay(datetime, this._replay_events, this._replay_next_index);
                } else {
                    this.startReplay(datetime, this._replay_events);
                }
            },

            stopReplay: function() {
                clearTimeout(this._replay_temporized);
            },

            /**
             * Alias of on
             */
            bind: function() {
                var args = Array.prototype.slice.call(arguments);
                return this.on.apply(this, args);
            },

            /**
             * Bind event handler
             * use it with startLoop
             * [@param String type]
             * @param Function callback
             */
            on: function(type, callback) {
                if (!callback) {
                    callback  = type;
                    type = null;
                }
                this.handlers.push({type: type,
                                    callback: callback});
                return this;
            },
            /**
             * Search event in current meeting
             */
            search: function(terms, options, callback) {
                terms.location = this.name;
                return this.client.search(terms, options, callback);
            },
            /**
             * Can the user make the action in the current meeting ?
             */
            can: function(uid, action, object, conditions, callback) {
                return this.client.user.can(uid, action, object, conditions, this.name, callback);
            },
            /**
             *
             */
            canCurrentUser: function(action, object, conditions, callback) {
                return this.can(this.uid, action, object, conditions, callback);
            }
        };

        var _presence = null;
        return {
            connected : false,
            uid: null,
            name: null,
            /**
             * Create user presence
             */
            auth: function(uname, credential, metadata, callback) {
                var params = {name: uname};
                name = uname;
                if (credential) {
                    params.credential = credential;
                }
                if (!callback) {
                    callback = metadata;
                } else {
                    params.metadata = metadata;
                }
                var that = this;
                post("/presence/", params, function(err, result, xhr) {
                    if (err) {
                        callback(err, result, xhr);
                    } else {
                        var uid = result.result.uid;
                        var p = {"user": uid, "id": result.result.sid, "name": name};
                        that.attachPresence(p);
                        callback(err, p, xhr);
                    }
                });
                return this;
            },
            /**
             * Get user presence
             */
            presence: function(callback) {
                get("/presence/" + _presence.id, {'uid': _presence.user,
                                                  'sid': _presence.id},
                    callback);
                return this;
            },
            /**
             * Close user presence
             */
            close: function(callback) {
                del("/presence/" + _presence.id, {'uid': _presence.user,
                                                  'sid': _presence.id},
                    callback);
                this.uid = null;
                this.connected = false;
                _presence = null;
                return this;
            },
            getWaiter : function(calls_needed, callback) {
                if(calls_needed == 0)
                    callback();
                var ok = true;
                var waiter = function(){
                    --calls_needed;
                    if (calls_needed == 0 && ok)
                        callback();
                    // XXX: should we raise an error if waiter called too many times?
                };
                return waiter;
            },
            /**
             * Attach presence to a new uce object
             */
            attachPresence : function(p) {
                _presence = p;
                this.connected = true;
                this.uid = p.user;
                this.name = p.name;
                return this;
            },
            /**
             * Domain infos
             */
            infos: {
                /**
                 * Get infos
                 */
                get: function(callback) {
                    get("/infos/", {'uid': _presence.user,
                                    'sid': _presence.id},
                        function(err, result, xhr) {
                            if (!err) {
                                callback(err, result.result, xhr);
                            } else {
                                callback(err, result, xhr);
                            }
                        });
                    return this;
                },
                /**
                 * Update infos
                 */
                update: function(metadata, callback) {
                    put("/infos/", {'uid': _presence.user,
                                    'sid': _presence.id,
                                    metadata: metadata},
                        function(err, result, xhr) {
                        if (!err) {
                            callback(err, result, xhr);
                        } else {
                            callback(err, result, xhr);
                        }
                    });
                    return this;
                }
            },

            /**
             * Search events
             */
            search: function(terms, params, callback) {
                if (!callback) {
                    callback = params;
                    params = {};
                }
                var query = terms.query || '';
                delete terms.query;
                var searchTerms = [];
                for (var i in terms) {
                    searchTerms.push(i+":"+terms[i]);
                }
                searchTerms.push(query);
                get("/search/event",
                    jQuery.extend({'uid': _presence.user,
                              'sid': _presence.id,
                              'searchTerms' : searchTerms.join(' ')}, params),
                    function (err, result, xhr) {
                        if (!callback) {
                            return;
                        }
                        callback(err, result.result, xhr);
                    });
            },
            _meetingsCache : {},
            meeting: function(meetingname) {
                if (!this._meetingsCache[meetingname])
                    this._meetingsCache[meetingname] = new UCEMeeting(this, meetingname, _presence);
                return this._meetingsCache[meetingname];
            },
            meetings : {
                opened: function(callback) {
                    return this._getCollection("opened", callback);
                },
                closed: function(callback) {
                    return this._getCollection("closed", callback);
                },
                upcoming: function(callback) {
                    return this._getCollection("upcoming", callback);
                },
                all: function(callback) {
                    return this._getCollection("all", callback);
                },
                _getCollection: function(type, callback) {
                    getCollection("/meeting/"+ type, {'uid': _presence.user,
                                                      'sid': _presence.id}, callback);
                    return this;
                }
            },
            user: {
                register: function(name, auth, credential, metadata, callback) {
                    post("/user/", jQuery.extend({}, {name: name, auth: auth, credential:credential, metadata:metadata}), function(err, result, xhr) {
                        callback(err, result, xhr);
                    });
                    return this;
                },
                registerWithPassword: function(name, credential, metadata, callback) {
                    return this.register(name, "password", credential, metadata, callback);
                },
                get: function(uid, callback) {
                    get("/user/"+ uid, jQuery.extend({}, {'uid': _presence.user,
                                                     'sid': _presence.id}),
                        function(err, result, xhr) {
                            callback(err, result, xhr);
                        });
                },
                addRole: function(uid, role, location, callback) {
                    post("/user/" + uid + "/roles", {'uid': _presence.user,
                                                     'sid': _presence.id,
                                                     'role': role,
                                                     'location': location},
                         function(err, result, xhr) {
                             callback(err, result, xhr);
                         });
                },
                delRole: function(uid, role, location, callback) {
                    del("/user/" + uid + "/roles/" + role + "/" + location,
                        {'uid': _presence.user,
                         'sid': _presence.id},
                        function(err, result, xhr) {
                            callback(err, result, xhr);
                        });

                },
                can: function(uid, action, object, conditions, location, callback) {
                    get("/user/" + uid + "/can/" + action + "/" + object + "/" + location,
                        {'conditions': conditions,
                         'uid': _presence.user,
                         'sid': _presence.id},
                        function(err, result, xhr) {
                            if (err)
                                callback(err, result, xhr);
                            else
                                callback(err, result.result === "true", xhr);
                        });
                }
            },
            role: {
                add: function(name, callback) {
                    post("/role", {'name': name,
                                   'uid': _presence.user,
                                   'sid': _presence.id}, callback);
                },
                del: function(name, callback) {
                    del("/role/" + name,
                        {'uid': _presence.user,
                         'sid': _presence.id}, callback);
                },
                addAccess: function(role, action, object, conditions, callback) {
                    post("/role/" + role + "/acl",
                         {'action': action,
                          'object': object,
                          'conditions': conditions,
                          'uid': _presence.user,
                          'sid': _presence.id}, callback);
                },
                delAccess: function(role, action, object, conditions, callback) {
                    del("/role/" + role + "/acl/" + action + "/" + object,
                        {'conditions': conditions,
                         'uid': _presence.user,
                         'sid': _presence.id}, callback);
                }
            },
            users: {
                get: function(callback) {
                    getCollection("/user/", {'uid': _presence.user,
                                             'sid': _presence.id}, callback);
                    return this;
                }
            },
            time: {
                get: function(callback) {
                    get("/time", {},
                        function(err, result, xhr) {
                            callback(err, result.result, xhr);
                        });
                    return this;
                }
            }
        };
    }
    g.uce = {
        version: VERSION,
        createClient : function(baseUrl) {
            return new UCEngine(baseUrl || '');
        },
        getWaiter : function(calls_needed, callback) {
            if(calls_needed == 0)
                callback();
            var ok = true;
            var waiter = function(){
                --calls_needed;
                if (calls_needed == 0 && ok)
                    callback();
                // XXX: should we raise an error if waiter called too many times?
            };
            return waiter;
        }
    };

})(window);
