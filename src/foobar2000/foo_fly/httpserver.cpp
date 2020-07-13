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


#include <thread>
#include "HttpServer.h"
#include "util.h"
#include "MyCallback.h"
#include "albumart.h"
#include "CJsonObject.hpp"


// 初始化HttpServer静态类成员
httplib::Server HttpServer::m_svr;
pfc::string8 HttpServer::m_versionNumber = "1.0.0";
std::string HttpServer::m_web_folder = "foo_fly";
std::string HttpServer::m_web_dir = "./" + m_web_folder;
std::string HttpServer::m_port;
bool HttpServer::m_Exiting;
int HttpServer::m_playlistItemsPerPage = 30;

pfc::string8 HttpServer::m_metafields = "%length_seconds%|%codec%|%bitrate%|$if(%album artist%,%album artist%,%artist%)|%album%|%date%|%genre%|%tracknumber%|%title%|%path%|";

using   namespace   std;

void HttpServer::Init(const std::string &port)
{
	m_port = port;

	neb::CJsonObject track;
	track.Add("albumArtist", "%album artist%");
	track.Add("artist", "%artist%");
	track.Add("album", "%album%");
	//track.Add("number", "$ifgreater(%totaldiscs%,1, D%discnumber%.,)%tracknumber%");
	track.Add("number", "%tracknumber%");
	track.Add("track", "%title%");
	track.Add("len", "%length%");
	track.Add("codec", "%codec%");
	track.Add("bitRate", "%bitrate%kbps");
	track.Add("sampleRate", "%samplerate%Hz");
	track.Add("chanel", "%channels%");
	track.Add("fileUrl", "%path%");
	std::string str = track.ToString();
	str = util::replaceall(str, "%path%", "$replace(%path%,\\\,\\\\\,\", \\\")");
	str = util::replaceall(str, "%title%", "$replace(%title%,\\\,\\\\\,\", \\\")");
	str = util::replaceall(str, "%album%", "$replace(%album%,\\\,\\\\\,\", \\\")");
	str = util::replaceall(str, "%artist%", "$replace(%artist%,\\\,\\\\\,\", \\\")");
	str = util::replaceall(str, "%album artist%", "$replace(%album artist%,\\\,\\\\\,\", \\\")");

	m_metafields = (char*)str.c_str();
}

bool HttpServer::Start(HANDLE endEvent)
{
	util::WriteLog("HttpServer start");
	using namespace httplib;
	
	m_svr.Get("/api", HttpServer::handleHttpGet);
	m_svr.Get("/getArtwork", HttpServer::handleHttpGetArtwork);
	
	//www root
	auto ret = m_svr.set_mount_point("/", m_web_dir.c_str());

	util::WriteLog("starting http server at port: " + m_port);
	m_svr.listen("0.0.0.0", std::stoi(m_port));

	return true;
}


bool HttpServer::Close()
{
	util::WriteLog("HttperServer close");
	m_svr.stop();
	Sleep(500);
	return true;
}

