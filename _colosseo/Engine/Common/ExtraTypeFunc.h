#pragma once
#include <tuple>
#include "..\..\Singleton.h"

//-------------------------------------------------------------------------------------------------------------
// �^����
//-------------------------------------------------------------------------------------------------------------

//�^����֐�����������
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
// �^����֐��@�ϐ���n���o�[�W�����@�g����
// 
// int val;
// int in;     float flo;    double dou;    bool boo;
// val�Ɠ����^�������̂� false ���Ԃ��Ă���
// bool result = CheckTypes(val, std::make_tuple( flo , dou , boo ));
//
// val�Ɠ����^������̂� true ���Ԃ��Ă���
// bool result2 = CheckTypes(val, std::make_tuple( in, flo , dou , boo ));
//
//-------------------------------------------------------------------------------------------------------------

/// <summary>
/// ������ �� ������ std::tuple �̒��̂����ꂩ�̌^�Ɠ������ǂ������𔻒肷��
/// </summary>
/// <typeparam name="T"></typeparam>
/// <typeparam name="...Types"></typeparam>
/// <param name="val">������</param>
/// <param name="types">������</param>
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
// �^����֐��@�^����n���o�[�W�����@�g����
// 
// int val = 0;
//
// val�Ɠ����^�������̂� false ���Ԃ��Ă���
// bool result = CheckTypes<bool, float, double>(val);
//
// val�Ɠ����^������̂� true ���Ԃ��Ă���
// bool result2 = CheckTypes<int, bool, float, double>(val);
//
//-------------------------------------------------------------------------------------------------------------

/// <summary>
/// ������ ���w�肵���^�̒��̂����ꂩ�̌^�Ɠ������ǂ������𔻒肷��
/// </summary>
/// <typeparam name="...Types"></typeparam>
/// <typeparam name="T"></typeparam>
/// <param name="val">������</param>
/// <returns></returns>
template <typename... Types, class T>
static bool CheckTypes(T val) {
	std::tuple<Types...> tup;
	return CheckTypes(val, tup);
}
