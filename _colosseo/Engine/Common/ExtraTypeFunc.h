#pragma once
#include <tuple>
#include "..\..\Singleton.h"

//-------------------------------------------------------------------------------------------------------------
// 型判定
//-------------------------------------------------------------------------------------------------------------

//型判定関数内部処理類
namespace CheckTypeCommands {

class CheckTypeClass : public Singleton<CheckTypeClass> {
	friend class Singleton<CheckTypeClass>;
public:
	CheckTypeClass() { Return = false; }
	~CheckTypeClass() {}
	bool Return;
};

template<std::size_t i, class tTuple, class T>
void checkCommandThird(tTuple const &iTuple, T const &type)
{
	if (typeid(type) == typeid(std::get<i>(iTuple))) {
		CheckTypeClass::Instance()->Return = true;
	}
	else if (CheckTypeClass::Instance()->Return != true) {
		CheckTypeClass::Instance()->Return = false;
	}
}

template<class tTuple, std::size_t... indices, class T>
void checkImplSecond(tTuple const &iTuple, std::size_t i, std::index_sequence<indices...>, T type)
{
	using Func = void (*)(tTuple const &, T const &);
	Func aFuncArray[] =
	{
		(checkCommandThird<indices, tTuple, T>)...
	};
	aFuncArray[i](iTuple, type);
}

template<class tTuple, class T>
void checkTypeCommandFirst(tTuple const &iTuple, T val, std::size_t i)
{
	constexpr std::size_t n = std::tuple_size<tTuple>::value;
	checkImplSecond(iTuple, i, std::make_index_sequence<n>{}, val);
}
}

//-------------------------------------------------------------------------------------------------------------
// 型判定関数　変数を渡すバージョン　使い方
// 
// int val;
// int in;     float flo;    double dou;    bool boo;
// valと同じ型が無いので false が返ってくる
// bool result = CheckTypes(val, std::make_tuple( flo , dou , boo ));
//
// valと同じ型があるので true が返ってくる
// bool result2 = CheckTypes(val, std::make_tuple( in, flo , dou , boo ));
//
//-------------------------------------------------------------------------------------------------------------

/// <summary>
/// 第一引数 が 第二引数 std::tuple の中のいずれかの型と同じかどうかをを判定する
/// </summary>
/// <typeparam name="T"></typeparam>
/// <typeparam name="...Types"></typeparam>
/// <param name="val">第一引数</param>
/// <param name="types">第二引数</param>
/// <returns></returns>
template <class T, typename... Types>
static bool CheckTypes(T val, std::tuple<Types...> types) {
	for (std::size_t i = 0; i < std::tuple_size<decltype(types)>::value; ++i) {
		CheckTypeCommands::checkTypeCommandFirst(types, val, i);
	}
	bool result = CheckTypeCommands::CheckTypeClass::Instance()->Return;
	CheckTypeCommands::CheckTypeClass::Instance()->Return = false;
	return result;
}

//-------------------------------------------------------------------------------------------------------------
// 型判定関数　型名を渡すバージョン　使い方
// 
// int val = 0;
//
// valと同じ型が無いので false が返ってくる
// bool result = CheckTypes<bool, float, double>(val);
//
// valと同じ型があるので true が返ってくる
// bool result2 = CheckTypes<int, bool, float, double>(val);
//
//-------------------------------------------------------------------------------------------------------------

/// <summary>
/// 第一引数 が指定した型の中のいずれかの型と同じかどうかをを判定する
/// </summary>
/// <typeparam name="...Types"></typeparam>
/// <typeparam name="T"></typeparam>
/// <param name="val">第一引数</param>
/// <returns></returns>
template <typename... Types, class T>
static bool CheckTypes(T val) {
	std::tuple<Types...> tup;
	return CheckTypes(val, tup);
}
