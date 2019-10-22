// Grant Duffey
// CPSC 2120-3
// HW 1
// Dean
#ifndef WEBSEARCH_H
#define WEBSEARCH_H

#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
using namespace std;

class Web {

private:
  struct Data {
    string data;
    int cache;
    Data * next;
    Data(string k, int c, Data *n) { data = k; cache = c; next = n;}
  };

  struct Word {
    string data;
    string urlparent;
    Word * next;
    Word(string k, string u, Word * n) {data = k; urlparent = u; next = n; }
  };

  struct Node {
    string key;
    Data * urls;
    int t;
    double weight;
    double newweight;
    Node * next;
    Node(string k, Node *n) { key = k; weight = 0.0; newweight = 0; t = 0; next = n;
      urls = NULL;}
  };

  Node ** table;
  Word ** words;
  int size;
  int wordsize;
  int totwords;
  int numpages;

  public:
    Web();
    ~Web();
    void insertnewpage(string);
    void inserturl(string, string);
    void insertword(string, string);
    void rankpage();
    void reverseindex();
    bool find(string);
};

#endif
