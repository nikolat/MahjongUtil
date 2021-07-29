#include "mj_machi.h"

//待ちの取得
string_t Machi::GetMachi(const string_t tehai)
{
	vector_str composition;
	int shanten = Shanten::GetShanten(tehai, composition);
	if (shanten != 0)
		return L"";
	vector<vector_str> hai_split = StringToArrayWithFuro(tehai);
	vector_str hai_normal = hai_split[0];
	vector_str machi;
	for (size_t i = 0; i < composition.size(); i++)
	{
		vector_str composition_split = SPLIT(composition[i], L",");
		if (composition[i] == L"chitoitsu")
		{
			machi.push_back(GetMachiChitoitsu(hai_normal));
		}
		else if (composition[i] == L"kokushimusou")
		{
			return GetMachiKokushimusou(hai_normal);
		}
		else if (composition_split[0] == L"")//雀頭無し
		{
			for (size_t j = 1; j < composition_split.size(); j++)
			{
				if (composition_split[j].size() == 2)
				{
					machi.push_back(composition_split[j]);//単騎
					break;
				}
			}
		}
		else
		{
			for (size_t j = 1; j < composition_split.size(); j++)
			{
				if (composition_split[j].size() != 4)
					continue;
				vector_str tatsu = StringToArray(composition_split[j]);
				vector<int> tatsu_num = StringArrayToNumArray(tatsu);
				string_t color = tatsu[0].substr(1, 1);
				int n1 = tatsu_num[0];
				int n2 = tatsu_num[1];
				if (n1 == 1 && n2 == 2)//辺張
				{
					machi.push_back(L"3" + color);
				}
				else if (n1 == 8 && n2 == 9)//辺張
				{
					machi.push_back(L"7" + color);
				}
				else if (n1 + 1 == n2)//両面
				{
					machi.push_back(SAORI_FUNC::intToString(n1 - 1) + color);
					machi.push_back(SAORI_FUNC::intToString(n2 + 1) + color);
				}
				else if (n1 + 2 == n2)//嵌張
				{
					machi.push_back(SAORI_FUNC::intToString(n1 + 1) + color);
				}
				else if (n1 == n2)//シャンポン
				{
					machi.push_back(tatsu[0]);
					machi.push_back(composition_split[0].substr(0, 2));
				}
				break;
			}
		}
	}
	machi = Uniq(SortHai(machi));
	return JOIN(machi, L"");
}

//待ちの取得(七対子)
string_t Machi::GetMachiChitoitsu(const vector_str tehai)
{
	for (size_t i = 0; i < tehai.size(); i++)
	{
		if (ASEARCHEX(tehai[i], tehai).size() == 1)
			return tehai[i];
	}
	return L"";
}

//待ちの取得(国士無双)
string_t Machi::GetMachiKokushimusou(const vector_str tehai)
{
	string_t yaochuhai_string = L"1m9m1p9p1s9s1z2z3z4z5z6z7z";
	vector_str yaochuhai_array = StringToArray(yaochuhai_string);
	for (size_t i = 0; i < yaochuhai_array.size(); i++)
	{
		if (ASEARCH(yaochuhai_array[i], tehai) == -1)
			return yaochuhai_array[i];
	}
	return yaochuhai_string;
}
