#include "mj_shanten.h"

//シャンテン数の取得
int Shanten::GetShanten(const string_t tehai)
{
	vector_str composition;
	return GetShanten(tehai, composition);
}
int Shanten::GetShanten(const string_t tehai, vector_str& composition)
{
	int r1 = SHANTEN_MAX;
	int r2 = SHANTEN_MAX;
	if (GetFuroStringWithComma(tehai).empty())
	{
		r1 = GetShantenChitoitsu(tehai);
		r2 = GetShantenKokushimusou(tehai);
	}
	vector_str ret_composition;
	vector_str ret_composition_normal;
	int r3 = GetShantenNormal(tehai, ret_composition_normal);
	int r = (std::min)({ r1, r2, r3 });
	if (r == r3)
		ret_composition = ret_composition_normal;
	if (r == r1)
		ret_composition.push_back(L"chitoitsu");
	if (r == r2)
		ret_composition.push_back(L"kokushimusou");
	composition = ret_composition;
	return r;
}

//シャンテン数の取得(役あり)
int Shanten::GetShantenYaku(const string_t tehai, const string_t bafu_hai, const string_t jifu_hai, vector_str& yaku)
{
	int r_chitoitsu = GetShantenChitoitsu(tehai);
	int r_kokushimusou = GetShantenKokushimusou(tehai);
	int r_menzen = GetShantenMenzen(tehai);
	int r_yakuhai = GetShantenYakuhai(tehai, bafu_hai, jifu_hai);
	int r_tanyao = GetShantenTanyao(tehai);
	int r_toitoi = GetShantenToitoi(tehai);
	int r_honitsu = GetShantenHonitsu(tehai);
	int r = (std::min)({ r_chitoitsu, r_kokushimusou, r_menzen, r_yakuhai, r_tanyao, r_toitoi, r_honitsu });
	std::map<string_t, int> yaku_info;
	yaku_info[L"七対子"] = r_chitoitsu;
	yaku_info[L"国士無双"] = r_kokushimusou;
	yaku_info[L"門前清自摸和"] = r_menzen;
	yaku_info[L"役牌"] = r_yakuhai;
	yaku_info[L"断ヤオ九"] = r_tanyao;
	yaku_info[L"対々和"] = r_toitoi;
	yaku_info[L"混一色"] = r_honitsu;
	for (auto itr = yaku_info.begin(); itr != yaku_info.end(); ++itr) {
		if (itr->second < SHANTEN_MAX)
			yaku.push_back(itr->first + L"," + SAORI_FUNC::intToString(itr->second));
	}
	return r;
}

