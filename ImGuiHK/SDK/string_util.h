/*
* author：syc
* website: www.cctry.com
* time：2018-12-18
*/
#pragma once
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
//#include <xfunctional>
#include <iostream>
#include <xlocbuf>
#include <codecvt>
using namespace std;

#ifdef _UNICODE
#define string_t wstring
#else
#define string_t string
#endif

//#define CODE_CONVERT_USE_CPP11
#define CODE_CONVERT_USE_WIN32
//#define CODE_CONVERT_USE_ICONV
class string_util
{
public:
	string_util();
	~string_util();

public:
	// 去掉字符串左侧的空白字符
	static std::string& trim_l(std::string &str); 
	// 去掉字符串右侧的空白字符
	static std::string& trim_r(std::string &str); 
	// 去掉字符串左右两侧的空白字符
	static std::string& trim(std::string &str); 

	// 对字符串进行首尾反转
	static std::string& reverse(std::string &str); 

	// 将字符串转换成大写
	static std::string& to_upper(std::string &str); 
	// 生成一个当前字符串的拷贝的大写字符串
	static std::string to_upper_copy(std::string &str); 

	// 将字符串转换成小写
	static std::string& to_lower(std::string &str); 
	// 生成一个当前字符串的拷贝的小写字符串
	static std::string to_lower_copy(std::string &str); 

	// 返回当前字符串左侧的n_count个字符
	static std::string left(std::string &str, size_t n_count);
	// 返回当前字符串右侧的n_count个字符
	static std::string right(std::string &str, size_t n_count);
	// 返回当前字符串中间的n个字符
	static std::string mid(std::string &str, size_t i_start, size_t n_count); 
	// 以首尾关键字来查找指定的字符串并返回
	static std::string mid_l_r(std::string &str, std::string str_left, std::string str_right, bool b_inc_start = false, bool b_inc_end = false); 
	// 返回在str中包含str_key中任意字符的子串
	static std::string span_including(std::string& str, std::string str_key); 
	// 返回在str中不包含str_key中任意字符的子串
	static std::string span_excluding(std::string& str, std::string str_key); 

	// str字符串是否以str_begin字符串开头
	static bool start_with(std::string &str, std::string str_begin); 
	// str字符串是否以str_end字符串结尾
	static bool end_with(std::string &str, std::string str_end); 
	// str字符串是否包含str_sub子串
	static bool contain(std::string &str, std::string str_sub); 
	// str字符串是否为空串
	static bool is_empty(std::string &str); 

	// 格式化字符串
	static std::string format(const std::string fmt, ...); 
	// 格式化字符串并添加到str结尾
	static std::string& append_format(std::string& str, const std::string fmt, ...); 
	// 删除str字符串中的所有ch字符
	static std::string& erase(std::string& str, char ch); 
	// 替换str字符串中的第一处from字符串为to字符串
	static bool replace(std::string& str, const std::string& from, const std::string& to); 
	// 替换str字符串中的所有from字符串为to字符串
	static void replace_all(std::string& str, const std::string& from, const std::string& to); 
	// 不区分大小写比较字符串
	static int string_i_cmp(std::string lhs, std::string rhs); 
	// 将str字符串以pattern分隔符进行分割，结果放到arrout数组中
	static void split(std::string& str, std::string pattern, vector<std::string>& arrout); 
	// 将arr_string数组中的多个字符串以pattern分隔符进行连接
	static std::string join(std::vector<string>& arr_string, std::string pattern = ""); 

	// 将窄字节字符串转换成宽字节unicode字符串
	static std::wstring a2w_string(std::string str_a); 
	// 将宽字节unicode字符串转换成窄字节字符串
	static std::string w2a_string(std::wstring str_w); 

	// 将窄字节字符串转换成通用的字符串类型string_t
	static string_t to_string(std::string str_a); 
	// 将宽字节unicode字符串转换成通用的字符串类型string_t
	static string_t to_string(std::wstring str_w); 

	// 将通用的字符串类型string_t转换成宽字节unicode字符串
	static std::wstring to_wstring(string_t str); 
	// 将通用的字符串类型string_t转换成窄字节字符串
	static std::string to_astring(string_t str); 

public:
	// 窄字节转换成unicode宽字节
	static std::wstring gbk2unicode(std::string str_gbk); 
	// unicode宽字节转换成窄字节
	static std::string unicode2gbk(std::wstring str_unicode); 

	// 窄字节转换成utf8编码的字符串
	static std::string gbk2utf8(std::string str_gbk); 
	// utf8编码的字符串转换成窄字节
	static std::string utf82gbk(std::string str_utf8); 

	// unicode宽字节字符串转换成utf8编码的字符串
	static std::string unicode2utf8(std::wstring str_unicode); 
	// utf8编码的字符串转换成unicode宽字节字符串
	static std::wstring utf82unicode(std::string str_utf8); 
};

/*
	例子
	wchar_t* 名字指针 = (wchar_t*)(对象 + 偏移_人物玩家名字);
	string name = string_util::w2a_string(名字指针);
*/

