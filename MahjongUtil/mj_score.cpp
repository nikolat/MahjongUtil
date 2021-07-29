#include "mj_score.h"

//点数の取得(SAORIの戻り値となる文字列の作成)
int Score::GetScoreValues(
	const string_t tehai,
	const string_t agari_hai,
	const string_t bafu_hai,
	const string_t jifu_hai,
	const string_t dora_hai,
	const bool is_tsumo,
	const int richi,
	const bool is_ippatsu,
	const bool is_chankan,
	const bool is_rinshan,
	const bool is_final_tile_win,
	const bool is_1st_round,
	vector_str& values
)
{
	int fu;
	std::map<string_t, int> han;
	std::map<string_t, int> yakuman;
	int score = GetScore(
		tehai,
		agari_hai,
		bafu_hai,
		jifu_hai,
		dora_hai,
		is_tsumo,
		richi,
		is_ippatsu,
		is_chankan,
		is_rinshan,
		is_final_tile_win,
		is_1st_round,
		fu,
		han,
		yakuman
	);
	vector_str ret_values;
	int count_yakuman = 0;
	for (auto itr = yakuman.begin(); itr != yakuman.end(); ++itr) {
		count_yakuman += itr->second;
	}
	if (count_yakuman > 0)
	{
		ret_values.push_back(SAORI_FUNC::intToString(count_yakuman));
		for (auto itr = yakuman.begin(); itr != yakuman.end(); ++itr) {
			ret_values.push_back(itr->first + L"," + SAORI_FUNC::intToString(itr->second));
		}
		values = ret_values;
		return score;
	}
	ret_values.push_back(SAORI_FUNC::intToString(fu));
	int count_han = 0;
	for (auto itr = han.begin(); itr != han.end(); ++itr) {
		count_han += itr->second;
	}
	ret_values.push_back(SAORI_FUNC::intToString(count_han));
	for (auto itr = han.begin(); itr != han.end(); ++itr) {
		ret_values.push_back(itr->first + L"," + SAORI_FUNC::intToString(itr->second));
	}
	values = ret_values;
	return score;
}

