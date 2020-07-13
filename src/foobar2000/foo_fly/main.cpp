/*
 * foo_fly - a plugin to enable remote control by mobile phone with fashion UI
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

#include "HttpServer.h"
#include "util.h"
#include "../SDK/foobar2000.h"
#include "../ATLHelpers/ATLHelpers.h"
#include "resource.h"


 // variable to store server port etc
static GUID guid1 = { 0xdfc08ea6, 0xb17f, 0x494d, 0xbb, 0xb3, 0x6c, 0x8f, 0xa1, 0x27, 0xa7, 0xcf };


t_size default_cfg_server_port = 8168;


static cfg_int cfg_server_port(guid1, default_cfg_server_port);


class prefs_config {
public:
	t_size			server_port;

	prefs_config() { reset(); }
	prefs_config(const prefs_config &cfg) { copy(cfg); }
	prefs_config &operator = (const prefs_config &cfg) { copy(cfg); return *this; }
	bool operator == (const prefs_config &c);
	void copy(const prefs_config &cfg);
	void reset();
};

void prefs_config::reset()
{
	server_port = default_cfg_server_port;
}

bool prefs_config::operator == (const prefs_config &c)
{
	bool isDiff = (c.server_port == server_port);

	return isDiff;
}

void prefs_config::copy(const prefs_config &cfg)
{
	server_port = cfg.server_port;
}

prefs_config cfg;

// this will handle some events we are interested in
class play_callback_httpserver : public play_callback_static
{
public:
    virtual void FB2KAPI on_playback_starting(play_control::t_track_command p_command,bool p_paused) { };

    virtual void FB2KAPI on_playback_new_track(metadb_handle_ptr p_track) { };

    virtual void FB2KAPI on_playback_stop(play_control::t_stop_reason p_reason) { };

    virtual void FB2KAPI on_playback_seek(double p_time) { };

    virtual void FB2KAPI on_playback_pause(bool p_state) { };

    virtual void FB2KAPI on_playback_edited(metadb_handle_ptr p_track) { };

    virtual void FB2KAPI on_playback_dynamic_info(const file_info & p_info) { };

    virtual void FB2KAPI on_playback_dynamic_info_track(const file_info & p_info) { };

    virtual void FB2KAPI on_playback_time(double p_time) { };

    // handle volume changes
    virtual void FB2KAPI on_volume_change(float p_new_val) { };

    // events we are interested in
    unsigned get_flags()
    {
        return flag_on_playback_new_track | flag_on_playback_stop |
               flag_on_playback_pause | flag_on_volume_change |
               flag_on_playback_seek;
    };

   FB2K_MAKE_SERVICE_INTERFACE(play_callback_httpserver, play_callback_static);
};

DECLARE_CLASS_GUID(play_callback_httpserver, 0xff3eb8d7, 0x97dd, 0x48b3, 0x80, 0xae, 0x15, 0x5b, 0x00, 0x99, 0x0c, 0x6d);

// used on initialization of foobar to initialize plugin
// we create the server thread with this and kill it on close
class initquit_httpserver : public initquit
{

private:
    static HANDLE h_thread;
    static HANDLE endEvent;

    static DWORD WINAPI g_threadfunc(void* endEvent)
    {
		HttpServer::Start(static_cast<HANDLE>(endEvent));
        return 0;
    };

    inline static HANDLE create_thread(HANDLE event)
    {
        DWORD id;

        assert(core_api::is_main_thread());

        return h_thread = CreateThread(0, 0, g_threadfunc, event, 0, &id);
    };

    static void stop_thread()
    {
        assert(core_api::is_main_thread());

        if (h_thread)
        {
            WaitForSingleObject(h_thread, INFINITE);
            CloseHandle(h_thread);
            CloseHandle(endEvent);
            h_thread = 0;
            endEvent = 0;
        }
    };

public:

    static void do_init()
    {
		util::WriteLog("Plugin start");

		HttpServer::m_Exiting = false;

        // create end event
        endEvent =  CreateEvent(NULL, TRUE, FALSE, NULL);

		//init http server
		HttpServer::Init(std::to_string(cfg_server_port));

        // create our server! this will spawn additional threads
        create_thread(endEvent);
    };

    static void do_quit()
    {
		HttpServer::m_Exiting = true;

		HttpServer::Close();  //close http server

        // create event to stop the init
        SetEvent(endEvent);
        stop_thread();

    };
    virtual void on_init()
    {
         do_init();
    };

    virtual void on_quit()
    {
        do_quit();
    };

    FB2K_MAKE_SERVICE_INTERFACE(initquit_httpserver, initquit);
};

DECLARE_CLASS_GUID(initquit_httpserver,0xf888b7e9, 0xf120, 0x41de, 0x84, 0xd2, 0xeb, 0x40, 0xcb, 0xe3, 0xd5, 0xcb);

// set these to null initially
HANDLE initquit_httpserver::endEvent = NULL;
HANDLE initquit_httpserver::h_thread = NULL;

// class for configuration form config, updates etc
class CMyPreferences : public CDialogImpl<CMyPreferences>, public preferences_page_instance
{
public:
	//Constructor - invoked by preferences_page_impl helpers - don't do Create() in here, preferences_page_impl does this for us
	CMyPreferences(preferences_page_callback::ptr callback) : m_callback(callback) {}

	//Note that we don't bother doing anything regarding destruction of our class.
	//The host ensures that our dialog is destroyed first, then the last reference to our preferences_page_instance object is released, causing our object to be deleted.

	//dialog resource ID
	enum { IDD = IDD_CS_CONFIGVIEW };
	// preferences_page_instance methods (not all of them - get_wnd() is supplied by preferences_page_impl helpers)
	t_uint32 get_state();
	void apply();
	void reset();

	//WTL message map
	BEGIN_MSG_MAP(CMyPreferences)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_HANDLER_EX(IDC_SERVERPORT, EN_CHANGE, OnEditChange)
		COMMAND_HANDLER_EX(IDC_BUTTON_TEST, BN_CLICKED, OnBtnClick)
	END_MSG_MAP()

private:
	BOOL OnInitDialog(CWindow, LPARAM);
	void OnEditChange(UINT, int, CWindow);
	void OnBtnClick(UINT, int, CWindow);
	bool HasChanged();
	void OnChanged();
	void updateDialog();
	
	const preferences_page_callback::ptr m_callback;
	HWND hwnd = NULL;
	prefs_config cfg_new;
	UINT codePage = GetACP();
};

BOOL CMyPreferences::OnInitDialog(CWindow, LPARAM) {

	hwnd = get_wnd();

	cfg_new.server_port = cfg_server_port;
	cfg.copy(cfg_new);

	updateDialog();

	return FALSE;
}

void CMyPreferences::OnEditChange(UINT, int nID, CWindow) 
{
	int i;

	switch (nID)
	{
	case IDC_SERVERPORT:
		cfg_new.server_port = GetDlgItemInt(IDC_SERVERPORT, NULL, FALSE);
		break;
	default: break;

	}

	OnChanged();
}

void CMyPreferences::OnBtnClick(UINT, int nID, CWindow)
{
	pfc::string8 localIps;
	localIps << util::getLocalHostIP();
	std::string ips = localIps.get_ptr();
	std::regex ws_re("\\/");
	std::vector<std::string> v(std::sregex_token_iterator(ips.begin(), ips.end(), ws_re, -1),std::sregex_token_iterator());
	for (auto&& ip : v) {
		std::string url = "\"http://" + ip + ":" + HttpServer::m_port + "\"";
		std::wstring wstr(url.begin(), url.end());
		_TCHAR* Tstr = (_TCHAR*)(&wstr[0]);
		ShellExecute(NULL, _T("open"), _T("explorer.exe"), Tstr, NULL, SW_SHOW);
	}
}

t_uint32 CMyPreferences::get_state() {
	t_uint32 state = preferences_state::resettable;
	if (HasChanged()) state |= preferences_state::changed;
	return state;
}

void CMyPreferences::updateDialog()
{
	//set font style
	CFont m_font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));   // Clear out structure.
	lf.lfHeight = 20;                  // Request a 20-pixel-high font
	//wcscpy(lf.lfFaceName, TEXT("YaHei"));    
	m_font.CreateFontIndirect(&lf);    // Create the font.
	GetDlgItem(IDC_LABEL_TITLE).SetFont(m_font);

	pfc::string8 version = "Version : ";

	if (codePage == 936) {//for GBK(chinese)
		version = u8"版本 : ";
		uSetDlgItemText(hwnd, IDC_LABEL_TITLE, u8"Foofly Http服务器设置");
		uSetDlgItemText(hwnd, IDC_LABEL_IP, u8"IP地址：");
		uSetDlgItemText(hwnd, IDC_LABEL_PORT, u8"监听端口：");
		uSetDlgItemText(hwnd, IDC_LABEL_WWW, u8"WWW根目录：");
		TCHAR test[] = TEXT("测试");
		GetDlgItem(IDC_BUTTON_TEST).SetWindowTextW(test);
	}

	version << HttpServer::m_versionNumber;
	uSetDlgItemText(hwnd, IDC_STATIC_VERSION, version);

	SetDlgItemInt(IDC_SERVERPORT, cfg_new.server_port, FALSE);

	pfc::string8 localIps;

	localIps << util::getLocalHostIP();
	uSetDlgItemText(hwnd, IDC_HOST_IP, localIps);

	std::string www = util::getExeFolder() + HttpServer::m_web_folder;
	pfc::string8 www8 = util::AnsiToUtf8(www.c_str());
	uSetDlgItemText(hwnd, IDC_WWW, www8);
}

void CMyPreferences::reset() {
	cfg_new.reset();

	updateDialog();

	OnChanged();

}

void CMyPreferences::apply()
{
	// Update new original configuration
	cfg.copy(cfg_new);

	initquit_httpserver::do_quit();

	cfg_server_port = cfg_new.server_port;

	// restart server
	initquit_httpserver::do_init();

	updateDialog();
	OnChanged(); //our dialog content has not changed but the flags have - our currently shown values now match the settings so the apply button can be disabled
}

bool CMyPreferences::HasChanged()
{
	if (!(cfg == cfg_new))
		return true;
	else
		return false;
}

void CMyPreferences::OnChanged() {
	//tell the host that our state has changed to enable/disable the apply button appropriately.
	m_callback->on_state_changed();

}

class preferences_page_myimpl : public preferences_page_impl<CMyPreferences> {
	// preferences_page_impl<> helper deals with instantiation of our dialog; inherits from preferences_page_v3.
public:
	const char * get_name() { 
		auto codePage = GetACP();
		if (codePage == 936) {//for GBK(chinese)
			return u8"FooFly手机遥控";
		}
		else {
			return "Foo Fly";
		}
	}
	GUID get_guid() {
	// This is our GUID.
	static const GUID guid = { 0xad5dada7, 0x9a45, 0x4c6c, 0xbc, 0x24, 0xf2, 0x25, 0x7f, 0x8f, 0x5a, 0xfc };
    return guid;
	}
	GUID get_parent_guid() { return guid_tools; }

};

static preferences_page_factory_t<preferences_page_myimpl> g_preferences_page_myimpl_factory;

static initquit_factory_t<initquit_httpserver> foo;

static play_callback_static_factory_t<play_callback_httpserver> foo3;

DECLARE_COMPONENT_VERSION("Foo Fly", HttpServer::m_versionNumber,
"foo_fly - a plugin to enable remote contorl by mobile phone with fasion UI.\n"
);
						  

