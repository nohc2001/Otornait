#pragma once
#include <Windows.h>
#include "HeapDebug.h"

class StringFuncs {
public:
	static wchar_t* stringToWStr(char* s) {
		wchar_t* wstr = HeapDebugClass::HeapArrNew<wchar_t>(strlen(s) + 1);
		MultiByteToWideChar(CP_ACP, 0, s, strlen(s), wstr, strlen(s));
		wstr[strlen(s)] = L'\0';
		return wstr;
	}

	static void AddStr(char* dest, const char* str1, const char* str2, int maxlen) {
		int len1 = strlen(str1);
		int len2 = strlen(str2);
		int n = len1 + len2;
		if (n > maxlen - 1) {
			n = maxlen - 1;
		}
		int i = 0;
		for (i = 0; i < n; ++i) {
			if (i < len1) {
				dest[i] = str1[i];
			}
			else {
				dest[i] = str2[i - len1];
			}
		}
		dest[i] = '\0';
	}

	static void AddTSTR(TCHAR* dest, const TCHAR* str1, const TCHAR* str2, int maxlen) {
		int len1 = lstrlen(str1);
		int len2 = lstrlen(str2);
		int n = len1 + len2;
		if (n > maxlen - 1) {
			n = maxlen - 1;
		}
		int i = 0;
		for (i = 0; i < n; ++i) {
			if (i < len1) {
				dest[i] = str1[i];
			}
			else {
				dest[i] = str2[i - len1];
			}
		}
		dest[i] = L'\0';
	}
};