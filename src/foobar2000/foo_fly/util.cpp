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

#include "util.h"
#include <vector>
#include <atlstr.h>
#include <locale.h>  
#include <locale>  


using   namespace   std;

bool util::isLog = false;
std::string util::logfilename = "./foo_mobilecontrol_log.txt";
//std::string util::logfilename = "R:/foo_mobilecontrol_log.txt";

string& util::replaceall(string& str, const string& old_value, const string& new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
		if ((pos = str.find(old_value, pos)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else   break;
	}
	return   str;
}

bool util::convertStrToI(char const* str, t_size& ival)
{
	char* ep;
	long lval;

	errno = 0;
	lval = strtol(str, &ep, 10);

	// not a number
	if (str[0] == '\0' || *ep != '\0')
	{
		return false;
	}

	// out of range
	if ((errno == ERANGE && (lval == LONG_MAX || lval == LONG_MIN)) || (lval > INT_MAX || lval < INT_MIN))
	{
		return false;
	}

	ival = lval;
	return true;
}

bool util::convertStrToF(char const* str, double& ival)
{
	char* ep = '\0';

	errno = 0;
	ival = strtod(str, &ep);

	// not a number
	if (str[0] == '\0' || *ep != '\0')
	{
		return false;
	}

	// out of range
	if ((errno == ERANGE && (ival == HUGE_VAL || ival == -HUGE_VAL)))
	{
		return false;
	}

	return true;
}

pfc::string8 util::trim(pfc::string8& str)
{
	pfc::string8 out = str;

	size_t i = 0;

	while (i < out.length() && out[i] == ' ')
		++i;

	if (i == out.length())
		return pfc::string8("");

	out.remove_chars(0, i);

	i = out.length() - 1;

	while (i > 0 && out[i] == ' ')
		--i;

	if (i < out.length())
		out.truncate(i + 1);

	return out;
}

bool util::convertFromWide(pfc::string8 const& incoming, pfc::string8& output)
{
	char* cleanData = NULL;

	cleanData = (char*)calloc(incoming.length() + 1, sizeof(char));

	// unable to allocate memory
	if (cleanData == NULL)
	{
		return false;
	}

	//out should be at least strlen(src)+1 long
	pfc::convert_to_lower_ascii(incoming, strlen(incoming.get_ptr()), cleanData);

	output = cleanData;

	return true;
}

void util::WriteLog(std::string szLog)
{
	if (!isLog)	return;

	szLog = util::Utf8ToAnsi(szLog.c_str());

	DWORD tid = GetCurrentThreadId();
	SYSTEMTIME st;
	GetLocalTime(&st);
	FILE* fp;
	fp = fopen(logfilename.c_str(), "at");
	//_setmode(_fileno(fp), _O_U8TEXT);
	fprintf(fp, "%d: %d:%d:%d:%d - ", tid, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	szLog += "\r\n";
	fprintf(fp, szLog.c_str());
	fclose(fp);
	OutputDebugStringA(szLog.c_str());
}

pfc::string8 util::getLocalHostIP()
{
	char host[256];
	pfc::string8 msg = "";
	pfc::string8 error_msg = "";

	WSADATA            wsd;
	// start up winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsd) == 0)
	{
		if (wsd.wVersion == MAKEWORD(2, 2))
		{

			if (gethostname(host, sizeof(host)) == SOCKET_ERROR)
			{
				error_msg << "no host info - error " << WSAGetLastError() << " when getting host";
			}
			else
			{
				//msg << host;

				struct hostent* phosts = gethostbyname(host);
				if (phosts == 0)
				{
					error_msg << " Bad host lookup";
				}
				else
				{

					for (int i = 0; phosts->h_addr_list[i] != 0; ++i)
					{
						struct in_addr addr;
						memcpy(&addr, phosts->h_addr_list[i], sizeof(struct in_addr));
						msg << inet_ntoa(addr) << "/";
					}
				}

			}

			WSACleanup();
		}
		else
		{
			error_msg = "no host info - error opening network";
		}
	}
	else
	{
		error_msg = "no host info - error opening network";
	}

	if (error_msg.get_length() > 0)
	{
		return error_msg;
	}
	else
	{
		//if (msg.ends_with('/')) { msg.truncate(msg.length()-1); }
		msg.truncate_last_char();
		return msg;
	}
}

bool util::readImageFile(char* img, unsigned char* buf)
{
	FILE* file = fopen(img, "rb");
	if (file != NULL)
	{
		struct stat finfo;
		stat(img, &finfo);
		int fileSize = finfo.st_size;

		//int fileLen;
		//fseek(file, 0, SEEK_END);
		//fileLen = ftell(file);
		//fseek(file, 0, SEEK_SET);

		buf = (unsigned char*)malloc(fileSize + 1);
		fread(buf, fileSize, 1, file);
		unsigned char x;
		for (int i = 0; i < fileSize; i++)
		{
			x = *buf++;
		}
		fclose(file);
		// convert to base64
	}
	return true;
}


bool util::writeImageFile(char* filename, unsigned char* buf, int size)
{
	FILE* fp = fopen(filename, "wb");
	if (fp != NULL)
	{
		int outsize = fwrite((void*)buf, sizeof(unsigned char), size, fp);
		fclose(fp);
		if (outsize == size)
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}

string util::getFolder(const string path) {
	std::string dir = getFolderPath(path);
	const size_t last_slash_idx = dir.find_last_of("\\/");
	std::string folder = (std::string::npos != last_slash_idx) ? path.substr(last_slash_idx+1, dir.length()-last_slash_idx-1) : "";
	return folder;
}

string util::getFolderPath(const string path) {
	const size_t last_slash_idx = path.find_last_of("\\/");
	std::string dir = (std::string::npos != last_slash_idx) ? path.substr(0, last_slash_idx) : "";
	return dir;
}

std::string util::base64_encode(const std::string& in) {
	static const auto lookup =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	std::string out;

	int val = 0, valb = -6;
	for (auto c : in) {
		val = (val << 8) + c;
		valb += 8;
		while (valb >= 0) {
			out.push_back(lookup[(val >> valb) & 0x3F]);
			valb -= 6;
		}
	}
	if (valb > -6) out.push_back(lookup[((val << 8) >> (valb + 8)) & 0x3F]);
	while (out.size() % 4) out.push_back('=');
	return out;
}

std::string util::base64_decode(const std::string& in) {
	static const auto lookup =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	std::string out;

	std::vector<int> T(256, -1);
	for (int i = 0; i < 64; i++) T[lookup[i]] = i;

	int val = 0, valb = -8;
	for (auto c : in) {
		if (T[c] == -1) break;
		val = (val << 6) + T[c];
		valb += 6;
		if (valb >= 0) {
			out.push_back(char((val >> valb) & 0xFF));
			valb -= 8;
		}
	}
	return out;
}

void util::convertUnCharToStr(char* str, unsigned char* UnChar, int ucLen)
{
	int i = 0;
	for (size_t i = 0; i < ucLen; i++) {
		UnChar[i] = (i + 1) & 255;
	}
	str = (char*)UnChar;
}

std::string util::getExeFolder() {
	TCHAR _szPath[MAX_PATH + 1] = { 0 };
	GetModuleFileName(NULL, _szPath, MAX_PATH);

	(_tcsrchr(_szPath, _T('\\')))[1] = 0;//删除文件名，只获得路径 字串
	CString strPath;
	for (int n = 0; _szPath[n]; n++)
	{
		if (_szPath[n] != _T('\\'))
		{
			strPath += _szPath[n];
		}
		else
		{
			strPath = strPath + "/";
		}
	}
	return CT2A(strPath.GetString());
}

char* util::AnsiToUtf8(const char* input)
{
	int len = MultiByteToWideChar(CP_ACP, 0, input, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, input, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

char* util::Utf8ToAnsi(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}