void HttpServer::handleHttpGet(const httplib::Request& req, httplib::Response& res)
{
	util::WriteLog("handleHttpGet start, path = " + req.path);

	util::WriteLog("=========params===========");
	for (const auto parm : req.params) {
		util::WriteLog(parm.first + "=" + parm.second);
	}

	//util::WriteLog("=========req headers===========");
	//for (const auto header : req.headers) {
	//	util::WriteLog(header.first + "=" + header.second);
	//}

	std::string cmd = "";
	if (req.has_param("cmd")) {
		cmd = req.get_param_value("cmd");
	}

	util::WriteLog("cmd=" + cmd);

	//closescreen
	if (cmd == "closeScreen") {
		SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
		returnSucess(res,true);
		return;
	}

	//shutdown
	if (cmd == "shutdown") {
		std::string param1 = (req.has_param("param1")) ? req.get_param_value("param1") : "0";
		t_size minutes = 0;
		if (!util::convertStrToI(param1.c_str(), minutes)) {
			returnSucess(res, false);
			return;
		}
		std::string cmds = "shutdown /s /t " + to_string(minutes*60);  //shutdown after minutes
		if (minutes > 999) cmds = "shutdown /a";  //cancel shutdown
		system(cmds.c_str());
		util::WriteLog(cmds);
		returnSucess(res,true);
		return;
	}

	util::WriteLog("start to call foobar2000");
	HANDLE fooEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	struct http_req_res* req_res = new http_req_res(req, &res, fooEvent);
	static static_api_ptr_t<main_thread_callback_manager> cm;

	MyCallBack::CallbackType type = MyCallBack::CallbackType::status;
	if (cmd == "playlistGet") {
		type = MyCallBack::CallbackType::playlistGet;
	}
	if (cmd == "playlistAdd") {
		type = MyCallBack::CallbackType::playlistAdd;
	}
	if (cmd == "playlistRemove") {
		type = MyCallBack::CallbackType::playlistRemove;
	}
	if (cmd == "playlistClear") {
		type = MyCallBack::CallbackType::playlistClear;
	}
	if (cmd == "playlistAddItem") {
		type = MyCallBack::CallbackType::playlistAddItem;
	}
	if (cmd == "playlistAddFile") {
		type = MyCallBack::CallbackType::playlistAddFile;
	}
	if (cmd == "playlistSwitch") {
		type = MyCallBack::CallbackType::playlistSwitch;
	}
	if (cmd == "playlistPlay") {
		type = MyCallBack::CallbackType::playlistPlay;
	}
	if (cmd == "playControl") {
		type = MyCallBack::CallbackType::playControl;
	}
	if (cmd == "libSearch") {
		type = MyCallBack::CallbackType::libSearch;
	}
	if (cmd == "albumart") {
		type = MyCallBack::CallbackType::albumart;
	}

	cm->add_callback(new service_impl_t<MyCallBack>(type, req_res));  //register callback into main thread of foobar2000

	//loop until callback run end
	DWORD start = GetTickCount();
	while (true) {
		DWORD dwRet, normRet;
		normRet = WaitForMultipleObjects(1, &fooEvent, FALSE, 100);
		if (normRet == WAIT_OBJECT_0)			//foobar2000 callback run end
		{
			break;
		}			  
		if(m_Exiting ||							  //exit foobar2000
		  (GetTickCount() - start) > 1000 * 10)  // 10 seconds timeout (to avoid dead loop/memory lack)
		{ 
			returnSucess(res, false);
			break; 
		}
		Sleep(100);
	}

	util::WriteLog("response body = " + res.body);
	CloseHandle(fooEvent);
	fooEvent = 0;
	delete req_res;
	req_res = nullptr;
}

void HttpServer::handleHttpGetArtwork(const httplib::Request& req, httplib::Response& res)
{
	//set header
	res.set_header("Cache-Control", "max-age=31536000");  //1 year

	//cover file should be named as cover.jpg and located at the same folder of the musical file
	std::string defaultCover = m_web_dir + "/assets/img/cover.jpg";

	util::WriteLog("handleHttpGetArtwork start, path = " + req.path);
	util::WriteLog("default cover = " + defaultCover);

	std::string fileUrl = "";
	if (req.has_param("fileUrl")) {
		fileUrl = req.get_param_value("fileUrl");
	}
	util::WriteLog("fileUrl = " + fileUrl);
	
	//const size_t last_slash_idx = fileUrl.find_last_of("\\/");
	//std::string dir = (std::string::npos != last_slash_idx) ? fileUrl.substr(0, last_slash_idx) : "";
	std::string dir = util::getFolderPath(fileUrl);
	std::string cover = dir +  "/cover.jpg";
	
	util::WriteLog("cover = " + cover);

	std::locale loc = std::locale::global(std::locale("zh_CN.UTF-8"));
	//cover = util::Utf8ToAnsi(cover.c_str());
	if (!httplib::detail::is_file(cover)) {
		cover = defaultCover;
	}
 
	util::WriteLog("final cover = " + cover);
	//std::string fileext = httplib::detail::file_extension(path);

	if (httplib::detail::is_file(cover)) {
		httplib::detail::read_file(cover, res.body);
		auto type = httplib::detail::find_content_type(cover, m_svr.get_file_extension_and_mimetype_mapping());
		if (type) { res.set_header("Content-Type", type); }
		res.status = 200;
		std::locale::global(loc);
		return;
	}else{
		std::locale::global(loc);
		returnSucess(res, false);
	}
}

void HttpServer::returnSucess(httplib::Response& res, bool succ) {
	std::string result = (succ)?"{\"isSucc\":true}":"{\"isSucc\":false}";
	res.set_content(result, "application/json");
}

