#include<iostream>
#include<fstream>
#include<vector>
#include<string.h>
#include<cmath>
#include<stdio.h>
using namespace std;
// global board b_init
class board;
struct Coordinate{
	int x;
	int y;
	Coordinate(){
		x=0;
		y=0;
	}
	Coordinate(int a, int b){
		x=a;
		y=b;
	}
	bool operator==(Coordinate c){
		return ((c.x==x) && (c.y==y));
	}
};

class Piece{
public:
	string type;
	int colour;    //1 for white, -1 for black.
	bool mult_by_constant;
	bool can_travel_opposite;
	vector<Coordinate> step_size;
	bool state_for_special_moves;
	Coordinate posn;
	Piece(){
		posn.x=1;
		posn.y=1;
	}
	Piece(int xi, int yi){
		posn.x=xi;
		posn.y=yi;
	}
	void set_posn(int xi, int yi){
		posn.x=xi;
		posn.y=yi;
	}
	void set_colour(int x){
		colour=x;
		if(type=="Pawn"){
			Coordinate c(0,colour);
			vector<Coordinate> v;
			v.push_back(c);
			step_size=v;
		}
	}
	void operator=(Piece& p){
		type=p.type;
		set_colour(p.colour);
		mult_by_constant=p.mult_by_constant;
		can_travel_opposite=p.can_travel_opposite;
		step_size=p.step_size;
		state_for_special_moves=p.state_for_special_moves;
	}
	vector<Coordinate> possibleMoves();
	bool is_empty(){
		return type=="Empty";
	}
	vector<Coordinate> validMoves(board);
	vector<Coordinate> specialMoves(board);
	vector<Coordinate> spAndValidMoves(board);
};


class Pawn : public Piece{
public:
	Pawn(){
		type="Pawn";
		mult_by_constant=false;
		can_travel_opposite=false;
		Coordinate c(0,colour); //white is below, black is above.
		step_size.push_back(c);
		state_for_special_moves=0;
	}
	void set_colour(int x){
		colour=x;
		Coordinate c(0,colour);
		vector<Coordinate> v;
		v.push_back(c);
		step_size=v;
	}
};

class Rook : public Piece{
public:
	Rook(){
		type="Rook";
		mult_by_constant=true;
		can_travel_opposite=true;
		Coordinate c1(0,1);
		step_size.push_back(c1);
		Coordinate c2(1,0);
		step_size.push_back(c2);
		state_for_special_moves=0;
	}
};

class Knight : public Piece{
public:
	Knight(){
		type="Knight";
		mult_by_constant=false;
		can_travel_opposite=true;
		Coordinate c1(2,1);
		step_size.push_back(c1);
		Coordinate c2(1,2);
		step_size.push_back(c2);
		Coordinate c3(2,-1);
		step_size.push_back(c3);
		Coordinate c4(1,-2);
		step_size.push_back(c4);
		state_for_special_moves=0;
	}
};

class Bishop : public Piece{
public:
	Bishop(){
		type="Bishop";
		mult_by_constant=true;
		can_travel_opposite=true;
		Coordinate c1(1,1);
		step_size.push_back(c1);
		Coordinate c2(1,-1);
		step_size.push_back(c2);
		state_for_special_moves=0;
	}
};

class Queen : public Piece{
public:
	Queen(){
		type="Queen";
		mult_by_constant=true;
		can_travel_opposite=true;
		Coordinate c1(0,1);
		step_size.push_back(c1);
		Coordinate c2(1,0);
		step_size.push_back(c2);
		Coordinate c3(1,1);
		step_size.push_back(c3);
		Coordinate c4(1,-1);
		step_size.push_back(c4);
		state_for_special_moves=0;
	}
};

class King : public Piece{
public:
	King(){
		type="King";
		mult_by_constant=false;
		can_travel_opposite=true;
		Coordinate c1(0,1);
		step_size.push_back(c1);
		Coordinate c2(1,0);
		step_size.push_back(c2);
		Coordinate c3(1,1);
		step_size.push_back(c3);
		Coordinate c4(1,-1);
		step_size.push_back(c4);
		state_for_special_moves=0;
	}
};

class Empty : public Piece{
public:
	Empty(){
		type="Empty";
		mult_by_constant=false;
		can_travel_opposite=false;
	}
};

