/* 
 *  Copyright (c) 2010,
 *  Gavriloaie Eugen-Andrei (shiretu@gmail.com)
 *
 *  This file is part of crtmpserver.
 *  crtmpserver is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  crtmpserver is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with crtmpserver.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dbaccessprotocolhandler.h"
using namespace app_samplefactory;

DBAccessProtocolHandler::DBAccessProtocolHandler(Variant &configuration)
: BaseAppProtocolHandler(configuration) {

}

DBAccessProtocolHandler::~DBAccessProtocolHandler() {
}

void DBAccessProtocolHandler::RegisterProtocol(BaseProtocol *pProtocol) {
	FINEST("Hello!!! A new DBAccessProtocol came to life");
}

void DBAccessProtocolHandler::UnRegisterProtocol(BaseProtocol *pProtocol) {
	FINEST("Hello!!! A DBAccessProtocol died");
}

