#pragma once
#include "vector"

namespace Smath_vector {
#pragma region vector範囲切り抜き
	/// <summary>
	/// vector 指定範囲だけ切り出し
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="data">切り出すvectorデータ</param>
	/// <param name="_minX">切り出し範囲の値１</param>
	/// <param name="_maxX">切り出し範囲の値２</param>
	/// <param name="">切り出し範囲の値は順不同</param>
	/// <returns></returns>
	template<typename T>
	static std::vector<T> vectorRangeCut(const std::vector<T> &data, int _Range1, int _Range2) {
		//カット範囲のマップを一時保存
		std::vector<T>  DumyVector;
		int minX = _Range1;
		int maxX = _Range2;
		//大きさが逆の時
		if (minX > _Range2) {
			minX = _Range2;
			maxX = _Range1;
		}
		//Cut後のサイズ
		int size = maxX - minX;
		size++;
		//一時保存用vectorのリサイズ
		DumyVector.resize(size);
		//データの切り出し
		for (int x = 0; x < size; x++) {
			DumyVector[x] = data[minX + x];
		}
		return DumyVector;
	}

	/// <summary>
	/// 2重vector 指定範囲だけ切り出し
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="data">切り出すvectorデータ</param>
	/// <param name="_RangeX1">切り出し範囲 X の値１</param>
	/// <param name="_RangeX2">切り出し範囲 X の値２</param>
	/// <param name="_RangeY1">切り出し範囲 Y の値１</param>
	/// <param name="_RangeY2">切り出し範囲 Y の値２</param>
	/// <returns></returns>
	template<typename T>
	static std::vector<std::vector<T>> vectorRangeCut(const std::vector<std::vector<T>> &data, int _RangeX1, int _RangeX2, int _RangeY1, int _RangeY2) {
		//カット範囲のマップを一時保存
		std::vector<std::vector<T>>  DumyVector;
		int minX = _RangeX1;
		int maxX = _RangeX2;
		//大きさが逆の時
		if (minX > _RangeX2) {
			minX = _RangeX2;
			maxX = _RangeX1;
		}
		int minY = _RangeY1;
		int maxY = _RangeY2;
		//大きさが逆の時
		if (minY > _RangeY2) {
			minY = _RangeY2;
			maxY = _RangeY1;
		}
		//Cut後のサイズ
		int sizeX = maxX - minX;
		int sizeY = maxY - minY;
		sizeX++;
		sizeY++;
		//一時保存用vectorのリサイズ
		DumyVector.resize(sizeX);
		for (int x = 0; x < sizeX; ++x) {
			DumyVector[x].resize(sizeY);
		}
		//データの切り出し
		for (int x = 0; x < sizeX; x++) {
			for (int y = 0; y < sizeY; y++) {
				DumyVector[x][y] = data[minX + x][minY + y];
			}
		}
		return DumyVector;
	}

	/// <summary>
	/// 3重vector 指定範囲だけ切り出し
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="data">切り出すvectorデータ</param>
	/// <param name="_RangeX1">切り出し範囲 X の値１</param>
	/// <param name="_RangeX2">切り出し範囲 X の値２</param>
	/// <param name="_RangeY1">切り出し範囲 Y の値１</param>
	/// <param name="_RangeY2">切り出し範囲 Y の値２</param>
	/// <param name="_RangeZ1">切り出し範囲 Z の値１</param>
	/// <param name="_RangeZ2">切り出し範囲 Z の値２</param>
	/// <returns></returns>
	template<typename T>
	static std::vector<std::vector<std::vector<T>>> vectorRangeCut(const std::vector<std::vector<std::vector<T>>> &data, int _RangeX1, int _RangeX2, int _RangeY1, int _RangeY2, int _RangeZ1, int _RangeZ2) {
		//カット範囲のマップを一時保存
		std::vector<std::vector<std::vector<T>>>  DumyVector;
		int minX = _RangeX1;
		int maxX = _RangeX2;
		//大きさが逆の時
		if (minX > _RangeX2) {
			minX = _RangeX2;
			maxX = _RangeX1;
		}
		int minY = _RangeY1;
		int maxY = _RangeY2;
		//大きさが逆の時
		if (minY > _RangeY2) {
			minY = _RangeY2;
			maxY = _RangeY1;
		}
		int minZ = _RangeZ1;
		int maxZ = _RangeZ2;
		//大きさが逆の時
		if (minZ > _RangeZ2) {
			minZ = _RangeZ2;
			maxZ = _RangeZ1;
		}
		//Cut後のサイズ
		int sizeX = maxX - minX;
		int sizeY = maxY - minY;
		int sizeZ = maxZ - minZ;
		sizeX++;
		sizeY++;
		sizeZ++;
		//一時保存用vectorのリサイズ
		DumyVector.resize(sizeX);
		for (int x = 0; x < sizeX; ++x) {
			DumyVector[x].resize(sizeY);
			for (int y = 0; y < sizeY; ++y) {
				DumyVector[x][y].resize(sizeZ);
			}
		}
		//データの切り出し
		for (int x = 0; x < sizeX; x++) {
			for (int y = 0; y < sizeY; y++) {
				for (int y = 0; y < sizeY; y++) {
					DumyVector[x][y][z] = data[minX + x][minY + y][minZ + z];
				}
			}
		}
		return DumyVector;
	}
#pragma endregion
}