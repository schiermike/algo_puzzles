#include <stdio.h>
#include <err.h>

#define NONE 0
#define ISWHITE 1
#define KING 2
#define QUEEN 4
#define ROOK 8
#define BISHOP 16
#define KNIGHT 32
#define PAWN 64

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

int kwc, kwr; // white king position
int board[8][8]; // game board

int piece(char code) {
	switch (code) {
		case 'K':
			return KING;
		case 'Q':
			return QUEEN;
		case 'R':
			return ROOK;
		case 'B':
			return BISHOP;
		case 'N':
			return KNIGHT;
		case 'P':
			return PAWN;
	}
	return NONE;
}

void read_piece(int color) {
	char p, c;
	int r;
	scanf("%c %c %d\n", &p, &c, &r);
	board[c-'a'][r-1] = color | piece(p);
	if (piece(p) == KING && color == ISWHITE) {
		kwc = c-'a';
		kwr = r-1;
	}
}

void read_input() {
	int c,r;
	int nw, nb;
	for (c=0; c<8; c++)
		for (r=0; r<8; r++)
			board[r][c] = NONE;
	
	scanf("%d %d\n", &nw, &nb);
	while (nw--)
		read_piece(ISWHITE);
	while (nb--)
		read_piece(NONE);
}

int check_piece(int p, int c, int r) {
	int i;
	int no_check;

	if (p == KING) {
		if ( abs(kwc-c)<=1 && abs(kwr-r)<=1 )
			return 1;
	}
	if (p == BISHOP || p == QUEEN) {
		if ( abs(kwc-c)==abs(kwr-r) ) {
			no_check = 0;
			for (i=1; i<abs(kwc-c); i++)
				no_check |= board[c<kwc?c+i:c-i][r<kwr?r+i:r-i];
			if (!no_check)
				return 1;
		}
	}
	if (p == KNIGHT) {
		if ( abs(kwc-c)==2 && abs(kwr-r)==1 || abs(kwc-c)==1 && abs(kwr-r)==2 )
			return 1;
	}
	if (p == ROOK || p == QUEEN) {
		if (kwc == c) {
			no_check = 0;
			for (i=MIN(kwr,r)+1; i<MAX(kwr,r); i++)
				no_check |= board[c][i];
			if (!no_check)
				return 1;
		}
		if (kwr == r) {
			no_check = 0;
			for (i=MIN(kwc,c)+1; i<MAX(kwc,c); i++)
				no_check |= board[i][r];
			if (!no_check)
				return 1;
		}
	}
	if (p == PAWN) {
		if ( abs(kwc-c)==1 && kwr+1==r )
			return 1;
	}

	return 0;
}

int is_check() {
	int c,r;
	for (c=0; c<8; c++)
		for (r=0; r<8; r++) {
			if (board[c][r] == NONE || board[c][r] & ISWHITE)
				continue;
			if (check_piece(board[c][r], c, r))
				return 1;
		}
	return 0;
}

int main(int argc, char** args) {
	int n;
	scanf("%d\n", &n);
	while (n-->0) {
		read_input();
		printf(is_check() ? "Check\n" : "No Check\n");
	}
	return 0;
}

