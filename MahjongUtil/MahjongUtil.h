#pragma once

#include "mj_score.h"
#include "mj_machi.h"
#include <regex>

class CSAORIMahjongUtil : public CSAORI
{
private:
	//入力値チェック:手牌
	bool IsValidTehai(const string_t tehai);
	//入力値チェック:牌(1枚)
	bool IsValidHaiSingle(const string_t hai);
	//入力値チェック:牌(副露無し)
	bool IsValidHaiWithoutFuro(const string_t hai);
	//入力値チェック:風牌
	bool IsValidFupai(const string_t hai);
	//牌の有効枚数を取得
	int CountEffectiveNumberOfHai(const string_t tehai);
	//引数取得:文字列型
	string_t GetStringOfArgument(const CSAORIInput& in, const size_t n);
	//引数取得:数値型
	int GetNumberOfArgument(const CSAORIInput& in, const size_t n);
	//引数取得:論理型
	bool GetFlagOfArgument(const CSAORIInput& in, const size_t n);

public:
	virtual bool load(void);
	virtual bool unload(void);
	virtual void exec(const CSAORIInput& in, CSAORIOutput& out);
};
