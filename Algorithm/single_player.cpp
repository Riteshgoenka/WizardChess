#include<iostream>
#include<fstream>
#include<vector>
#include<string.h>
#include<cmath>
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
	int value;
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
	void operator=(const Piece& p){
		const string s = p.type;
		type=s;
		set_colour(p.colour);
		mult_by_constant=p.mult_by_constant;
		can_travel_opposite=p.can_travel_opposite;
		step_size=p.step_size;
		value=p.value;
		state_for_special_moves=p.state_for_special_moves;
	}

	bool operator==(const Piece& p){

		if(type==p.type && colour==p.colour && mult_by_constant==p.mult_by_constant &&
			can_travel_opposite==p.can_travel_opposite && 
			state_for_special_moves==p.state_for_special_moves)
			return 1;
		return 0;
	}

	vector<Coordinate> possibleMoves();
	bool is_empty(){
		return type=="Empty";
	}
	vector<Coordinate> spAndValidMoves(board);
	vector<Coordinate> validMoves(board);
	vector<Coordinate> specialMoves(board);
};


class Pawn : public Piece{
public:
	Pawn(){
		type="Pawn";
		value=1;
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
		value=5;
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
		value=3;
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
		value=3;
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
		value=9;
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
		value=1500000000;
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
		value=0;
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
	bool operator==(board& b){
		int ch=1;
		for(int i=0;i<8;i++)
			for(int j=0;j<8;j++){
				if(!(a[i][j]==b.a[i][j])) return 0;
			}
		return 1;
	}
	board p(){return *this;}
	vector<Coordinate> allvalidmoves(int);
	bool check_influence(Coordinate,int);
	bool check(int state);
	Coordinate findking(int);
	
};

struct infoboard{
	board b;
	Coordinate i,f;
	infoboard(){
		i.x=0;
	}
	infoboard(board a,Coordinate i1,Coordinate f1){
		b=a;i=i1;f=f1;
	}
};

struct tree
{
	infoboard node;
	int alpha;
	int beta;
	int x;
	int value;
	vector<tree*> vec;
	tree()
	{	
		x=0;
		value=0;
	}
	// ~tree()
	// {
	// 	for(int i=0;i<vec.size();i++)
	//  	{
	//  		delete vec[i];
	//  	}
	// }
};






bool inside_board(Coordinate);
vector<Coordinate> merge(vector<Coordinate>, vector<Coordinate>);
void initialise_board(board &);
board update_castle(Coordinate,board);
board update_board(Coordinate,Coordinate,board);
int game_end(board,int);
bool draw_piece(board);
board b_init;
int inf=100000000;
string piece_cut;
bool is_present(Coordinate,vector<Coordinate>);
string comp_move(tree*& ,string);
string is_valid(string);
string is_piece_cut(Coordinate);
vector<board> generate_all_possible_boards(board,int);
void generate_tree(tree* &,int,string);
void increase_tree(tree* &,string);
int evaluate(board ,string);
tree* alpha_beta(tree*&,int,string);
vector<infoboard> all_boards(board ,int);
void delete_all(tree* &);
string is_valid_single(string,tree* &);

int main(int argc,char* argv[]){
	// string s;
	// ifstream file;
	// file.open("coord_file");
	// getline(file,s);
	// cout<<s;
	//int check;
	//vector<tree*> v;
	initialise_board(b_init);
	int state=1;
	char* p =argv[0];
	string z;
	if(*p == 'w')
	{
		z="white";
	}
	else
		z="black";
	cout<<z;
	FILE* file;
	FILE* write_file;
	FILE* new_file;
	write_file = fopen("valid_file", "w");
	new_file = fopen("diff_file","w");
	string write_file_start = ")";
	string new_file_start = ")";
	char c[] = ")00";
	fprintf(write_file, "%s", c);
	fprintf(new_file, "%s", c);
	fclose(write_file);
	fclose(new_file);
	char input[5];
	string s = "", n = "", res;
	string kes;
	infoboard a;
	tree* t = new tree;
	if(z=="black")
	{
	    	state *= -1;
	    	a.b=b_init;
            t->node = a;
            generate_tree(t,3,"black");
	}
	else
		{
			while(1)
			{
			file = fopen("coord_file", "r");
			if (1){
			fscanf(file, "%[^\n]", input);
			string n(input);
			fclose(file);
		}
			if(n == s){
				continue;
			}
			else{
				// while(!write_file.is_open()){
				// 	write_file.open("valid_file", ios::trunc);
				// }
				
				if (write_file_start == ")"){
					res = "(" + is_valid(n) + piece_cut;
					write_file_start = "(";
				}
				else {
					res = ")" + is_valid(n) + piece_cut;
					write_file_start = ")";
				}
				write_file = fopen("valid_file", "w");
				fprintf(write_file, "%s", res.c_str());
				fclose(write_file);
				//write_file.close();
				s = n;
			}
                if(is_valid(n) == "1")
                {
                	state *= -1;
                	int xi = int(n[0] - 'a') + 1;
					int yi = int(n[1] - '0');
					int xf = int(n[3] - 'a') + 1;
					int yf = int(n[4] - '0');
					Coordinate ci(xi,yi);
	    			Coordinate cf(xf,yf);
                	b_init=update_board(ci,cf,b_init);
                	a.b=b_init;
            		t->node = a;
            		generate_tree(t,3,"white");
            		break;
                }
                else
                	continue;
			}
			
	    }
	while(1){
		if(state==-1)
		{
			new_file = fopen("diff_file", "w");
				if (new_file_start == ")"){
					kes = "(" + comp_move(t,z) + piece_cut;
					new_file_start = "(";
				}
				else {
					kes = ")" + comp_move(t,z) + piece_cut;
					new_file_start = ")";
				}
				state*=-1;
				//new_file << kes;
				fprintf(new_file, "%s", kes.c_str());
				fclose(new_file);
			
		}
		else
		{
			file = fopen("coord_file", "r");
			if (1){
			fscanf(file, "%[^\n]", input);
			string n(input);
			fclose(file);
			if(n == s){
				continue;
			}
			else{
				write_file = fopen("valid_file", "w");
				string s = is_valid_single(n,t);
				if (write_file_start == ")"){
					res = "(" + s + piece_cut;
					write_file_start = "(";
				}
				else {
					res = ")" + s + piece_cut;
					write_file_start = ")";
				}
				if(s=="1")
				{
					state *= -1;
                	increase_tree(t,z);
				}
				// write_file << res;
				// write_file.close();
				fprintf(write_file, "%s", res.c_str());
				fclose(write_file);
				s = n;
			    }
			}
		else continue;

		}
	}
}

// int main()
// {
// 	tree* t =new tree;
// 	infoboard a;
// 	initialise_board(b_init);
	
// 	// Coordinate c1(4,7);
// 	// Coordinate c2(4,5);
// 	// Coordinate c3(2,1);
// 	// Coordinate c4(3,3);
// 	// b_init = update_board(c1,c2,b_init);
// 	// b_init = update_board(c3,c4,b_init);
// 	a.b=b_init;
// 	t->node=a;
// 	generate_tree(t,5,"black");
// 	// cout<<is_piece_cut(c);
// 	// t=alpha_beta(t,1,"black");
// 	// tree* ab = alpha_beta(t,1,"black");
// 	// cout<<ab->node.b.a[0][0].type<<endl<<ab->node.b.a[0][0].type<<endl;
// 	string s=comp_move(t,"black");
// 	cout<<s<<endl;
// 	b_init=t->vec[0]->node.b;
// 	t=t->vec[0];
// 	increase_tree(t,"black");
// 	t->alpha=-inf;
// 	t->beta=inf;
// 	t->value=0;
// 	//cout<<"h"<<endl;
// 	string p=comp_move(t,"black");
// 	cout<<p;
// 	delete_all(t);

// }

// int main()
// {
// 	tree* t = new tree;
// 	initialise_board(b_init);
// 	t->node.b=b_init;
// 	generate_tree(t,3,"black");
// 	cout<<2<<endl;
// 	cout<<comp_move(t,"black")<<endl;
// 	cout<<3<<endl;
// 	cout<<is_valid_single("a7 a6",t)<<endl;
// 	cout<<4<<endl;
// 	increase_tree(t,"black");
// 	cout<<5<<endl;
// 	cout<<comp_move(t,"black")<<endl;
// }
void delete_all(tree* &t)
{
	if(t->vec.size()==0)
		delete t;
	else
	{
		for(int i=0;i<t->vec.size();i++)
		{	delete t;
		    delete_all(t->vec[i]);
		}	
	}
}


vector<infoboard> all_boards(board b,int state){
	vector<infoboard> u;
	vector<Coordinate> v;
	int i,j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++){
			
			if(b.a[i][j].colour==state && b.a[i][j].type!="King"){
				v=merge(b.a[i][j].validMoves(b),b.a[i][j].specialMoves(b));
			}   
			else if(b.a[i][j].colour==state){
				v=b.a[i][j].validMoves(b);
				//vector<Coordinate> p=b.a[i][j].specialMoves(b);
				//for(int k=0;k<v.size();k++){
				//	board t=update_castle(p[k],b);
				//	if(!t.check(b.a[i][j].colour)){
				//		u.push_back(t);
				//	}
					
				}
			
			for(int k=0;k<v.size();k++){
				Coordinate p(i+1,j+1);
				board b2=update_board(p,v[k],b);
				if(b2==b){}
				else
				{
					infoboard y(b2,p,v[k]);
					u.push_back(y);
				}
			}
		}
	}
	return u;
}


