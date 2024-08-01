#include "GB_Function.h"

using namespace std;

LINFO drawLInfo;
void record(LPARAM lParam, ULONGLONG pTime, UINT state) 
{
	PINFO drawPInfo;
	drawPInfo.lParam = lParam;
	drawPInfo.pTime = pTime;
	drawPInfo.state = state;
	drawLInfo.pInfo.push_back(drawPInfo);
}