//点数の取得(SAORIの戻り値となる役の情報を含む)
int Score::GetScore(
	const string_t tehai,
	const string_t agari_hai,
	const string_t bafu_hai,
	const string_t jifu_hai,
	const string_t dora_hai,
	const bool is_tsumo,
	const int richi,
	const bool is_ippatsu,
	const bool is_chankan,
	const bool is_rinshan,
	const bool is_finalTileWin,
	const bool is_1st_round,
	int& fu,
	std::map<string_t, int>& han,
	std::map<string_t, int>& yakuman
)
{
	bool is_oya = (jifu_hai == L"1z");
	vector<vector_str> hai_split = StringToArrayWithFuro(tehai);
	vector_str hai_normal = hai_split[0];
	vector_str hai_furo = hai_split[1];
	vector_str hai_ankan = hai_split[2];
	bool is_menzen = (hai_furo.size() == 0);
	hai_normal.push_back(agari_hai);
	hai_normal = SortHai(hai_normal);
	vector_str hai_plain = SortHai(StringToArrayPlain(tehai + agari_hai));
	vector_str composition;
	int shanten_normal = Shanten::GetShantenNormal(tehai + agari_hai, composition);
	bool is_normal = true;
	if (shanten_normal != -1)
	{
		composition.clear();
		is_normal = false;
	}
	//仮に最初の面子セットで役判定
	vector_str composition_group_first;
	string_t atama_hai;
	vector<vector_str> mentsu;
	int count_kantsu = 0;
	if (is_normal)
	{
		composition_group_first = SPLIT(composition[0], L",");
		atama_hai = StringToArray(composition_group_first[0])[0];
		for (size_t i = 1; i < composition_group_first.size(); i++)
		{
			if (composition_group_first[i].substr(0, 1) == L"<")
			{
				string_t s = composition_group_first[i].substr(1, composition_group_first[i].size() - 2);
				if (s.size() == 8)
					count_kantsu++;
				mentsu.push_back(StringToArray(s));
			}
			else if (composition_group_first[i].substr(0, 1) == L"(")
			{
				string_t s = composition_group_first[i].substr(1, 8);
				count_kantsu++;
				mentsu.push_back(StringToArray(s));
			}
			else
			{
				mentsu.push_back(StringToArray(composition_group_first[i]));
			}
		}
	}
	std::map<string_t, int> ret_han;
	std::map<string_t, int> ret_yakuman;
	//役満判定
	if (is_1st_round && is_tsumo && is_oya)
	{
		ret_yakuman[L"天和"] = 1;
	}
	if (is_1st_round && is_tsumo && !is_oya)
	{
		ret_yakuman[L"地和"] = 1;
	}
	if (is_normal && IsRyuiso(hai_plain))
	{
		ret_yakuman[L"緑一色"] = 1;
	}
	if (IsTsuiso(hai_plain))
	{
		ret_yakuman[L"字一色"] = 1;
	}
	if (is_normal && IsDaisangen(mentsu))
	{
		ret_yakuman[L"大三元"] = 1;
	}
	if (is_normal && IsShousushi(mentsu))
	{
		ret_yakuman[L"小四喜"] = 1;
	}
	if (is_normal && IsDaisushi(mentsu))
	{
		ret_yakuman[L"大四喜"] = 2;
	}
	if (!is_normal && Shanten::GetShantenKokushimusou(tehai + agari_hai) == -1)
	{
		if (ASEARCH(agari_hai, hai_normal) >= 0)
		{
			ret_yakuman[L"国士無双"] = 2;
		}
		else
		{
			ret_yakuman[L"国士無双"] = 1;
		}
	}
	if (is_normal && is_menzen && IsChuren(hai_normal))
	{
		size_t n = ASEARCHEX(agari_hai, hai_normal).size();
		if (n == 1 || n == 3)
		{
			ret_yakuman[L"九蓮宝燈"] = 2;
		}
		else
		{
			ret_yakuman[L"九蓮宝燈"] = 1;
		}
	}
	if (is_normal && IsChinroutou(hai_plain))
	{
		ret_yakuman[L"清老頭"] = 1;
	}
	if (count_kantsu == 4)
	{
		ret_yakuman[L"四槓子"] = 1;
	}
	//役判定
	if (is_tsumo && is_menzen)
	{
		ret_han[L"門前清自摸和"] = 1;
	}
	if (richi == 2)
	{
		ret_han[L"W立直"] = 2;
	}
	else if (richi == 1)
	{
		ret_han[L"立直"] = 1;
	}
	if (is_ippatsu)
	{
		ret_han[L"一発"] = 1;
	}
	if (IsTanyao(hai_plain))
	{
		ret_han[L"断ヤオ九"] = 1;
	}
	int count_yakuhai = 0;
	if (is_normal)
		count_yakuhai = CountYakuhai(hai_plain, bafu_hai, jifu_hai);
	if (count_yakuhai >= 1)
	{
		ret_han[L"役牌"] = count_yakuhai;
	}
	if (is_finalTileWin && is_tsumo && !is_rinshan)
	{
		ret_han[L"海底撈月"] = 1;
	}
	if (is_finalTileWin && !is_tsumo && !is_chankan)
	{
		ret_han[L"河底撈魚"] = 1;
	}
	if (is_rinshan)
	{
		ret_han[L"嶺上開花"] = 1;
	}
	if (is_chankan)
	{
		ret_han[L"槍槓"] = 1;
	}
	int count_peko = 0;
	if (is_normal)
		count_peko = CountPeko(mentsu);
	if (count_peko != 2 && Shanten::GetShantenChitoitsu(tehai + agari_hai) == -1)
	{
		ret_han[L"七対子"] = 2;
	}
	int is_honrou = IsHonroutou(hai_plain);
	if (is_honrou)
	{
		ret_han[L"混老頭"] = 2;
	}
	if (is_normal && IsShousangen(mentsu))
	{
		ret_han[L"小三元"] = 2;
	}
	if (count_kantsu == 3)
	{
		ret_han[L"三槓子"] = 2;
	}
	if (IsHonitsu(hai_plain))
	{
		if (is_menzen)
			ret_han[L"混一色"] = 3;
		else
			ret_han[L"混一色"] = 2;
	}
	if (IsChinitsu(hai_plain))
	{
		if (is_menzen)
			ret_han[L"清一色"] = 6;
		else
			ret_han[L"清一色"] = 5;
	}
	int count_dora = CountDora(hai_plain, StringToArray(dora_hai));
	if (count_dora > 0)
	{
		ret_han[L"ドラ"] = count_dora;
	}
	//面子の形によって変わる役の判定
	std::map<string_t, int> ret_han_sub;
	std::map<string_t, int> ret_yakuman_sub;
	int yakuman_sub_max = -1;
	int han_sub_max = -1;
	int fu_sub_max = -1;
	for (size_t i = 0; i < composition.size(); i++)
	{
		std::map<string_t, int> ret_han_sub_tmp;
		std::map<string_t, int> ret_yakuman_sub_tmp;
		int fu_sub = GetYakuInfoFromComposition(is_tsumo, agari_hai, bafu_hai, jifu_hai, composition[i], ret_han_sub_tmp, ret_yakuman_sub_tmp);
		int yakuman_sub = 0;
		int han_sub = 0;
		for (auto itr = ret_yakuman_sub_tmp.begin(); itr != ret_yakuman_sub_tmp.end(); ++itr) {
			yakuman_sub += itr->second;
		}
		for (auto itr = ret_han_sub_tmp.begin(); itr != ret_han_sub_tmp.end(); ++itr) {
			han_sub += itr->second;
		}
		if (yakuman_sub_max < yakuman_sub || yakuman_sub_max == yakuman_sub && han_sub_max < han_sub || yakuman_sub_max == yakuman_sub && han_sub_max == han_sub && fu_sub_max < fu_sub)
		{
			yakuman_sub_max = yakuman_sub;
			han_sub_max = han_sub;
			fu_sub_max = fu_sub;
			ret_yakuman_sub = ret_yakuman_sub_tmp;
			ret_han_sub = ret_han_sub_tmp;
		}
	}
	//役の合成
	for (auto itr = ret_yakuman_sub.begin(); itr != ret_yakuman_sub.end(); ++itr) {
		ret_yakuman[itr->first] = itr->second;
	}
	for (auto itr = ret_han_sub.begin(); itr != ret_han_sub.end(); ++itr) {
		ret_han[itr->first] = itr->second;
	}
	int count_yakuman = 0;
	for (auto itr = ret_yakuman.begin(); itr != ret_yakuman.end(); ++itr) {
		count_yakuman += itr->second;
	}
	int count_han = 0;
	for (auto itr = ret_han.begin(); itr != ret_han.end(); ++itr) {
		count_han += itr->second;
	}

	int count_fu = 0;
	if (ret_han.find(L"平和") != ret_han.end() && ret_han.find(L"門前清自摸和") != ret_han.end())
		count_fu = 20;
	else if (ret_han.find(L"七対子") != ret_han.end())
		count_fu = 25;
	else if (ret_han.find(L"国士無双") == ret_han.end())
		count_fu = fu_sub_max;
	int score = GetScorePoint(count_yakuman, count_han, count_dora, count_fu, is_oya);
	fu = count_fu;
	yakuman = ret_yakuman;
	han = ret_han;
	return score;
}

