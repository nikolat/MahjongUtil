#pragma once

#include "mj_shanten.h"

class Machi
{
private:
	//待ちの取得(七対子)
	static string_t GetMachiChitoitsu(const vector_str tehai);
	//待ちの取得(国士無双)
	static string_t GetMachiKokushimusou(const vector_str tehai);

public:
	//待ちの取得
	static string_t GetMachi(const string_t tehai);
};
