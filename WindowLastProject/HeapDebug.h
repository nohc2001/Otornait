#pragma once
#include <map>

using namespace std;

class HeapDebugClass {
public:
	HeapDebugClass() {

	}

	virtual ~HeapDebugClass() {

	}

	static map<void*, bool> HeapDebug;
	static map<void*, bool> HeapArrDebug;

	static int heap_data_num;

	template<typename T> static T* HeapNew() {
		T* newvalue = new T();
		HeapDebugClass::heap_data_num += 1;
		HeapDebugClass::HeapDebug[newvalue] = true;
		return newvalue;
	}

	template<typename T> static bool HeapDelete(T* heapvalue) {
		if (HeapDebugClass::HeapDebug[heapvalue] == true) {
			HeapDebugClass::heap_data_num -= 1;
			HeapDebugClass::HeapDebug[heapvalue] = false;
			delete heapvalue;
			return true;
		}
		else return false;
	}

	template<typename T> static T* HeapArrNew(const int& num) {
		T* newvalue = new T[num];
		HeapDebugClass::heap_data_num += 1;
		HeapDebugClass::HeapDebug[newvalue] = true;
		return newvalue;
	}

	template<typename T> static bool HeapArrDelete(T* heapvalue) {
		void* ptr = (void*)heapvalue;
		if (HeapDebugClass::HeapDebug[ptr] == true) {
			HeapDebugClass::heap_data_num -= 1;
			HeapDebugClass::HeapDebug[ptr] = false;
			delete[] heapvalue;
			return true;
		}
		else return false;
	}

	//현재 해제되지 않은 힙 메모리의 개수
	static int GetPresentHeapDataCount() {
		return HeapDebugClass::heap_data_num;
	}

	static void MapClear() {
		if (HeapDebug.size() == 0) {
			return;
		}

		int nonE = 0;
		for (std::map<void*, bool>::iterator itr = HeapDebug.begin(); itr != HeapDebug.end(); ++itr) {
			if (HeapDebug[itr->first] == false) {
				nonE += 1;
			}
		}

		if (nonE > 100) {
			void** ptrarr = new void* [nonE];
			int n = 0;

			for (std::map<void*, bool>::iterator itr = HeapDebug.begin(); itr != HeapDebug.end(); ++itr) {
				if (HeapDebug[itr->first] == false) {
					if (n + 1 < nonE) {
						ptrarr[n] = itr->first;
						n += 1;
					}
					else {
						break;
					}
				}
			}

			for (int i = 0; i < n; ++i) {
				HeapDebug.erase(ptrarr[i]);
			}

			delete[] ptrarr;
		}
	}
};

