/*
This program implements the AGE game engine, with a basic architecture of Model-View-Controller model.
*/
#ifndef AGE_H
#define AGE_H
#include <iostream>
#include <ncurses.h>
#include <utility>
#include <string>
#include <map>
#include <tuple>
#include <memory>
#include <algorithm>
#include <vector>

namespace Game {
  class MoveChar {
    public:
     MoveChar() {}
     virtual ~MoveChar() {}
     std::pair<int,int>& getdir() { return this->getdirp(); }
     void conPC(std::pair<int,int> p) {this->conPCp(p);}
     void hanb() {this->hanbp();}
    private:
     virtual std::pair<int,int>& getdirp()=0;
     virtual void conPCp(std::pair<int,int> p) {}
     virtual void hanbp() {}
  };
  
  class Element {
    int h;
    std::map<int,std::vector<int>> colt;
    std::vector<std::shared_ptr<MoveChar>> move;
    std::pair<int,int> totm;
    size_t curk;
    public:
     Element(int h,std::map<int,std::vector<int>> colt,std::vector<std::shared_ptr<MoveChar>> move): h{h}, colt{colt}, move{move}, totm{std::make_pair(0,0)}, curk{0} {}
     virtual ~Element() {}
     int geth() { return h; }
     size_t& getcurk() {return curk;}
     std::vector<std::shared_ptr<MoveChar>>& getmove() {return move;}
     std::vector<std::tuple<int,int,char>>& getv(){return this->getvp();}
     std::map<int,std::vector<int>>& getcolt(){ return colt; }
     std::pair<int,int>& gettotm() {return totm;}
     void rotate() {this->rotatep();}
     void flip() {this->flipp();}
     void delc(int c,std::pair<int,int> p) {this->delcp(c,p);}
    private:
     virtual std::vector<std::tuple<int,int,char>>& getvp()=0;
     virtual void rotatep() {}
     virtual void flipp() {}
     virtual void delcp(int c,std::pair<int,int> p) {}
  };
  
  class SLP : public MoveChar {
    std::pair<int,int> dir;
    public:
     SLP(): dir{std::make_pair(0,0)} {}
     SLP(std::pair<int,int> dir): dir{dir} {}
     SLP(const SLP &other): dir{other.dir} {}
     std::pair<int,int>& getdirp() override { return dir; }
     void hanbp() override {
       dir = std::make_pair(-(dir.first),-(dir.second));
     }
  };
  class CP : public MoveChar {
    std::pair<int,int> dir;
    std::vector<std::vector<std::tuple<int,int,char>>> forms;
    public:
     CP(): dir{std::make_pair(0,0)}, forms{std::vector<std::vector<std::tuple<int,int,char>>>()} {}
     CP(std::vector<std::vector<std::tuple<int,int,char>>> forms): dir{std::make_pair(0,0)}, forms{forms} {}
     std::pair<int,int>& getdirp() override { return dir; }
     std::vector<std::vector<std::tuple<int,int,char>>>& getf() {return forms;}
     
  };
  class GR : public MoveChar {
    std::pair<int,int> dir;
    public:
     GR(): dir{std::make_pair(0,0)} {}
     GR(std::pair<int,int> dir): dir{dir} {}
     GR(const GR &other): dir{other.dir} {}
     std::pair<int,int>& getdirp() override { return dir; }
  };
  class PC : public MoveChar {
    std::pair<int,int> dir;
    public:
     PC(): dir{std::make_pair(0,0)} {}
     PC(std::pair<int,int> dir): dir{dir} {}
     PC(const PC &other): dir{other.dir} {}
     std::pair<int,int>& getdirp() override { return dir; }
     void conPCp(std::pair<int,int> p) override {dir=p;}
  };
  