//シャンテン数の取得(一般手)
int Shanten::GetShantenNormal(const string_t tehai, vector_str& composition)
{
	vector<vector_str> hai_split = StringToArrayWithFuro(tehai);
	vector_str hai_normal = SortHai(hai_split[0]);
	vector_str hai_furo = hai_split[1];
	vector_str hai_ankan = hai_split[2];
	//孤立牌の除去
	vector<vector_str> hai_remove = RemoveKoritsuHai(hai_normal);
	vector_str hai_without_koritsuhai = hai_remove[0];
	vector_str koritsu_hai = hai_remove[1];
	//雀頭候補
	vector_str atama_kouho = GetToitsu(hai_without_koritsuhai);
	//雀頭無しの場合も追加
	atama_kouho.push_back(L"");
	int shanten = SHANTEN_MAX;
	vector_str ret_composition;
	for (size_t i = 0; i < atama_kouho.size(); i++)
	{
		string_t atama = atama_kouho[i];
		vector_str hai_without_atama = RemoveElementByName(hai_without_koritsuhai, atama, 2);
		vector<vector_str> hai_color = SeparateColor(hai_without_atama);
		vector_str man = hai_color[0];
		vector_str pin = hai_color[1];
		vector_str sou = hai_color[2];
		vector_str jihai = hai_color[3];
		vector<vector<vector<int>>> composition_man = GetComposition(man);
		vector<vector<vector<int>>> composition_pin = GetComposition(pin);
		vector<vector<vector<int>>> composition_sou = GetComposition(sou);
		vector<vector<vector<int>>> composition_jihai = GetCompositionJihai(jihai);
		for (size_t jm = 0; jm < composition_man.size(); jm++)
		{
			for (size_t jp = 0; jp < composition_pin.size(); jp++)
			{
				for (size_t js = 0; js < composition_sou.size(); js++)
				{
					size_t count_mentsu = 0;
					size_t count_tatsu = 0;
					size_t count_koritsu = 0;
					for (size_t k = 0; k < composition_man[jm].size(); k++)
					{
						if (composition_man[jm][k].size() == 3)
							count_mentsu++;
						else if (composition_man[jm][k].size() == 2)
							count_tatsu++;
						else if (composition_man[jm][k].size() == 1)
							count_koritsu++;
					}
					for (size_t k = 0; k < composition_pin[jp].size(); k++)
					{
						if (composition_pin[jp][k].size() == 3)
							count_mentsu++;
						else if (composition_pin[jp][k].size() == 2)
							count_tatsu++;
						else if (composition_pin[jp][k].size() == 1)
							count_koritsu++;
					}
					for (size_t k = 0; k < composition_sou[js].size(); k++)
					{
						if (composition_sou[js][k].size() == 3)
							count_mentsu++;
						else if (composition_sou[js][k].size() == 2)
							count_tatsu++;
						else if (composition_sou[js][k].size() == 1)
							count_koritsu++;
					}
					for (size_t k = 0; k < composition_jihai[0].size(); k++)
					{
						if (composition_jihai[0][k].size() == 3)
							count_mentsu++;
						else if (composition_jihai[0][k].size() == 2)
							count_tatsu++;
						else if (composition_jihai[0][k].size() == 1)
							count_koritsu++;
					}
					count_mentsu += hai_furo.size();
					count_mentsu += hai_ankan.size();
					count_koritsu += koritsu_hai.size();
					bool has_atama = !atama.empty();
					int shanten_kouho = CalcShanten(count_mentsu, count_tatsu, count_koritsu, has_atama);
					//シャンテン数が小さい方を選択
					if (shanten_kouho < shanten)
					{
						shanten = shanten_kouho;
						ret_composition = { MakeCompositionString(atama, composition_man[jm], composition_pin[jp], composition_sou[js], composition_jihai[0]) };
					}
					else if (shanten_kouho == shanten)
					{
						ret_composition.push_back(MakeCompositionString(atama, composition_man[jm], composition_pin[jp], composition_sou[js], composition_jihai[0]));
					}
				}
			}
		}
	}
	ret_composition = Uniq(ret_composition);
	//最初に除いた孤立牌を加える
	for (size_t i = 0; i < ret_composition.size(); i++)
	{
		for (size_t j = 0; j < koritsu_hai.size(); j++)
		{
			ret_composition[i] += L"," + koritsu_hai[j];
		}
	}
	//重複を除く
	vector<vector_str> ret_composition_split;
	for (size_t i = 0; i < ret_composition.size(); i++)
	{
		ret_composition_split.push_back(SPLIT(ret_composition[i], L","));
	}
	ret_composition_split = Uniq(ret_composition_split);
	//副露を追加
	string_t furo_string = GetFuroStringWithComma(tehai);
	ret_composition = {};
	for (size_t i = 0; i < ret_composition_split.size(); i++)
	{
		ret_composition.push_back(JOIN(ret_composition_split[i], L",") + furo_string);
	}
	composition = ret_composition;
	return shanten;
}

//孤立牌の除去
vector<vector_str> Shanten::RemoveKoritsuHai(const vector_str hai)
{
	vector_str ret_removed_hai;
	vector_str ret_koritsu_hai;
	for (size_t i = 0; i < hai.size(); i++)
	{
		int num = stoi(hai[i].substr(0, 1));
		string_t color = hai[i].substr(1, 1);
		if (color == L"m" || color == L"p" || color == L"s")
		{
			string_t hai_p2 = SAORI_FUNC::intToString(num - 2) + color;
			string_t hai_p1 = SAORI_FUNC::intToString(num - 1) + color;
			string_t hai_n1 = SAORI_FUNC::intToString(num + 1) + color;
			string_t hai_n2 = SAORI_FUNC::intToString(num + 2) + color;
			if (ASEARCH(hai_p2, hai) >= 0
				|| ASEARCH(hai_p1, hai) >= 0
				|| ASEARCHEX(hai[i], hai).size() > 1
				|| ASEARCH(hai_n1, hai) >= 0
				|| ASEARCH(hai_n2, hai) >= 0)
			{
				ret_removed_hai.push_back(hai[i]);
			}
			else
			{
				ret_koritsu_hai.push_back(hai[i]);
			}
		}
		else if (color == L"z")
		{
			if (ASEARCHEX(hai[i], hai).size() > 1)
			{
				ret_removed_hai.push_back(hai[i]);
			}
			else
			{
				ret_koritsu_hai.push_back(hai[i]);
			}
		}
	}
	return { ret_removed_hai, ret_koritsu_hai };
}