class board{
public:
	Piece a[8][8];
	board(){
		for(int i=0; i<8; i++){
			for(int j=0; j<8; j++){
				Coordinate c;
				c.x=i+1;
				c.y=j+1;
				a[i][j].posn=c;
				a[i][j].type="Empty";
			}
		}
	}
	void operator=(board b){
		for(int i=0;i<8;i++)
			for(int j=0;j<8;j++){
				a[i][j]=b.a[i][j];
			}
	}
	board p(){return *this;}
	vector<Coordinate> allvalidmoves(int);
	bool check_influence(Coordinate,int);
	bool check(int state);
	Coordinate findking(int);
	
};

bool inside_board(Coordinate);
vector<Coordinate> merge(vector<Coordinate>, vector<Coordinate>);
void initialise_board(board &);
board update_castle(Coordinate,board);
board update_board(Coordinate,Coordinate,board);
int game_end(board,int);
bool draw_piece(board);
board b_init;
// int main(){
// 	/*Piece a(3,2);
// 	Knight r;
// 	a=r;
// 	vector<Coordinate> res=a.possibleMoves();
// 	for(int i=0; i<res.size(); i++){
// 		cout<<res[i].x<<" , "<<res[i].y<<"\n";
// 	}
// 	*/
// 	initialise_board(b_init);
// 	Empty e;
// 	Pawn p;
// 	Rook r;
// 	//initialise_board();
// 	cout<<game_end(b_init,1)<<endl;
// 	p.set_colour(-1);
// 	//r.colour=1;
// 	//b_init.a[4][1]=p;
// 	//b_init.a[2][0]=e;
// 	//cout<<b_init.check(1);
// 	// int x=3,y=0;
// 	// vector<Coordinate> res=b_init.a[x][y].specialMoves(b_init);
// 	// cout<<b_init.a[x][y].type<<endl;
// 	// for(int i=0; i<res.size(); i++){
// 	// 	cout<<res[i].x<<" , "<<res[i].y<<"\n";
// 	// }
// 	//Coordinate t=b_init.findking(-1);
// 	//cout<<t.x<<t.y;//<<b_init.a[0][0].type;
// 	// Coordinate a(1,1),b(2,4);
// 	// b_init=update_castle(a,b_init);
// 	// for(int i=0;i<8;i++){
// 	// 	for(int j=0;j<8;j++){
// 	// 		cout<<b_init.a[j][i].type<<j<<i<<b_init.a[j][i].colour<<" ";	
// 	// 	}
// 	// 	cout<<endl;
// 	// }
// }
int game_end(board b,int state){
	int mate=0,draw=0;
	if(b.check(state)){
		Coordinate k=b.findking(state);
		vector<Coordinate> v=b.a[k.x-1][k.y-1].validMoves(b);
		for(int i=0;i<v.size();i++)
			if(!(b.check_influence(v[i],-1*state))) {mate=1;break;}
		if(mate==0){
			for(int i=0;i<8;i++){
				for(int j=0;j<8;j++){
					if(b.a[i][j].type!="King" && b.a[i][j].colour==state){
						vector<Coordinate> v=merge(b.a[i][j].validMoves(b),b.a[i][j].specialMoves(b));
						for(int k=0;k<v.size();k++){
							Coordinate ini(i+1,j+1);
							board b2=update_board(ini,v[k],b);
							if(!b2.check(state)){mate=1;break;}
						}
					}
					if(mate==0) return 1;
				}
			}
		}
		if(mate==1){
			//if(draw_piece(b)) return 2;
			for(int i=0;i<8;i++){
				for(int j=0;j<8;j++){
					if(b.a[i][j].colour==state){
						vector<Coordinate> v=b.a[i][j].validMoves(b);
						for(int k=0;k<v.size();k++){
							Coordinate ini(i+1,j+1);
							board b2=update_board(ini,v[k],b);
							if(!b2.check(state)){draw=1;break;}
						}
					}
					if(draw==0) return 2;
				}
			}
		}
	}
	return 0;
}

