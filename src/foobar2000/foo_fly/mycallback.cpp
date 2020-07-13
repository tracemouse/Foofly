/*
 * foo_mobilecontrol - a plugin to enable remote control by mobile phone with fashion UI
 * Copyright(C) 2020, tracemouse(tracemouse@163.com). All rights reserved.
 *
 * This program is free software; you can redistribute itand /or
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
 */
 /*
  * Thanks:
  *   - foobar2000(http://www.foobar2000.org/)
  *   - foo_controlserver(https://github.com/audiohead/foo_controlserver)
  *   - cpp-httplib(https://github.com/yhirose/cpp-httplib)
  *   - cJsonObject(https://github.com/Bwar/CJsonObject)
  *   - cJSON(https://github.com/DaveGamble/cJSON)
  */

#include "MyCallback.h"

MyCallBack::MyCallBack(CallbackType t)
    : m_ctype(t),
    main_thread_callback()
{

}

MyCallBack::MyCallBack(CallbackType t, struct http_req_res *req_res)
    : m_ctype(t),
    //m_req(req),
    m_req_res(req_res),
    main_thread_callback()
{

}

void
MyCallBack::callback_run()
{
    switch (m_ctype)
    {
    case status:
        HttpServer::status(m_req_res);
        break;
    case playlistGet:
        HttpServer::playlist_get(m_req_res);
        break;
    case playlistAdd:
        HttpServer::playlist_add(m_req_res);
        break;
    case playlistClear:
        HttpServer::playlist_clear(m_req_res);
        break;
    case playlistSwitch:
        HttpServer::playlist_switch(m_req_res);
        break;
    case playlistAddItem:
        HttpServer::playlist_add_item(m_req_res);
        break;
    case playlistAddFile:
        HttpServer::playlist_add_file(m_req_res);
        break;
    case playlistPlay:
        HttpServer::playlist_play(m_req_res);
        break;
    case playControl:
        HttpServer::playControl(m_req_res);
        break;
    case libSearch:
        HttpServer::libSearch(m_req_res);
        break;
    case albumart:
        HttpServer::albumart(m_req_res);
        break;
    default:
        break;
    };

    SetEvent(m_req_res->endEvent);
}

