/*
* author£ºsyc
* website: www.cctry.com
* time£º2018-12-18
*/
#include "../pch.h"
#include "string_util.h"
#include <windows.h>
#include <functional>

string_util::string_util()
{
}

string_util::~string_util()
{
}

std::string& string_util::trim_l(std::string &str)
{
	if (str.empty()) return str;

	str.erase(0, str.find_first_not_of(" \n\r\t"));
	return str;
}

std::string& string_util::trim_r(std::string &str)
{
	if (str.empty()) return str;

	str.erase(str.find_last_not_of(" \n\r\t") + 1);
	return str;
}

std::string& string_util::trim(std::string &str)
{
	return trim_l(trim_r(str));
}

std::string& string_util::reverse(std::string &str)
{
	std::reverse(str.begin(), str.end());
	return str;
}

std::string& string_util::to_upper(std::string &str)
{
	transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}

std::string string_util::to_upper_copy(std::string &str)
{
	std::string string_str_ret = str;
	transform(string_str_ret.begin(), string_str_ret.end(), string_str_ret.begin(), ::toupper);
	return string_str_ret;
}

std::string& string_util::to_lower(std::string &str)
{
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

std::string string_util::to_lower_copy(std::string &str)
{
	std::string string_str_ret = str;
	transform(string_str_ret.begin(), string_str_ret.end(), string_str_ret.begin(), ::tolower);
	return string_str_ret;
}

std::string string_util::left(std::string &str, size_t n_count)
{
	if (str.size() <= 0 || n_count <= 0) return "";
	if (str.size() <= n_count) return str;
	return str.substr(0, n_count);
}

std::string string_util::right(std::string &str, size_t n_count)
{
	if (str.size() <= 0 || n_count <= 0) return "";
	if (str.size() <= n_count) return str;
	return str.substr(str.size() - n_count, n_count);
}

std::string string_util::mid(std::string &str, size_t i_start, size_t n_count)
{
	if (str.size() <= 0 || n_count <= 0 || i_start >= str.size()) return "";
	return str.substr(i_start, n_count);
}

std::string string_util::mid_l_r(std::string &str, std::string str_left, std::string str_right, bool b_inc_start, bool b_inc_end)
{
	if (str.size() <= 0 || str_left.size() <= 0 || str_right.size() <= 0 || str.size() < str_left.size() || str.size() < str_right.size()) return "";

	std::string str_ret;
	std::string::size_type pos_begin = str.find(str_left);
	if (pos_begin != std::string::npos)
	{
		if (!b_inc_start) pos_begin += str_left.size();

		std::string::size_type pos_end = str.find(str_right, pos_begin + 1);
		if (pos_end != std::string::npos)
		{
			if (!b_inc_end)
			{
				str_ret = str.substr(pos_begin, pos_end - pos_begin);
			}
			else{
				pos_end += str_right.size();
				str_ret = str.substr(pos_begin, pos_end - pos_begin);
			}
		}
	}

	return str_ret;
}

std::string string_util::span_including(std::string& str, std::string str_key)
{
	if (str.size() <= 0 || str_key.size() <= 0) return str;

	std::string str_ret;
	std::size_t found = str.find_first_of(str_key);
	while (found != std::string::npos)
	{
		str_ret.push_back(str[found]);
		found = str.find_first_of(str_key, found + 1);
	}

	return str_ret;
}

std::string string_util::span_excluding(std::string& str, std::string str_key)
{
	if (str.size() <= 0 || str_key.size() <= 0) return str;

	std::string str_ret;
	std::size_t found = str.find_first_not_of(str_key);
	while (found != std::string::npos)
	{
		str_ret.push_back(str[found]);
		found = str.find_first_not_of(str_key, found + 1);
	}

	return str_ret;
}

bool string_util::start_with(std::string &str, std::string str_begin)
{
	if (str.size() <= 0 || str_begin.size() <= 0) return false;
	bool startwith = str.compare(0, str_begin.size(), str_begin) == 0;
	return startwith;
}

bool string_util::end_with(std::string &str, std::string str_end)
{
	if (str.size() <= 0 || str_end.size() <= 0) return false;
	bool endwith = str.compare(str.size() - str_end.size(), str_end.size(), str_end) == 0;
	return endwith;
}

bool string_util::contain(std::string &str, std::string str_sub)
{
	if (str.size() <= 0 || str_sub.size() <= 0 || str_sub.size() > str.size()) return false;
	std::string::size_type pos = str.find(str_sub);
	if (pos == std::string::npos) return false;
	return true;
}

bool string_util::is_empty(std::string &str)
{
	return str.size() > 0 ? true : false;
}

std::string string_util::format(const std::string fmt, ...)
{
	std::string str_ret;
	int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
	va_list ap;
	while (true)
	{
		// Maximum two passes on a POSIX system...
		str_ret.resize(size);
		va_start(ap, fmt);
		int n = vsnprintf((char *)str_ret.data(), size, fmt.c_str(), ap);
		va_end(ap);
		if (n > -1 && n < size)
		{
			// Everything worked
			str_ret.resize(n);
			return str_ret;
		}

		if (n > -1)  // Needed size returned
			size = n + 1;   // For null char
		else
			size *= 2;      // Guess at a larger size (OS specific)
	}

	return str_ret;
}

std::string& string_util::append_format(std::string& str, const std::string fmt, ...)
{
	std::string str_ret;
	int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
	va_list ap;
	while (true)
	{
		// Maximum two passes on a POSIX system...
		str_ret.resize(size);
		va_start(ap, fmt);
		int n = vsnprintf((char *)str_ret.data(), size, fmt.c_str(), ap);
		va_end(ap);
		if (n > -1 && n < size)
		{
			// Everything worked
			str_ret.resize(n);
			str += str_ret;
			return str;
		}

		if (n > -1)  // Needed size returned
			size = n + 1;   // For null char
		else
			size *= 2;      // Guess at a larger size (OS specific)
	}

	str += str_ret;
	return str;
}

std::string& string_util::erase(std::string& str, char ch)
{
	if (str.size() <= 0) return str;
	str.erase(remove_if(str.begin(), str.end(), bind2nd(equal_to<char>(), ch)), str.end());
	return str;
}

bool string_util::replace(std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos) return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

void string_util::replace_all(std::string& str, const std::string& from, const std::string& to)
{
	if (from.empty()) return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

int string_util::string_i_cmp(std::string lhs, std::string rhs)
{
	return _stricmp(lhs.c_str(), rhs.c_str());
}

void string_util::split(std::string& str, std::string pattern, vector<std::string>& arrout)
{
	arrout.clear();
	size_t idx = 0, pos = 0;
	size_t size = str.size();
	for (idx = 0; idx < size; ++idx)
	{
		pos = str.find(pattern, idx);
		if (pos != std::string::npos)
		{
			std::string s = str.substr(idx, pos - idx);
			if (s.size() > 0) arrout.push_back(s);
			idx = pos + pattern.size() - 1;
		}
		else
		{
			if (idx < size)
			{
				std::string s = str.substr(idx, str.size() - idx);
				if (s.size() > 0) arrout.push_back(s);
				break;
			}
		}
	}
}

std::string string_util::join(std::vector<string>& arr_string, std::string pattern)
{
	string str_ret;
	if (arr_string.size() <= 0) return "";
	for (size_t idx = 0; idx < arr_string.size(); ++idx)
	{
		if (str_ret.size() <= 0) str_ret = arr_string[idx];
		else str_ret += pattern + arr_string[idx];
	}

	return str_ret;
}

std::wstring string_util::a2w_string(std::string str_a)
{
	return gbk2unicode(str_a);
}

std::string string_util::w2a_string(std::wstring str_w)
{
	return unicode2gbk(str_w);
}

string_t string_util::to_string(std::string str_a)
{
#ifdef _UNICODE
	std::wstring str_ret = a2w_string(str_a);
#else
	std::string str_ret = str_a;
#endif
	return str_ret;
}

string_t string_util::to_string(std::wstring str_w)
{
#ifdef _UNICODE
	std::wstring str_ret = str_w;
#else
	std::string str_ret = w2a_string(str_w);
#endif
	return str_ret;
}

std::wstring string_util::to_wstring(string_t str)
{
	std::wstring str_ret;

#ifdef _UNICODE
	str_ret = str;
#else
	str_ret = a2w_string(str);
#endif
	return str_ret;
}

std::string string_util::to_astring(string_t str)
{
	std::string str_ret;

#ifdef _UNICODE
	str_ret = w2a_string(str);
#else
	str_ret = str;
#endif
	return str_ret;
}

#if defined(CODE_CONVERT_USE_CPP11)
std::wstring string_util::gbk2unicode(std::string str_gbk)
{
	if (str_gbk.size() <= 0) return std::wstring(L"");

#if 0
	try
	{
		wstring_convert<codecvt<wchar_t, char, mbstate_t>> gbk_cvt(new codecvt<wchar_t, char, mbstate_t>("chs"));
		return gbk_cvt.from_bytes(str_gbk);
	}
	catch (exception& e)
	{
		cout << "string_util::gbk2unicode:" << e.what() << endl;
	}

	return std::wstring(L"");
#endif

	std::locale sys_locale("");

	const char* data_from = str_gbk.c_str();
	const char* data_from_end = str_gbk.c_str() + str_gbk.size();
	const char* data_from_next = 0;

	wchar_t* data_to = new wchar_t[str_gbk.size() + 1];
	wchar_t* data_to_end = data_to + str_gbk.size() + 1;
	wchar_t* data_to_next = 0;

	wmemset(data_to, 0, str_gbk.size() + 1);

	mbstate_t in_state = 0;
	typedef std::codecvt<wchar_t, char, mbstate_t> convert_facet;
	auto result = std::use_facet<convert_facet>(sys_locale).in(
		in_state, data_from, data_from_end, data_from_next,
		data_to, data_to_end, data_to_next);
	if (result == convert_facet::ok)
	{
		std::wstring dst = data_to;
		delete[] data_to;
		return dst;
	}
	else
	{
		cout << "string_util::gbk2unicode: convert error." << endl;
		delete[] data_to;
		return std::wstring(L"");
	}
}

std::string string_util::unicode2gbk(std::wstring str_unicode)
{
	if (str_unicode.size() <= 0) return std::string("");

#if 0
	try
	{
		wstring_convert<codecvt<wchar_t, char, mbstate_t>> gbk_cvt(new codecvt<wchar_t, char, mbstate_t>("chs"));
		return gbk_cvt.to_bytes(str_unicode);
	}
	catch (exception& e)
	{
		cout << "string_util::unicode2gbk:" << e.what() << endl;
	}

	return std::string("");
#endif

	std::locale sys_locale("");

	const wchar_t* data_from = str_unicode.c_str();
	const wchar_t* data_from_end = str_unicode.c_str() + str_unicode.size();
	const wchar_t* data_from_next = 0;

	int wchar_size = 4;
	char* data_to = new char[(str_unicode.size() + 1) * wchar_size];
	char* data_to_end = data_to + (str_unicode.size() + 1) * wchar_size;
	char* data_to_next = 0;

	memset(data_to, 0, (str_unicode.size() + 1) * wchar_size);

	typedef std::codecvt<wchar_t, char, mbstate_t> convert_facet;
	mbstate_t out_state = 0;
	auto result = std::use_facet<convert_facet>(sys_locale).out(
		out_state, data_from, data_from_end, data_from_next,
		data_to, data_to_end, data_to_next);
	if (result == convert_facet::ok)
	{
		std::string dst = data_to;
		delete[] data_to;
		return dst;
	}
	else
	{
		cout << "string_util::unicode2gbk: convert error." << endl;
		delete[] data_to;
		return std::string("");
	}
}

std::string string_util::gbk2utf8(std::string str_gbk)
{
	if (str_gbk.size() <= 0) return std::string("");

#if 0
	try
	{
		wstring_convert<codecvt_utf8<wchar_t>> utf8_cvt; //utf8=>unicode
		wstring_convert<codecvt<wchar_t, char, mbstate_t>> gbk_cvt(new codecvt<wchar_t, char, mbstate_t>("chs")); //unicode=>gbk
		wstring t = gbk_cvt.from_bytes(str_gbk);
		return utf8_cvt.to_bytes(t);
	}
	catch (exception& e)
	{
		cout << "string_util::gbk2utf8:" << e.what() << endl;
	}

	return std::string("");
#endif

	std::wstring str_w = gbk2unicode(str_gbk);
	return unicode2utf8(str_w);
}

std::string string_util::utf82gbk(std::string str_utf8)
{
	if (str_utf8.size() <= 0) return std::string("");

#if 0
	try
	{
		wstring_convert<codecvt_utf8<wchar_t>> utf8_cvt; //utf8=>unicode
		wstring_convert<codecvt<wchar_t, char, mbstate_t>> gbk_cvt(new codecvt<wchar_t, char, mbstate_t>("chs")); //unicode=>gbk
		wstring t = utf8_cvt.from_bytes(str_utf8);
		return gbk_cvt.to_bytes(t);
	}
	catch (exception& e)
	{
		cout << "string_util::utf82gbk:" << e.what() << endl;
	}

	return std::string("");
#endif

	std::wstring str_w = utf82unicode(str_utf8);
	return unicode2gbk(str_w);
}

std::string string_util::unicode2utf8(std::wstring str_unicode)
{
	if (str_unicode.size() <= 0) return std::string("");

	try
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_cvt;
		return utf8_cvt.to_bytes(str_unicode);
	}
	catch (exception& e)
	{
		cout << "string_util::unicode2utf8:" << e.what() << endl;
	}

	return std::string("");
}

std::wstring string_util::utf82unicode(std::string str_utf8)
{
	if (str_utf8.size() <= 0) return std::wstring(L"");

	try
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_cvt;
		return utf8_cvt.from_bytes(str_utf8);
	}
	catch (exception& e)
	{
		cout << "string_util::utf82unicode:" << e.what() << endl;
	}

	return std::wstring(L"");
}