void HttpServer::playControl(struct http_req_res* req_res) {
	static_api_ptr_t<play_control> pbc;
	static_api_ptr_t<playlist_manager_v2> plm;

	std::string param1 = "";
	if (req_res->req.has_param("param1")) {
		param1 = req_res->req.get_param_value("param1");
	}

	if (param1 == "playpause") {
		if (pbc->is_playing()) {
			pbc->play_or_pause();
		}
		else {
			pbc->start(playback_control_v2::track_command_settrack);
		}
	}
	if (param1 == "play") {
		pbc->start(playback_control_v2::track_command_settrack);
	}
	if (param1 == "pause") {
		pbc->pause(true);
	}
	if (param1 == "stop")  pbc->stop();
	if (param1 == "mute")  pbc->volume_mute_toggle();
	if (param1 == "next") {
		//pbc->next();
		pbc->start(play_control::track_command_next);
	}
	if (param1 == "prev") {
		//pbc->previous();
		pbc->start(play_control::track_command_prev);
	}
	if (param1 == "FocusOnPlaying") {
		t_size playlistIdx, trackIdx;
		plm->get_playing_item_location(&playlistIdx, &trackIdx);
		plm->set_active_playlist(playlistIdx);
		plm->playlist_set_focus_item(playlistIdx, trackIdx);
	}

	//set playback order
	if (param1 == "playbackOrder") {
		std::string param2 = "";
		if (req_res->req.has_param("param2")) {
			param2 = req_res->req.get_param_value("param2");
		}
		t_size playbackOrder = 0;
		if (!util::convertStrToI(param2.c_str(), playbackOrder)) {
			returnSucess(*(req_res->res), false);
			return;
		}
		t_size cnt = plm->playback_order_get_count();
		if (playbackOrder < 0 || playbackOrder >= cnt) {
			returnSucess(*(req_res->res), false);
			return;
		}
		plm->playback_order_set_active(playbackOrder);
	}

	//seek position
	if (param1 == "seek") {
		std::string param2 = "";
		if (req_res->req.has_param("param2")) {
			param2 = req_res->req.get_param_value("param2");
		}
		if (pbc->playback_can_seek()) {
			double pos = 0;
			if (!util::convertStrToF(param2.c_str(), pos)) {
				returnSucess(*(req_res->res), false);
				return;
			}
			pbc->playback_seek(pos);
		}
	}

	//set volume
	if (param1 == "volume") {
		std::string param2 = "";
		if (req_res->req.has_param("param2")) {
			param2 = req_res->req.get_param_value("param2");
		}
		double vol = 0;
		if (!util::convertStrToF(param2.c_str(), vol)) {
			returnSucess(*(req_res->res), false);
			return;
		}
		if (vol < 0 && vol > 100) {
			returnSucess(*(req_res->res), false);
			return;
		}
		vol = vol - 100;
		util::WriteLog("set volume=" + to_string(vol));
		pbc->set_volume((float)vol);
	}
	
	HttpServer::status(req_res);
	//returnSucess(*(req_res->res), true);
}

