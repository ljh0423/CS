/*
This code constructs the RegExp class with subclasses Disjunction, Concatenation, Star and Word.
I have implemented the function matches, which takes in a string and returns a boolean value indicating whether the string is an exact match of the regex expression.
*/
#ifndef RE_H
#define RE_H
#include <string>

namespace CS246E {
  class RegExp {
   public:
    bool matches(std::string s) {
      return matchp(s,nullptr);
    }
    bool matchp(std::string &s, RegExp *re) {
      return match(s,re);
    }
    virtual ~RegExp() {}
   protected:
    virtual bool match(std::string &s, RegExp *re) = 0;
  };
  class Disjunction final : public RegExp {
    RegExp *left,*right;
   public:
    Disjunction(RegExp *left, RegExp *right): left{left}, right{right} {}
    bool match(std::string &s, RegExp *re) override {
      std::string s0 = s;
      left->matchp(s,re);
      if (s0.compare(s)!=0) {
        if (s.empty()) return 1;
        else return 0;
      }
      else {
        s = s0;
        right->matchp(s,re);
        if (s0.compare(s)!=0) {
          if (s.empty()) return 1;
          else return 0;
        }
        else {
          if (s.empty()) return 1;
          else return 0;
        }
      }
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
    bool match(std::string &s, RegExp *re) override {
      std::string s0 = s;
      bool b = left->matchp(s,right);
      if (b && re!=nullptr) return re->matchp(s,nullptr);
      else {
        if (b) return 1;
        else {
          s = s0;
          return 0;
        }
      }
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
    bool match(std::string &s, RegExp *re) override {
      //change format with for loop with number of exp runs, then or it
      std::string s0 = s;
      bool b = exp->matchp(s,nullptr);
      while (s0.compare(s) && !s.empty()) {
        s0 = s;
        b = exp->matchp(s,nullptr);
      }
      if (re!=nullptr) return re->matchp(s,nullptr);
      if (s.empty()) return 1;
      else return 0;
    }
    ~Star() {
      delete exp;
    }
  };
  class Word final : public RegExp {
    std::string w;
   public:
    Word(std::string w): w{w} {}
    bool match(std::string &s, RegExp *re) override {
      if (s.find(w) == 0) {
        s = s.substr(w.length());
        if (re!=nullptr) {
          return re->matchp(s,nullptr);
        }
        else {
          if (s.empty()) return 1;
          else return 0;
        }
      }
      else return 0;
    }
    ~Word() {}
  };

}
#endif