  class SingleC : public Element {
    std::vector<std::tuple<int,int,char>> v;
    public:
     SingleC(int h,std::map<int,std::vector<int>> colt,std::vector<std::shared_ptr<MoveChar>> move,char c) : Element(h,colt,move), v{std::vector<std::tuple<int,int,char>>()} {
       v.push_back(std::make_tuple(0,0,c));
     }
     std::vector<std::tuple<int,int,char>>& getvp() override {return v;}
     void delcp(int c,std::pair<int,int> p) override { 
       if(std::get<1>(v[0])+p.second==c) {v.clear();}
     }
  };
  class Rect : public Element {
    std::vector<std::tuple<int,int,char>> v;
    int l,w;
    char c;
    public:
     Rect(int h,std::map<int,std::vector<int>> colt,std::vector<std::shared_ptr<MoveChar>> move,int l,int w,char c) : Element(h,colt,move), v{std::vector<std::tuple<int,int,char>>()}, l{l}, w{w}, c{c} {
       for (int a=0;a<l;++a) {
         for (int b=0;b<w;++b) {
           v.push_back(std::make_tuple(a,b,c));
         }
       }
     }
     std::vector<std::tuple<int,int,char>>& getvp() override {return v;}
     void rotatep() override {
       int t = l;
       l = w;
       w = t;
       t = std::max(l,w)+1;
       for (auto &k:v){
         int temp = std::get<0>(k);
         std::get<0>(k) = std::get<1>(k);
         std::get<1>(k) = t-1-temp;
       }
     }
     void delcp(int c,std::pair<int,int> p) override {
       for(size_t t=0;t<v.size();++t){
         if(c==(std::get<1>(v[t])+p.second)) {
           v.erase(v.begin()+t);
           --t;
         }
       }
       for(size_t t=0;t<v.size();++t){
         if(c>(std::get<1>(v[t])+p.second)) {
           std::get<1>(v[t]) += 1;
         }
       }
       --l;
     }
  };
  class Bitmap : public Element {
    std::vector<std::tuple<int,int,char>> v;
    public:
     Bitmap(int h,std::map<int,std::vector<int>> colt,std::vector<std::shared_ptr<MoveChar>> move,std::vector<std::tuple<int,int,char>> v) : Element(h,colt,move), v{v} {}
     std::vector<std::tuple<int,int,char>>& getvp() override {return v;}
     void rotatep() override {
       int t=0;
       for(auto k1:v){
         t = std::max(std::max(std::get<0>(k1),std::get<1>(k1)),t);
       }
       ++t;
       for (auto &k:v){
         int temp = std::get<0>(k);
         std::get<0>(k) = std::get<1>(k);
         std::get<1>(k) = t-1-temp;
       }
     }
     void delcp(int c,std::pair<int,int> p) override {
       for(size_t t=0;t<v.size();++t){
         if(c==(std::get<1>(v[t])+p.second)) {
           v.erase(v.begin()+t);
           --t;
         }
       }
       for(size_t t=0;t<v.size();++t){
         if(c>(std::get<1>(v[t])+p.second)) {
           std::get<1>(v[t]) += 1;
         }
       }
     }
     void flipp() override {
       for(auto &k:v){
         int temp = std::get<0>(k);
         std::get<0>(k) = std::get<1>(k);
         std::get<1>(k) = temp;
       }
       this->rotatep();
     }
  };
  class Border : public Element {
    std::vector<std::tuple<int,int,char>> v;
    public:
     Border(int h,std::map<int,std::vector<int>> colt,std::vector<std::shared_ptr<MoveChar>> move,int l,int w,char corner,char row,char col) : Element(h,colt,move), v{std::vector<std::tuple<int,int,char>>()} {
      int i=0,i1=0;
      v.push_back(std::make_tuple(0,0,corner));
      for (i=1;i<w-1;++i) { v.push_back(std::make_tuple(0,i,row)); }
      v.push_back(std::make_tuple(0,i,corner));
      for (i=1;i<l-1;++i) {
        v.push_back(std::make_tuple(i,0,col));
        v.push_back(std::make_tuple(i,w-1,col));
      }
      v.push_back(std::make_tuple(i,0,corner));
      i1=i;
      for (i=1;i<w-1;++i) { v.push_back(std::make_tuple(i1,i,row)); }
      v.push_back(std::make_tuple(i1,i,corner));
     }
     std::vector<std::tuple<int,int,char>>& getvp() override {return v;}
  };

