#pragma once

#include "mj_common.h"

using namespace MJ_FUNC;

class Shanten
{
private:
	//孤立牌の除去
	static vector<vector_str> RemoveKoritsuHai(const vector_str hai);
	//シャンテン数の計算
	static int CalcShanten(const int count_mentsu, const int count_tatsu, const int count_koritsu, const bool has_atama);
	//SAORIの戻り値となる雀頭・面子・塔子・孤立牌を表す文字列の作成
	static string_t MakeCompositionString(
		const string_t atama,
		const vector<vector<int>> composition_man,
		const vector<vector<int>> composition_pin,
		const vector<vector<int>> composition_sou,
		const vector<vector<int>> composition_jihai
	);
	//面子・塔子・孤立牌(数牌)の取得
	static vector<vector<vector<int>>> GetComposition(const vector_str hai);
	//面子・塔子・孤立牌(数牌)の再帰的取得
	static vector<vector<int>> GetCompositionRecursion(const vector<int> hai_count_array, const int n, vector<vector<vector<int>>>& composition_a, vector<vector<vector<int>>>& composition_b);
	//塔子・孤立牌(数牌)の再帰的取得
	static int GetTatsuAndKoritsu(const vector<int> hai_count_array, const int n, vector<vector<vector<int>>>& ret_tatsu_and_koritsu);
	//面子・塔子・孤立牌(字牌)の取得
	static vector<vector<vector<int>>> GetCompositionJihai(const vector_str hai);
	//数牌・字牌の分離
	static vector<vector_str> SeparateColor(const vector_str hai);
	//3個以上同じ牌がある要素をすべて返す
	static vector_str GetKotsu(const vector_str hai);
	static vector<int> GetKotsu(const vector<int> hai);
	//2個以上同じ牌がある要素をすべて返す
	static vector_str GetToitsu(const vector_str hai);
	static vector<int> GetToitsu(const vector<int> hai);
	//重複した要素を返す
	static vector_str GetDuplicatedElement(const vector_str ary, const int n);
	static vector<int> GetDuplicatedElement(const vector<int> ary, const int n);
	//シャンテン数の取得(門前清自摸和)
	static int GetShantenMenzen(const string_t tehai);
	//シャンテン数の取得(役牌)
	static int GetShantenYakuhai(const string_t tehai, const string_t bafu_hai, const string_t jifu_hai);
	//シャンテン数の取得(断么九)
	static int GetShantenTanyao(const string_t tehai);
	//シャンテン数の取得(対々和)
	static int GetShantenToitoi(const string_t tehai);
	//シャンテン数の取得(混一色)
	static int GetShantenHonitsu(const string_t tehai);
	//シャンテン数の取得(混一色 指定色)
	static int GetShantenHonitsuByColor(const string_t tehai, const string_t color);

public:
	//シャンテン数の取得
	static int GetShanten(const string_t tehai);
	static int GetShanten(const string_t tehai, vector_str& composition);
	//シャンテン数の取得(役あり)
	static int GetShantenYaku(const string_t tehai, string_t bafu_hai, string_t jifu_hai, vector_str& yaku);
	//シャンテン数の取得(一般手)
	static int GetShantenNormal(const string_t tehai, vector_str& composition);
	//シャンテン数の取得(七対子)
	static int GetShantenChitoitsu(const string_t tehai);
	//シャンテン数の取得(国士無双)
	static int GetShantenKokushimusou(const string_t tehai);
};