//面子の形によって変わる役の情報を取得
int Score::GetYakuInfoFromComposition(
	const bool is_tsumo,
	const string_t agari_hai,
	const string_t bafu_hai,
	const string_t jifu_hai,
	const string_t composition,
	std::map<string_t, int>& han,
	std::map<string_t, int>& yakuman
)
{
	std::map<string_t, int> ret_han;
	std::map<string_t, int> ret_yakuman;

	vector_str composition_array = SPLIT(composition, L",");
	string_t atama_hai = StringToArray(composition_array[0])[0];
	vector<vector_str> mentsu;
	vector<vector_str> mentsu_furo;
	vector<vector_str> mentsu_without_furo;
	bool is_menzen = true;
	for (size_t i = 1; i < composition_array.size(); i++)
	{
		if (composition_array[i].substr(0, 1) == L"<")
		{
			string_t s = composition_array[i].substr(1, composition_array[i].size() - 2);
			mentsu.push_back(StringToArray(s));
			mentsu_furo.push_back(StringToArray(s));
			is_menzen = false;
		}
		else if (composition_array[i].substr(0, 1) == L"(")
		{
			string_t s = composition_array[i].substr(1, 8);
			mentsu.push_back(StringToArray(s));
			mentsu_without_furo.push_back(StringToArray(s));
		}
		else
		{
			mentsu.push_back(StringToArray(composition_array[i]));
			mentsu_without_furo.push_back(StringToArray(composition_array[i]));
		}
	}
	//役満判定
	int count_ankou = CountAnkou(mentsu_without_furo, is_tsumo, agari_hai, atama_hai);
	if (count_ankou == 4)
	{
		if (agari_hai == atama_hai)
		{
			ret_yakuman[L"四暗刻"] = 2;
		}
		else
		{
			ret_yakuman[L"四暗刻"] = 1;
		}
	}
	//役判定
	if (is_menzen && IsPinfu(atama_hai, mentsu, agari_hai, bafu_hai, jifu_hai))
	{
		ret_han[L"平和"] = 1;
	}
	int count_peko = 0;
	if (is_menzen)
		count_peko = CountPeko(mentsu);
	if (count_peko == 1)
	{
		ret_han[L"一盃口"] = 1;
	}
	if (count_ankou == 3)
	{
		ret_han[L"三暗刻"] = 2;
	}
	if (IsSanshokudoukou(mentsu))
	{
		ret_han[L"三色同刻"] = 2;
	}
	if (IsSanshokudoujun(mentsu))
	{
		if (is_menzen)
			ret_han[L"三色同順"] = 2;
		else
			ret_han[L"三色同順"] = 1;
	}
	if (IsIkkitsuukan(mentsu))
	{
		if (is_menzen)
			ret_han[L"一気通貫"] = 2;
		else
			ret_han[L"一気通貫"] = 1;
	}
	if (IsToitoi(mentsu))
	{
		ret_han[L"対々和"] = 2;
	}
	if (IsChanta(mentsu, atama_hai))
	{
		if (is_menzen)
			ret_han[L"混全帯ヤオ九"] = 2;
		else
			ret_han[L"混全帯ヤオ九"] = 1;
	}
	if (IsJunchan(mentsu, atama_hai))
	{
		if (is_menzen)
			ret_han[L"純全帯ヤオ九"] = 3;
		else
			ret_han[L"純全帯ヤオ九"] = 2;
	}
	if (count_peko == 2)
	{
		ret_han[L"二盃口"] = 3;
	}
	int fu;
	if (ret_han.find(L"平和") != ret_han.end() && is_tsumo)
		fu = 20;
	else if (ret_han.find(L"平和") != ret_han.end() && !is_tsumo)
		fu = 30;
	else
		fu = GetFu(atama_hai, mentsu_without_furo, mentsu_furo, agari_hai, bafu_hai, jifu_hai, is_tsumo, is_menzen);
	yakuman = ret_yakuman;
	han = ret_han;
	return fu;
}

