#ifndef INC_HTTPSERVER_H
#define INC_HTTPSERVER_H

#include "httplib.h"

#include "../SDK/foobar2000.h"
#include "../../pfc/pfc.h"

struct http_req_res {
	httplib::Request req;
	httplib::Response* res;
	HANDLE endEvent;

	http_req_res(httplib::Request req1, httplib::Response* res1, HANDLE endEvent1) {
		req = req1;
		res = res1;
		endEvent = endEvent1;
	}
};

 
class HttpServer
{
public:
	HttpServer() {}
	~HttpServer() {}

	static void Init(const std::string &port); // 初始化设置
	static bool Start(HANDLE); // 启动httpserver
	static bool Close(); // 关闭httserver
	
	static void status(struct http_req_res* req_res);
	static void playlist_get(struct http_req_res* req_res);
	static void playlist_clear(struct http_req_res* req_res);
	static void playlist_add(struct http_req_res* req_res);
	static void playlist_remove(struct http_req_res* req_res);
	static void playlist_add_item(struct http_req_res* req_res);
	static void playlist_add_file(struct http_req_res* req_res);
	static void playlist_switch(struct http_req_res* req_res);
	static void playlist_play(struct http_req_res* req_res);
	static void playControl(struct http_req_res* req_res);
	static void libSearch (struct http_req_res* req_res);
	static void albumart(struct http_req_res* req_res);

	static pfc::string8 m_versionNumber;
	static std::string m_web_folder;  //www folder name
	static std::string m_web_dir; // 网页根目录 
	static std::string m_port;    // 端口
	static bool m_Exiting;

private:

	static httplib::Server m_svr;    //http server
	static int m_playlistItemsPerPage;  //return 30 tracks per page
	static pfc::string8 m_metafields;

	static void returnSucess(httplib::Response& res, bool succ);
	static void handleHttpGet(const httplib::Request& req, httplib::Response& res);
	static void handleHttpGetArtwork(const httplib::Request& req, httplib::Response& res);

	static void generateTrackString(metadb_handle_ptr handle, pfc::string8& str);
};

#endif  // INC_HTTPSERVER_H