#elif defined(CODE_CONVERT_USE_WIN32)
std::string string_util::gbk2utf8(std::string str_gbk)
{
	if (str_gbk.size() <= 0) return std::string("");

	//gbk=>unicode
	int nLen = MultiByteToWideChar(CP_ACP, 0, str_gbk.c_str(), str_gbk.size(), NULL, 0);
	if (nLen <= 0) std::string("");

	wchar_t* lpsz_w = nullptr;
	try
	{
		lpsz_w = new wchar_t[nLen + 2];
		memset(lpsz_w, 0, (nLen + 2)*sizeof(wchar_t));
	}
	catch (bad_alloc& e)
	{
		cout << "gbk2utf8 : " << e.what() << endl;
		return std::string("");
	}

	int nRtn = MultiByteToWideChar(CP_ACP, 0, str_gbk.c_str(), str_gbk.size(), lpsz_w, nLen);
	if (nRtn != nLen)
	{
		delete[] lpsz_w;
		return std::string("");
	}
	
	//unicode=>utf8
	int utf8_len = WideCharToMultiByte(CP_UTF8, 0, lpsz_w, nLen, NULL, 0, NULL, NULL);
	if (utf8_len <= 0) return std::string("");

	char* lpsz_m = nullptr;
	try
	{
		lpsz_m = new char[utf8_len + 2];
		memset(lpsz_m, 0, (utf8_len + 2)*sizeof(char));
	}
	catch (bad_alloc& e)
	{
		cout << "gbk2utf8 : " << e.what() << endl;
		return std::string("");
	}

	nRtn = WideCharToMultiByte(CP_UTF8, 0, lpsz_w, nLen, lpsz_m, utf8_len, NULL, NULL);
	delete[] lpsz_w;

	if (nRtn != utf8_len)
	{
		delete[] lpsz_m;
		return std::string("");
	}

	std::string str_utf8;
	str_utf8 = lpsz_m;
	delete[] lpsz_m;

	return str_utf8;
}