//符を取得
int Score::GetFu(
	const string_t atama_hai,
	const vector<vector_str> mentsu_without_furo,
	const vector<vector_str> mentsu_furo,
	const string_t agari_hai,
	const string_t bafu_hai,
	const string_t jifu_hai,
	const int is_tsumo,
	const int is_menzen
)
{
	//(1)副底20符
	int fu = 20;
	//(2)門前加符 or ツモ符
	if (is_tsumo)
		fu += 2;
	else if (is_menzen)
		fu += 10;
	//(3)各面子
	string_t yaochu_string = L"1m9m1p9p1s9s1z2z3z4z5z6z7z";
	for (size_t i = 0; i < mentsu_without_furo.size(); i++)
	{
		if (mentsu_without_furo[i][0] == mentsu_without_furo[i][1])
		{
			if (mentsu_without_furo[i].size() == 4)
			{
				if (yaochu_string.find(mentsu_without_furo[i][0]) != string_t::npos)
					fu += 32;
				else
					fu += 16;
			}
			else
			{
				if (yaochu_string.find(mentsu_without_furo[i][0]) != string_t::npos)
					fu += 8;
				else
					fu += 4;
			}
		}
	}
	for (size_t i = 0; i < mentsu_furo.size(); i++)
	{
		if (mentsu_furo[i][0] == mentsu_furo[i][1])
		{
			if (mentsu_furo[i].size() == 4)
			{
				if (yaochu_string.find(mentsu_furo[i][0]) != string_t::npos)
					fu += 16;
				else
					fu += 8;
			}
			else
			{
				if (yaochu_string.find(mentsu_furo[i][0]) != string_t::npos)
					fu += 4;
				else
					fu += 2;
			}
		}
	}
	//(4)雀頭
	if (atama_hai == bafu_hai)
		fu += 2;
	if (atama_hai == jifu_hai)
		fu += 2;
	if (atama_hai == L"5z" || atama_hai == L"6z" || atama_hai == L"7z")
		fu += 2;
	//(5)待ち
	//待ちの形を再現する
	vector<vector<int>> machi_tatsu;
	int fu_machi;
	int fu_machi_add = -99;
	for (size_t i = 0; i < mentsu_without_furo.size(); i++)
	{
		if (ASEARCH(agari_hai, mentsu_without_furo[i]) >= 0)
			machi_tatsu.push_back(StringArrayToNumArray(RemoveElementByName(mentsu_without_furo[i], agari_hai, 1)));
	}
	for (size_t i = 0; i < machi_tatsu.size(); i++)
	{
		if ((machi_tatsu[i][0] != 1) && (machi_tatsu[i][0] + 1 == machi_tatsu[i][1]) && (machi_tatsu[i][1] != 9))//両面待ち
		{
			fu_machi = 0;
			if (fu_machi_add < fu_machi)
				fu_machi_add = fu_machi;
		}
		else if (machi_tatsu[i][0] == machi_tatsu[i][1])//シャボ待ち
		{
			if (is_tsumo)
			{
				fu_machi = 0;
				if (fu_machi_add < fu_machi)
					fu_machi_add = fu_machi;
			}
			else
			{
				if (yaochu_string.find(agari_hai) != string_t::npos)
				{
					fu_machi = -4;
					if (fu_machi_add < fu_machi)
						fu_machi_add = fu_machi;
				}
				else
				{
					fu_machi = -2;
					if (fu_machi_add < fu_machi)
						fu_machi_add = fu_machi;
				}
			}
		}
		else if (machi_tatsu[i][0] == 1 && machi_tatsu[i][1] == 2 || machi_tatsu[i][0] == 8 && machi_tatsu[i][1] == 9)//ペンチャン待ち
		{
			fu_machi = 2;
			if (fu_machi_add < fu_machi)
				fu_machi_add = fu_machi;
		}
		else if (machi_tatsu[i][0] + 2 == machi_tatsu[i][1])//カンチャン待ち
		{
			fu_machi = 2;
			if (fu_machi_add < fu_machi)
				fu_machi_add = fu_machi;
		}
	}
	if (atama_hai == agari_hai)//単騎待ち
	{
		fu_machi = 2;
		if (fu_machi_add < fu_machi)
			fu_machi_add = fu_machi;
	}
	fu += fu_machi_add;
	//切り上げ
	int shou = fu / 10;
	int amari = fu % 10;
	if (amari > 0)
		fu = 10 * shou + 10;
	if (fu == 20)
		fu = 30;
	return fu;
}