  class Gamestate {
    public:
     inline static int gamestate;
     inline static int point;
     Gamestate() {gamestate=1;}
     ~Gamestate() {}
  };
  class View {
    std::map<int,std::vector<std::pair<std::pair<int,int>,std::shared_ptr<Element>>>> gameobj;
    public:
     View(): gameobj{std::map<int,std::vector<std::pair<std::pair<int,int>,std::shared_ptr<Element>>>>()} {}
     View(std::map<int,std::vector<std::pair<std::pair<int,int>,std::shared_ptr<Element>>>>& gameobj): gameobj{gameobj} {}
     ~View() {}
     void addEl(std::shared_ptr<Element> e, std::pair<int,int> p) {
       gameobj[e->geth()].push_back(std::make_pair(p,e));
     }
     void remEl(std::shared_ptr<Element> e) {
       std::vector<std::pair<std::pair<int,int>,std::shared_ptr<Element>>> &p = gameobj[e->geth()];
       for(size_t k=0;k<p.size();++k){
         if(e==p[k].second){
           p.erase(p.begin()+k);
           break;
         }
       }
     }
     bool checkin(std::shared_ptr<Element> e){
       for(size_t i=0;i<gameobj[e->geth()].size();++i){
         if (gameobj[e->geth()][i].second==e){
           return 1;
         }
       }
       return 0;
     }
     std::pair<int,int> getpos(std::shared_ptr<Element> e){
       for(size_t i=0;i<gameobj[e->geth()].size();++i){
         if (gameobj[e->geth()][i].second==e){
           return gameobj[e->geth()][i].first;
         }
       }
       return std::make_pair(0,0);
     }
     void merge(std::shared_ptr<Element> e1,std::shared_ptr<Element> e2) {
       std::pair<int,int> p1 = getpos(e2);
       for (size_t i=0;i<e2->getv().size();++i) {
         std::tuple<int,int,char> t = e2->getv()[i];
         std::get<0>(t) += p1.first;
         std::get<1>(t) += p1.second;
         e1->getv().push_back(t);
       }
       remEl(e2);
     }
     void setPC(std::pair<int,int> input){
       for(auto &i:gameobj){
         for(auto &t:i.second){
           for(auto &k:t.second->getmove()){
             k->conPC(input);
           }
         }
       }
     }
     void rotPC(){
       for(auto &i:gameobj){
         for(auto &t:i.second){
           for(auto &k:t.second->getmove()){
             if(PC *pc = dynamic_cast<PC*>(k.get())) {
               t.second->rotate();
               if((overlap(t.second,t.first,t.second)).first==1){
                 for(int m=0;m<3;++m) {t.second->rotate();}
               }
               return;
             }
           }
         }
       }
     }
     void fliPC(){
       for(auto &i:gameobj){
         for(auto &t:i.second){
           for(auto &k:t.second->getmove()){
             if(PC *pc = dynamic_cast<PC*>(k.get())) {
               t.second->flip();
               if((overlap(t.second,t.first,t.second)).first==1){
                 t.second->flip();
               }
               return;
             }
           }
         }
       }
     }
     void nextCP() {
       for (auto &i:gameobj){
        for(size_t k=0;k<i.second.size();++k){
          std::pair<std::pair<int,int>,std::shared_ptr<Element>>& pt = i.second[k];
          for(auto &t:pt.second->getmove()){
            if(CP *cp = dynamic_cast<CP*>(t.get())) {
              if(cp->getf().size()!=0){
                std::vector<std::tuple<int,int,char>> temp = pt.second->getv();
                cp->getf().push_back(temp);
                pt.second->getv() = cp->getf()[0];
                cp->getf().erase(cp->getf().begin());
                break;
              }
            }
          }
        }
      }
     }
     std::pair<int,int> overlap(std::shared_ptr<Element> e1,std::pair<int,int> p1, std::shared_ptr<Element> e2){
       bool b=0;
         for(auto &t1:gameobj[e1->geth()]){
           b=0;
           if(t1.second==e1) continue;
           for(auto i:(e1->getcolt())[4]){
             std::vector<int> &f = t1.second->getcolt()[4];
             if(std::find(f.begin(),f.end(),i)!=f.end()){
               b=1;
               break;
             }
           }
           if(b) continue;
           for(auto &tv:t1.second->getv()){
             int y2 = t1.first.first + std::get<0>(tv);
             int x2 = t1.first.second + std::get<1>(tv);
             for(auto &ev:e1->getv()){
              int y1 = p1.first + std::get<0>(ev);
              int x1 = p1.second + std::get<1>(ev);
              if(y1==y2 && x1==x2) {
                if(t1.second==e2) {return std::make_pair(1,1);}
                else {return std::make_pair(1,0);}
              }
             }
           }
         }
       return std::make_pair(0,0);
     }
     void collision(std::shared_ptr<Element> e1,std::shared_ptr<Element> e2,std::shared_ptr<MoveChar> mc){
       if(common(e1->getcolt()[1],e2->getcolt()[1],0)!=0){
         Gamestate::gamestate = common(e1->getcolt()[1],e2->getcolt()[1],0);
       }
       if(common(e1->getcolt()[2],e2->getcolt()[2],0)){
         remEl(e1);
         remEl(e2);
       }
       if(common(e1->getcolt()[2],e2->getcolt()[2],1)){
         if(common(e1->getcolt()[2],e2->getcolt()[2],1)>0){
           remEl(e2);
           Gamestate::point += 10;
         }
         else {
           remEl(e1);
         }
       }
       if(common(e1->getcolt()[3],e2->getcolt()[3],0)){
         //solid
       }
       else if(common(e1->getcolt()[4],e2->getcolt()[4],0)){
         //view
       }
       else if(common(e1->getcolt()[5],e2->getcolt()[5],0)){
         mc->hanb();
       }
     }
     void checkcol(std::vector<std::pair<std::pair<int,int>,std::shared_ptr<Element>>>& ve) {
       for (size_t i=0;i<ve.size();++i){
         if(ve[i].second->getv().size()==0) {continue;}
         if(ve[i].second->getmove().size()==0) {continue;}
         for (size_t a=i+1;a<ve.size();++a){
           if(ve[a].second->getv().size()==0) {continue;}
           bool b=0,bs=1,ft=1;
           size_t k=0,k2=0;
           std::pair<int,int> temp(0,0),temp2(0,0);
           while(bs){
             b=0;
             for(;k<ve[i].second->getmove().size();++k){
             std::shared_ptr<MoveChar> a1 = (ve[i].second->getmove())[k];
             temp.first += a1->getdir().first;
             temp.second += a1->getdir().second;
             ve[a].first.first += temp2.first;
             ve[a].first.second += temp2.second;
             std::pair<int,int> overl = overlap(ve[i].second,std::make_pair(ve[i].first.first+temp.first,ve[i].first.second+temp.second),ve[a].second);
             ve[a].first.first -= temp2.first;
             ve[a].first.second -= temp2.second;
             if(overl.first){
               temp.first -= a1->getdir().first;
               temp.second -= a1->getdir().second;
               if(overl.second) {
                 if(ft) {ft=0;b=1;}
                 else {
                  if(k>ve[i].second->getcurk()) {ve[i].second->getcurk()=k;ve[i].second->gettotm()=temp;}
                  if(k2>ve[a].second->getcurk()) {ve[a].second->getcurk()=k2;ve[a].second->gettotm()=temp2;}
                  collision(ve[i].second,ve[a].second,a1);
                 }
               }
               break;
             }
           }
           for(;k2<ve[a].second->getmove().size();++k2){
             std::shared_ptr<MoveChar> a2 = (ve[a].second->getmove())[k2];
             temp2.first += a2->getdir().first;
             temp2.second += a2->getdir().second;
             ve[i].first.first += temp.first;
             ve[i].first.second += temp.second;
             std::pair<int,int> overl = overlap(ve[a].second,std::make_pair(ve[a].first.first+temp2.first,ve[a].first.second+temp2.second),ve[i].second);
             ve[i].first.first -= temp.first;
             ve[i].first.second -= temp.second;
             if(overl.first){
               temp2.first -= a2->getdir().first;
               temp2.second -= a2->getdir().second;
               if(overl.second) {
                 if(k>ve[i].second->getcurk()) {ve[i].second->getcurk()=k;ve[i].second->gettotm()=temp;}
                 if(k2>ve[a].second->getcurk()) {ve[a].second->getcurk()=k2;ve[a].second->gettotm()=temp2;}
                 collision(ve[a].second,ve[i].second,a2);
                 bs=0;}
               break;
             }
           }
             if(b==0) {
               if(k>ve[i].second->getcurk()) {ve[i].second->getcurk()=k;ve[i].second->gettotm()=temp;}
               if(k2>ve[a].second->getcurk()) {ve[a].second->getcurk()=k2;ve[a].second->gettotm()=temp2;}
               bs=0;
             }
           }
          }
       }
     }
     
