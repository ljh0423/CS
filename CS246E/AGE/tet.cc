/*
This program implements a simple tetris game using the AGE game engine.
Blocks are created randomly, and tetris is implemented horizontally to maximize playability in the given play area dimensions.
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

//create 4space square block of char at the given position
void add2(std::vector<std::tuple<int,int,char>>& v,std::pair<int,int> p,char c){
  v.push_back(std::make_tuple(p.first,p.second,c));
  v.push_back(std::make_tuple(p.first,p.second+1,c));
  v.push_back(std::make_tuple(p.first+1,p.second,c));
  v.push_back(std::make_tuple(p.first+1,p.second+1,c));
}
//checkcolumn checks if a column is full
int ccol(std::shared_ptr<Element> e,std::pair<int,int> p){
  for(int w=1;w<79;++w){
    bool b=1;
    for(int l=1;l<21;++l){
      bool b1=0;
      for(auto &t:e->getv()){
        if(std::get<0>(t)+p.first==l && std::get<1>(t)+p.second==w) {b1=1;break;}
      }
      if(b1==0) {b=0;break;}
    }
    if(b==1) {
      {return w;}
    }
  }
  return 0;
}
//checkdeletecolumn deletes the column if full, and increments point
void cdcol(std::shared_ptr<Element> e,std::pair<int,int> p){
  int i = ccol(e,p);
  while(i!=0){
    Gamestate::point += 5;
    e->delc(i,p);
    i=ccol(e,p);
  }
}
//returns a random block from 7 types of blocks in tetris
std::vector<std::tuple<int,int,char>> getblock() {
  std::vector<std::tuple<int,int,char>> block1;
  add2(block1,std::make_pair(0,0),'B');
  add2(block1,std::make_pair(2,0),'B');
  add2(block1,std::make_pair(2,2),'B');
  add2(block1,std::make_pair(2,4),'B');
  std::vector<std::tuple<int,int,char>> block2;
  add2(block2,std::make_pair(0,4),'B');
  add2(block2,std::make_pair(2,0),'B');
  add2(block2,std::make_pair(2,2),'B');
  add2(block2,std::make_pair(2,4),'B');
  std::vector<std::tuple<int,int,char>> block3;
  add2(block3,std::make_pair(0,0),'B');
  add2(block3,std::make_pair(0,2),'B');
  add2(block3,std::make_pair(2,0),'B');
  add2(block3,std::make_pair(2,2),'B');
  std::vector<std::tuple<int,int,char>> block4;
  add2(block4,std::make_pair(0,2),'B');
  add2(block4,std::make_pair(0,4),'B');
  add2(block4,std::make_pair(2,0),'B');
  add2(block4,std::make_pair(2,2),'B');
  std::vector<std::tuple<int,int,char>> block5;
  add2(block5,std::make_pair(0,2),'B');
  add2(block5,std::make_pair(2,0),'B');
  add2(block5,std::make_pair(2,2),'B');
  add2(block5,std::make_pair(2,4),'B');
  std::vector<std::tuple<int,int,char>> block6;
  add2(block6,std::make_pair(0,0),'B');
  add2(block6,std::make_pair(0,2),'B');
  add2(block6,std::make_pair(2,2),'B');
  add2(block6,std::make_pair(2,4),'B');
  std::vector<std::tuple<int,int,char>> block7;
  add2(block7,std::make_pair(0,0),'B');
  add2(block7,std::make_pair(0,2),'B');
  add2(block7,std::make_pair(0,4),'B');
  add2(block7,std::make_pair(0,6),'B');
  std::vector<std::vector<std::tuple<int,int,char>>> blocks{block1,block2,block3,block4,block5,block6,block7};
  int k = rand() % 7;
  return blocks[k];

}
//allocates a random tetris block and returns shared_ptr
std::shared_ptr<Element> randb() {
  std::shared_ptr<PC> pc = std::make_shared<PC>();
  std::shared_ptr<GR> g = std::make_shared<GR>(std::make_pair(0,1));
  std::vector<std::shared_ptr<MoveChar>> v1;
  v1.push_back(g);
  v1.push_back(pc);
  std::map<int,std::vector<int>> c;
  c.insert(std::make_pair(4,std::vector<int>{2}));
  c.insert(std::make_pair(3,std::vector<int>{1}));
  std::vector<std::tuple<int,int,char>> n1 = getblock();
  std::shared_ptr<Bitmap> b = std::make_shared<Bitmap>(1,c,v1,n1);
  return b;
}

int main() {
  srand(time(NULL));
  std::shared_ptr<View> v = std::make_shared<View>();
  //create border
  std::shared_ptr<CP> cp = std::make_shared<CP>();
  std::vector<std::shared_ptr<MoveChar>> v1;
  v1.push_back(cp);
  std::map<int,std::vector<int>> c;
  c.insert(std::make_pair(3,std::vector<int>{1}));
  c.insert(std::make_pair(4,std::vector<int>{1}));
  c.insert(std::make_pair(5,std::vector<int>{1}));
  std::shared_ptr<Border> r = std::make_shared<Border>(1,c,v1,22,80,'+','-','|');
  v->addEl(r,std::make_pair(0,0));
  //create baseline
  cp = std::make_shared<CP>();
  v1.clear();
  v1.push_back(cp);
  c.insert(std::make_pair(3,std::vector<int>{1}));
  c.insert(std::make_pair(1,std::vector<int>{2}));
  std::vector<std::tuple<int,int,char>> n1;
  for(int a=1;a<21;++a){
    n1.push_back(std::make_tuple(a,79,'|'));
  }
  std::shared_ptr<Bitmap> accb = std::make_shared<Bitmap>(1,c,v1,n1);
  v->addEl(accb,std::make_pair(0,0));
  //create upperline
  cp = std::make_shared<CP>();
  v1.clear();
  v1.push_back(cp);
  c.clear();
  c.insert(std::make_pair(3,std::vector<int>{1}));
  c.insert(std::make_pair(1,std::vector<int>{2}));
  n1.clear();
  for(int a=1;a<21;++a){
    n1.push_back(std::make_tuple(a,0,'|'));
  }
  std::shared_ptr<Bitmap> hibnd = std::make_shared<Bitmap>(1,c,v1,n1);
  v->addEl(hibnd,std::make_pair(0,0));
  //two single chars to show bounce collision type and view border
  std::shared_ptr<SLP> slp = std::make_shared<SLP>(std::make_pair(0,1));
  v1.clear();
  v1.push_back(slp);
  slp = std::make_shared<SLP>(std::make_pair(1,0));
  v1.push_back(slp);
  c.clear();
  c.insert(std::make_pair(4,std::vector<int>{2}));
  c.insert(std::make_pair(5,std::vector<int>{1}));
  std::shared_ptr<SingleC> sc = std::make_shared<SingleC>(1,c,v1,'@');
  v->addEl(sc,std::make_pair(18,1));
  c.clear();
  c.insert(std::make_pair(4,std::vector<int>{1,2}));
  slp = std::make_shared<SLP>(std::make_pair(0,1));
  v1.clear();
  v1.push_back(slp);
  sc = std::make_shared<SingleC>(1,c,v1,'@');
  v->addEl(sc,std::make_pair(19,1));
  //create and bring in the random tetris block at start
  std::shared_ptr<Element> e = randb();
  v->addEl(e,std::make_pair(8,0));
  //background
  std::vector<std::tuple<int,int,char>> exp1;
  add2(exp1,std::make_pair(6,38),'.');
  add2(exp1,std::make_pair(9,34),'.');
  add2(exp1,std::make_pair(12,38),'.');
  add2(exp1,std::make_pair(9,42),'.');

  std::vector<std::tuple<int,int,char>> exp2;
  add2(exp2,std::make_pair(3,38),'.');
  add2(exp2,std::make_pair(9,26),'.');
  add2(exp2,std::make_pair(15,38),'.');
  add2(exp2,std::make_pair(9,50),'.');

  std::vector<std::vector<std::tuple<int,int,char>>> nv;
  nv.push_back(exp1);
  nv.push_back(exp2);
  cp = std::make_shared<CP>(nv);
  v1.clear();
  v1.push_back(cp);
  std::map<int,std::vector<int>> c1;
  std::vector<std::tuple<int,int,char>> exp0;
  add2(exp0,std::make_pair(9,38),'.');
  std::shared_ptr<Bitmap> backg = std::make_shared<Bitmap>(0,c1,v1,exp0);
  v->addEl(backg,std::make_pair(0,0));
  //create model with v, and create actions for arrow input
  Model m(v);
  std::vector<std::pair<int,int>> mvment {std::make_pair(-1,0),std::make_pair(1,0),std::make_pair(0,0),std::make_pair(0,1)};

  initscr();
  noecho();
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  int i=0,bch=1;
  while ((i=getch())!='e'){
    if(bch) {std::pair<int,int> p=m.getview()->getpos(sc);if(p.first>19 || p.second>78){m.getview()->remEl(sc);bch=0;}}
    std::pair<int,int> np = m.getview()->getpos(e);
    if((m.getview()->overlap(e,std::make_pair(np.first,np.second+1),accb)).first==1){
      m.getview()->merge(accb,e);
      e = randb();
      m.getview()->addEl(e,std::make_pair(8,0));
    }
    cdcol(accb,std::make_pair(0,0));
    m.updatef(i,mvment);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  endwin();
}
