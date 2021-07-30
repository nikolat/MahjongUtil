#ifdef _MSC_VER
#pragma warning( disable : 4786 )
#endif

#include "MahjongUtil.h"

/*---------------------------------------------------------
	初期化
---------------------------------------------------------*/
bool CSAORIMahjongUtil::load()
{
	return true;
}

/*---------------------------------------------------------
	解放
---------------------------------------------------------*/
bool CSAORIMahjongUtil::unload()
{
	return true;
}

/*---------------------------------------------------------
	実行
---------------------------------------------------------*/
void CSAORIMahjongUtil::exec(const CSAORIInput& in, CSAORIOutput& out) {
	out.result_code = SAORIRESULT_BAD_REQUEST;
	if (in.args.size() < 2) { return; }
	string_t command = in.args[0];
	string_t tehai = in.args[1];
	if (command != L"shanten" && command != L"shanten_normal" && command != L"shanten_yaku" && command != L"machi" && command != L"score") { return; }
	if (!IsValidTehai(tehai)) { return; }

	if (command == L"shanten" || command == L"shanten_normal")
	{
		int shanten;
		vector_str composition;
		if (command == L"shanten")
			shanten = Shanten::GetShanten(tehai, composition);
		else if (command == L"shanten_normal")
			shanten = Shanten::GetShantenNormal(tehai, composition);
		out.result = SAORI_FUNC::numToString(shanten);
		for (size_t i = 0; i < composition.size(); i++)
		{
			out.values.push_back(composition[i]);
		}
		out.result_code = SAORIRESULT_OK;
	}
	else if (command == L"shanten_yaku")
	{
		string_t bafu_hai = GetStringOfArgument(in, 2);
		if (bafu_hai != L"" && !IsValidFupai(bafu_hai)) { return; }
		string_t jifu_hai = GetStringOfArgument(in, 3);
		if (jifu_hai != L"" && !IsValidFupai(jifu_hai)) { return; }
		vector_str yaku;
		int shanten = Shanten::GetShantenYaku(tehai, bafu_hai, jifu_hai, yaku);
		if (shanten >= SHANTEN_MAX)
		{
			out.result_code = SAORIRESULT_NO_CONTENT;
			return;
		}
		out.result = SAORI_FUNC::numToString(shanten);
		for (size_t i = 0; i < yaku.size(); i++)
		{
			out.values.push_back(yaku[i]);
		}
		out.result_code = SAORIRESULT_OK;
	}
	else if (command == L"machi")
	{
		if (CountEffectiveNumberOfHai(tehai) != 13) { return; }
		if (Shanten::GetShanten(tehai) != 0) { return; }
		string_t machi = Machi::GetMachi(tehai);
		out.result = machi;
		out.result_code = SAORIRESULT_OK;
	}
	else if (command == L"score")
	{
		if (CountEffectiveNumberOfHai(tehai) != 13) { return; }
		if (in.args.size() < 3) { return; }
		string_t agari_hai = in.args[2];
		if (!IsValidHaiSingle(agari_hai)) { return; }
		if (Shanten::GetShanten(tehai + agari_hai) != -1) { return; }
		string_t bafu_hai = GetStringOfArgument(in, 3);
		if (bafu_hai != L"" && !IsValidFupai(bafu_hai)) { return; }
		string_t jifu_hai = GetStringOfArgument(in, 4);
		if (jifu_hai != L"" && !IsValidFupai(jifu_hai)) { return; }
		string_t dora_hai = GetStringOfArgument(in, 5);
		if (dora_hai != L"" && !IsValidHaiWithoutFuro(dora_hai)) { return; }
		bool is_tsumo = GetFlagOfArgument(in, 6);
		int richi = GetNumberOfArgument(in, 7);
		bool is_ippatsu = GetFlagOfArgument(in, 8);
		bool is_chankan = GetFlagOfArgument(in, 9);
		bool is_rinshan = GetFlagOfArgument(in, 10);
		bool is_final_tile_win = GetFlagOfArgument(in, 11);
		bool is_1st_round = GetFlagOfArgument(in, 12);
		//矛盾が無いかチェック
		bool is_menzen = (StringToArrayWithFuro(tehai)[1].size() == 0);
		bool is_richi = (richi > 0);
		bool is_wrichi = (richi == 2);
		if (!is_menzen && is_richi || !is_menzen && is_1st_round || is_tsumo && is_chankan || !is_tsumo && is_rinshan || !is_richi && is_ippatsu
			|| is_wrichi && is_chankan || is_richi && is_1st_round || is_ippatsu && is_rinshan || is_final_tile_win && is_1st_round)
			return;
		vector_str values;
		int score = Score::GetScoreValues(
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
			values
		);
		out.result = SAORI_FUNC::numToString(score);
		for (size_t i = 0; i < values.size(); i++)
		{
			out.values.push_back(values[i]);
		}
		out.result_code = SAORIRESULT_OK;
	}
}

