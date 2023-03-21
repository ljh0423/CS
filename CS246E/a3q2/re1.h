/*
This code constructs the RegExp class with subclasses Disjunction, Concatenation, Star and Word.
I have implemented the function matches, which takes in a string and returns a boolean value indicating whether the string is an exact match of the regex expression.
*/
#ifndef RE_H
#define RE_H
#include <string>
#include <iostream>

namespace CS246E {
  class RegExp {
   public:
    bool matches(std::string s) {
      size_t i=0;
      bool b = matchp(s,nullptr,i);
      return (b && i==s.length());
    }
    bool matchp(std::string &s, RegExp *re, size_t &i) {
      return match(s,re,i);
    }
    virtual void delexp(RegExp *e){};
    virtual ~RegExp() {}
   protected:
    virtual bool match(std::string &s, RegExp *re, size_t &i) = 0;
  };
  class Disjunction final : public RegExp {
    RegExp *left,*right;
   public:
    Disjunction(RegExp *left, RegExp *right): left{left}, right{right} {}
    bool match(std::string &s, RegExp *re, size_t &i) override {
      size_t i0 = i,i1;
      bool b1 = left->matchp(s,re,i);
      i1 = i;
      i = i0;
      bool b2 = right->matchp(s,re,i);
      if (!b1 && !b2) i = i0;
      else if (b1) {
        if (b2) i = std::max(i1,i);
        else i = i1;
      }
      return (b1 || b2);
    }
    ~Disjunction() {
      delete left;
      delete right;
    }
  };
  class Concatenation final : public RegExp {
    RegExp *left,*right;
   public:
    Concatenation(RegExp *left, RegExp *right): left{left}, right{right} {}
    bool match(std::string &s, RegExp *re, size_t &i) override {
      size_t i0=i;
      bool b;
      if (re!=nullptr) {
        re = new CS246E::Concatenation{right,re};
        b = left->matchp(s,re,i);
        operator delete (re);
      }
      else {
        b = left->matchp(s,right,i);
      }
      if (!b) {i=i0;return 0;}
      return 1;
    }
    void delexp(RegExp *e) override {
      if (right!=e) {
        right->delexp(e);
      }
      operator delete (this);
    }
    ~Concatenation() {
      delete left;
      delete right;
    }
  };
  class Star final : public RegExp {
    RegExp *exp;
   public:
    Star(RegExp *exp): exp{exp} {}
    bool match(std::string &s, RegExp *re, size_t &i) override {
      size_t i0=i,i1=i;
      if (re!=nullptr) {
        if (re->matchp(s,nullptr,i)) return 1;
        else {
          auto r = exp;
          while (r->matchp(s,nullptr,i) && i!=i0) {
            i0 = i;
            i = i1;
            if (r->matchp(s,re,i)) {if (r!=exp) r->delexp(exp);return 1;}
            r = new Concatenation{exp,r};
          }
          if (r!=exp) r->delexp(exp);
          bool b=re->matchp(s,nullptr,i);
          if (!b) i=i1;
          return b;
        }
      }
      else {
        while ((exp->matchp(s,nullptr,i)) && i0!=i && i!=s.length()) {i0 = i;}
      }
      return 1;
    }
    ~Star() {
      delete exp;
    }
  };
  class Word final : public RegExp {
    std::string w;
   public:
    Word(std::string w): w{w} {}
    bool match(std::string &s, RegExp *re, size_t &i) override {
      size_t i0=i;
      if (s.find(w,i) == i) {
        i=i+w.length();
        bool b=1;
        if (re!=nullptr) {b = re->matchp(s,nullptr,i);}
        if (!b) {i=i0;return 0;}
        return 1;
      }
      else return 0;
    }
    ~Word() {}
  };

}
#endif