void HttpServer::status(struct http_req_res* req_res) {
	util::WriteLog("status start");
	static_api_ptr_t<play_control> pbc;
	static_api_ptr_t<playlist_manager_v2> plm;
	
	neb::CJsonObject status;
	status.AddEmptySubArray("playlist");
	status.AddEmptySubArray("playlists");
	status.Add("playBackOrder", plm->playback_order_get_active());
	std::string volume = pfc::format_float(100 - pbc->get_volume(), 2, 0);
	status.Add("volume",volume);
	int isPlaying = 0;
	if (pbc->is_playing()) {
		if (!pbc->is_paused()) {
			isPlaying = 1;
		}
	}
	status.Add("isPlaying", isPlaying);
	status.Add("isMuted", pbc->is_muted() ? "1" : "0");
	std::string len = pfc::format_float(pbc->playback_get_length(), 10, 0);
	std::string pos = pfc::format_float(pbc->playback_get_position(), 10, 0);
	status.Add("tracklen", len);
	status.Add("trackpos", pos);
	status.Add("albumArt", "");
	status.Add("playlistItemsPerPage", m_playlistItemsPerPage);

	t_size currentTrack = pfc::infinite_size;
	t_size currentPlaylist = pfc::infinite_size;
	t_size currentPage = pfc::infinite_size;
	metadb_handle_ptr ptr_nowPlaying = NULL;
	pbc->get_now_playing(ptr_nowPlaying);
	if (pbc->get_now_playing(ptr_nowPlaying))
	{
		plm->get_playing_item_location(&currentPlaylist, &currentTrack);  // returns playlist # and index to playing
	}
	else {
		currentPlaylist = plm->get_active_playlist();
		plm->playlist_get_focus_item_handle(ptr_nowPlaying, currentPlaylist);
		currentTrack = plm->playlist_get_focus_item(currentPlaylist);
	}

	currentPage = currentTrack / m_playlistItemsPerPage + 1;

	pfc::string8 strTrack;
	HttpServer::generateTrackString(ptr_nowPlaying, strTrack);
	neb::CJsonObject track = neb::CJsonObject((char*)strTrack.get_ptr());
	track.Add("playlistIdx", currentPlaylist);
	track.Add("trackIdx", currentTrack);
	status.Add("playing", track);

	t_size tot = plm->playlist_get_item_count(currentPlaylist);
	t_size start = (currentPage - 1) * m_playlistItemsPerPage;
	t_size end = ((currentPage * m_playlistItemsPerPage) > tot) ? (tot - 1) : (currentPage * m_playlistItemsPerPage - 1);

	util::WriteLog("start=" + to_string(start) + ",end=" + to_string(end) + ",tot=" + to_string(tot));

	//get all track on current page
	/*
	bit_array_range r(start, m_playlistItemsPerPage);
	metadb_handle_list list;
	plm->playlist_get_items(currentPlaylist, list, r);
	for (t_size i = 0; i < list.get_count(); i++) {
		pfc::string8 strTrack;
		generateTrackString(list[i], strTrack);
		neb::CJsonObject track = neb::CJsonObject((char*)strTrack.get_ptr());
		track.Add("playlistIdx", currentPlaylist);
		int trackIdx = start + i;
		track.Add("trackIdx", trackIdx);
		status["playlist"].Add(track);
	}
 	*/
	status.Add("currentPage", currentPage);
	status.Add("currentTrack", currentTrack);
	status.Add("currentPlaylist", currentPlaylist);

	//get all playlist
	int cntPlaylist = plm->get_playlist_count();
	for (int i = 0; i < cntPlaylist; i++) {
		pfc::string8 name;
		plm->playlist_get_name(i, name);
		t_size count = plm->playlist_get_item_count(i);
		neb::CJsonObject playlist;
		playlist.Add("name", (char*)name.get_ptr());
		playlist.Add("count", count);
		playlist.Add("playlistIdx", i);
		status["playlists"].Add(playlist);
	}
	req_res->res->set_content(status.ToString(), "application/json;charset=utf-8");
	//util::WriteLog("body=" + req_res->res->body);
}

void HttpServer::playlist_get(struct http_req_res* req_res) {
	util::WriteLog("playlist_get start");
	static_api_ptr_t<playlist_manager_v2> plm;

	std::string param1 = "";   //playlist index
	if (req_res->req.has_param("param1")) {
		param1 = req_res->req.get_param_value("param1");
	}
	std::string param2 = "1";  //page no.
	if (req_res->req.has_param("param2")) {
		param2 = req_res->req.get_param_value("param2");
	}
	t_size playlistIdx = 0;
	if (!util::convertStrToI(param1.c_str(), playlistIdx)) {
		returnSucess(*(req_res->res), false);
		return;
	}
	t_size totPlaylist = plm->get_playlist_count();
	if (playlistIdx > totPlaylist || playlistIdx < 0) {
		returnSucess(*(req_res->res), false);
		return;
	}
	t_size page=1;
	t_size playlistItemsPerPage = m_playlistItemsPerPage;
	t_size totTrack = plm->playlist_get_item_count(playlistIdx);
	if (param2 == "all") {
		playlistItemsPerPage = totTrack;
	}else{
		if (!util::convertStrToI(param2.c_str(), page)) {
			returnSucess(*(req_res->res), false);
			return;
		}
	}

	util::WriteLog("playlistIdx=" + to_string(playlistIdx) + ",page=" + to_string(page));

	t_size totPage = (totTrack <= playlistItemsPerPage)?1:(totTrack / playlistItemsPerPage + 1);
	if(page > totPage || page < 1){ 
		returnSucess(*(req_res->res), false); 
		return;
	}

	t_size start = (page - 1) * playlistItemsPerPage;
	t_size end = ((page * playlistItemsPerPage) > totTrack) ? (totTrack - 1) : (page * playlistItemsPerPage - 1);

	util::WriteLog("start=" + to_string(start) + ",end=" + to_string(end) + ",tot=" + to_string(totTrack));

	neb::CJsonObject status;
	status.AddEmptySubArray("playlist");
	//bit_array_range r(start, end);
	bit_array_range r(start, playlistItemsPerPage);
	metadb_handle_list list;
	plm->playlist_get_items(playlistIdx, list, r);

	for (t_size i = 0; i < list.get_count(); i++) {
		pfc::string8 strTrack;
		generateTrackString(list[i], strTrack);
		neb::CJsonObject track = neb::CJsonObject((char*)strTrack.get_ptr());
		track.Add("playlistIdx", playlistIdx);
		int trackIdx = start + i;
		track.Add("trackIdx", trackIdx);
		status["playlist"].Add(track);
	}

	status.Add("currentPage", page);
	status.Add("currentPlaylist", playlistIdx);
	status.Add("totTrack", totTrack);
	status.Add("totPage", totPage);
	status.Add("playlistItemsPerPage", playlistItemsPerPage);

	req_res->res->set_content(status.ToString(), "application/json;charset=utf-8");
	util::WriteLog("body=" + req_res->res->body);
}

