#include "HeapDebug.h"

map<void*, bool> HeapDebugClass::HeapDebug;
map<void*, bool> HeapDebugClass::HeapArrDebug;

int HeapDebugClass::heap_data_num = 0;