//点数を取得
int Score::GetScorePoint(const int count_yakuman, const int count_han, const int count_dora, const int count_fu, const bool is_oya)
{
	int r;
	if (count_yakuman > 0)
	{
		r = 32000 * count_yakuman;
	}
	else
	{
		if (count_han - count_dora == 0)
			r = 0;
		else if (count_han >= 13)
			r = 32000;
		else if (count_han >= 11)
			r = 24000;
		else if (count_han >= 8)
			r = 16000;
		else if (count_han >= 6)
			r = 12000;
		else if (count_han >= 5)
			r = 8000;
		else
			return GetScorePointWithFu(count_han, count_fu, is_oya);
	}
	if (is_oya)
		r = (int)(r * 1.5);
	return r;
}

//符を用いた点数計算
int Score::GetScorePointWithFu(const int count_han, const int count_fu, const bool is_oya)
{
	std::string key = std::to_string(count_fu) + "-" + std::to_string(count_han);
	if (is_oya)
	{
		std::map<std::string, int> d{
								{"20-2" , 2000} ,	{"20-3", 3900} ,	{"20-4", 7700} ,
								{"25-2" , 2400} ,	{"25-3", 4800} ,	{"25-4", 9600} ,
			{"30-1" , 1500},	{"30-2" , 2900} ,	{"30-3", 5800} ,	{"30-4", 11600},
			{"40-1" , 2000},	{"40-2" , 3900} ,	{"40-3", 7700} ,
			{"50-1" , 2400},	{"50-2" , 4800} ,	{"50-3", 9600} ,
			{"60-1" , 2900},	{"60-2" , 5800} ,	{"60-3", 11600},
			{"70-1" , 3400},	{"70-2" , 6800} ,
			{"80-1" , 3900},	{"80-2" , 7700} ,
			{"90-1" , 4400},	{"90-2" , 8700} ,
			{"100-1", 4800},	{"100-2", 9600} ,
			{"110-1", 5300},	{"110-2", 10600}
		};
		int score;
		if (d.find(key) != d.end())
			score = d[key];
		else
			score = 12000;
		return score;
	}
	else
	{
		std::map<std::string, int> d{
								{"20-2" , 1300},	{"20-3", 2600},	{"20-4", 5200},
								{"25-2" , 1600},	{"25-3", 3200},	{"25-4", 6400},
			{"30-1" , 1000},	{"30-2" , 2000},	{"30-3", 3900},	{"30-4", 7700},
			{"40-1" , 1300},	{"40-2" , 2600},	{"40-3", 5200},
			{"50-1" , 1600},	{"50-2" , 3200},	{"50-3", 6400},
			{"60-1" , 2000},	{"60-2" , 3900},	{"60-3", 7700},
			{"70-1" , 2300},	{"70-2" , 4500},
			{"80-1" , 2600},	{"80-2" , 5200},
			{"90-1" , 2900},	{"90-2" , 5800},
			{"100-1", 3200},	{"100-2", 6400},
			{"110-1", 3600},	{"110-2", 7100}
		};
		int score;
		if (d.find(key) != d.end())
			score = d[key];
		else
			score = 8000;
		return score;
	}
}

//断么九判定
bool Score::IsTanyao(const vector_str hai_plain)
{
	string_t yaochu_string = L"1m9m1p9p1s9s1z2z3z4z5z6z7z";
	for (size_t i = 0; i < hai_plain.size(); i++)
	{
		if (yaochu_string.find(hai_plain[i]) != string_t::npos)
			return false;
	}
	return true;
}