void generate_tree(tree* &t,int level,string p)
{
	//cout<<level<<endl;
     if(level==0)
     {}
     else
     {
     	if(level==3)
		{
			t->alpha = -inf;
			t->beta = inf;
		}
	    vector<infoboard> a;
     	if(p=="black")
     	{
     		a = all_boards(t->node.b,1);
     	}
     	else
     	{
     		a = all_boards(t->node.b,-1);
     	}
     	t->vec.resize(a.size());
     	//cout<<a.size()<<endl;
     	for(int i=0;i<a.size();i++)
     	{

     		tree* next_t = new tree;
     		next_t->node = a[i];
     		t->vec[i] = next_t;
     		//cout<<t->vec[i]->node.b.a[1][2].type<<endl;
     	}
     	for(int i=0;i<a.size();i++)
     	{
     		if(p=="black")
     		{
     			int k= game_end(t->vec[i]->node.b,-1);
     			if(game_end(t->vec[i]->node.b,1)==0)
     			generate_tree(t->vec[i],level-1,"white");
     			else
     				t->vec[i]->x=k;
     		}
     		
     		else
     		{   
     			int k= game_end(t->vec[i]->node.b,1);
     			if(game_end(t->vec[i]->node.b,-1)==0)
     	    	generate_tree(t->vec[i],level-1,"black");
     	    	else
     				t->vec[i]->x=k;
     		}
     			
     	}


     }
}