bool draw_piece(board b){
	vector<string> v1,v2;
	for(int i=0;i<8;i++)
		for(int j=0;j<8;j++){
			if(!b.a[i][j].is_empty()){
				if(b.a[i][j].colour==1) v1.push_back(b.a[i][j].type);
				v2.push_back(b.a[i][j].type);
			}
		}
	if(v1.size()==1 && v2.size()==1) return 1;
	return 0;
}
board update_castle(Coordinate c,board b){
	vector<Coordinate> u=b.a[3][c.y-1].specialMoves(b);
	int ch=0;
	for(int p=0;p<u.size();p++)
		if(u[p]==c){ch=1; break;}
	if(ch=1){
		Empty e;
		if(c.x==8){
			b.a[5][c.y-1]=b.a[3][c.y-1];
			b.a[4][c.y-1]=b.a[7][c.y-1];
			b.a[3][c.y-1]=e;
			b.a[7][c.y-1]=e;
			b.a[5][c.y-1].state_for_special_moves=1;
			b.a[4][c.y-1].state_for_special_moves=1;
		}
		else{
			b.a[1][c.y-1]=b.a[3][c.y-1];
			b.a[2][c.y-1]=b.a[0][c.y-1];
			b.a[3][c.y-1]=e;
			b.a[0][c.y-1]=e;
			b.a[1][c.y-1].state_for_special_moves=1;
			b.a[2][c.y-1].state_for_special_moves=1;
		}
	}
	return b;
}

board update_board(Coordinate i, Coordinate f, board b){
	board b_init=b;
	Piece a=b_init.a[i.x-1][i.y-1];
	if(!(b_init.a[i.x-1][i.y-1].is_empty())){
		vector<Coordinate> v=merge(b_init.a[i.x-1][i.y-1].validMoves(b_init),b_init.a[i.x-1][i.y-1].specialMoves(b_init));
		int ch=0;
		for(int p=0;p<v.size();p++)
			if(f==v[p]) ch=1;
		if(ch==1){
			b_init.a[f.x-1][f.y-1]=b_init.a[i.x-1][i.y-1];
			Empty e;
			b_init.a[i.x-1][i.y-1]=e;
			b_init.a[f.x-1][f.y-1].state_for_special_moves=1;
		}
	}
	return b_init;
}

Coordinate board::findking(int state){
	Coordinate u;
	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			if(a[i][j].colour==state && a[i][j].type=="King"){
				//cout<<"hehe";
				Coordinate p(i+1,j+1);
				return p;//cout<<u.x;
			}
		}
		//Coordinate q;
		//if(!(u==q)) break;
	}
	return u;
}
bool board::check(int state){
	Coordinate b=findking(state);
	return check_influence(b,-1*state);
}
bool board::check_influence(Coordinate b,int state){
	vector<Coordinate> a=allvalidmoves(state);
	bool ch=0;
	for(int i=0;i<a.size();i++)
		if(a[i]==b)
			{ch=1;break;}	
	return ch;
}
vector<Coordinate> board::allvalidmoves(int state){
	vector<Coordinate> v;
	for(int i=0; i<8; i++){
		for(int j=0; j<8; j++){
			board b=p();
			if(a[i][j].colour==state){
				vector<Coordinate> u=a[i][j].validMoves(b), u2=a[i][j].specialMoves(b);
				if(i==0 && j==0) v=u;
				else v=merge(v,u);
				v=merge(v,u2);
			}
		}
	}
	return v;
}
vector<Coordinate> merge(vector<Coordinate> a, vector<Coordinate> b){
	vector<Coordinate> v=a;
	size_t na=a.size(), nb=b.size();
	v.resize(na+nb);
	for(size_t i=na; i<na+nb;i++)
		v[i]=b[i-na];
	return v;
}

