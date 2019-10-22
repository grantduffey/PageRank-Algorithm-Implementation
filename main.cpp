// Grant Duffey
// CPSC 2120-3
// HW 1
// Dean
#include "websearch.h"

int main (void) {

  Web w;
  ifstream in;
  string s;
  string urlhash;

  // Read in all the URLS labeled NEWPAGE to fill the hashtable
  in.open("webpages.txt");
  while (in >> s) {
    if (s == "NEWPAGE") {
      in >> s;
      w.insertnewpage(s);
    }
  }

  in.clear();
  in.seekg(0);

  // Use the NEWPAGEs to hash the urls to the correct place, and to store
  // the newpage in the words on its page. 
  while (in >> s) {
    if (s == "NEWPAGE") {
      in >> urlhash;
    }
    else if (s.compare(0,7, "http://") == 0) {
      if (w.find(s)) {
        w.inserturl(s, urlhash);
      }
    }
    else {
      w.insertword(s, urlhash);
    }
  }

  in.close();

  w.rankpage();
  w.reverseindex();

  return 0;
}
