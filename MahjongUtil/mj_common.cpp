#include "mj_common.h"

namespace MJ_FUNC {
	//理牌(ソート)
	vector_str SortHai(const vector_str hai)
	{
		vector_str hai_normal;
		vector_str hai_furo;
		for (size_t i = 0; i < hai.size(); i++)
		{
			if (hai[i].size() == 2)
				hai_normal.push_back(hai[i]);
			else
				hai_furo.push_back(hai[i]);
		}
		vector_str a = hai_normal;
		vector_str ret;
		//バブルソートでいいや…
		while (a.size() > 0)
		{
			int n = a.size();
			int index = -1;
			int min_value = SORT_CODE_MAX;
			vector_str an = {};
			for (int i = 0; i < n; i++)
			{
				int v = GetSortCode(a[i]);
				if (v < min_value) {
					if (index >= 0)
						an.push_back(a[index]);
					index = i;
					min_value = v;
				}
				else
				{
					an.push_back(a[i]);
				}
			}
			ret.push_back(a[index]);
			a = an;
		}
		ret.insert(ret.end(), hai_furo.begin(), hai_furo.end());
		return ret;
	}

	//ソート順定義
	int GetSortCode(const string_t hai)
	{
		string_t s = L"1m2m3m4m5m6m7m8m9m1p2p3p4p5p6p7p8p9p1s2s3s4s5s6s7s8s9s1z2z3z4z5z6z7z";
		size_t index = s.find(hai);
		if (index == string_t::npos)
			return -1;
		int r = index / 2;
		return r;
	}

	//数値のみの配列に変換
	vector<int> StringArrayToNumArray(const vector_str hai)
	{
		vector<int> ret;
		for (size_t i = 0; i < hai.size(); i++)
		{
			ret.push_back(stoi(hai[i].substr(0, 1)));
		}
		return ret;
	}

	//1-9までの個数の配列に変換
	vector<int> NumArrayToCountArray(const vector<int> hai)
	{
		vector<int> a = { -1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };//a[0]はreserved
		for (size_t i = 0; i < hai.size(); i++)
		{
			a[hai[i]] += 1;
		}
		return a;
	}

	//NumArrayToCountArrayの逆変換
	vector<int> CountArrayToNumArray(const vector<int> hai)
	{
		vector<int> a;
		for (size_t i = 1; i < hai.size(); i++)
		{
			for (size_t j = 0; j < (size_t)hai[i]; j++)
			{
				a.push_back(i);
			}
		}
		return a;
	}

	//配列に変換
	vector_str StringToArray(const string_t hai)
	{
		vector_str ret;
		for (size_t i = 0; i < hai.size(); i++)
		{
			if (hai.substr(i, 1) == L"<")
			{
				if (hai.substr(i + 7, 1) == L">")
				{
					ret.push_back(hai.substr(i + 1, 6));
					i += 7;
				}
				else if (hai.substr(i + 9, 1) == L">")
				{
					ret.push_back(hai.substr(i + 1, 8));
					i += 9;
				}
			}
			else if (hai.substr(i, 1) == L"(")
			{
				if (hai.substr(i + 9, 1) == L")")
				{
					ret.push_back(hai.substr(i + 1, 8));
					i += 9;
				}
			}
			else
			{
				ret.push_back(hai.substr(i, 2));
				i += 1;
			}
		}
		return ret;
	}

	//配列に変換(副露を分離)
	vector<vector_str> StringToArrayWithFuro(const string_t hai)
	{
		vector_str normal;
		vector_str furo;
		vector_str ankan;
		for (size_t i = 0; i < hai.size(); i++)
		{
			if (hai.substr(i, 1) == L"<")
			{
				if (hai.substr(i + 7, 1) == L">")
				{
					furo.push_back(hai.substr(i + 1, 6));
					i += 7;
				}
				else if (hai.substr(i + 9, 1) == L">")
				{
					furo.push_back(hai.substr(i + 1, 8));
					i += 9;
				}
			}
			else if (hai.substr(i, 1) == L"(")
			{
				if (hai.substr(i + 9, 1) == L")")
				{
					ankan.push_back(hai.substr(i + 1, 8));
					i += 9;
				}
			}
			else
			{
				normal.push_back(hai.substr(i, 2));
				i += 1;
			}
		}
		return { normal, furo, ankan };
	}