void HttpServer::playlist_clear(struct http_req_res* req_res) {
	util::WriteLog("playlist_clear start");
	static_api_ptr_t<playlist_manager_v2> plm;

	std::string param1 = "";  //playlist index
	if (req_res->req.has_param("param1")) {
		param1 = req_res->req.get_param_value("param1");
	}

	t_size playlistIdx = 0;
	if (!util::convertStrToI(param1.c_str(), playlistIdx)) {
		returnSucess(*(req_res->res), false);
		return;
	}
 
	t_size totPlaylist = plm->get_playlist_count();
	if (playlistIdx > totPlaylist || playlistIdx < 0) { 
		returnSucess(*(req_res->res), false); 
		return;
	}

	plm->playlist_clear(playlistIdx);

	//HttpServer::status(req_res);
	returnSucess(*(req_res->res), true);
}

void HttpServer::playlist_add(struct http_req_res* req_res) {
	util::WriteLog("playlist_add start");
	static_api_ptr_t<playlist_manager_v2> plm;

	std::string param1 = "";  //playlist name
	if (req_res->req.has_param("param1")) {
		param1 = req_res->req.get_param_value("param1");
	}

	if (param1 == ""){ 
		returnSucess(*(req_res->res), false);
		return;
	}	

	pfc::string8 name = param1.c_str();
	t_size playlistIdx = plm->find_or_create_playlist(name, pfc::infinite_size);

	HttpServer::status(req_res);
	//returnSucess(*(req_res->res), true);
}

void HttpServer::playlist_remove(struct http_req_res* req_res) {
	util::WriteLog("playlist_remove start");
	static_api_ptr_t<playlist_manager_v2> plm;

	std::string param1 = "";  //playlist index
	if (req_res->req.has_param("param1")) {
		param1 = req_res->req.get_param_value("param1");
	}

	t_size playlistIdx = 0;
	if (!util::convertStrToI(param1.c_str(), playlistIdx)) {
		returnSucess(*(req_res->res), false);
		return;
	}

	t_size totPlaylist = plm->get_playlist_count();
	if (playlistIdx > totPlaylist || playlistIdx < 0) {
		returnSucess(*(req_res->res), false);
		return;
	}

	plm->remove_playlist(playlistIdx);

	//HttpServer::status(req_res);
	returnSucess(*(req_res->res), true);
}

void HttpServer::playlist_add_item(struct http_req_res* req_res) {
	util::WriteLog("playlist_add_item start");
	static_api_ptr_t<playlist_manager_v2> plm;

	std::string param1 = "";   //playlist index
	if (req_res->req.has_param("param1")) {
		param1 = req_res->req.get_param_value("param1");
	}
	std::string param2 = "";  //track
	if (req_res->req.has_param("param2")) {
		param2 = req_res->req.get_param_value("param2");
	}

	t_size playlistIdx = 0;
	if (!util::convertStrToI(param1.c_str(), playlistIdx)) {
		returnSucess(*(req_res->res), false);
		return;
	}

	if (param2 == "") {
		returnSucess(*(req_res->res), false);
		return;
	}

	pfc::string8 fileUrl = param1.c_str();

	util::WriteLog("playlistIdx=" + to_string(playlistIdx) + ",fileUrl=" + param1);

	t_size totPlaylist = plm->get_playlist_count();
	if (playlistIdx > totPlaylist || playlistIdx < 0) {
		returnSucess(*(req_res->res), false);
		return;
	}

	neb::CJsonObject track = neb::CJsonObject(param2);
	int fPlaylistIdx=0;
	track.Get("playlistIdx", fPlaylistIdx);
	int fTrackIdx = 0;
	track.Get("trackIdx", fTrackIdx);

	bit_array_one f(fTrackIdx);
	metadb_handle_list list;
	plm->playlist_get_items(fPlaylistIdx, list, f);
	
	bit_array_one r(0);
	plm->playlist_add_items(playlistIdx, list, r);

	//HttpServer::status(req_res);
	returnSucess(*(req_res->res), true);
}