//シャンテン数の計算
int Shanten::CalcShanten(const int count_mentsu, const int count_tatsu, const int count_koritsu, const bool has_atama)
{
	int c_mentsu = count_mentsu;
	int c_tatsu = count_tatsu;
	int c_koritsu = count_koritsu;
	//雀頭がない場合は5ブロック必要
	int max_block = has_atama ? 4 : 5;
	//面子過多の補正
	if (c_mentsu > 4) { c_tatsu += c_mentsu - 4; c_mentsu = 4; }
	//搭子過多の補正
	if (c_mentsu + c_tatsu > 4) { c_koritsu += c_mentsu + c_tatsu - 4; c_tatsu = 4 - c_mentsu; }
	//孤立牌過多の補正
	if (c_mentsu + c_tatsu + c_koritsu > max_block) { c_koritsu = max_block - c_mentsu - c_tatsu; }
	//雀頭がある場合は搭子として数える
	if (has_atama) c_tatsu++;
	return 13 - c_mentsu * 3 - c_tatsu * 2 - c_koritsu;
}

//SAORIの戻り値となる雀頭・面子・塔子を表す文字列の作成
string_t Shanten::MakeCompositionString(
	const string_t atama,
	const vector<vector<int>> composition_man,
	const vector<vector<int>> composition_pin,
	const vector<vector<int>> composition_sou,
	const vector<vector<int>> composition_jihai
)
{
	string_t ret_atama = atama + atama;
	string_t ret_m = L"";
	for (size_t i = 0; i < composition_man.size(); i++)
	{
		ret_m += L",";
		for (size_t j = 0; j < composition_man[i].size(); j++)
		{
			ret_m += SAORI_FUNC::intToString(composition_man[i][j]) + L"m";
		}
	}
	string_t ret_p = L"";
	for (size_t i = 0; i < composition_pin.size(); i++)
	{
		ret_p += L",";
		for (size_t j = 0; j < composition_pin[i].size(); j++)
		{
			ret_p += SAORI_FUNC::intToString(composition_pin[i][j]) + L"p";
		}
	}
	string_t ret_s = L"";
	for (size_t i = 0; i < composition_sou.size(); i++)
	{
		ret_s += L",";
		for (size_t j = 0; j < composition_sou[i].size(); j++)
		{
			ret_s += SAORI_FUNC::intToString(composition_sou[i][j]) + L"s";
		}
	}
	string_t ret_j = L"";
	for (size_t i = 0; i < composition_jihai.size(); i++)
	{
		ret_j += L",";
		for (size_t j = 0; j < composition_jihai[i].size(); j++)
		{
			ret_j += SAORI_FUNC::intToString(composition_jihai[i][j]) + L"z";
		}
	}
	return ret_atama + ret_m + ret_p + ret_s + ret_j;
}

//面子・塔子・孤立牌(数牌)の取得
vector<vector<vector<int>>> Shanten::GetComposition(const vector_str hai)
{
	vector<int> hai_count_array = NumArrayToCountArray(StringArrayToNumArray(hai));
	int start = 1;
	vector<vector<vector<int>>> composition_a = { {} };
	vector<vector<vector<int>>> composition_b = { {} };
	vector<vector<int>> max = GetCompositionRecursion(hai_count_array, start, composition_a, composition_b);
	vector<vector<vector<int>>> ret = composition_a;
	ret.insert(ret.end(), composition_b.begin(), composition_b.end());
	return ret;
}