//入力値チェック:手牌
bool CSAORIMahjongUtil::IsValidTehai(const string_t tehai)
{
	std::regex re(R"(^([1-9][mps]|[1-7]z)+(<([1-9][mps]|[1-7]z)\3{3}>|\(([1-9][mps]|[1-7]z)\4{3}\)|<([1-9]m){3}>|<([1-9]p){3}>|<([1-9]s){3}>|<([1-7]z){3}>){0,4}$)");
	if (!regex_match(SAORI_FUNC::UnicodeToMultiByte(tehai), re))
		return false;
	//刻子や順子以外は弾くべき
	vector_str furo = StringToArrayWithFuro(tehai)[1];
	for (size_t i = 0; i < furo.size(); i++)
	{
		vector<int> a = StringArrayToNumArray(StringToArray(furo[i]));
		if (a.size() == 3)
		{
			string_t color = furo[i].substr(1, 1);
			if (!(color != L"z" && a[0] + 1 == a[1] && a[1] + 1 == a[2] || a[0] == a[1] && a[1] == a[2]))
				return false;
		}
	}
	//同一牌が4枚を超える場合は弾くべき
	vector_str tehai_plain = SortHai(StringToArrayPlain(tehai));
	vector_str tehai_uniq = Uniq(tehai_plain);
	for (size_t i = 0; i < tehai_uniq.size(); i++)
	{
		if (ASEARCHEX(tehai_uniq[i], tehai_plain).size() > 4)
			return false;
	}
	return true;
}

//入力値チェック:牌(1枚)
bool CSAORIMahjongUtil::IsValidHaiSingle(const string_t hai)
{
	std::string s = SAORI_FUNC::UnicodeToMultiByte(hai);
	std::regex re(R"(^[1-9][mps]|[1-7]z$)");
	return regex_match(s, re);
}

//入力値チェック:牌(副露無し)
bool CSAORIMahjongUtil::IsValidHaiWithoutFuro(const string_t hai)
{
	std::string s = SAORI_FUNC::UnicodeToMultiByte(hai);
	std::regex re(R"(^([1-9][mps]|[1-7]z)+$)");
	if (!regex_match(s, re))
		return false;
	//同一牌が4枚を超える場合は弾くべき
	vector_str hai_plain = SortHai(StringToArrayPlain(hai));
	vector_str hai_uniq = Uniq(hai_plain);
	for (size_t i = 0; i < hai_uniq.size(); i++)
	{
		if (ASEARCHEX(hai_uniq[i], hai_plain).size() > 4)
			return false;
	}
	return true;
}

//入力値チェック:風牌
bool CSAORIMahjongUtil::IsValidFupai(const string_t hai)
{
	std::string s = SAORI_FUNC::UnicodeToMultiByte(hai);
	std::regex re(R"(^[1-4]z$)");
	return regex_match(s, re);
}

//牌の有効枚数を取得
int CSAORIMahjongUtil::CountEffectiveNumberOfHai(const string_t tehai)
{
	vector<vector_str> a = StringToArrayWithFuro(tehai);
	vector_str hai_normal = SortHai(a[0]);
	vector_str hai_furo = a[1];
	vector_str hai_ankan = a[2];
	return hai_normal.size() + (3 * hai_furo.size()) + (3 * hai_ankan.size());
}

//引数取得:文字列型
string_t CSAORIMahjongUtil::GetStringOfArgument(const CSAORIInput& in, const size_t n)
{
	string_t ret = L"";
	if (in.args.size() >= n + 1)
	{
		ret = in.args[n];
	}
	return ret;
}

//引数取得:数値型
int CSAORIMahjongUtil::GetNumberOfArgument(const CSAORIInput& in, const size_t n)
{
	int ret = 0;
	if (in.args.size() >= n + 1)
	{
		if (in.args[n] == L"1")
			ret = 1;
		else if (in.args[n] == L"2")
			ret = 2;
	}
	return ret;
}

//引数取得:論理型
bool CSAORIMahjongUtil::GetFlagOfArgument(const CSAORIInput& in, const size_t n)
{
	bool ret = false;
	if (in.args.size() >= n + 1)
	{
		if (in.args[n] == L"1")
			ret = true;
	}
	return ret;
}

/*---------------------------------------------------------
	構築
---------------------------------------------------------*/
CSAORIBase* CreateInstance(void)
{
	return new CSAORIMahjongUtil();
}
