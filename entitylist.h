
#ifndef ENTITYLIST_H
#define ENTITYLIST_H
#include "ComponentList.h"
#include <string>
#include <map>
#include <math.h>
#include <vector>

#define WALLCH ' '
#define FLOORCH '.'

class Entity;
class HeroEntity;
class MobEntity;
class MapEntity;
struct Room;
struct Tile;

class Entity{
public:
  int ID;
  Visible vis;
  MapEntity *map;

  Entity(MapEntity *m = nullptr, int id = 0, POS * p = nullptr, WINDOW * p_win = nullptr, Transform * trans = nullptr, char ch = ' ', int col = 0):
    ID(id),
    vis(Visible(p_win, p, trans, ch, 1, col)),
    map(m)
    {
    }

};

class MobEntity : public Entity{
  public:
    std::string name;
    Transform trans;
    Health hp;
    POS pos;
    //Stats stats;

    MobEntity(MapEntity *m = nullptr, WINDOW * p_win = nullptr, POS p = POS(), int id = 0, int h = 50, int col = 0, char ch = '%'):
    hp(h),
    //stats(),
    trans(),
    pos(p),
    Entity(m, id, &pos, p_win, &trans, ch, col)
    {}

};

class HeroEntity : public Entity{
public:
	std::string name;
  Transform trans;
  POS pos;
	Health hp;
	Joystick joystick;
  int sight;
  //Stats stats;

	HeroEntity(std::string p_name, WINDOW * p_win, MapEntity *p_map, int col = 0):
    trans(),
    hp(),
    //stats(25, 15),
    pos(POS(5,5)),
		name(p_name),
		joystick(&pos, &trans),
    Entity(p_map, 0, &pos, p_win, &trans, '@', col)
	{
    sight = 4;
  }

};


struct Tile{

  bool iswall, isseen, islit;
  char icon;

  Tile(char c = '%'): icon(c){
    iswall = isseen = islit = 0;
  }

  void makewall(){
    iswall = 1;
    icon = WALLCH;
  }
  void makefloor(){
    iswall = 0;
    icon = FLOORCH;
  }
  void makeseen(){
    isseen = 1;
  }
  void makelit(){
    islit = 1;
  }

  int getcolor(){
    if(iswall){
      return (isseen)? 3 : 2;
    }else{
      return (isseen)? 3 : 2;//essentially it doesn'y yet matter if its a wall or floor, the color will be the same. i just want the infrastructure for different colored walls/floors
    }
  }

  bool hasentity(){
    return (false);
  }


};

struct Room{
  POS origin;
  int height, width;

  Room(POS p, int h, int w){
    origin = p;
    height = h;
    width = w;
  }
  Room(){
    origin = POS(0,0);
    height = width = 0;
  }

  bool intersects(Room  &l_room){
    POS l1(this->origin.x, this->origin.y);
    POS r1(l1.x + this->width, l1.y + this->height);//l2 and r2 are same but l_room.coords
    POS l2(l_room.origin.x, l_room.origin.y);
    POS r2(l2.x + l_room.width, l2.y + l_room.height);

    if(l1.x > r2.x || l2.x > r1.x || l1.y > r2.y || l2.y > r1.y){
      return false;
    }

    return true;

  }
};

class MapEntity{
private:
  Room n[10];
public:
	WINDOW * m_win;
  std::vector<Room> rooms;
	int height, width;
  bool ischanged;
  std::vector<std::vector<Tile> > matrix;

	std::string name;

	MapEntity(WINDOW * p_win = nullptr, const std::string &p_name = "Default"):
		m_win(p_win), name(p_name)
	{
		getmaxyx(m_win, height, width);
    height -= 2;
    matrix = std::vector<std::vector<Tile> > (width, std::vector<Tile>(height));
 		for(int i = 0; i < width; i++){
			for(int j = 0; j < height; j++){
        matrix[i][j].makewall();
			}
		}
    ischanged = 1;
	}

  void connect_rooms(Room *begin, Room *end){
    int bmidx, bmidy, emidx, emidy;

    bmidx = begin->origin.x + (begin->width / 2);
    bmidy = begin->origin.y + (begin->height / 2);

    emidx = end->origin.x + (end->width / 2);
    emidy = end->origin.y + (end->height / 2);

    int x = fmin(bmidx, emidx);
    int xe = fmax(bmidx, emidx);

    int y = fmin(bmidy, emidy);
    int ye = fmax(bmidy, emidy);

    if(x == bmidx){
      for(x; x <= xe; x++){
        matrix[x][bmidy].makefloor();
      }
    }else if(x == emidx){
        for(x; x <= xe; x++){
          matrix[x][emidy].makefloor();
        }
    }
    for(y; y <= ye; y++){
      matrix[x][y].makefloor();
    }
  }
};



#endif
