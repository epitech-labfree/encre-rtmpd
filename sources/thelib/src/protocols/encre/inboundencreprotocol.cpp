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

#include "netio/kqueue/iohandlermanager.h"


#ifdef HAS_PROTOCOL_ENCRE
#include "protocols/encre/inboundencreprotocol.h"
#include "protocols/encre/baseencreprotocol.h"

#include <boost/spirit/include/qi_char.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_lit.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_rule.hpp>

namespace qi = boost::spirit::qi;

InboundEncreProtocol::InboundEncreProtocol()
  : BaseEncreProtocol(PT_INBOUND_ENCRE), _bTypeStream(false), _bSender(false), _bReceiver(false), _bStreamName("") {
}

InboundEncreProtocol::~InboundEncreProtocol() {
}

bool InboundEncreProtocol::AllowFarProtocol(uint64_t type) {
  return (type == PT_TCP);
}

bool InboundEncreProtocol::AllowNearProtocol(uint64_t type) {
	return true;
}

bool InboundEncreProtocol::SignalInputData(int32_t recvAmount) {
	ASSERT("Operation not supported");
	return false;
}

template <typename Iterator>
struct header_p : qi::grammar<Iterator, std::string()>
{
  header_p() : header_p::base_type(header)
	{
		header = keyword >> qi::blank >> zname >> qi::lit("\n\n");
		keyword = (qi::string("GET") | qi::string("PUT"));
		zname = *qi::char_("a-zA-Z0-9");
	}
  qi::rule<Iterator, std::string()> zname, keyword, header;
};

bool InboundEncreProtocol::TypeStream(std::string& str)
{
	std::string::iterator begin = str.begin();
	std::string::iterator end = str.end();
	header_p<std::string::iterator> p;
	std::string v;
	bool ret = qi::parse(begin, end, p, v);
	if (ret == false)
		return false;
	std::string tmp = v.substr(0, 3);
	v = v.substr(3);
	if (tmp == "GET")
	  _bReceiver = true;
	else
	  _bSender = true;
	for (std::string::iterator it = v.begin(); it != v.end(); ++it)
	{
	  if (*it == ' ')
	    it = v.erase(it);
	}
	_bStreamName = v;
	return ret;
}

// TODO : send data to the 'GET' streams
bool InboundEncreProtocol::SendData(IOBuffer &buffer) {
	if (_pNearProtocol == 0) {
		FATAL("The near protocol is not initialized");
		return false;
	}
	return _pNearProtocol->SignalInputData(buffer);
}

bool InboundEncreProtocol::SignalInputData(IOBuffer &buffer) {
	//1. Get the buffer and the length
	uint8_t *pBuffer = GETIBPOINTER(buffer);
	uint32_t length = GETAVAILABLEBYTESCOUNT(buffer);

	bool	 ret = true;

	if (length == 0)
		return ret;
	// if the type of the stream is unknown
	if (_bTypeStream == false) {
		// Protocol have unless 7 characteres
		if (length < 7)
			return ret;
		// XXX : convert to string is not a good idea?
		std::string str((char*)pBuffer, length);
		ret = TypeStream(str);
		_bTypeStream = ret;
		buffer.IgnoreAll();
	}
	// Send the data | I don't know : maybe a map
	else {
		if (_bReceiver == true) {
			std::clog << "message discarded : " << std::string((char*)pBuffer, length) << std::endl;
			buffer.IgnoreAll();
		} else {
			ret = SendData(buffer);
		}
	}
	return ret;
}

bool InboundEncreProtocol::Initialize(Variant &parameters) {
	_customParameters = parameters;
	return true;
}

IOBuffer * InboundEncreProtocol::GetOutputBuffer() {
	if (GETAVAILABLEBYTESCOUNT(_outputBuffer) > 0)
		return &_outputBuffer;
	return NULL;
}

#endif /* HAS_PROTOCOL_ENCRE */
