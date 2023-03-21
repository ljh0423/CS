/*
This code constructs the RegExp class with subclasses Disjunction, Concatenation, Star and Word.
I have implemented the function matches, which takes in a string and returns a boolean value indicating whether the string is an exact match of the regex expression.
*/
#ifndef RE_H
#define RE_H
#include <string>
#include <memory>

namespace CS246E {
  class RegExp {
   public:
    RegExp() {}
    bool matches(std::string s) {
      return matchp(s,nullptr);
    }
    bool matchp(std::string &s, std::unique_ptr<RegExp>&& re) {
      return match(s,std::move(re));
    }
    std::ostream &print(std::ostream &out) {
      printc(out);
      return out;
    }
    virtual ~RegExp() {}
    class Disjunction;
   protected:
    virtual bool match(std::string &s, std::unique_ptr<RegExp>&& re) = 0;
    virtual void printc(std::ostream &out)=0;
  };
  class Disjunction final : public RegExp {
    std::unique_ptr<RegExp> left,right;
   public:
    Disjunction(std::unique_ptr<RegExp>&& left, std::unique_ptr<RegExp>&& right): left{std::move(left)}, right{std::move(right)} {}
    bool match(std::string &s, std::unique_ptr<RegExp>&& re) override {
      std::string s0 = s;
      left->matchp(s,std::move(re));
      if (s0.compare(s)!=0) {
        if (s.empty()) return 1;
        else return 0;
      }
      else {
        s = s0;
        right->matchp(s,std::move(re));
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
    void printc(std::ostream &out) override {
      if (left!=nullptr) {
      out << "Disjunction (";
      left->print(out);
      out << ",";
      right->print(out);
      out << ")";
      }
    }
    ~Disjunction() {}
  };
  class Concatenation final : public RegExp {
    std::unique_ptr<RegExp> left,right;
   public:
    Concatenation(std::unique_ptr<RegExp>&& left, std::unique_ptr<RegExp>&& right): left{std::move(left)}, right{std::move(right)} {}
    bool match(std::string &s, std::unique_ptr<RegExp>&& re) override {
      std::string s0 = s;
      bool b = left->matchp(s,std::move(right));
      if (b && re!=nullptr) return re->matchp(s,nullptr);
      else {
        if (b) return 1;
        else {
          s = s0;
          return 0;
        }
      }
    }
    ~Concatenation() {}
  };
  class Star final : public RegExp {
    std::unique_ptr<RegExp> exp;
   public:
    Star(std::unique_ptr<RegExp>&& exp): exp{std::move(exp)} {}
    bool match(std::string &s, std::unique_ptr<RegExp>&& re) override {
      std::string s0 = s;
      bool b = exp->matchp(s,nullptr);
      while (s0.compare(s) && !s.empty()) {
        s0 = s;
        b = exp->matchp(s,nullptr);
      }
      if (re!=nullptr) return re->matchp(s,nullptr);
      return 1;
    }
    ~Star() {}
  };
  class Word final : public RegExp {
    std::string w;
   public:
    Word(std::string w): w{w} {}
    bool match(std::string &s, std::unique_ptr<RegExp>&& re) override {
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
