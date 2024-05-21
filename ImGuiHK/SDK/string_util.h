/*
* author��syc
* website: www.cctry.com
* time��2018-12-18
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
	// ȥ���ַ������Ŀհ��ַ�
	static std::string& trim_l(std::string &str); 
	// ȥ���ַ����Ҳ�Ŀհ��ַ�
	static std::string& trim_r(std::string &str); 
	// ȥ���ַ�����������Ŀհ��ַ�
	static std::string& trim(std::string &str); 

	// ���ַ���������β��ת
	static std::string& reverse(std::string &str); 

	// ���ַ���ת���ɴ�д
	static std::string& to_upper(std::string &str); 
	// ����һ����ǰ�ַ����Ŀ����Ĵ�д�ַ���
	static std::string to_upper_copy(std::string &str); 

	// ���ַ���ת����Сд
	static std::string& to_lower(std::string &str); 
	// ����һ����ǰ�ַ����Ŀ�����Сд�ַ���
	static std::string to_lower_copy(std::string &str); 

	// ���ص�ǰ�ַ�������n_count���ַ�
	static std::string left(std::string &str, size_t n_count);
	// ���ص�ǰ�ַ����Ҳ��n_count���ַ�
	static std::string right(std::string &str, size_t n_count);
	// ���ص�ǰ�ַ����м��n���ַ�
	static std::string mid(std::string &str, size_t i_start, size_t n_count); 
	// ����β�ؼ���������ָ�����ַ���������
	static std::string mid_l_r(std::string &str, std::string str_left, std::string str_right, bool b_inc_start = false, bool b_inc_end = false); 
	// ������str�а���str_key�������ַ����Ӵ�
	static std::string span_including(std::string& str, std::string str_key); 
	// ������str�в�����str_key�������ַ����Ӵ�
	static std::string span_excluding(std::string& str, std::string str_key); 

	// str�ַ����Ƿ���str_begin�ַ�����ͷ
	static bool start_with(std::string &str, std::string str_begin); 
	// str�ַ����Ƿ���str_end�ַ�����β
	static bool end_with(std::string &str, std::string str_end); 
	// str�ַ����Ƿ����str_sub�Ӵ�
	static bool contain(std::string &str, std::string str_sub); 
	// str�ַ����Ƿ�Ϊ�մ�
	static bool is_empty(std::string &str); 

	// ��ʽ���ַ���
	static std::string format(const std::string fmt, ...); 
	// ��ʽ���ַ�������ӵ�str��β
	static std::string& append_format(std::string& str, const std::string fmt, ...); 
	// ɾ��str�ַ����е�����ch�ַ�
	static std::string& erase(std::string& str, char ch); 
	// �滻str�ַ����еĵ�һ��from�ַ���Ϊto�ַ���
	static bool replace(std::string& str, const std::string& from, const std::string& to); 
	// �滻str�ַ����е�����from�ַ���Ϊto�ַ���
	static void replace_all(std::string& str, const std::string& from, const std::string& to); 
	// �����ִ�Сд�Ƚ��ַ���
	static int string_i_cmp(std::string lhs, std::string rhs); 
	// ��str�ַ�����pattern�ָ������зָ����ŵ�arrout������
	static void split(std::string& str, std::string pattern, vector<std::string>& arrout); 
	// ��arr_string�����еĶ���ַ�����pattern�ָ�����������
	static std::string join(std::vector<string>& arr_string, std::string pattern = ""); 

	// ��խ�ֽ��ַ���ת���ɿ��ֽ�unicode�ַ���
	static std::wstring a2w_string(std::string str_a); 
	// �����ֽ�unicode�ַ���ת����խ�ֽ��ַ���
	static std::string w2a_string(std::wstring str_w); 

	// ��խ�ֽ��ַ���ת����ͨ�õ��ַ�������string_t
	static string_t to_string(std::string str_a); 
	// �����ֽ�unicode�ַ���ת����ͨ�õ��ַ�������string_t
	static string_t to_string(std::wstring str_w); 

	// ��ͨ�õ��ַ�������string_tת���ɿ��ֽ�unicode�ַ���
	static std::wstring to_wstring(string_t str); 
	// ��ͨ�õ��ַ�������string_tת����խ�ֽ��ַ���
	static std::string to_astring(string_t str); 

public:
	// խ�ֽ�ת����unicode���ֽ�
	static std::wstring gbk2unicode(std::string str_gbk); 
	// unicode���ֽ�ת����խ�ֽ�
	static std::string unicode2gbk(std::wstring str_unicode); 

	// խ�ֽ�ת����utf8������ַ���
	static std::string gbk2utf8(std::string str_gbk); 
	// utf8������ַ���ת����խ�ֽ�
	static std::string utf82gbk(std::string str_utf8); 

	// unicode���ֽ��ַ���ת����utf8������ַ���
	static std::string unicode2utf8(std::wstring str_unicode); 
	// utf8������ַ���ת����unicode���ֽ��ַ���
	static std::wstring utf82unicode(std::string str_utf8); 
};

/*
	����
	wchar_t* ����ָ�� = (wchar_t*)(���� + ƫ��_�����������);
	string name = string_util::w2a_string(����ָ��);
*/