void increase_tree(tree* &t,string p)
{


    if(t->vec.size()==0)
    {   
    	if(p=="black")
    	generate_tree(t,2,"black");
    	else
    	generate_tree(t,2,"white");
    	//cout<<p<<endl;
	}
    else
    {
    	for(int i=0;i<t->vec.size();i++)
    	{

    		if(p=="black")
    	{
    		if(t->vec[i]->x==0)
    		increase_tree(t->vec[i],"white");
    	}
    	    else
    	    	if(t->vec[i]->x==0)
    	    	increase_tree(t->vec[i],"black");
    	}

    }
}

int evaluate(board b,string p)
{
	int value=0;
        for(int i=0;i<8;i++)
        {
        	for(int j=0;j<8;j++)
        	{
            	if(b.a[i][j].colour=1)
            	value=value-b.a[i][j].value;
            	else
            		value=value+b.a[i][j].value;
        	}
        }
	if(p=="white")
		return value;
	else
		return -value;
}
void alpha_betaset(tree* &t,int state,string p){
	if(t->vec.size()==0){
		if(t->x==0){
			t->value=evaluate(t->node.b,p);
		}
		else{
			if(t->x==2) t->value=0;
			else t->value=state*inf;
		}
	}
	else{
		for(int i=0;i<t->vec.size();i++){
			t->vec[i]->alpha=t->alpha;
			t->vec[i]->beta=t->beta;
			alpha_betaset(t->vec[i],-1*state,p);
			if(state==-1){
				if(t->beta>t->vec[i]->value){
					t->beta=t->vec[i]->value;
					t->value=t->beta;
				}	
			}
			if(state==1){
				if(t->alpha<t->vec[i]->value){
					t->alpha=t->vec[i]->value;
					t->value=t->alpha;
				}
			}
			if(t->alpha > t->beta){
				t->value=inf*state;
				//cout<<1<<endl;
				break;
				//return t;
			}	
		}

	}
}
tree* alpha_beta(tree* &t,int state,string p){
	alpha_betaset(t,state,p);
	//cout<<t->vec.size()<<"a"<<endl;
	for(int i=0;i<t->vec.size();i++){
		//cout<<t->value<<" "<<t->vec[i]->value<<endl;
		if(t->value==t->vec[i]->value){
			//cout<<t->value<<endl;
			return t->vec[i];

		}
	}
}
string comp_move(tree* &t,string p)
{
	//cout<<"c"<<endl;
	//tree* next;
	t=alpha_beta(t,1,p);
	// for(int i=0;i<t->vec.size();i++)
	// {
	// 	if(t->vec[i]!=next)
	// 		delete_all(t->vec[i]);
	// }
	// delete t;
	// t=new tree;
	//t=next;
	//cout<<"as"<<endl;
	//cout<<t->value<<endl;
	//cout<<t->node.i.x<<endl;
 	b_init = t->node.b;
 	//cout<<2<<endl;
	Coordinate ci = t->node.i;
	Coordinate cf = t->node.f;
	//cout<<3<<endl;
	piece_cut = is_piece_cut(cf);
	char a=ci.x+96;
	char b=cf.x+96;
	char c=ci.y+48;
	char d=cf.y+48;
	string s="     ";
	s[0]=a;
	s[1]=c;
	s[2]=' ';
	s[3]=b;
	s[4]=d;
 	return s;
	int g = t->x;
		if(g == 1){
			return s+"2";
		}
		if (g == 2){
			return s+"3";
		}
	//cout<<4<<endl;
	
}

