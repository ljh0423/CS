#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <tuple>
#include <memory>
#include <vector>
#include "AGE2.h"

using namespace Game;

std::vector<MoveChar*> f() {
  std::unique_ptr<SLP> s = std::make_unique<SLP>(std::make_pair(1,0));
  std::vector<MoveChar*> m;
  m.push_back(s.release());
  return m;
}
void add2(std::vector<std::tuple<int,int,char>>& v,std::pair<int,int> p,char c){
  v.push_back(std::make_tuple(p.first,p.second,c));
  v.push_back(std::make_tuple(p.first,p.second+1,c));
  v.push_back(std::make_tuple(p.first+1,p.second,c));
  v.push_back(std::make_tuple(p.first+1,p.second+1,c));
}
std::shared_ptr<Element> g() {
  std::shared_ptr<PC> pc = std::make_shared<PC>();
  std::shared_ptr<GR> g = std::make_shared<GR>(std::make_pair(0,1));
  std::vector<std::shared_ptr<MoveChar>> v1;
  v1.push_back(g);
  v1.push_back(pc);
  std::map<int,std::vector<int>> c;
  c.insert(std::make_pair(3,std::vector<int>{1}));
  std::vector<std::tuple<int,int,char>> n1;
  add2(n1,std::make_pair(0,2),'B');add2(n1,std::make_pair(2,2),'B');add2(n1,std::make_pair(4,0),'B');add2(n1,std::make_pair(4,2),'B');
  std::shared_ptr<Bitmap> s = std::make_shared<Bitmap>(1,c,v1,n1);
  return s;
}

int ccol1(std::shared_ptr<Element> e,std::pair<int,int> p){
  bool f=0;
  for(int w=1;w<79;++w){
    bool b=1;
    for(int l=1;l<21;++l){
      bool b1=0;
      for(auto &t:e->getv()){
        if(std::get<0>(t)+p.first==l && std::get<1>(t)+p.second==w) {b1=1;break;}
      }
      if(b1==0) {b=0;f=0;break;}
    }
    if(b==1) {
      if(f==1) {return w-1;}
      f=1;
    }
    else {f=0;}
  }
  return 0;
}
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
void cdcol(std::shared_ptr<Element> e,std::pair<int,int> p){
  int i = ccol(e,p);
  while(i!=0){
    e->delc(i,p);
    i=ccol(e,p);
  }
}
int main() {
  //std::vector<MoveChar*> m = f();
  /*
  std::tuple<int,int,char> t (1,1,'k');
  std::vector<std::tuple<int,int,char>> v;
  v.push_back(t);
  Bitmap b(0,'c',m,v);
  Bitmap b1(b);
  b.merge(b1);*/


  /*if (SLP* slp = dynamic_cast<SLP*>(m[0])) {
    std::cout<<slp->dir.first<<std::endl;
  }*/
  std::shared_ptr<View> v = std::make_shared<View>();
  std::shared_ptr<CP> p = std::make_shared<CP>();
  std::vector<std::shared_ptr<MoveChar>> v1;
  v1.push_back(p);
  std::map<int,std::vector<int>> c;
  c.insert(std::make_pair(3,std::vector<int>{1}));
  std::shared_ptr<Border> r = std::make_shared<Border>(1,c,v1,22,80,'+','-','|');
  v->addEl(r,std::make_pair(0,0));
  std::shared_ptr<Element> e;
  for (int k=5;k<6;++k){
    e=g();
    v->addEl(e,std::make_pair(k,5));
  }
  std::vector<std::vector<std::tuple<int,int,char>>> n1;
  std::vector<std::tuple<int,int,char>> nv {std::make_tuple(0,0,'N'),std::make_tuple(0,4,'N'),std::make_tuple(4,0,'N'),std::make_tuple(4,4,'N')};
  n1.push_back(nv);
  std::shared_ptr<CP> np = std::make_shared<CP>(n1);
  std::vector<std::shared_ptr<MoveChar>> v2;
  v2.push_back(np);
  std::map<int,std::vector<int>> c1;
  c1.insert(std::make_pair(3,std::vector<int>{1}));
  std::vector<std::tuple<int,int,char>> nv1 {std::make_tuple(0,2,'N'),std::make_tuple(2,0,'N'),std::make_tuple(2,4,'N'),std::make_tuple(4,2,'N')};
  std::shared_ptr<Bitmap> nr = std::make_shared<Bitmap>(0,c1,v2,nv1);
  v->addEl(nr,std::make_pair(7,34));
  nv1.clear();
  for(int a=1;a<21;++a){
    nv1.push_back(std::make_tuple(a,79,'|'));
  }
  std::shared_ptr<Bitmap> kk = std::make_shared<Bitmap>(1,c1,v1,nv1);
  v->addEl(kk,std::make_pair(0,0));
  Model m(v);
  
  initscr();
  noecho();
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  int i=0;
  Gamestate::gamestate = 1;
  while ((i=getch())!='e'){
    if(i==KEY_LEFT) {i=0;}
    std::pair<int,int> np = m.getview()->getpos(e);
    if((m.getview()->overlap(e,std::make_pair(np.first,np.second+1),kk)).first==1){
      m.getview()->merge(kk,e);
      e=g();
      m.getview()->addEl(e,std::make_pair(5,5));
    }
    cdcol(kk,std::make_pair(0,0));
    m.updatef(i);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  endwin();
}