void HttpServer::playlist_add_file(struct http_req_res* req_res) {
	util::WriteLog("playlist_add_item start");
	static_api_ptr_t<playlist_manager_v2> plm;

	std::string param1 = "";   //playlist index
	if (req_res->req.has_param("param1")) {
		param1 = req_res->req.get_param_value("param1");
	}
	std::string param2 = "";  //fileUrl
	if (req_res->req.has_param("param2")) {
		param2 = req_res->req.get_param_value("param2");
	}

	t_size playlistIdx = 0;
	if (!util::convertStrToI(param1.c_str(), playlistIdx)) {
		returnSucess(*(req_res->res), false);
		return;
	}

	if (param2 == "") {
		returnSucess(*(req_res->res), false);
		return;
	}

	pfc::string8 fileUrl = param2.c_str();

	util::WriteLog("playlistIdx=" + to_string(playlistIdx) + ",fileUrl=" + param2);

	t_size totPlaylist = plm->get_playlist_count();
	if (playlistIdx > totPlaylist || playlistIdx < 0) {
		returnSucess(*(req_res->res), false);
		return;
	}
 
	HWND hwnd = core_api::get_main_window();
	pfc::list_single_ref_t<const char*> list(fileUrl);
	plm->playlist_add_locations(playlistIdx, list, true, hwnd);

	//HttpServer::status(req_res);
	returnSucess(*(req_res->res), true);
}

void HttpServer::playlist_switch(struct http_req_res* req_res) {
	util::WriteLog("playlist_switch start");
	static_api_ptr_t<playlist_manager_v2> plm;

	std::string param1 = "";   //playlist index
	if (req_res->req.has_param("param1")) {
		param1 = req_res->req.get_param_value("param1");
	}
 
	t_size playlistIdx = 0;
	if (!util::convertStrToI(param1.c_str(), playlistIdx)) {
		returnSucess(*(req_res->res), false);
		return;
	}

	t_size totPlaylist = plm->get_playlist_count();
	if (playlistIdx > totPlaylist || playlistIdx < 0) {
		returnSucess(*(req_res->res), false);
		return;
	}

	plm->set_active_playlist(playlistIdx);

	//HttpServer::status(req_res);
	returnSucess(*(req_res->res), true);
}

void HttpServer::playlist_play(struct http_req_res* req_res) {
	util::WriteLog("playlist_play start");
	static_api_ptr_t<playlist_manager_v2> plm;
	static_api_ptr_t<play_control> pbc;

	std::string param1 = "";   //playlist index
	if (req_res->req.has_param("param1")) {
		param1 = req_res->req.get_param_value("param1");
	}

	std::string param2 = "";   //track index
	if (req_res->req.has_param("param2")) {
		param2 = req_res->req.get_param_value("param2");
	}
	util::WriteLog("param1=" + param1);
	util::WriteLog("param2=" + param2);

	t_size playlistIdx = 0;
	if (!util::convertStrToI(param1.c_str(), playlistIdx)) {
		returnSucess(*(req_res->res), false);
		return;
	}
	t_size trackIdx = 0;
	if (!util::convertStrToI(param2.c_str(), trackIdx)) {
		returnSucess(*(req_res->res), false);
		return;
	}
	t_size totPlaylist = plm->get_playlist_count();
	if (playlistIdx > totPlaylist || playlistIdx < 0) {
		returnSucess(*(req_res->res), false);
		return;
	}
	t_size totTrack = plm->playlist_get_item_count(playlistIdx);
	if (trackIdx > totTrack || trackIdx < 0) {
		returnSucess(*(req_res->res), false);
		return;
	}

	plm->set_active_playlist(playlistIdx);
	plm->playlist_execute_default_action(playlistIdx, trackIdx);
//	pbc->start(playback_control_v2::track_command_play, false);
	HttpServer::status(req_res);
}