bool inside_board(Coordinate c){
	int cx=c.x;
	int cy=c.y;
	return (((cx>=1)&&(cx<=8))&&((cy>=1)&&(cy<=8)));
}
vector<Coordinate> Piece::validMoves(board b){
	vector<Coordinate> res;
	int px=posn.x;
	int py=posn.y;
	if (can_travel_opposite){
		if (mult_by_constant){
			int lim=max(max(px-1,8-px),max(py-1,8-py));
			for(int n=1; n>=-1; n-=2){
				for(int i=0; i<step_size.size(); i++){
					for(int j=1; j<=lim; j++){
						Coordinate c;
						c.x=px+n*j*step_size[i].x;
						c.y=py+n*j*step_size[i].y;
						if (inside_board(c)) {
							if (b.a[c.x-1][c.y-1].is_empty()){
								res.push_back(c);
							}
							else{
								Piece p=b.a[c.x-1][c.y-1];
								if (p.colour==colour) break;
								else{
									res.push_back(c);
									break;
								}
							}
						}
					}
				}
			}
		}
		else {
			for(int i=0; i<step_size.size(); i++){
				Coordinate c;
				c.x=px+step_size[i].x;
				c.y=py+step_size[i].y;
				if (inside_board(c)) {
					if (b.a[c.x-1][c.y-1].is_empty()){
						res.push_back(c);
					}
						else{
							Piece p=b.a[c.x-1][c.y-1];
							if (p.colour!=colour) res.push_back(c);
						}
					}
				c.x=px-step_size[i].x;
				c.y=py-step_size[i].y;
				if (inside_board(c)) {
					if (b.a[c.x-1][c.y-1].is_empty()){
						res.push_back(c);
					}
					else{
						Piece p=b.a[c.x-1][c.y-1];
						if (p.colour!=colour) res.push_back(c);
					}
				}
			}
		}
	}
	else {
		if (mult_by_constant){
			int lim=max(max(px-1,8-px),max(py-1,8-py));
			for(int j=1; j<=lim; j++){
				for(int i=0; i<step_size.size(); i++){
					Coordinate c;
					c.x=px+j*step_size[i].x;
					c.y=py+j*step_size[i].y;
					if (inside_board(c)) {
						if (b.a[c.x-1][c.y-1].is_empty()){
							res.push_back(c);
						}
						else{
							Piece p=b.a[c.x-1][c.y-1];
							if (p.colour==colour) break;
							else{
								res.push_back(c);
								break;
							}
						}
					}
				}
			}
		}
		else {
			for(int i=0; i<step_size.size(); i++){
				Coordinate c;
				c.x=px+step_size[i].x;
				c.y=py+step_size[i].y;
				if (inside_board(c)) {
					if (b.a[c.x-1][c.y-1].is_empty()){
						res.push_back(c);
					}
				}
				for (int j=-1; j<=1 ; j+=2){
					c.x = px + j;
					if (inside_board(c)){
						if (!(b.a[c.x-1][c.y-1].is_empty())){
							res.push_back(c);
						}
					}
				}
			}
		}
	}
	return res;
}

vector<Coordinate> Piece::specialMoves(board b){
	vector<Coordinate> res;
	if (!(state_for_special_moves)){
		if(type=="Pawn"){
			int n = step_size[0].y;
			Piece p = b.a[posn.x-1][posn.y-1+n];
			if (p.is_empty()){
				Piece p2 = b.a[posn.x-1][posn.y-1+2*n];
				if (p2.is_empty()){
					Coordinate c;
					c.x=p2.posn.x;
					c.y=p2.posn.y;
					res.push_back(c);					
				}
				else{
					if (p2.colour!=colour){
						Coordinate c;
						c.x=p2.posn.x;
						c.y=p2.posn.y;
						res.push_back(c);
					}
				}	
			}
		}	
		
			// The movement of rook in case of castling will be controlled in the main function 
		if(type=="King"){
			Coordinate c;
			c.x = 1;
			c.y = posn.y;
			if (b.a[c.x-1][c.y-1].type == "Rook" && b.a[c.x-1][c.y-1].state_for_special_moves == 0 && b.a[1][c.y-1].type == "Empty" 
				&& b.a[2][c.y-1].type == "Empty" && (!b.check(colour))){
				//c.x = posn.x - 2;
				res.push_back(c);
			}
			c.x = 8;
			if (b.a[c.x-1][c.y-1].type == "Rook" && b.a[c.x-1][c.y-1].state_for_special_moves == 0 && b.a[4][c.y-1].type == "Empty" 
				&& b.a[5][c.y-1].type == "Empty" && b.a[6][c.y-1].type == "Empty" && (!b.check(colour))){
				//c.x = posn.x + 2;
				res.push_back(c);
			}
		}
	}
	else{
		return res;
	}
}