string is_valid_single(string s,tree* &t)
{
	int xi = int(s[0] - 'a') + 1;
	int yi = int(s[1] - '0');
	int xf = int(s[3] - 'a') + 1;
	int yf = int(s[4] - '0');
	Coordinate ci(xi,yi);
	Coordinate cf(xf,yf);
	//int check=-1;
	// vector<tree*> v;
	int i;
	piece_cut = is_piece_cut(cf);
	// board b_present=update_board(ci,cf,b_init);
	b_init=update_board(ci,cf,b_init);
	for(i=0;i<t->vec.size();i++)
       {
       	if(t->vec[i]->node.b==b_init)
       		{//check=i
       		t=t->vec[i];
       		t->alpha = -inf;
       		t->beta = inf;
       		break;
       		} 
       	// else
       	// 	v.push_back(t->vec[i]);
       }
       // if(check==-1)
       // 	return "0";
       if(i==t->vec.size())
       		return "0";
       // else
       // 	{	
       // 		//b_init=b_present;
       // 		// delete t;
       // 		// t=new tree;
       // 	 // 	t=t->vec[check];
       // 		// t->alpha = -inf;
       // 		// t->beta = inf;
       // 		// for(int i=0;i<v.size();i++)
       // 		// {
       // 		// 	delete_all(v[i]);
       // 		// }
       // 	}
       	if(t->x==2)
       			return "2";
       	else if(t->x==3)
       			return "3";
       	else return "1";	
                			
}

bool is_present(Coordinate c, vector<Coordinate> v){
	if (v.size() == 0) return 0;
	if (v.back() == c) return 1;
	v.pop_back();
	return is_present(c,v);
}
vector<Coordinate> Piece::spAndValidMoves(board b){
	vector<Coordinate> a = validMoves(b);
	vector<Coordinate> c;
	vector<Coordinate> v;
	if (type == "Pawn"){
		c = specialMoves(b);
		a=merge(a,c);
	}
	return a;
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



string is_piece_cut(Coordinate cf){
	Piece p = b_init.a[cf.x-1][cf.y-1];
	string s = p.type;
	if (s == "Empty"){
		return "0";

	}
	return "1";
}

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
		vector<Coordinate> v=merge(b_init.a[i.x-1][i.y-1].validMoves(b_init),
			b_init.a[i.x-1][i.y-1].specialMoves(b_init));
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
			if (b.a[c.x-1][c.y-1].type == "Rook" && b.a[c.x-1][c.y-1].state_for_special_moves == 0 
				&& b.a[1][c.y-1].type == "Empty" 
				&& b.a[2][c.y-1].type == "Empty" && (!b.check(colour))){
				//c.x = posn.x - 2;
				res.push_back(c);
			}
			c.x = 8;
			if (b.a[c.x-1][c.y-1].type == "Rook" && b.a[c.x-1][c.y-1].state_for_special_moves == 0 
				&& b.a[4][c.y-1].type == "Empty" 
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