//面子・塔子・孤立牌(数牌)の再帰的取得
vector<vector<int>> Shanten::GetCompositionRecursion(const vector<int> hai_count_array, const int n, vector<vector<vector<int>>>& composition_a, vector<vector<vector<int>>>& composition_b)
{
	vector<int> hai = hai_count_array;
	vector<vector<vector<int>>> ret_a = composition_a;
	vector<vector<vector<int>>> ret_b = composition_b;
	//面子の抜き取りが終わったら搭子の数を数える
	if (n > 9)
	{
		vector<vector<vector<int>>> ret_tatsu_and_koritsu = { {} };
		int start = 1;
		int count_tatsu = GetTatsuAndKoritsu(hai_count_array, start, ret_tatsu_and_koritsu);
		vector<vector<vector<int>>> ret_a_new;
		vector<vector<vector<int>>> ret_b_new;
		for (size_t i = 0; i < ret_tatsu_and_koritsu.size(); i++)
		{
			for (size_t j = 0; j < ret_a.size(); j++)
			{
				vector<vector<int>> a = ret_a[j];
				for (size_t k = 0; k < ret_tatsu_and_koritsu[i].size(); k++)
				{
					a.push_back(ret_tatsu_and_koritsu[i][k]);
				}
				ret_a_new.push_back(a);
			}
			for (size_t j = 0; j < ret_b.size(); j++)
			{
				vector<vector<int>> a = ret_b[j];
				for (size_t k = 0; k < ret_tatsu_and_koritsu[i].size(); k++)
				{
					a.push_back(ret_tatsu_and_koritsu[i][k]);
				}
				ret_b_new.push_back(a);
			}
		}
		composition_a = ret_a_new;
		composition_b = ret_b_new;
		return { { 0, count_tatsu }, { 0, count_tatsu } };
	}

	//まずは面子を抜かず位置を1つ進め試行
	vector<vector<int>> max = GetCompositionRecursion(hai, n + 1, ret_a, ret_b);//仮の最適値とする

	//順子抜き取り
	if (n <= 7 && hai[n] > 0 && hai[n + 1] > 0 && hai[n + 2] > 0) {
		vector<vector<vector<int>>> ret_a_shuntsu = composition_a;
		vector<vector<vector<int>>> ret_b_shuntsu = composition_b;
		hai[n]--; hai[n + 1]--; hai[n + 2]--;
		for (size_t i = 0; i < ret_a_shuntsu.size(); i++)
		{
			ret_a_shuntsu[i].push_back({ n, n + 1, n + 2 });
		}
		for (size_t i = 0; i < ret_b_shuntsu.size(); i++)
		{
			ret_b_shuntsu[i].push_back({ n, n + 1, n + 2 });
		}
		vector<vector<int>> r = GetCompositionRecursion(hai, n, ret_a_shuntsu, ret_b_shuntsu);//抜き取ったら同じ位置でもう一度試行
		hai[n]++; hai[n + 1]++; hai[n + 2]++;
		r[0][0]++; r[1][0]++;//各パターンの面子の数を1増やす
		//必要であれば最適値の入替えをする
		if (r[0][0] * 2 + r[0][1] > max[0][0] * 2 + max[0][1])
		{
			max[0] = r[0];
			ret_a = ret_a_shuntsu;
		}
		else if (r[0][0] * 2 + r[0][1] == max[0][0] * 2 + max[0][1])
		{
			for (size_t i = 0; i < ret_a_shuntsu.size(); i++)
			{
				ret_a.push_back(ret_a_shuntsu[i]);
			}
		}
		if (r[1][0] * 10 + r[1][1] > max[1][0] * 10 + max[1][1])
		{
			max[1] = r[1];
			ret_b = ret_b_shuntsu;
		}
		else if (r[1][0] * 10 + r[1][1] == max[1][0] * 10 + max[1][1])
		{
			for (size_t i = 0; i < ret_b_shuntsu.size(); i++)
			{
				ret_b.push_back(ret_b_shuntsu[i]);
			}
		}
	}

	//刻子抜き取り
	if (hai[n] >= 3) {
		vector<vector<vector<int>>> ret_a_kotsu = composition_a;
		vector<vector<vector<int>>> ret_b_kotsu = composition_b;
		hai[n] -= 3;
		for (size_t i = 0; i < ret_a_kotsu.size(); i++)
		{
			ret_a_kotsu[i].push_back({ n, n, n });
		}
		for (size_t i = 0; i < ret_b_kotsu.size(); i++)
		{
			ret_b_kotsu[i].push_back({ n, n, n });
		}
		vector<vector<int>> r = GetCompositionRecursion(hai, n, ret_a_kotsu, ret_b_kotsu);//抜き取ったら同じ位置でもう一度試行
		hai[n] += 3;
		r[0][0]++; r[1][0]++;//各パターンの面子の数を1増やす
		//必要であれば最適値の入替えをする
		if (r[0][0] * 2 + r[0][1] > max[0][0] * 2 + max[0][1])
		{
			max[0] = r[0];
			ret_a = ret_a_kotsu;
		}
		else if (r[0][0] * 2 + r[0][1] == max[0][0] * 2 + max[0][1])
		{
			for (size_t i = 0; i < ret_a_kotsu.size(); i++)
			{
				ret_a.push_back(ret_a_kotsu[i]);
			}
		}
		if (r[1][0] * 10 + r[1][1] > max[1][0] * 10 + max[1][1])
		{
			max[1] = r[1];
			ret_b = ret_b_kotsu;
		}
		else if (r[1][0] * 10 + r[1][1] == max[1][0] * 10 + max[1][1])
		{
			for (size_t i = 0; i < ret_b_kotsu.size(); i++)
			{
				ret_b.push_back(ret_b_kotsu[i]);
			}
		}
	}
	composition_a = ret_a;
	composition_b = ret_b;
	return max;
}