//盃口カウント
int Score::CountPeko(const vector<vector_str> mentsu)
{
	int count_peko = 0;
	vector<int> used_index;
	for (size_t i = 0; i < mentsu.size(); i++)
	{
		if (ASEARCH(i, used_index) >= 0)
			continue;
		vector_str prev = mentsu[i];
		for (size_t j = i + 1; j < mentsu.size(); j++)
		{
			if (ASEARCH(j, used_index) >= 0)
				continue;
			vector_str next = mentsu[j];
			if (prev[0] == next[0] && prev[1] == next[1] && prev[2] == next[2])
			{
				count_peko++;
				used_index.push_back(i);
				used_index.push_back(j);
				break;
			}
		}
	}
	return count_peko;
}

//役牌カウント
int Score::CountYakuhai(const vector_str hai_plain, const string_t bafu_hai, const string_t jifu_hai)
{
	int count_yakuhai = 0;
	if (ASEARCHEX(bafu_hai, hai_plain).size() >= 3)
		count_yakuhai++;
	if (ASEARCHEX(jifu_hai, hai_plain).size() >= 3)
		count_yakuhai++;
	if (ASEARCHEX(L"5z", hai_plain).size() >= 3)
		count_yakuhai++;
	if (ASEARCHEX(L"6z", hai_plain).size() >= 3)
		count_yakuhai++;
	if (ASEARCHEX(L"7z", hai_plain).size() >= 3)
		count_yakuhai++;
	return count_yakuhai;
}

//対々和判定
bool Score::IsToitoi(const vector<vector_str> mentsu)
{
	if (mentsu.size() < 4)
		return false;
	//すべて刻子
	for (size_t i = 0; i < mentsu.size(); i++)
	{
		if (!(mentsu[i][0] == mentsu[i][1] && mentsu[i][0] == mentsu[i][2]))
			return false;
	}
	return true;
}

//混老頭判定
bool Score::IsHonroutou(const vector_str hai_plain)
{
	string_t yaochu_string = L"1m9m1p9p1s9s1z2z3z4z5z6z7z";
	for (size_t i = 0; i < hai_plain.size(); i++)
	{
		if (yaochu_string.find(hai_plain[i]) == string_t::npos)
			return false;
	}
	return true;
}

//混全帯么九、純全帯么九判定用
int Score::SubChanta(const vector<vector_str> mentsu, string_t atama_hai)
{
	if (mentsu.size() < 4)
		return 0;
	string_t yaochu_string = L"1m9m1p9p1s9s1z2z3z4z5z6z7z";
	string_t jihai_string = L"1z2z3z4z5z6z7z";
	bool has_jihai = false;
	bool is_honrou = true;
	if (yaochu_string.find(atama_hai) != string_t::npos)
	{
		if (jihai_string.find(atama_hai) != string_t::npos)
			has_jihai = true;
	}
	else
	{
		return 0;
	}
	for (size_t i = 0; i < mentsu.size(); i++)
	{
		bool has_yaochu = false;
		if (mentsu[i][0] != mentsu[i][1])
			is_honrou = false;
		for (size_t j = 0; j < mentsu[i].size(); j++)
		{
			if (yaochu_string.find(mentsu[i][j]) != string_t::npos)
			{
				if (jihai_string.find(mentsu[i][j]) != string_t::npos)
					has_jihai = true;
				has_yaochu = true;
				break;
			}
		}
		if (!has_yaochu)
			return 0;
	}
	//混老頭・清老頭とは複合しない
	if (is_honrou)
		return 0;
	if (has_jihai)
		return 1;//混全帯么九
	else
		return 2;//純全帯么九
}

//混全帯么九判定
bool Score::IsChanta(const vector<vector_str> mentsu, string_t atama_hai)
{
	int r = SubChanta(mentsu, atama_hai);
	if (r == 1)
		return true;
	return false;
}

//純全帯么九判定
bool Score::IsJunchan(const vector<vector_str> mentsu, string_t atama_hai)
{
	int r = SubChanta(mentsu, atama_hai);
	if (r == 2)
		return true;
	return false;
}

//小三元判定
bool Score::IsShousangen(const vector<vector_str>  mentsu)
{
	int count_sangenpai = 0;
	bool use_atama = false;
	for (size_t i = 0; i < mentsu.size(); i++)
	{
		if (mentsu[i].size() == 2)
		{
			if (mentsu[i][0] == L"5z" || mentsu[i][0] == L"6z" || mentsu[i][0] == L"7z")
			{
				count_sangenpai++;
				use_atama = true;
			}
		}
		else
		{
			if (mentsu[i][0] == L"5z" || mentsu[i][0] == L"6z" || mentsu[i][0] == L"7z")
				count_sangenpai++;
		}
	}
	if (count_sangenpai == 3 && use_atama)
		return true;
	return false;
}

//大三元判定
bool Score::IsDaisangen(const vector<vector_str>  mentsu)
{
	int count_sangenpai = 0;
	for (size_t i = 0; i < mentsu.size(); i++)
	{
		if (mentsu[i].size() >= 3)
		{
			if (mentsu[i][0] == L"5z" || mentsu[i][0] == L"6z" || mentsu[i][0] == L"7z")
				count_sangenpai++;
		}
	}
	if (count_sangenpai == 3)
		return true;
	return false;
}

