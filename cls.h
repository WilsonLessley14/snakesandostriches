#ifndef CLS_H
#define CLS_H

#include <vector>
#include <iostream>
#include "EntityList.h"
#include <ncurses.h>


std::vector<Visible*> Visible::visible_list;
std::vector<Joystick*> Joystick::stick_list;


namespace position{

	struct LessThan{
		bool operator()(const POS &l, const POS &r)const{
			return (l.x + l.y) + (r.x + r.y);
		}
	};

}


namespace transform{
  void move(Entity * ent, const int &dx, const int &dy){

  }
}

namespace joystick{

  bool checkifspaceoccupied(HeroEntity * hero, Joystick * tbm, POS &temp_pos);
  void move(HeroEntity * hero, const int &dx, const int &dy);


	void move(HeroEntity * hero, const int &dx, const int &dy){
    Joystick * tbm = &hero->joystick;
    MapEntity * map = hero->map;
    POS temp_pos = POS(hero->vis.c_pos->x + dx, hero->vis.c_pos->y + dy);
    bool isoccupied = checkifspaceoccupied(hero, tbm, temp_pos),
         iswall = map->matrix[tbm->c_pos->x + dx][tbm->c_pos->y + dy].iswall;

    if(isoccupied){
      tbm->t_form->hasmoved = 0;

    }else if((tbm->t_form->canmove && !iswall) /*&& !isoccupied*/){
			tbm->t_form->hasmoved = 1;
			tbm->t_form->px = tbm->c_pos->x;//saving previous position
			tbm->t_form->py = tbm->c_pos->y;

			tbm->c_pos->x += dx;//updating position to new position
			tbm->c_pos->y += dy;

		}else{
      tbm->t_form->hasmoved = 0;
			return;
		}
	}

  bool checkifspaceoccupied(HeroEntity * hero, Joystick * tbm, POS &temp_pos){
    for(auto i : Visible::visible_list){
      return (i->c_pos->x == temp_pos.x && i->c_pos->y == temp_pos.y) && !(i == &(hero->vis));
    }
  }
}

namespace visible{

	void update(MapEntity *map){
		for(auto i : Visible::visible_list){
			if(i->isvis && i->t_form->hasmoved){
        int px = i->t_form->px;
        int py = i->t_form->py;
				mvwaddch(i->m_win, py, px, map->matrix[px][py].icon | COLOR_PAIR(map->matrix[px][py].getcolor()));//first print a space at previous spot
				mvwaddch(i->m_win, i->c_pos->y, i->c_pos->x, i->icon | COLOR_PAIR(i->color));//then print icon at the new spot
				i->t_form->hasmoved = 0;
			}else if(i->isvis && !i->t_form->hasmoved){
        mvwaddch(i->m_win, i->c_pos->y, i->c_pos->x, i->icon | COLOR_PAIR(i->color));
      }
		}
	}

}
namespace hero{
  void update(HeroEntity * hero);
  void see_octant(HeroEntity * hero, int oct);
  void see_square(HeroEntity * hero);


  void update(HeroEntity *hero){
    mvwaddstr(hero->map->m_win, hero->map->height, 0, hero->hp.str().c_str());
    mvwaddstr(hero->map->m_win, hero->map->height + 1, 0, hero->pos.str().c_str());
    //waddstr(hero->map->m_win, hero->vis.c_pos->str().c_str());
  }

  void see_octant(HeroEntity *hero, int oct){

    for(int row = 1; row <= hero->sight; row++){
      for(int col = 0; col <= row; col++){
        int y = 0, x = 0;
        switch(oct){
          case 0:
            x = hero->pos.x + col;
            y = hero->pos.y - row;
            break;
          case 1:
            x = hero->pos.x - col;
            y = hero->pos.y - row;
            break;
          case 2:
            x = hero->pos.x + col;
            y = hero->pos.y + row;
            break;
          case 3:
            x = hero->pos.x - col;
            y = hero->pos.y + row;
            break;
          case 4://
            x = hero->pos.x + row;
            y = hero->pos.y - col;
            break;
          case 5:
            x = hero->pos.x - row;
            y = hero->pos.y - col;
            break;
          case 6:
            x = hero->pos.x + row;
            y = hero->pos.y + col;
            break;
          case 7:
            x = hero->pos.x - row;
            y = hero->pos.y + col;
            break;
          default:
            break;
        }

        hero->map->matrix[x][y].makeseen();
        hero->map->matrix[x][y].makelit();
        hero->map->ischanged = 1;
      }
    }
  }

  void clear_square(HeroEntity *hero) {
    for(int row = 0; row < hero->sight; row++) {
      for (int col = 0; col < hero->sight; col++) {
        
      }
    }
  }

  void see_square(HeroEntity *hero){

    for(int i = 0; i < 8; i++){
       see_octant(hero, i);
    }
  }
}

namespace map{
  void render(MapEntity * map){
   if(!map->ischanged){return;}
   for(int i = 0; i < map->width; i++){
      for(int j = 0; j < map->height; j++){
        mvwaddch(map->m_win, j, i, map->matrix[i][j].icon | COLOR_PAIR(map->matrix[i][j].getcolor()));
      }
    }
   map->ischanged = 0;
  }

  void make_room(MapEntity *map, POS pos, int width, int height, int num){
    map->ischanged = true;
    Room r(pos, height, width);
    map->rooms.push_back(r);
    for(int i = pos.x; i <= width + pos.x; i++){
      for(int j = pos.y; j <= height + pos.y; j++){
        map->matrix[i][j].makefloor();
      }
    }
    //map->matrix[pos.x][pos.y].icon = num + 48;
  }

  void connect_room_list(MapEntity *map){
    for(int i = 0; i < map->rooms.size(); i++){
      if(i == 0)
          continue;

      map->connect_rooms(&map->rooms[i-1], &map->rooms[i]);
      /*map->ischanged = 1;
      render(map);
      wgetch(map->m_win);*/
    }
  }
}



#endif
