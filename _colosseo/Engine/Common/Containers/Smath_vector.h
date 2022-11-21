#pragma once
#include "vector"

namespace Smath_vector {
#pragma region vector�͈͐؂蔲��
	/// <summary>
	/// vector �w��͈͂����؂�o��
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="data">�؂�o��vector�f�[�^</param>
	/// <param name="_minX">�؂�o���͈͂̒l�P</param>
	/// <param name="_maxX">�؂�o���͈͂̒l�Q</param>
	/// <param name="">�؂�o���͈͂̒l�͏��s��</param>
	/// <returns></returns>
	template<typename T>
	static std::vector<T> vectorRangeCut(const std::vector<T> &data, int _Range1, int _Range2) {
		//�J�b�g�͈͂̃}�b�v���ꎞ�ۑ�
		std::vector<T>  DumyVector;
		int minX = _Range1;
		int maxX = _Range2;
		//�傫�����t�̎�
		if (minX > _Range2) {
			minX = _Range2;
			maxX = _Range1;
		}
		//Cut��̃T�C�Y
		int size = maxX - minX;
		size++;
		//�ꎞ�ۑ��pvector�̃��T�C�Y
		DumyVector.resize(size);
		//�f�[�^�̐؂�o��
		for (int x = 0; x < size; x++) {
			DumyVector[x] = data[minX + x];
		}
		return DumyVector;
	}

	/// <summary>
	/// 2�dvector �w��͈͂����؂�o��
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="data">�؂�o��vector�f�[�^</param>
	/// <param name="_RangeX1">�؂�o���͈� X �̒l�P</param>
	/// <param name="_RangeX2">�؂�o���͈� X �̒l�Q</param>
	/// <param name="_RangeY1">�؂�o���͈� Y �̒l�P</param>
	/// <param name="_RangeY2">�؂�o���͈� Y �̒l�Q</param>
	/// <returns></returns>
	template<typename T>
	static std::vector<std::vector<T>> vectorRangeCut(const std::vector<std::vector<T>> &data, int _RangeX1, int _RangeX2, int _RangeY1, int _RangeY2) {
		//�J�b�g�͈͂̃}�b�v���ꎞ�ۑ�
		std::vector<std::vector<T>>  DumyVector;
		int minX = _RangeX1;
		int maxX = _RangeX2;
		//�傫�����t�̎�
		if (minX > _RangeX2) {
			minX = _RangeX2;
			maxX = _RangeX1;
		}
		int minY = _RangeY1;
		int maxY = _RangeY2;
		//�傫�����t�̎�
		if (minY > _RangeY2) {
			minY = _RangeY2;
			maxY = _RangeY1;
		}
		//Cut��̃T�C�Y
		int sizeX = maxX - minX;
		int sizeY = maxY - minY;
		sizeX++;
		sizeY++;
		//�ꎞ�ۑ��pvector�̃��T�C�Y
		DumyVector.resize(sizeX);
		for (int x = 0; x < sizeX; ++x) {
			DumyVector[x].resize(sizeY);
		}
		//�f�[�^�̐؂�o��
		for (int x = 0; x < sizeX; x++) {
			for (int y = 0; y < sizeY; y++) {
				DumyVector[x][y] = data[minX + x][minY + y];
			}
		}
		return DumyVector;
	}

	/// <summary>
	/// 3�dvector �w��͈͂����؂�o��
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="data">�؂�o��vector�f�[�^</param>
	/// <param name="_RangeX1">�؂�o���͈� X �̒l�P</param>
	/// <param name="_RangeX2">�؂�o���͈� X �̒l�Q</param>
	/// <param name="_RangeY1">�؂�o���͈� Y �̒l�P</param>
	/// <param name="_RangeY2">�؂�o���͈� Y �̒l�Q</param>
	/// <param name="_RangeZ1">�؂�o���͈� Z �̒l�P</param>
	/// <param name="_RangeZ2">�؂�o���͈� Z �̒l�Q</param>
	/// <returns></returns>
	template<typename T>
	static std::vector<std::vector<std::vector<T>>> vectorRangeCut(const std::vector<std::vector<std::vector<T>>> &data, int _RangeX1, int _RangeX2, int _RangeY1, int _RangeY2, int _RangeZ1, int _RangeZ2) {
		//�J�b�g�͈͂̃}�b�v���ꎞ�ۑ�
		std::vector<std::vector<std::vector<T>>>  DumyVector;
		int minX = _RangeX1;
		int maxX = _RangeX2;
		//�傫�����t�̎�
		if (minX > _RangeX2) {
			minX = _RangeX2;
			maxX = _RangeX1;
		}
		int minY = _RangeY1;
		int maxY = _RangeY2;
		//�傫�����t�̎�
		if (minY > _RangeY2) {
			minY = _RangeY2;
			maxY = _RangeY1;
		}
		int minZ = _RangeZ1;
		int maxZ = _RangeZ2;
		//�傫�����t�̎�
		if (minZ > _RangeZ2) {
			minZ = _RangeZ2;
			maxZ = _RangeZ1;
		}
		//Cut��̃T�C�Y
		int sizeX = maxX - minX;
		int sizeY = maxY - minY;
		int sizeZ = maxZ - minZ;
		sizeX++;
		sizeY++;
		sizeZ++;
		//�ꎞ�ۑ��pvector�̃��T�C�Y
		DumyVector.resize(sizeX);
		for (int x = 0; x < sizeX; ++x) {
			DumyVector[x].resize(sizeY);
			for (int y = 0; y < sizeY; ++y) {
				DumyVector[x][y].resize(sizeZ);
			}
		}
		//�f�[�^�̐؂�o��
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