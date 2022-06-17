#include <ncurses.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "CLS.h"

#define MAPWIDTH 120
#define MAPHEIGHT 50
#define ROOMCAP 17
#define MAXROOMHEIGHT (MAPHEIGHT/(ROOMCAP/3)) - 1
#define MAXROOMWIDTH (MAPWIDTH/(ROOMCAP/3)) - 1
#define MINROOMHEIGHT 4
#define MINROOMWIDTH 4

void setup_map(MapEntity *map);
void fill_map(MapEntity *map);
void game_loop(HeroEntity *hero, WINDOW * win, MapEntity * map);
void update(MapEntity * map, HeroEntity * hero);

int main(){
  srand(time(nullptr));
	setlocale(LC_ALL, "");
 
	initscr(); cbreak(); noecho();
	
	WINDOW * win = newwin(MAPHEIGHT, MAPWIDTH, 0, 0);

	nonl();
	//curs_set(0);
	intrflush(win, FALSE);
	keypad(win, TRUE);
  start_color();
  init_pair(1, COLOR_YELLOW, COLOR_BLACK);//Hero
  init_pair(2, COLOR_BLUE, COLOR_BLACK);//unseen floor
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);//seen floor
  init_pair(4, COLOR_MAGENTA, COLOR_BLACK);//Mob
 
  //ENTITY INITIALIZATION SECTION::::::::

	MapEntity map(win, "Main Map");
  MobEntity mob(&map, win, POS(6, 6), 1, 50, 4, 'O');
	HeroEntity hero("Player", win, &map, 1);

  map::make_room(&map, hero.pos, 8, 8, -1);
  setup_map(&map);
  
	wmove(win, hero.pos.x, hero.pos.y);
	waddch(win, hero.vis.icon);
  mvwaddch(win, mob.pos.y, mob.pos.x, mob.vis.icon);
		
	nodelay(win, FALSE);	
	
  game_loop(&hero, win, &map);


	endwin();	
  std::cout << "Got here\n";
	return 0;


}

void fill_map(MapEntity *map){
  for(int i = 0; i < ROOMCAP; i++){
    int sx = (rand() % (MAPWIDTH - MAXROOMWIDTH - 2)) + 1;
    int sy = (rand() % (MAPHEIGHT - MAXROOMHEIGHT - 2)) + 1;
    int swidth = (rand() % (MAXROOMWIDTH - MINROOMWIDTH)) + MINROOMWIDTH;
    int sheight = (rand() % (MAXROOMHEIGHT - MINROOMHEIGHT)) + MINROOMHEIGHT;

    Room temp(POS(sx, sy), sheight, swidth);
    
    bool doesIntersect = false;
    for(int j = 0; j < map->rooms.size(); j++){
      if(temp.intersects(map->rooms[j])){
        doesIntersect = true;
        break;
      }
    }
    
    if(!doesIntersect){
       map::make_room(map, POS(sx, sy),
          swidth, 
          sheight,
          i);
    }else{
      i--;
    }
  }
}


void setup_map(MapEntity *map){
  fill_map(map);
  map::connect_room_list(map);
  map::render(map);
}
//
void game_loop(HeroEntity * hero, WINDOW * win, MapEntity * map){
  int ch = 0;
	while((ch = wgetch(win)) != 27){
		//std::cout << "I entered the loop!\n";
		switch(ch){
			case 1:
			case ERR:
			//	std::cout << "ERR";
				break;
			case 'W':
			case 'w':
				joystick::move(hero, 0, -1);
				break;
			case 'A':
			case 'a':
				joystick::move(hero, -1, 0);
				break;
			case 'S':
			case 's':
				joystick::move(hero, 0, 1);
				break;
			case 'D':
			case 'd':
				joystick::move(hero, 1, 0);
				break;
      case ' ':
        hero::see_square(hero);
        break;
			default:
				break;
			}
    update(map, hero); 
    wrefresh(win);
    wmove(win, MAPHEIGHT, MAPWIDTH);
	}
}

void update(MapEntity * map, HeroEntity * hero){
    hero::see_square(hero); 
    map::render(map);
    visible::update(map);
    hero::update(hero);
}