//混一色、清一色、字一色判定用
vector<bool> Score::GetUsedHaiGroup(const vector_str hai)
{
	vector<bool> a = { false, false, false, false };
	for (size_t i = 0; i < hai.size(); i++)
	{
		string_t s = hai[i].substr(1, 1);
		if (s == L"m")
			a[0] = true;
		else if (s == L"p")
			a[1] = true;
		else if (s == L"s")
			a[2] = true;
		else if (s == L"z")
			a[3] = true;
	}
	return a;
}

//混一色判定
bool Score::IsHonitsu(const vector_str hai_plain)
{
	vector<bool> a = GetUsedHaiGroup(hai_plain);
	if (a[0] && a[1] || a[1] && a[2] || a[2] && a[0] || !a[3])
		return false;
	return true;
}

//清一色判定
bool Score::IsChinitsu(const vector_str hai_plain)
{
	vector<bool> a = GetUsedHaiGroup(hai_plain);
	if (a[0] && a[1] || a[1] && a[2] || a[2] && a[0] || a[3])
		return false;
	return true;
}

//字一色判定
bool Score::IsTsuiso(const vector_str hai_plain)
{
	vector<bool> a = GetUsedHaiGroup(hai_plain);
	if (!a[0] && !a[1] && !a[2])
		return true;
	return false;
}

//緑一色判定
bool Score::IsRyuiso(const vector_str hai_plain)
{
	string_t green_string = L"2s3s4s6s8s6z";
	for (size_t i = 0; i < hai_plain.size(); i++)
	{
		if (green_string.find(hai_plain[i]) == string_t::npos)
			return false;
	}
	return true;
}

//小四喜判定
bool Score::IsShousushi(const vector<vector_str> mentsu)
{
	int count_fupai = 0;
	bool use_atama = false;
	for (size_t i = 0; i < mentsu.size(); i++)
	{
		if (mentsu[i].size() == 2)
		{
			if (mentsu[i][0] == L"1z" || mentsu[i][0] == L"2z" || mentsu[i][0] == L"3z" || mentsu[i][0] == L"4z")
			{
				count_fupai++;
				use_atama = true;
			}
		}
		else
		{
			if (mentsu[i][0] == L"1z" || mentsu[i][0] == L"2z" || mentsu[i][0] == L"3z" || mentsu[i][0] == L"4z")
				count_fupai++;
		}
	}
	if (count_fupai == 4 && use_atama)
		return true;
	return false;
}

//大四喜判定
bool Score::IsDaisushi(const vector<vector_str> mentsu)
{
	int count_fupai = 0;
	for (size_t i = 0; i < mentsu.size(); i++)
	{
		if (mentsu[i].size() >= 3)
		{
			if (mentsu[i][0] == L"1z" || mentsu[i][0] == L"2z" || mentsu[i][0] == L"3z" || mentsu[i][0] == L"4z")
			{
				count_fupai++;
			}
		}
	}
	if (count_fupai == 4)
		return true;
	return false;
}

//九蓮宝燈判定
bool Score::IsChuren(const vector_str hai_plain)
{
	//清一色でない
	if (!IsChinitsu(hai_plain))
		return false;
	vector<int> hai_num = StringArrayToNumArray(hai_plain);
	string_t hai_str = JOIN(hai_num, L"");
	vector_str churen_array = {
		L"11112345678999",
		L"11122345678999",
		L"11123345678999",
		L"11123445678999",
		L"11123455678999",
		L"11123456678999",
		L"11123456778999",
		L"11123456788999",
		L"11123456789999"
	};
	if (ASEARCH(hai_str, churen_array) >= 0)
		return true;
	return false;
}

//清老頭判定
bool Score::IsChinroutou(const vector_str hai_plain)
{
	string_t routou_string = L"1m9m1p9p1s9s";
	for (size_t i = 0; i < hai_plain.size(); i++)
	{
		if (routou_string.find(hai_plain[i]) == string_t::npos)
			return 0;
	}
	return 1;
}

//ドラ牌カウント
int Score::CountDora(const vector_str hai_plain, const vector_str dora)
{
	int n = 0;
	for (size_t i = 0; i < dora.size(); i++)
	{
		n += ASEARCHEX(dora[i], hai_plain).size();
	}
	return n;
}

