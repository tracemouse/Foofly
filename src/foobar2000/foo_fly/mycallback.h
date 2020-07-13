//  $Id: chandlecallbackrun.h 75 2006-12-04 21:51:48Z jcpolos $
/*
 * foo_controlserver - server plugin to control foobar2000 over TCP/IP
 * Copyright (C) 2003, 2004, 2005, 2006  Jason Poloski
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Ste 500, Boston, MA 02110-1301, USA.
 *
 * Sept 2017 - added album art, library search functions - Walter Hartman
 *
 */

#ifndef INC_MYCALLBACK_H
#define INC_MYCALLBACK_H

#include "httpserver.h"

#include "../SDK/foobar2000.h"

class MyCallBack : public main_thread_callback
{
public:
    enum CallbackType { status, \
                        playlistGet, playlistClear, playlistAdd, playlistAddItem, playlistAddFile, playlistRemove, playlistSwitch, playlistPlay, \
                        playControl, \
                        libSearch, \
                        albumart };

    MyCallBack(CallbackType);
    MyCallBack(CallbackType, struct http_req_res *req_res);
    virtual void callback_run();

private:
    CallbackType m_ctype;
    struct http_req_res *m_req_res;
};

#endif  // INC_MYCALLBACK_H