     void display(){
       for (auto &i:gameobj){
         for(size_t k=0;k<i.second.size();++k){
           if(i.second[k].second->getv().size()==0) {continue;}
           i.second[k].first.first += i.second[k].second->gettotm().first;
           i.second[k].first.second += i.second[k].second->gettotm().second;
           std::pair<int,int> &pos = i.second[k].first;
           std::shared_ptr<Element> el = i.second[k].second;
           for (size_t n=0;n<el->getv().size();++n){
             int p1 = std::get<0>(el->getv()[n]);
             int p2 = std::get<1>(el->getv()[n]);
             mvaddch(pos.first+p1,pos.second+p2,std::get<2>((el->getv())[n]));
           }
         }
       }
     }
     void resettotm() {
       for(auto &i:gameobj){
         for(size_t k=0;k<i.second.size();++k){
           std::pair<int,int> temp (0,0);
           i.second[k].second->gettotm() = temp;
           i.second[k].second->getcurk() = 0;
         }
       }
     }
     void update() {
       display();
       refresh();
       resettotm();
       nextCP();
       for (auto &i: gameobj){
         checkcol(i.second);
       }
     }
    private:
     int common(std::vector<int> v1,std::vector<int> v2, bool des){
       for(size_t t=0;t<v1.size();++t){
         for(size_t t1=0;t1<v2.size();++t1){
           if(des && v1[t]==-(v2[t1])) {return v1[t];}
           else if(v1[t]==v2[t1]) {return v1[t];}
         }
       }
       return 0;
     }
  };
  class Controller {
    public:
     Controller() {}
     ~Controller() {}
     void input(int ch, std::shared_ptr<View>& v,std::vector<std::pair<int,int>> mv) {
       std::pair<int,int> p(0,0);
       if(ch==0) {return;}
       else if(ch=='q') {Gamestate::gamestate = 2;return;}
       else if(ch=='r') {v->rotPC();}
       else if(ch=='f') {v->fliPC();}
       else if(ch=='s') {Gamestate::gamestate = 0;return;}
       else if(ch==KEY_UP) {p=mv[0];}
       else if(ch==KEY_DOWN) {p=mv[1];}
       else if(ch==KEY_LEFT) {p=mv[2];}
       else if(ch==KEY_RIGHT) {p=mv[3];}
       v->setPC(p);
     }
  };
  class Model {
    Controller con;
    std::shared_ptr<View> view;
    public:
     Model(): con{Controller()}, view{std::make_shared<View>()} {Gamestate::gamestate=1;}
     Model(std::shared_ptr<View>& view): con{Controller()}, view{view} {Gamestate::gamestate=1;}
     ~Model() {}
     std::shared_ptr<View>& getview() { return view; }
     void updatef(int ch,std::vector<std::pair<int,int>> mv) {
       if(Gamestate::gamestate==0 || Gamestate::gamestate==3){
         erase();
         mvprintw(22,0,"Points: %d",Gamestate::point);
         if(Gamestate::gamestate==3) {mvprintw(22,30,"Blue Mode");}
         mvaddstr(23,0,"Press 'q' to quit!");
         mvaddstr(24,0,"Press 'e' to exit!");
         con.input(ch,view,mv);
         view->update();
         view->setPC(std::make_pair(0,0));
       }
       else if(Gamestate::gamestate==1){
         con.input(ch,view,mv);
         mvaddstr(22,0,"Press 's' to start!");
         mvaddstr(23,0,"Press 'q' to quit!");
         mvaddstr(24,0,"Press 'e' to exit!");
         refresh();
       }
       else if(Gamestate::gamestate==2){
         mvprintw(22,0,"Game Over!   Points Earned: %d",Gamestate::point);
         mvaddstr(24,0,"Press 'e' to exit!");
         refresh();
       }
     }
  };
}
#endif
