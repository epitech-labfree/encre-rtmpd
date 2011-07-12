/*
 * Copyright (c) 2009, Gavriloaie Eugen-Andrei (shiretu@gmail.com)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *              *Redistributions of source code must retain the above copyright notice,
 *               this list of conditions and the following disclaimer.
 *              *Redistributions in binary form must reproduce the above copyright
 *               notice, this list of conditions and the following disclaimer in the
 *               documentation and/or other materials provided with the distribution.
 *              *Neither the name of the DEVSS nor the names of its
 *               contributors may be used to endorse or promote products derived from
 *               this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _ENCREAPPLICATION_H
#define _ENCREAPPLICATION_H

#include "application/baseclientapplication.h"
#include "users.h"

namespace app_encre
{
#ifdef HAS_PROTOCOL_RTMP
  class RTMPAppProtocolHandler;
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_CLI
  class CLIAppProtocolHandler;
#endif /* HAS_PROTOCOL_CLI */


  class EncreApplication
    : public BaseClientApplication
  {
  private:
#ifdef HAS_PROTOCOL_RTMP
    RTMPAppProtocolHandler *_pRTMPHandler;
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_CLI
    CLIAppProtocolHandler *_pCLIHandler;
#endif /* HAS_PROTOCOL_CLI */

    user_map                    m_users;
    stream_map                  m_streams;

  public:
    EncreApplication(Variant &configuration);
    virtual ~EncreApplication();
    virtual bool Initialize();

    user_map                    &users();
    stream_map                  &streams();

#ifdef HAS_PROTOCOL_RTMP
    RTMPAppProtocolHandler      &rtmp();
#endif /* HAS_PROTOCOL_RTMP */
#ifdef HAS_PROTOCOL_CLI
    CLIAppProtocolHandler       &cli();
#endif /* HAS_PROTOCOL_CLI */

  };
}

#endif	/* _ENCREAPPLICATION_H */