std::string string_util::utf82gbk(std::string str_utf8)
{
	if (str_utf8.size() <= 0) return std::string("");

	//utf8=>unicode
	int nLen = MultiByteToWideChar(CP_UTF8, 0, str_utf8.c_str(), str_utf8.size(), NULL, 0);
	if (nLen <= 0) return std::string("");

	wchar_t* lpsz_w = nullptr;
	try
	{
		lpsz_w = new wchar_t[nLen + 2];
		memset(lpsz_w, 0, (nLen + 2)*sizeof(wchar_t));
	}
	catch (bad_alloc &e)
	{
		cout << "utf82gbk : " << e.what() << endl;
		return std::string("");
	}

	int nRtn = MultiByteToWideChar(CP_UTF8, 0, str_utf8.c_str(), str_utf8.size(), lpsz_w, nLen);
	if (nRtn != nLen)
	{
		delete[] lpsz_w;
		return std::string("");
	}

	//unicode=>gbk
	int gbk_len = WideCharToMultiByte(CP_ACP, 0, lpsz_w, nLen, NULL, 0, NULL, NULL);
	if (gbk_len <= 0) return std::string("");

	char* lpsz_m = nullptr;
	try
	{
		lpsz_m = new char[gbk_len + 2];
		memset(lpsz_m, 0, (gbk_len + 2)*sizeof(char));
	}
	catch (bad_alloc& e)
	{
		cout << "utf82gbk : " << e.what() << endl;
		return std::string("");
	}

	nRtn = WideCharToMultiByte(CP_ACP, 0, lpsz_w, nLen, lpsz_m, gbk_len, NULL, NULL);
	delete[] lpsz_w;

	if (nRtn != gbk_len)
	{
		delete[] lpsz_m;
		return std::string("");
	}

	std::string str_gbk;
	str_gbk = lpsz_m;
	delete[] lpsz_m;
	return str_gbk;
}

