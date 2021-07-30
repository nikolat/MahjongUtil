#pragma once

#include "csaori.h"
#include <algorithm>

using std::vector;

namespace MJ_FUNC {
	const int SHANTEN_MAX = 99;
	const int SORT_CODE_MAX = 99;
	//理牌(ソート)
	vector_str SortHai(const vector_str hai);
	//ソート順定義
	int GetSortCode(const string_t hai);
	//数値のみの配列に変換
	vector<int> StringArrayToNumArray(const vector_str hai);
	//1-9までの個数の配列に変換
	vector<int> NumArrayToCountArray(const vector<int> hai);
	//NumArrayToCountArrayの逆変換
	vector<int> CountArrayToNumArray(const vector<int> hai);
	//配列に変換
	vector_str StringToArray(const string_t hai);
	//配列に変換(副露を分離)
	vector<vector_str> StringToArrayWithFuro(const string_t hai);
	//配列に変換(副露を区別しない)
	vector_str StringToArrayPlain(const string_t hai);
	//副露部分をカンマ区切りで返す
	string_t GetFuroStringWithComma(const string_t hai);
	//指定した要素を削除
	vector_str RemoveElementByName(const vector_str ary, const string_t name, const size_t count);
	vector<int> RemoveElementByName(const vector<int> ary, const int name, const size_t count);
	string_t RemoveElementByName(const string_t tehai, const string_t name, const size_t count);
	//重複した要素を削除
	vector_str Uniq(const vector_str ary);
	vector<vector_str> Uniq(const vector<vector_str> ary);
	vector<vector<int>> Uniq(const vector<vector<int>> ary);
	vector<vector<vector<vector<int>>>> Uniq(const vector<vector<vector<vector<int>>>> ary);
	//YAYAのASEARCH
	int ASEARCH(const string_t key, const vector_str ary);
	int ASEARCH(const int key, const vector<int> ary);
	//YAYAのASEARCHEX
	vector<int> ASEARCHEX(const string_t key, const vector_str ary);
	vector<int> ASEARCHEX(const int key, const vector<int> ary);
	//YAYAのSPLIT
	vector_str SPLIT(const string_t tgt_str, const string_t sep_str);
	//YAYA(システム辞書)のJOIN
	string_t JOIN(const vector_str a, const string_t s);
	string_t JOIN(const vector<int> a, const string_t s);
}
