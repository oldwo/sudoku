/* sudoku.h
2017jul02:(VK) Created
*/
#ifndef SUDOKU_H
#define SUDOKU_H

struct Tsudoku
{
	char nums[9][9];
	short cans[9][9];
	int hcan[9],vcan[9],qcan[9];
public:
	Tsudoku();
	void load(const char *Atxt);
	void setValue(int ARow,int AColumn,int AValue) {nums[ARow][AColumn]=AValue;}
	void calccans();
};//Tsudoku

#endif // SUDOKU_H