//塔子・孤立牌(数牌)の再帰的取得
int Shanten::GetTatsuAndKoritsu(const vector<int> hai_count_array, const int n, vector<vector<vector<int>>>& ret_tatsu_and_koritsu)
{
	vector<int> hai = hai_count_array;
	vector<vector<vector<int>>> ret = ret_tatsu_and_koritsu;
	//搭子の抜き取りが終わったら孤立牌を加える
	if (n > 9)
	{
		vector<int> koritsu = CountArrayToNumArray(hai);
		if (koritsu.size() > 0)
		{
			vector<vector<vector<int>>> ret_koritsu;
			for (size_t i = 0; i < ret.size(); i++)
			{
				for (size_t j = 0; j < koritsu.size(); j++)
				{
					vector<vector<int>> a = ret[i];
					a.push_back({ koritsu[j] });
					ret_koritsu.push_back(a);
				}
			}
			ret_tatsu_and_koritsu = ret_koritsu;
		}
		return 0;
	}

	//まずは塔子を抜かず位置を1つ進め試行
	int max = GetTatsuAndKoritsu(hai, n + 1, ret);//仮の最適値とする

	//嵌張抜き取り
	if (n <= 7 && hai[n] > 0 && hai[n + 2] > 0) {
		vector<vector<vector<int>>> ret1 = ret_tatsu_and_koritsu;
		hai[n]--; hai[n + 2]--;
		for (size_t i = 0; i < ret1.size(); i++)
		{
			ret1[i].push_back({ n, n + 2 });
		}
		int r = GetTatsuAndKoritsu(hai, n, ret1);//抜き取ったら同じ位置でもう一度試行
		hai[n]++; hai[n + 2]++;
		r++;//塔子の数を1増やす
		//必要であれば最適値の入替えをする
		if (r > max)
		{
			max = r;
			ret = ret1;
		}
		else if (r == max)
		{
			for (size_t i = 0; i < ret1.size(); i++)
			{
				ret.push_back(ret1[i]);
			}
		}
	}

	//両面、辺張抜き取り
	if (n <= 8 && hai[n] > 0 && hai[n + 1] > 0) {
		vector<vector<vector<int>>> ret2 = ret_tatsu_and_koritsu;
		hai[n]--; hai[n + 1]--;
		for (size_t i = 0; i < ret2.size(); i++)
		{
			ret2[i].push_back({ n, n + 1 });
		}
		int r = GetTatsuAndKoritsu(hai, n, ret2);//抜き取ったら同じ位置でもう一度試行
		hai[n]++; hai[n + 1]++;
		r++;//塔子の数を1増やす
		//必要であれば最適値の入替えをする
		if (r > max)
		{
			max = r;
			ret = ret2;
		}
		else if (r == max)
		{
			for (size_t i = 0; i < ret2.size(); i++)
			{
				ret.push_back(ret2[i]);
			}
		}
	}

	//対子抜き取り
	if (hai[n] >= 2) {
		vector<vector<vector<int>>> ret3 = ret_tatsu_and_koritsu;
		hai[n] -= 2;
		for (size_t i = 0; i < ret3.size(); i++)
		{
			ret3[i].push_back({ n, n });
		}
		int r = GetTatsuAndKoritsu(hai, n, ret3);//抜き取ったら同じ位置でもう一度試行
		hai[n] += 2;
		r++;//塔子の数を1増やす
		//必要であれば最適値の入替えをする
		if (r > max)
		{
			max = r;
			ret = ret3;
		}
		else if (r == max)
		{
			for (size_t i = 0; i < ret3.size(); i++)
			{
				ret.push_back(ret3[i]);
			}
		}
	}
	ret_tatsu_and_koritsu = ret;
	return max;
}