	//配列に変換(副露を区別しない)
	vector_str StringToArrayPlain(const string_t hai)
	{
		vector_str ret;
		for (size_t i = 0; i < hai.size(); i++)
		{
			if (hai.substr(i, 1) == L"<")
			{
				if (hai.substr(i + 7, 1) == L">")
				{
					ret.push_back(hai.substr(i + 1, 2));
					ret.push_back(hai.substr(i + 3, 2));
					ret.push_back(hai.substr(i + 5, 2));
					i += 7;
				}
				else if (hai.substr(i + 9, 1) == L">")
				{
					ret.push_back(hai.substr(i + 1, 2));
					ret.push_back(hai.substr(i + 3, 2));
					ret.push_back(hai.substr(i + 5, 2));
					ret.push_back(hai.substr(i + 7, 2));
					i += 9;
				}
			}
			else if (hai.substr(i, 1) == L"(")
			{
				if (hai.substr(i + 9, 1) == L")")
				{
					ret.push_back(hai.substr(i + 1, 2));
					ret.push_back(hai.substr(i + 3, 2));
					ret.push_back(hai.substr(i + 5, 2));
					ret.push_back(hai.substr(i + 7, 2));
					i += 9;
				}
			}
			else
			{
				ret.push_back(hai.substr(i, 2));
				i += 1;
			}
		}
		return ret;
	}

	//副露部分をカンマ区切りで返す
	string_t GetFuroStringWithComma(const string_t hai)
	{
		string_t furo;
		for (size_t i = 0; i < hai.size(); i++)
		{
			if (hai.substr(i, 1) == L"<")
			{
				if (hai.substr(i + 7, 1) == L">")
				{
					furo += L"," + hai.substr(i, 8);
					i += 7;
				}
				else if (hai.substr(i + 9, 1) == L">")
				{
					furo += L"," + hai.substr(i, 10);
					i += 9;
				}
			}
			else if (hai.substr(i, 1) == L"(")
			{
				if (hai.substr(i + 9, 1) == L")")
				{
					furo += L"," + hai.substr(i, 10);
					i += 9;
				}
			}
			else
			{
				i += 1;
			}
		}
		return furo;
	}

	//指定した要素を削除
	vector_str RemoveElementByName(const vector_str ary, const string_t name, const size_t count)
	{
		vector_str ret;
		size_t n = 0;
		for (size_t i = 0; i < ary.size(); i++)
		{
			if (ary[i] == name && n < count)
			{
				n++;
			}
			else
			{
				ret.push_back(ary[i]);
			}
		}
		return ret;
	}
	vector<int> RemoveElementByName(const vector<int> ary, const int name, const size_t count)
	{
		vector<int> ret;
		size_t n = 0;
		for (size_t i = 0; i < ary.size(); i++)
		{
			if (ary[i] == name && n < count)
			{
				n++;
			}
			else
			{
				ret.push_back(ary[i]);
			}
		}
		return ret;
	}
	string_t RemoveElementByName(const string_t tehai, const string_t name, const size_t count)
	{
		string_t ret;
		size_t n = 0;
		for (size_t i = 0; i < tehai.size(); i++)
		{
			if (tehai.substr(i, 1) == L"<")
			{
				if (tehai.substr(i + 7, 1) == L">")
				{
					ret += tehai.substr(i, 8);
					i += 7;
				}
				else if (tehai.substr(i + 9, 1) == L">")
				{
					ret += tehai.substr(i, 10);
					i += 9;
				}
			}
			else if (tehai.substr(i, 1) == L"(")
			{
				if (tehai.substr(i + 9, 1) == L")")
				{
					ret += tehai.substr(i, 10);
					i += 9;
				}
			}
			else
			{
				if (tehai.substr(i, 2) == name && n < count)
				{
					n++;
				}
				else
				{
					ret += tehai.substr(i, 2);
				}
				i += 1;
			}
		}
		return ret;
	}

