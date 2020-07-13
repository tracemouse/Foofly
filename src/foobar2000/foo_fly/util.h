#ifndef INC_MOBILECONTROLUTIL_H
#define INC_MOBILECONTROLUTIL_H


#include "../../pfc/pfc.h"

using namespace std;
 
class util
{
public:
	util() {}
	~util() {}

	static pfc::string8 getLocalHostIP(); //get local ip address
	static void WriteLog(std::string);  // writeLog
	
	static string& replaceall(string& str, const string& old_value, const string& new_value);
	static bool convertStrToI(char const* str, t_size& ival);
	static bool convertStrToF(char const* str, double& ival);
	static pfc::string8 trim(pfc::string8& str);
	static bool convertFromWide(pfc::string8 const& incoming, pfc::string8& output);
	
	static bool readImageFile(char* img, unsigned char* buf);
	static bool writeImageFile(char* filename, unsigned char* buf, int size);

	static string getFolderPath(const string path);
	static string getFolder(const string path);

	static std::string base64_encode(const std::string& in);
	static std::string base64_decode(const std::string& in);
	
	static std::string getExeFolder();

	static void convertUnCharToStr(char* str, unsigned char* UnChar, int ucLen);

	static char* AnsiToUtf8(const char* input);
	static char* Utf8ToAnsi(const char* input);

private:

	static std::string logfilename; //log filename
	static bool isLog;  // true = write log
};

#endif  // INC_MOBILECONTROLUTIL_H