//暗刻カウント
int Score::CountAnkou(const vector<vector_str> mentsu, const bool is_tsumo, const string_t agari_hai, const string_t atama_hai)
{
	//暗刻をカウント
	int count_ankou = 0;
	bool is_ankou = false;
	for (size_t i = 0; i < mentsu.size(); i++)
	{
		if (mentsu[i][0] == mentsu[i][1])
		{
			count_ankou++;
		}
		else
		{
			if (ASEARCH(agari_hai, mentsu[i]) >= 0)
				is_ankou = true;//順子に和了牌が含まれている
		}
	}
	//雀頭に和了牌が含まれている
	if (atama_hai == agari_hai)
		is_ankou = true;
	//自摸なら常に暗刻
	if (is_tsumo)
		is_ankou = true;
	//暗刻でなく明刻のパターン
	if (!is_ankou)
		count_ankou--;
	return count_ankou;
}

//三色同刻判定
bool Score::IsSanshokudoukou(const vector<vector_str> mentsu)
{
	vector<int> kotsu_number;
	for (size_t i = 0; i < mentsu.size(); i++)
	{
		if (mentsu[i][0] == mentsu[i][1] && mentsu[i][0] == mentsu[i][2])
		{
			int n = stoi(mentsu[i][0].substr(0, 1));
			string_t color = mentsu[i][0].substr(1, 1);
			if (color != L"z")
				kotsu_number.push_back(n);
		}
	}
	if (kotsu_number.size() < 3)
		return false;
	for (size_t i = 0; i + 2 < kotsu_number.size(); i++)
	{
		if (ASEARCHEX(kotsu_number[i], kotsu_number).size() == 3)
			return true;
	}
	return false;
}

//三色同順判定
bool Score::IsSanshokudoujun(const vector<vector_str> mentsu)
{
	vector_str shuntsu_string;
	vector<int> shuntsu_number;
	for (size_t i = 0; i < mentsu.size(); i++)
	{
		if (mentsu[i][0] != mentsu[i][1])
		{
			int n = stoi(mentsu[i][0].substr(0, 1));
			if (ASEARCH(mentsu[i][0], shuntsu_string) == -1)
			{
				shuntsu_string.push_back(mentsu[i][0]);
				shuntsu_number.push_back(n);
			}
		}
	}
	if (shuntsu_number.size() < 3)
		return false;
	for (size_t i = 0; i + 2 < shuntsu_number.size(); i++)
	{
		if (ASEARCHEX(shuntsu_number[i], shuntsu_number).size() == 3)
			return true;
	}
	return false;
}

//一気通貫判定
bool Score::IsIkkitsuukan(const vector<vector_str> mentsu)
{
	vector_str shuntsu_string;
	for (size_t i = 0; i < mentsu.size(); i++)
	{
		if (mentsu[i][0] != mentsu[i][1])
		{
			int n = stoi(mentsu[i][0].substr(0, 1));
			if (ASEARCH(mentsu[i][0], shuntsu_string) == -1 && (n == 1 || n == 4 || n == 7))
			{
				shuntsu_string.push_back(mentsu[i][0]);
			}
		}
	}
	if (shuntsu_string.size() < 3)
		return false;
	if (ASEARCH(L"1m", shuntsu_string) >= 0 && ASEARCH(L"4m", shuntsu_string) >= 0 && ASEARCH(L"7m", shuntsu_string) >= 0)
		return true;
	if (ASEARCH(L"1p", shuntsu_string) >= 0 && ASEARCH(L"4p", shuntsu_string) >= 0 && ASEARCH(L"7p", shuntsu_string) >= 0)
		return true;
	if (ASEARCH(L"1s", shuntsu_string) >= 0 && ASEARCH(L"4s", shuntsu_string) >= 0 && ASEARCH(L"7s", shuntsu_string) >= 0)
		return true;
	return false;
}

//平和判定
bool Score::IsPinfu(const string_t atama_hai, const vector<vector_str> mentsu, const string_t agari_hai, const string_t bafu_hai, const string_t jifu_hai)
{
	//一般手の和了でない
	if (atama_hai == L"")
		return false;
	//雀頭は役牌以外でないといけない
	if (atama_hai == bafu_hai || atama_hai == jifu_hai || atama_hai == L"5z" || atama_hai == L"6z" || atama_hai == L"7z")
		return false;
	//待ちの形を再現する
	vector<vector<int>> machi_tatsu;
	for (size_t i = 0; i < mentsu.size(); i++)
	{
		//面子はすべて順子でなければならない
		if (mentsu[i][0] == mentsu[i][1])
			return false;
		if (ASEARCH(agari_hai, mentsu[i]) >= 0)
			machi_tatsu.push_back(StringArrayToNumArray(RemoveElementByName(mentsu[i], agari_hai, 1)));
	}
	for (size_t i = 0; i < machi_tatsu.size(); i++)
	{
		if ((machi_tatsu[i][0] != 1) && (machi_tatsu[i][0] + 1 == machi_tatsu[i][1]) && (machi_tatsu[i][1] != 9))//両面待ち
			return true;
	}
	return false;
}
