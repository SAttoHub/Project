#pragma once
#include <forward_list>

template <class T>
class My_F_List : public std::forward_list<T> {
	std::forward_list<T>* ptr;
public:
	My_F_List() {
		ptr = this;
	}
	~My_F_List() {
	}

	size_t size() {
		return (int)std::distance(ptr->begin(), ptr->end());
	}

	T *operator[](int index) {
		int i = 0;
		for (auto itr = ptr->begin(); itr != ptr->end(); ++itr) {
			if (i == index) {
				return &(*itr);
				break;
			}
			i++;
		}
	};
};