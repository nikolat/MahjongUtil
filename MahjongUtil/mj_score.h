#pragma once

#include "mj_shanten.h"

class Score
{
private:
	//点数の取得(SAORIの戻り値となる役の情報を含む)
	static int GetScore(
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
	);
	//面子の形によって変わる役の情報を取得
	static int GetYakuInfoFromComposition(
		const bool is_tsumo,
		const string_t agari_hai,
		const string_t bafu_hai,
		const string_t jifu_hai,
		const string_t composition,
		std::map<string_t, int>& han,
		std::map<string_t, int>& yakuman
	);
	//符を取得
	static int Score::GetFu(
		const string_t atama_hai,
		const vector<vector_str> mentsu_without_furo,
		const vector<vector_str> mentsu_furo,
		const string_t agari_hai,
		const string_t bafu_hai,
		const string_t jifu_hai,
		const int is_tsumo,
		const int is_menzen
	);
	//点数を取得
	static int GetScorePoint(const int count_yakuman, const int count_han, const int count_dora, const int count_fu, const bool is_oya);
	//符を用いた点数計算
	static int GetScorePointWithFu(const int count_han, const int count_fu, const bool is_oya);
	//断么九判定
	static bool IsTanyao(const vector_str hai_plain);
	//盃口カウント
	static int CountPeko(const vector<vector_str> mentsu);
	//役牌カウント
	static int CountYakuhai(const vector_str hai_plain, const string_t bafu_hai, const string_t jifu_hai);
	//対々和判定
	static bool IsToitoi(const vector<vector_str> mentsu);
	//混老頭判定
	static bool IsHonroutou(const vector_str hai_plain);
	//混全帯么九、純全帯么九判定用
	static int SubChanta(const vector<vector_str> mentsu, string_t atama_hai);
	//混全帯么九判定
	static bool IsChanta(const vector<vector_str> mentsu, string_t atama_hai);
	//純全帯么九判定
	static bool IsJunchan(const vector<vector_str> mentsu, string_t atama_hai);
	//小三元判定
	static bool IsShousangen(const vector<vector_str>  mentsu);
	//大三元判定
	static bool IsDaisangen(const vector<vector_str>  mentsu);
	//混一色、清一色、字一色判定用
	static vector<bool> GetUsedHaiGroup(const vector_str hai);
	//混一色判定
	static bool IsHonitsu(const vector_str hai_plain);
	//清一色判定
	static bool IsChinitsu(const vector_str hai_plain);
	//字一色判定
	static bool IsTsuiso(const vector_str hai_plain);
	//緑一色判定
	static bool IsRyuiso(const vector_str hai_plain);
	//小四喜判定
	static bool IsShousushi(const vector<vector_str> mentsu);
	//大四喜判定
	static bool IsDaisushi(const vector<vector_str> mentsu);
	//九蓮宝燈判定
	static bool IsChuren(const vector_str hai_plain);
	//清老頭判定
	static bool IsChinroutou(const vector_str hai_plain);
	//ドラ牌カウント
	static int CountDora(const vector_str hai_plain, const vector_str dora);
	//暗刻カウント
	static int CountAnkou(const vector<vector_str> mentsu, const bool is_tsumo, const string_t agari_hai, const string_t atama_hai);
	//三色同刻判定
	static bool IsSanshokudoukou(const vector<vector_str> mentsu);
	//三色同順判定
	static bool IsSanshokudoujun(const vector<vector_str> mentsu);
	//一気通貫判定
	static bool IsIkkitsuukan(const vector<vector_str> mentsu);
	//平和判定
	static bool IsPinfu(const string_t atama_hai, const vector<vector_str> mentsu, const string_t agari_hai, const string_t bafu_hai, const string_t jifu_hai);

public:
	//点数の取得(SAORIの戻り値となる文字列の作成)
	static int GetScoreValues(
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
	);
};