void initialise_board(board &b_init){
	Pawn p;
	Rook r;
	Knight kn;
	Bishop b;
	Queen q;
	King k;

	b_init.a[0][0]=r;
	b_init.a[0][7]=r;
	b_init.a[7][0]=r;
	b_init.a[7][7]=r;
	b_init.a[1][0]=kn;
	b_init.a[6][0]=kn;
	b_init.a[1][7]=kn;
	b_init.a[6][7]=kn;
	b_init.a[2][0]=b;
	b_init.a[5][0]=b;
	b_init.a[2][7]=b;
	b_init.a[5][7]=b;
	b_init.a[4][0]=q;
	b_init.a[4][7]=q;
	b_init.a[3][0]=k;
	b_init.a[3][7]=k;
	for(int i=0;i<8;i++)
	{
		b_init.a[i][1]=p;
		b_init.a[i][6]=p;
		b_init.a[i][0].set_colour(1);
		b_init.a[i][1].set_colour(1);
		b_init.a[i][6].set_colour(-1);
		b_init.a[i][7].set_colour(-1);
	}
	for(int i=0;i<8;i++)
		for(int j=0;j<8;j++)
			b_init.a[i][j].state_for_special_moves=0;
}

string piece_cut;

bool is_present(Coordinate c, vector<Coordinate> v){
	if (v.size() == 0) return 0;
	if (v.back() == c) return 1;
	v.pop_back();
	return is_present(c, v);
}
vector<Coordinate> Piece::spAndValidMoves(board b){
	vector<Coordinate> a = validMoves(b);
	vector<Coordinate> c;
	if (type == "Pawn"){
		c = specialMoves(b);
		// for (int i =0; i<c.size(); i++){
		// 	Coordinate d = c[i];
		// 	a.push_back[d];
		// }
		a = merge(a,c);
	}
	return a;
}

string is_piece_cut(Coordinate cf){
	if (b_init.a[cf.x-1][cf.y-1].type == "Empty"){
		return "0";

	}
	return "1";
}

string is_valid(string s){
	int xi = int(s[0] - 'a') + 1;
	int yi = int(s[1] - '0');
	int xf = int(s[3] - 'a') + 1;
	int yf = int(s[4] - '0');
	Coordinate ci(xi,yi);
	Coordinate cf(xf,yf);
	piece_cut = is_piece_cut(cf);
	if (is_present(cf, b_init.a[xi-1][yi-1].spAndValidMoves(b_init))){

		b_init = update_board(ci, cf, b_init);
		int g = game_end(b_init, -1 * b_init.a[xi-1][yi-1].colour);
		if(g == 1){
			return "2";
		}
		if (g == 2){
			return "3";
		}
		return "1";
	}
	return "0";
}



int main(){
	// string s;
	// ifstream file;
	// file.open("coord_file");
	// getline(file,s);
	// cout<<s;
	printf("%s\n", "c++ program started");
	initialise_board(b_init);
	// ifstream file;
	// ofstream write_file;
	// write_file.open("valid_file", ios::trunc);
	// string write_file_start = ")";
	// write_file << ")00";
	// write_file.close();

	// ifstream test_file;
	// test_file.open("valid_file");
	// string t;
	// getline(test_file,t);
	// test_file.close();
	// printf("%s\n", t);
	string write_file_start = ")";
	char ch[] = ")00";
	FILE *coord;
	FILE *fptr;
	fptr = fopen("write_file", "w");
	fprintf(fptr, "%s", ch);
	fclose(fptr);
	printf("%s\n", "Hi!");


	string s = "", n = "", res;
	while(1){
		//file.open("coord_file");
		coord = fopen("coord_file", "r");
		if (file.is_open()){
			getline(file,n);
			file.close();
			if(n == s){
				continue;
			}
			else{
				while(!write_file.is_open()){
					write_file.open("valid_file", ios::trunc);
				}
				if (write_file_start == ")"){
					res = "(" + is_valid(n) + piece_cut;
					write_file_start = "(";
				}
				else {
					res = ")" + is_valid(n) + piece_cut;
					write_file_start = ")";
				}
				write_file << res;
				write_file.close();
				s = n;
			}
		}
		else continue;

	}

}