std::wstring string_util::gbk2unicode(std::string str_gbk)
{
	if (str_gbk.size() <= 0) return std::wstring(L"");

	int u_len = MultiByteToWideChar(CP_ACP, 0, str_gbk.c_str(), str_gbk.size(), NULL, 0);
	if (u_len <= 0) return std::wstring(L"");

	wchar_t* lpsz_w = nullptr;
	try
	{
		lpsz_w = new wchar_t[u_len + 2];
		memset(lpsz_w, 0, (u_len + 2)*sizeof(wchar_t));
	}
	catch (bad_alloc& e)
	{
		cout << "gbk2unicode : " << e.what() << endl;
		return std::wstring(L"");
	}

	int nRtn = MultiByteToWideChar(CP_ACP, 0, str_gbk.c_str(), str_gbk.size(), lpsz_w, u_len);
	if (nRtn != u_len)
	{
		delete[] lpsz_w;
		return std::wstring(L"");
	}

	std::wstring str_unicode;
	str_unicode = lpsz_w;
	delete[] lpsz_w;
	return str_unicode;
}

std::string string_util::unicode2gbk(std::wstring str_unicode)
{
	if (str_unicode.size() <= 0) return std::string("");
	int gbk_len = WideCharToMultiByte(CP_ACP, 0, str_unicode.c_str(), str_unicode.size(), NULL, 0, NULL, NULL);
	if (gbk_len <= 0) return std::string("");

	char* lpsz_m = nullptr;
	try
	{
		lpsz_m = new char[gbk_len + 2];
		memset(lpsz_m, 0, (gbk_len + 2)*sizeof(char));
	}
	catch (bad_alloc& e)
	{
		cout << "unicode2gbk : " << e.what() << endl;
		return std::string("");
	}

	int nRtn = WideCharToMultiByte(CP_ACP, 0, str_unicode.c_str(), str_unicode.size(), lpsz_m, gbk_len, NULL, NULL);
	if (nRtn != gbk_len)
	{
		delete[] lpsz_m;
		return std::string("");
	}

	std::string str_gbk;
	str_gbk = lpsz_m;
	delete[] lpsz_m;
	return str_gbk;
}