//面子(字牌)の取得
vector<vector<vector<int>>> Shanten::GetCompositionJihai(const vector_str hai)
{
	vector<int> hai_num = StringArrayToNumArray(hai);
	vector<int> kotsu = GetKotsu(hai_num);
	//面子の配列を作成
	vector<vector<int>> composition = {};
	for (size_t i = 0; i < kotsu.size(); i++)
	{
		int n = kotsu[i];
		composition.push_back({ n, n, n });
	}
	for (size_t i = 0; i < kotsu.size(); i++)
	{
		hai_num = RemoveElementByName(hai_num, kotsu[i], 3);
	}
	vector<int> toitsu = GetToitsu(hai_num);
	for (size_t i = 0; i < toitsu.size(); i++)
	{
		int n = toitsu[i];
		composition.push_back({ n, n });
	}
	return { composition };
}

//数牌・字牌の分離
vector<vector_str> Shanten::SeparateColor(const vector_str hai)
{
	vector<vector_str> ret = { {}, {}, {}, {} };
	for (size_t i = 0; i < hai.size(); i++)
	{
		string_t color = hai[i].substr(1, 1);
		if (color == L"m")
			ret[0].push_back(hai[i]);
		else if (color == L"p")
			ret[1].push_back(hai[i]);
		else if (color == L"s")
			ret[2].push_back(hai[i]);
		else if (color == L"z")
			ret[3].push_back(hai[i]);
	}
	return ret;
}

//3個以上同じ牌がある要素をすべて返す
vector_str Shanten::GetKotsu(const vector_str hai)
{
	return GetDuplicatedElement(hai, 3);
}
vector<int> Shanten::GetKotsu(const vector<int> hai)
{
	return GetDuplicatedElement(hai, 3);
}

//2個以上同じ牌がある要素をすべて返す
vector_str Shanten::GetToitsu(const vector_str hai)
{
	return GetDuplicatedElement(hai, 2);
}
vector<int> Shanten::GetToitsu(const vector<int> hai)
{
	return GetDuplicatedElement(hai, 2);
}

//重複した要素を返す
vector_str Shanten::GetDuplicatedElement(const vector_str ary, const int n)
{
	vector_str ret;
	for (size_t i = 0; i < ary.size(); i++)
	{
		string_t key = ary[i];
		if (ASEARCH(key, ret) == -1)
		{
			if (ASEARCHEX(key, ary).size() >= (size_t)n)
				ret.push_back(key);
		}
	}
	return ret;
}
vector<int> Shanten::GetDuplicatedElement(const vector<int> ary, const int n)
{
	vector<int> ret;
	for (size_t i = 0; i < ary.size(); i++)
	{
		int key = ary[i];
		if (ASEARCH(key, ret) == -1)
		{
			if (ASEARCHEX(key, ary).size() >= (size_t)n)
				ret.push_back(key);
		}
	}
	return ret;
}

//七対子
int Shanten::GetShantenChitoitsu(const string_t tehai)
{
	vector<vector_str> hai_split = StringToArrayWithFuro(tehai);
	vector_str hai_normal = SortHai(hai_split[0]);
	vector_str hai_furo = hai_split[1];
	vector_str hai_ankan = hai_split[2];
	if (hai_furo.size() > 0 || hai_ankan.size() > 0)
		return SHANTEN_MAX;
	int count_toitsu = GetToitsu(hai_normal).size();
	int count_koritsu = Uniq(hai_normal).size() - count_toitsu;
	if (count_toitsu > 7)
		count_toitsu = 7;
	if (count_toitsu + count_koritsu > 7)
		count_koritsu = 7 - count_toitsu;
	return 13 - (2 * count_toitsu) - count_koritsu;
}

