/* sudoku.cpp
2017jul02:(VK) Created
*/
#include "sudoku.h"
#include <memory.h>
//#include <QString>

/*
FUNCTIONS
*****************************************************************************/
/*
static QString bitstring(int mask) {
	QString q("");
	q.reserve(10);
	for(int i=9;--i>=0;mask>>=1) if(mask&1) q+=QString('0'+9-i);
	return q;
}//bitstring
*/

/*
CLASSES
*****************************************************************************/
Tsudoku::Tsudoku()
{
	memset(nums,0,sizeof(nums));
	memset(cans,-1,sizeof(cans));
}//Tsudoku::Tsudoku

void Tsudoku::load(const char *Atxt) {
	int r=0,c=0;
	--Atxt;
	while(*++Atxt) {
		switch(*Atxt) {
		case '\n':++r;c=0;continue;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if(r>8 or c>8) continue;
			nums[r][c]=*Atxt-'0';
			++c;
			continue;
		case ' ':
		case '.':
		case '*':
		default:
			if(r>8 or c>8) continue;
			nums[r][c]=0;
			++c;
		}//switch
	}//while
}//Tsudoku::load

void Tsudoku::calccans() {
	char f[256];
	memset(f,0,sizeof(f));
	for(int i=9;--i>=0;) hcan[i]=vcan[i]=qcan[i]=1023;
	for(int i=9;--i>=0;) {
		for(int j=9;--j>=0;) {
			int mask=1<<nums[i][j]; mask&=1023; mask=1023-mask;
			hcan[i]&=mask;
			vcan[j]&=mask;
			qcan[i/3*3+j/3]&=mask;
			++f[nums[i][j]];
		}//for
	}//for
	for(int i=9;--i>=0;) {
		for(int j=9;--j>=0;) {
			cans[i][j]=hcan[i]&vcan[j]&qcan[i/3*3+j/3];
			if(nums[i][j])cans[i][j]=0;//(nums[i][j]>'0' && nums[i][j]<':')?1<<nums[i][j]-'1':
			//qDebug("%d,%d=%c->%d",i,j,nums[i][j],f[nums[i][j]]);
		}//for
	}//for
}//Tsudoku::calccans
