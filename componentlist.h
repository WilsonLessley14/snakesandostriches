#ifndef COMPONENTLIST_H
#define COMPONENTLIST_H 
#include <vector>
#include <map>
#include <ncurses.h>
#include <string>
#include <iostream>

struct Health;
//struct Stats;
struct POS;
struct Visible;
struct Joystick;
struct Transform;
struct Tile;
struct POSLessThan;

struct Health{
	int maxhp;
	int curhp;
	Health(int h = 100){
		maxhp = curhp = h;
	}
  std::string str(){
    return std::to_string(curhp) + "/" + std::to_string(maxhp);
  }
};

/*struct Stats{
  int attack;
  int defense;
  int luck;

  Stats(int atk = 20, int def = 10, lck = 5){
    attack = atk;
    defense = def;
    luck = lck;
  }

};*/

struct POS{
	int x, y;	
	POS(int nx = 0, int ny = 0){
		x = nx; y = ny;
	}
  bool operator ==(POS *r)const{
    return (x == r->x) && (y == r->y);
  }
  std::string str(){
    return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
  }
};

struct POSLessThan{
	bool operator()(const POS &l, const POS &r)const{
		return (l.x + l.y) < (r.x + r.y);
	}
};

struct Visible{
	static std::vector<Visible*> visible_list;
	char icon;
	bool isvis;
  int color;
	POS * c_pos;
	Transform * t_form;
	WINDOW * m_win;
	Visible(WINDOW * win = nullptr, POS * n_pos = nullptr, Transform * t = nullptr, char i = '%', bool v = 1, int col = 0){
		icon = i;
    color = col;
		m_win = win;
		t_form = t;
		isvis = v;
		c_pos = n_pos;
		visible_list.push_back(this);
	} 
};


struct Transform{
	int px, py;
	bool hasmoved, canmove;
	Transform(bool m = 1){
		canmove = m;
		hasmoved = m;
    px = py = 0;
	}
};

struct Joystick{
	static std::vector<Joystick*> stick_list;
	POS * c_pos;
	Transform * t_form;
	Joystick(POS * n_pos = nullptr, Transform * t = nullptr, bool m = 1) {
		c_pos = n_pos;
		t_form = t;
		stick_list.push_back(this);
	}
	

};








#endif