//国士無双
int Shanten::GetShantenKokushimusou(const string_t tehai)
{
	vector<vector_str> hai_split = StringToArrayWithFuro(tehai);
	vector_str hai_normal = SortHai(hai_split[0]);
	vector_str hai_furo = hai_split[1];
	vector_str hai_ankan = hai_split[2];
	if (hai_furo.size() > 0 || hai_ankan.size() > 0)
		return SHANTEN_MAX;
	string_t yaochu_string = L"1m9m1p9p1s9s1z2z3z4z5z6z7z";
	vector_str hai_yaochu;
	for (size_t i = 0; i < hai_normal.size(); i++)
	{
		if (yaochu_string.find(hai_normal[i]) != string_t::npos)
			hai_yaochu.push_back(hai_normal[i]);
	}
	int count_toitsu = GetToitsu(hai_yaochu).size();
	int count_type = Uniq(hai_yaochu).size();
	int has_toitsu = 0;
	if (count_toitsu > 0)
		has_toitsu = 1;
	return 13 - count_type - has_toitsu;
}

//門前清自摸和
int Shanten::GetShantenMenzen(const string_t tehai)
{
	vector<vector_str> hai_split = StringToArrayWithFuro(tehai);
	vector_str hai_normal = hai_split[0];
	vector_str hai_furo = hai_split[1];
	vector_str hai_ankan = hai_split[2];
	if (hai_furo.size() > 0)
		return SHANTEN_MAX;
	return GetShanten(tehai);
}

//役牌
int Shanten::GetShantenYakuhai(const string_t tehai, const string_t bafu_hai, const string_t jifu_hai)
{
	vector<vector_str> hai_split = StringToArrayWithFuro(tehai);
	vector_str hai_normal = hai_split[0];
	vector_str hai_furo = hai_split[1];
	vector_str hai_ankan = hai_split[2];
	vector_str kotsu = GetKotsu(hai_normal);
	vector_str yakuhai = { L"5z", L"6z", L"7z" };
	if (bafu_hai != L"")
		yakuhai.push_back(bafu_hai);
	if (jifu_hai != L"")
		yakuhai.push_back(jifu_hai);
	bool has_yakuhai_kotsu = false;
	for (size_t i = 0; i < kotsu.size(); i++)
	{
		if (ASEARCH(kotsu[i], yakuhai) >= 0)
			has_yakuhai_kotsu = true;
	}
	for (size_t i = 0; i < hai_furo.size(); i++)
	{
		string_t hai = hai_furo[i].substr(0, 2);
		if (ASEARCH(hai, yakuhai) >= 0)
			has_yakuhai_kotsu = true;
	}
	for (size_t i = 0; i < hai_ankan.size(); i++)
	{
		string_t hai = hai_ankan[i].substr(0, 2);
		if (ASEARCH(hai, yakuhai) >= 0)
			has_yakuhai_kotsu = true;
	}
	if (has_yakuhai_kotsu)
		return GetShanten(tehai);
	vector_str toitsu = GetToitsu(hai_normal);
	bool has_yakuhai_toitsu = false;
	string_t yakuhai_target;
	for (size_t i = 0; i < toitsu.size(); i++)
	{
		if (ASEARCH(toitsu[i], yakuhai) >= 0)
		{
			has_yakuhai_toitsu = true;
			yakuhai_target = toitsu[i];
			break;
		}
	}
	if (!has_yakuhai_toitsu)
		return SHANTEN_MAX;
	string_t tehai_pon = RemoveElementByName(tehai, yakuhai_target, 2) + L"<" + yakuhai_target + yakuhai_target + yakuhai_target + L">";
	return GetShanten(tehai_pon) + 1;
}