	//重複した要素を削除
	vector_str Uniq(const vector_str ary)
	{
		vector_str ret;
		for (size_t i = 0; i < ary.size(); i++)
		{
			string_t key = ary[i];
			if (ASEARCH(key, ret) == -1)
				ret.push_back(key);
		}
		return ret;
	}
	vector<vector_str> Uniq(const vector<vector_str> ary)
	{
		vector<vector_str> ret;
		vector_str retCheck;
		for (size_t i = 0; i < ary.size(); i++)
		{
			vector_str key = RemoveElementByName(ary[i], L"", 1);
			sort(key.begin(), key.end());
			if (ASEARCH(JOIN(key, L","), retCheck) == -1)
			{
				retCheck.push_back(JOIN(key, L","));
				ret.push_back(ary[i]);
			}
		}
		return ret;
	}
	vector<vector<int>> Uniq(const vector<vector<int>> ary)
	{
		vector<vector<int>> ret;
		vector_str retCheck;
		for (size_t i = 0; i < ary.size(); i++)
		{
			vector<int> key = ary[i];
			sort(key.begin(), key.end());
			if (ASEARCH(JOIN(key, L","), retCheck) == -1)
			{
				retCheck.push_back(JOIN(key, L","));
				ret.push_back(ary[i]);
			}
		}
		return ret;
	}
	vector<vector<vector<vector<int>>>> Uniq(vector<vector<vector<vector<int>>>> ary)
	{
		vector<vector<vector<vector<int>>>> ret;
		vector_str retCheck;
		for (size_t i = 0; i < ary.size(); i++)
		{
			string_t key;
			key += L"I";
			for (size_t j = 0; j < ary[i].size(); j++)
			{
				key += L"J";
				for (size_t k = 0; k < ary[i][j].size(); k++)
				{
					key += L"K";
					for (size_t l = 0; l < ary[i][j][k].size(); l++)
					{
						key += L"L";
						key += ary[i][j][k][l];
					}
				}
			}
			if (ASEARCH(key, retCheck) == -1)
			{
				retCheck.push_back(key);
				ret.push_back(ary[i]);
			}
		}
		return ret;
	}

	//YAYAのASEARCH
	int ASEARCH(const string_t key, const vector_str ary)
	{
		for (size_t i = 0; i < ary.size(); i++)
		{
			if (key == ary[i])
				return i;
		}
		return -1;
	}
	int ASEARCH(const int key, const vector<int> ary)
	{
		for (size_t i = 0; i < ary.size(); i++)
		{
			if (key == ary[i])
				return i;
		}
		return -1;
	}

	//YAYAのASEARCHEX
	vector<int> ASEARCHEX(const string_t key, const vector_str ary)
	{
		vector<int> ret;
		for (size_t i = 0; i < ary.size(); i++)
		{
			if (key == ary[i])
				ret.push_back(i);
		}
		return ret;
	}
	vector<int> ASEARCHEX(const int key, const vector<int> ary)
	{
		vector<int> ret;
		for (size_t i = 0; i < ary.size(); i++)
		{
			if (key == ary[i])
				ret.push_back(i);
		}
		return ret;
	}

	//YAYAのSPLIT
	vector_str SPLIT(const string_t tgt_str, const string_t sep_str)
	{
		vector_str result;
		const string_t::size_type sep_strlen = sep_str.size();
		const string_t::size_type tgt_strlen = tgt_str.size();
		string_t::size_type seppoint = 0;
		string_t::size_type spoint;
		for (string_t::size_type i = 1; ; i++) {
			spoint = tgt_str.find(sep_str, seppoint);
			if (spoint == string_t::npos) {
				result.push_back(tgt_str.substr(seppoint, tgt_strlen - seppoint));
				break;
			}
			result.push_back(tgt_str.substr(seppoint, spoint - seppoint));
			seppoint = spoint + sep_strlen;
		}
		return result;
	}

	//YAYA(システム辞書)のJOIN
	string_t JOIN(const vector_str a, const string_t s)
	{
		string_t r;
		if (a.empty())
			return L"";
		r = a[0];
		for (size_t i = 1; i < a.size(); i++)
		{
			r += s + a[i];
		}
		return r;
	}
	string_t JOIN(const vector<int> a, const string_t s)
	{
		string_t r;
		if (a.empty())
			return L"";
		r = SAORI_FUNC::numToString(a[0]);
		for (size_t i = 1; i < a.size(); i++)
		{
			r += s + SAORI_FUNC::numToString(a[i]);
		}
		return r;
	}
}