void HttpServer::albumart(struct http_req_res* req_res) {
	//set header
	req_res->res->set_header("Cache-Control", "max-age=31536000");  //1 year

	util::WriteLog("albumart start");
	static_api_ptr_t<playback_control_v2> pbc;
	static_api_ptr_t<playlist_manager_v2> plm;
	metadb_handle_ptr ptr_track;
	//albumArt albumArt;
	
	//albumArt.reset();
	//albumArt.timeStampTicks = GetTickCount(); // timestamp request time, not used currently

	std::string param1 = "";   //playlist index
	if (req_res->req.has_param("param1")) {
		param1 = req_res->req.get_param_value("param1");
	}

	std::string param2 = "";   //track index
	if (req_res->req.has_param("param2")) {
		param2 = req_res->req.get_param_value("param2");
	}

	t_size playlistIdx = 0;
	if (!util::convertStrToI(param1.c_str(), playlistIdx)) {
		returnSucess(*(req_res->res), false);
		return;
	}

	t_size trackIdx = 0;
	if (!util::convertStrToI(param2.c_str(), trackIdx)) {
		returnSucess(*(req_res->res), false);
		return;
	}

	t_size totPlaylist = plm->get_playlist_count();
	if (playlistIdx > totPlaylist || playlistIdx < 0) {
		returnSucess(*(req_res->res), false);
		return;
	}
	t_size totTrack = plm->playlist_get_item_count(playlistIdx);
	if (trackIdx > totTrack || trackIdx < 0) {
		returnSucess(*(req_res->res), false);
		return;
	}
	bit_array_one r(trackIdx);
	metadb_handle_list list;
	plm->playlist_get_items(playlistIdx, list, r);
	ptr_track = list.get_item(0);
 
	//albumArt.pb_albumart_status = albumArt::AS_NOT_FOUND;

	pfc::list_t<GUID> guids;
	guids.add_item(album_art_ids::cover_front);
	metadb_handle_list tracks;
	tracks.add_item(ptr_track);

	// Foobar2000 do album art lookup using settings defined under :
	// Files>Preferences>Display and Files>Preferences>Advanced>Display>Album art
	// So either image file or embedded album art. 
	// If an external file, Foobar2000 will look for jpg files named "folder", "cover",
	// "front", "album", "filename" by default.

	static_api_ptr_t<album_art_manager_v3> aam;
	abort_callback_dummy abortCallback;
	album_art_data::ptr art_ptr;

	bool found = false;
	album_art_extractor_instance_v2::ptr extractor = aam->open_v3(tracks, guids, NULL, abortCallback);

	found = extractor->query(album_art_ids::cover_front, art_ptr, abortCallback);
	if (!found)
	{
		found = extractor->query(album_art_ids::cover_back, art_ptr, abortCallback);
		if (!found)
		{
			found = extractor->query(album_art_ids::disc, art_ptr, abortCallback);
			if (!found)
			{
				found = extractor->query(album_art_ids::artist, art_ptr, abortCallback);
			}
		}
	}

	if (found)
	{
		if (art_ptr.is_valid())
		{
			//albumArt.imageSize = art_ptr->get_size();
			//albumArt.pb_albumart_status = albumArt::AS_FOUND;

			// Convert binary image to base64 text for transfer to client
			// The base64size can be like 1.5x larger than the original image size
			//if (albumArt.imageSize > 0)
			if(art_ptr->get_size() > 0)
			{
				//util::writeImageFile("r:/cover.jpg", (unsigned char*)art_ptr->get_ptr(), albumArt.imageSize);
 
				//return binaray(unsigned char* -> base64 -> string)
				//pfc::base64_encode(albumArt.base64String, (const void*)art_ptr->get_ptr(), albumArt.imageSize);
				//albumArt.base64Size = albumArt.base64String.get_length();
				//std::string bin = util::base64_decode((char*)albumArt.base64String.get_ptr());
				//req_res->res->set_content(bin, "image/jpeg");

				//return base64 string
				//pfc::string8 strImg;
				//strImg << "data:image/jpg; base64," << albumArt.base64String;
				//req_res->res->set_content((char*)strImg.get_ptr(),"image/jpeg");
				//req_res->res->set_content((char*)strImg.get_ptr(), "text/plain");
				//req_res->res->set_content((char*)strImg.get_ptr(), "application/octet-stream");
				//req_res->res->body = (char*)strImg.get_ptr();

				//return binary(unsigned char* -> string)
				unsigned char* ptr = (unsigned char*)art_ptr->get_ptr();
				std::string str(reinterpret_cast<const char*>(ptr), art_ptr->get_size());
				req_res->res->set_content(str, "image/jpeg");
				return;
			}
		}
	}

	std::string cover = m_web_dir + "/assets/img/cover.jpg";
	if (httplib::detail::is_file(cover)) {
		httplib::detail::read_file(cover, req_res->res->body);
		auto type = httplib::detail::find_content_type(cover, m_svr.get_file_extension_and_mimetype_mapping());
		if (type) { req_res->res->set_header("Content-Type", type); }
		req_res->res->status = 200;
		return;
	}
	else {
		returnSucess(*(req_res->res), false);
	}
}

