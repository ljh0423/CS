/*
This program implements a basic version of Pacman using the AGE created.
P represents Pacman, G represents ghosts, and . are pellets with # as super pellets.
*/
#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <tuple>
#include <memory>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "AGE.h"

using namespace Game;
//create 4 space square blocks with input
void add2(std::vector<std::tuple<int,int,char>>& v,int p1,int p2,char c){
  v.push_back(std::make_tuple(p1,p2,c));
  v.push_back(std::make_tuple(p1,p2+1,c));
  v.push_back(std::make_tuple(p1+1,p2,c));
  v.push_back(std::make_tuple(p1+1,p2+1,c));
}
//flip the given set of points up(verticallly)
std::vector<std::tuple<int,int,char>> uflip(std::vector<std::tuple<int,int,char>> v){
  std::vector<std::tuple<int,int,char>> nv;
  for(auto t:v){
    std::get<0>(t) = 10-std::get<0>(t);
    nv.push_back(t);
  }
  return nv;
}
//flip the given set of points right(horizontally)
std::vector<std::tuple<int,int,char>> rflip(std::vector<std::tuple<int,int,char>> v){
  std::vector<std::tuple<int,int,char>> nv;
  for(auto t:v){
    std::get<1>(t) = 39-std::get<1>(t);
    nv.push_back(t);
  }
  return nv;
}
//add a wall on the map
void rwall(std::vector<std::tuple<int,int,char>>& v,int y,int x,int l,int w) {
  for(int a=0;a<l;++a){
    v.push_back(std::make_tuple(y+a,x,'-'));
    v.push_back(std::make_tuple(y+a,x+w-1,'-'));
  }
  for (int b=1;b<w-1;++b) {
      v.push_back(std::make_tuple(y,x+b,'-'));
      v.push_back(std::make_tuple(y+l-1,x+b,'-'));
  }
}
//create ghost entities
std::shared_ptr<Element> ghost() {
  std::shared_ptr<SLP> slp = std::make_shared<SLP>(std::make_pair(1,0));
  std::vector<std::shared_ptr<MoveChar>> v1;
  v1.push_back(slp);
  std::map<int,std::vector<int>> c;
  c.insert(std::make_pair(4,std::vector<int>{1}));
  c.insert(std::make_pair(3,std::vector<int>{1}));
  c.insert(std::make_pair(1,std::vector<int>{2}));
  std::vector<std::tuple<int,int,char>> n1;
  std::shared_ptr<Rect> s = std::make_shared<Rect>(1,c,v1,2,2,'G');
  return s;
}
//create player entity
std::shared_ptr<Element> g() {
  std::shared_ptr<PC> pc = std::make_shared<PC>();
  std::vector<std::shared_ptr<MoveChar>> v1;
  v1.push_back(pc);
  std::map<int,std::vector<int>> c;
  c.insert(std::make_pair(3,std::vector<int>{1}));
  c.insert(std::make_pair(2,std::vector<int>{1}));
  c.insert(std::make_pair(1,std::vector<int>{2,3}));
  std::vector<std::tuple<int,int,char>> n1;
  add2(n1,0,0,'P');
  std::shared_ptr<Bitmap> s = std::make_shared<Bitmap>(1,c,v1,n1);
  return s;
}
//create pellets/super pellets
std::shared_ptr<Element> pel(int s) {
  std::vector<std::shared_ptr<MoveChar>> v1;
  std::map<int,std::vector<int>> c;
  c.insert(std::make_pair(4,std::vector<int>{1}));
  c.insert(std::make_pair(2,std::vector<int>{-1}));
  if(s) {c.insert(std::make_pair(1,std::vector<int>{3}));}
  std::vector<std::tuple<int,int,char>> n1;
  if(s) {add2(n1,0,0,'#');}
  else {add2(n1,0,0,'.');}
  std::shared_ptr<Bitmap> b = std::make_shared<Bitmap>(1,c,v1,n1);
  return b;
}
//construct whole map from 1 quadrant using flip
void ad4(std::vector<std::tuple<int,int,char>>& o){
  std::vector<std::tuple<int,int,char>> temp = uflip(o);
  for(auto t:temp) {
    std::get<0>(t) += 11;
    o.push_back(t);
  }
  temp = rflip(o);
  for(auto t:temp) {
    std::get<1>(t) += 40;
    o.push_back(t);
  }
}
//set new direction for the ghost entities
//randomly, but with higher chance of approaching/fleeing from the pacman depending on state
void setdir(std::shared_ptr<View> v,std::shared_ptr<Element> e1,std::shared_ptr<Element> e2){
  std::vector<std::pair<int,int>> mv {std::make_pair(-1,0),std::make_pair(1,0),std::make_pair(0,-2),std::make_pair(0,2)};
  std::pair<int,int> e1p = v->getpos(e1);
  std::pair<int,int> e2p = v->getpos(e2);
  int ydif = e2p.first-e1p.first;
  int xdif = e2p.second-e1p.second;
  if(ydif!=0) {ydif /= abs(ydif);}
  if(xdif!=0) {xdif /= abs(xdif);xdif *= 2;}
  if(Gamestate::gamestate==3) {ydif = -ydif;xdif=-xdif;}
  if(ydif!=0) {mv.push_back(std::make_pair(ydif,0));}
  if(xdif!=0) {mv.push_back(std::make_pair(0,xdif));}
  int k = rand() % mv.size();
  (e1->getmove()[0])->getdir() = mv[k];
}
//enter blue state if u=0, unblue if u=1
void blue(std::vector<std::shared_ptr<Element>>& g,bool u){
  if(u){
    for(auto &t:g){
      (t->getcolt())[2].clear();
      (t->getcolt())[1].push_back(2);
    }
  }
  else {
    for(auto &t:g){
      (t->getcolt())[1].clear();
      (t->getcolt())[2].push_back(-1);
    }
  }
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  std::shared_ptr<View> v = std::make_shared<View>();
  //create border
  std::shared_ptr<CP> cp = std::make_shared<CP>();
  std::vector<std::shared_ptr<MoveChar>> v1;
  v1.push_back(cp);
  std::map<int,std::vector<int>> c;
  c.insert(std::make_pair(3,std::vector<int>{1}));
  std::shared_ptr<Border> r = std::make_shared<Border>(1,c,v1,22,80,'+','-','|');
  v->addEl(r,std::make_pair(0,0));
  //create map
  cp = std::make_shared<CP>();
  std::vector<std::shared_ptr<MoveChar>> v2;
  v2.push_back(cp);
  std::vector<std::tuple<int,int,char>> n1;
  for(int t1=3;t1<9;t1+=2){add2(n1,t1,5,'-');}
  add2(n1,8,5,'-');
  for(int t1=5;t1<13;t1+=2){add2(n1,3,t1,'-');}
  rwall(n1,3,17,4,10);
  rwall(n1,7,11,4,2);
  rwall(n1,9,17,2,10);
  rwall(n1,3,31,4,9);
  rwall(n1,9,31,2,9);
  ad4(n1);
  std::shared_ptr<Bitmap> wa = std::make_shared<Bitmap>(1,c,v2,n1);
  v->addEl(wa,std::make_pair(0,0));
  //create pacman 'P'
  std::shared_ptr<Element> e = g();
  v->addEl(e,std::make_pair(1,2));
  //create pellets
  std::shared_ptr<Element> pell = pel(1);
  v->addEl(pell,std::make_pair(19,2));
  pell = pel(1);
  v->addEl(pell,std::make_pair(1,76));
  pell = pel(1);
  v->addEl(pell,std::make_pair(19,76));
  pell = pel(0);
  v->addEl(pell,std::make_pair(5,2));
  pell = pel(0);
  v->addEl(pell,std::make_pair(5,70));
  pell = pel(0);
  v->addEl(pell,std::make_pair(5,76));
  pell = pel(0);
  v->addEl(pell,std::make_pair(7,22));
  pell = pel(0);
  v->addEl(pell,std::make_pair(15,2));
  pell = pel(0);
  v->addEl(pell,std::make_pair(15,8));
  pell = pel(0);
  v->addEl(pell,std::make_pair(15,76));
  pell = pel(0);
  v->addEl(pell,std::make_pair(13,58));
  //create 3 ghosts
  std::vector<std::shared_ptr<Element>> ghs;
  std::shared_ptr<Element> gh = ghost();
  ghs.push_back(gh);
  v->addEl(gh,std::make_pair(15,8));
  gh = ghost();
  ghs.push_back(gh);
  v->addEl(gh,std::make_pair(5,70));
  gh = ghost();
  ghs.push_back(gh);
  v->addEl(gh,std::make_pair(15,70));

  //create model and define actions for arrow input
  Model m(v);
  std::vector<std::pair<int,int>> mvment {std::make_pair(-1,0),std::make_pair(1,0),std::make_pair(0,-2),std::make_pair(0,2)};

  int fps=50;
  if(argc==2) {fps = std::stoi(argv[1]);}
  initscr();
  noecho();
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  int i=0,timer=0;
  while ((i=getch())!='e'){
    if(Gamestate::point>=110+((3-ghs.size())*10)) {Gamestate::gamestate=2;timer=0;}
    if(timer==30) {timer=0;Gamestate::gamestate=0;}
    if(Gamestate::gamestate==3) {++timer;blue(ghs,0);}
    else if(Gamestate::gamestate==0) {blue(ghs,1);}
    for(size_t k=0;k<ghs.size();++k) {if(!(m.getview()->checkin(ghs[k]))){ghs.erase(ghs.begin()+k);--k;}}
    for(size_t k=0;k<ghs.size();++k) {setdir(m.getview(),ghs[k],e);}
    m.updatef(i,mvment);
    std::this_thread::sleep_for(std::chrono::milliseconds(fps));
  }
  endwin();
}