//断么九
int Shanten::GetShantenTanyao(const string_t tehai)
{
	vector<vector_str> hai_split = StringToArrayWithFuro(tehai);
	vector_str hai_normal = hai_split[0];
	vector_str hai_furo = hai_split[1];
	vector_str hai_ankan = hai_split[2];
	const vector_str yaochu_array = StringToArray(L"1m9m1p9p1s9s1z2z3z4z5z6z7z");
	for (size_t i = 0; i < hai_furo.size(); i++)
	{
		vector_str a = StringToArray(hai_furo[i]);
		for (size_t j = 0; j < a.size(); j++)
		{
			if (ASEARCH(a[j], yaochu_array) >= 0)
				return SHANTEN_MAX;
		}
	}
	for (size_t i = 0; i < hai_ankan.size(); i++)
	{
		if (ASEARCH(hai_ankan[0].substr(0, 2), yaochu_array) >= 0)
			return SHANTEN_MAX;
	}
	string_t new_tehai;
	for (size_t i = 0; i < tehai.size(); i++)
	{
		if (tehai.substr(i, 1) == L"<")
		{
			if (tehai.substr(i + 7, 1) == L">")
			{
				new_tehai += tehai.substr(i, 8);
				i += 7;
			}
			else if (tehai.substr(i + 9, 1) == L">")
			{
				new_tehai += tehai.substr(i, 10);
				i += 9;
			}
		}
		else if (tehai.substr(i, 1) == L"(")
		{
			if (tehai.substr(i + 9, 1) == L")")
			{
				new_tehai += tehai.substr(i, 10);
				i += 9;
			}
		}
		else
		{
			if (tehai.substr(i, 1) == L"1" || tehai.substr(i, 1) == L"9" || tehai.substr(i + 1, 1) == L"z")
			{
				//除外
			}
			else
			{
				new_tehai += tehai.substr(i, 2);
			}
			i += 1;
		}
	}
	return GetShanten(new_tehai);
}

//対々和
int Shanten::GetShantenToitoi(const string_t tehai)
{
	vector<vector_str> hai_split = StringToArrayWithFuro(tehai);
	vector_str hai_normal = hai_split[0];
	vector_str hai_furo = hai_split[1];
	vector_str hai_ankan = hai_split[2];
	for (size_t i = 0; i < hai_furo.size(); i++)
	{
		vector_str a = StringToArray(hai_furo[i]);
		if (a[0] != a[1])
			return SHANTEN_MAX;
	}
	size_t count_kotsu = hai_furo.size() + hai_ankan.size() + GetKotsu(hai_normal).size();
	size_t count_toitsu = GetToitsu(hai_normal).size() - GetKotsu(hai_normal).size();
	if (count_kotsu + count_toitsu > 5)
		count_toitsu = 5 - count_kotsu;
	return 8 - (2 * count_kotsu) - count_toitsu;
}

//混一色
int Shanten::GetShantenHonitsu(const string_t tehai)
{
	int m = GetShantenHonitsuByColor(tehai, L"m");
	int p = GetShantenHonitsuByColor(tehai, L"p");
	int s = GetShantenHonitsuByColor(tehai, L"s");
	return (std::min)({ m, p, s });
}

//混一色(色指定)
int Shanten::GetShantenHonitsuByColor(const string_t tehai, const string_t color)
{
	vector<vector_str> hai_split = StringToArrayWithFuro(tehai);
	vector_str hai_normal = hai_split[0];
	vector_str hai_furo = hai_split[1];
	vector_str hai_ankan = hai_split[2];
	for (size_t i = 0; i < hai_furo.size(); i++)
	{
		if (hai_furo[i].substr(1, 1) != color)
			return SHANTEN_MAX;
	}
	for (size_t i = 0; i < hai_ankan.size(); i++)
	{
		if (hai_ankan[i].substr(1, 1) != color)
			return SHANTEN_MAX;
	}
	string_t new_tehai;
	for (size_t i = 0; i < tehai.size(); i++)
	{
		if (tehai.substr(i, 1) == L"<")
		{
			if (tehai.substr(i + 7, 1) == L">")
			{
				new_tehai += tehai.substr(i, 8);
				i += 7;
			}
			else if (tehai.substr(i + 9, 1) == L">")
			{
				new_tehai += tehai.substr(i, 10);
				i += 9;
			}
		}
		else if (tehai.substr(i, 1) == L"(")
		{
			if (tehai.substr(i + 9, 1) == L")")
			{
				new_tehai += tehai.substr(i, 10);
				i += 9;
			}
		}
		else
		{
			if (tehai.substr(i + 1, 1) != color && tehai.substr(i + 1, 1) != L"z")
			{
				//除外
			}
			else
			{
				new_tehai += tehai.substr(i, 2);
			}
			i += 1;
		}
	}
	return GetShanten(new_tehai);
}