std::wstring string_util::utf82unicode(std::string str_utf8)
{
	if (str_utf8.size() <= 0) return std::wstring(L"");
	int u_len = MultiByteToWideChar(CP_UTF8, 0, str_utf8.c_str(), str_utf8.size(), NULL, 0);
	if (u_len <= 0) return std::wstring(L"");

	wchar_t* lpsz_w = nullptr;
	try
	{
		lpsz_w = new wchar_t[u_len + 2];
		memset(lpsz_w, 0, (u_len + 2)*sizeof(wchar_t));
	}
	catch (bad_alloc& e)
	{
		cout << "utf82unicode : " << e.what() << endl;
		return std::wstring(L"");
	}

	int nRtn = MultiByteToWideChar(CP_UTF8, 0, str_utf8.c_str(), str_utf8.size(), lpsz_w, u_len);
	if (nRtn != u_len)
	{
		delete[] lpsz_w;
		return std::wstring(L"");
	}

	std::wstring str_unicode;
	str_unicode = lpsz_w;
	delete[] lpsz_w;
	return str_unicode;
}

std::string string_util::unicode2utf8(std::wstring str_unicode)
{
	if (str_unicode.size() <= 0) return std::string("");
	int utf8_len = WideCharToMultiByte(CP_UTF8, 0, str_unicode.c_str(), str_unicode.size(), NULL, 0, NULL, NULL);
	if (utf8_len <= 0) return std::string("");

	char* lpsz_m = nullptr;
	try
	{
		lpsz_m = new char[utf8_len + 2];
		memset(lpsz_m, 0, (utf8_len + 2)*sizeof(char));
	}
	catch (bad_alloc& e)
	{
		cout << "unicode2utf8 : " << e.what() << endl;
		return std::string("");
	}

	int nRtn = WideCharToMultiByte(CP_UTF8, 0, str_unicode.c_str(), str_unicode.size(), lpsz_m, utf8_len, NULL, NULL);
	if (nRtn != utf8_len)
	{
		delete[] lpsz_m;
		return std::string("");
	}

	std::string str_utf8;
	str_utf8 = lpsz_m;
	delete[] lpsz_m;
	return str_utf8;
}