void HttpServer::libSearch(struct http_req_res* req_res) {
	util::WriteLog("libSearch start");
	static_api_ptr_t<playlist_manager_v2> plm;
	static_api_ptr_t<autoplaylist_manager> apm;

	pfc::string8 playlistName = "Foofly Search";
	pfc::string8 searchFor;
	pfc::string8 searchFilter = "%artist%|%album artist%|%album%|%%title%";
	t_size playlistIdx;
	t_size playlist_item_count;

	std::string param1 = "";   //search filter
	if (req_res->req.has_param("param1")) {
		param1 = req_res->req.get_param_value("param1");
	}
	std::string param2 = "";  //search for
	if (req_res->req.has_param("param2")) {
		param2 = req_res->req.get_param_value("param2");
	}
	if (param2 == "") {
		returnSucess(*(req_res->res), false);
		return;
	}
	searchFor = param2.c_str();
	if (param1 == "artist") {searchFilter = "%artist%|%album artist%";}
	if (param1 == "folder") { searchFilter = "%path%"; }
	if (param1 == "album") { searchFilter = "%album"; }
	if (param1 == "artist") { searchFilter = "%artist%|%album artist%"; }
	if (param1 == "title") { searchFilter = "%title%"; }

	playlistIdx = plm->find_or_create_playlist(playlistName.toString(), pfc::infinite_size);

	if (playlistIdx != pfc::infinite_size)
	{
		//plm->set_active_playlist(playlistIdx);

		if (apm->is_client_present(playlistIdx))
			apm->remove_client(playlistIdx);

		if (searchFor.get_length())
		{
			plm->playlist_undo_backup(playlistIdx);

			try
			{
				apm->add_client_simple(searchFor.toString(), searchFilter, playlistIdx, true);
			}
			catch (pfc::exception& e)
			{
				util::WriteLog(e.what());
			}

			if (apm->is_client_present(playlistIdx))
				apm->remove_client(playlistIdx);
		}

		if (playlistIdx != pfc::infinite_size)
			playlist_item_count = plm->playlist_get_item_count(playlistIdx);
		else {
			playlist_item_count = pfc::infinite_size;
			returnSucess(*(req_res->res), false);
			return;
		}
		//plm->set_active_playlist(playlistIdx);
	}
	else {
		returnSucess(*(req_res->res), false);
		return;
	}

	util::WriteLog("playlistIdx=" + to_string(playlistIdx));
 
 	neb::CJsonObject status;
	status.AddEmptySubArray("playlist");
	metadb_handle_list list;
	plm->playlist_get_all_items(playlistIdx,list);
	for (t_size i = 0; i < list.get_count(); i++) {
		pfc::string8 strTrack;
		generateTrackString(list[i], strTrack);
		neb::CJsonObject track = neb::CJsonObject((char*)strTrack.get_ptr());
		track.Add("playlistIdx", playlistIdx);
		int trackIdx = i;
		track.Add("trackIdx", trackIdx);
		status["playlist"].Add(track);
	}

	status.Add("currentPage", 1);
	status.Add("currentPlaylist", playlistIdx);
	status.Add("totTrack", playlist_item_count);
	status.Add("totPage", 1);
	status.Add("playlistItemsPerPage", playlist_item_count);

	req_res->res->set_content(status.ToString(), "application/json;charset=utf-8");
	util::WriteLog("body=" + req_res->res->body);

	//plm->remove_playlist(playlistIdx);
}

void HttpServer::generateTrackString(metadb_handle_ptr handle, pfc::string8& str)
{
	pfc::string8 field;
	static_api_ptr_t<playback_control_v2> pbc;
	unsigned int i = 0;

	if (handle == NULL)
	{
		return;
	}

 	service_ptr_t<titleformat_object> script;
	pfc::string8 text;

	// mutex on metafields
	static_api_ptr_t<titleformat_compiler>()->compile_safe(script, HttpServer::m_metafields);

	pbc->playback_format_title_ex(handle, NULL, text, script, NULL, play_control::display_level_all);

	str << text ;
	//util::WriteLog((char*)str.get_ptr());
}