#elif defined(CODE_CONVERT_USE_ICONV)
#include "iconv_inc.h"
std::wstring string_util::gbk2unicode(std::string str_gbk)
{
	int nSize = str_gbk.size() + 2;
	wchar_t *pBuf = new wchar_t[nSize];
	if (!pBuf) return std::wstring(L"");
	wmemset(pBuf, 0, nSize);

	int ret = code_convert("UCS-2LE//TRANSLIT//IGNORE", "GBK", str_gbk.c_str(), str_gbk.size(), (char*)pBuf, nSize * sizeof(wchar_t));
	if (ret <= 0)
	{
		delete[] pBuf;
		return std::wstring(L"");
	}

	std::wstring str_unicode;
	str_unicode = pBuf;
	delete[] pBuf;
	return str_unicode;
}

std::string string_util::unicode2gbk(std::wstring str_unicode)
{
	int nSize = str_unicode.size() * sizeof(wchar_t) + 2;
	char *pBuf = new char[nSize];
	if (!pBuf) return std::string("");
	memset(pBuf, 0, nSize);

	int ret = code_convert("GBK//TRANSLIT//IGNORE", "UCS-2LE", (char*)str_unicode.c_str(), str_unicode.size() * sizeof(wchar_t), pBuf, nSize);
	if (ret <= 0)
	{
		delete[] pBuf;
		return string("");
	}

	std::string str_gbk;
	str_gbk = pBuf;
	delete[] pBuf;
	return str_gbk;
}

std::string string_util::gbk2utf8(std::string str_gbk)
{
	int nSize = str_gbk.size() * 2 + 2;
	char *pBuf = new char[nSize];
	if (!pBuf) return std::string("");
	memset(pBuf, 0, nSize);

	int ret = code_convert("UTF-8//TRANSLIT//IGNORE", "GBK", str_gbk.c_str(), str_gbk.size(), pBuf, nSize);
	if (ret <= 0)
	{
		delete[] pBuf;
		return std::string("");
	}

	std::string str_utf8;
	str_utf8 = pBuf;
	delete[] pBuf;
	return str_utf8;
}

std::string string_util::utf82gbk(std::string str_utf8)
{
	int nSize = str_utf8.size() * 2 + 2;
	char *pBuf = new char[nSize];
	if (!pBuf) return std::string("");
	memset(pBuf, 0, nSize);

	int ret = code_convert("GBK//TRANSLIT//IGNORE", "UTF-8", str_utf8.c_str(), str_utf8.size(), pBuf, nSize);
	if (ret <= 0)
	{
		delete[] pBuf;
		return std::string("");
	}

	std::string str_gbk;
	str_gbk = pBuf;
	delete[] pBuf;
	return str_gbk;
}

std::string string_util::unicode2utf8(std::wstring str_unicode)
{
	int nSize = str_unicode.size() * sizeof(wchar_t) + 2;
	char *pBuf = new char[nSize];
	if (!pBuf) return std::string("");
	memset(pBuf, 0, nSize);

	int ret = code_convert("UTF-8//TRANSLIT//IGNORE", "UCS-2LE", (char*)str_unicode.c_str(), str_unicode.size() * sizeof(wchar_t), pBuf, nSize);
	if (ret <= 0)
	{
		delete[] pBuf;
		return std::string("");
	}

	std::string str_utf8;
	str_utf8 = pBuf;
	delete[] pBuf;
	return str_utf8;
}

std::wstring string_util::utf82unicode(std::string str_utf8)
{
	int nSize = str_utf8.size() + 2;
	wchar_t *pBuf = new wchar_t[nSize];
	if (!pBuf) return std::wstring(L"");
	wmemset(pBuf, 0, nSize);

	int ret = code_convert("UCS-2LE//TRANSLIT//IGNORE", "UTF-8", str_utf8.c_str(), str_utf8.size(), (char*)pBuf, nSize * sizeof(wchar_t));
	if (ret <= 0)
	{
		delete[] pBuf;
		return std::wstring(L"");
	}

	std::wstring str_unicode;
	str_unicode = pBuf;
	delete[] pBuf;
	return str_unicode;
}
#else
#error string_util required must define CODE_CONVERT_USE_CPP11 or CODE_CONVERT_USE_WIN32 or CODE_CONVERT_USE_ICONV.
#endif