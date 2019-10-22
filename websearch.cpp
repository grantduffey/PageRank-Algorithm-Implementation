#include "websearch.h"

Web::Web() {

  size = 64;
  wordsize = 64;
  table = new Node *[size];
  words = new Word *[wordsize];
  numpages = 0;
  totwords = 0;
  for (int i=0; i<size; i++) table[i] = NULL;
  for (int i=0; i<wordsize; i++) words[i] = NULL;
}

Web::~Web() {

  for (int i=0; i<size; i++) {
    while (table[i] != NULL) {
      while(table[i] != NULL) {
        Data * current = table[i]->urls;
        if (current->next == NULL) {
          delete current;
          break;
        }
        table[i]->urls = table[i]->urls->next;
        delete current;
      }
      Node *temp = table[i];
      table[i] = table[i]->next;
      delete temp;
    }
  }
  delete[] table;

  for (int i=0; i<wordsize; i++) {
    while (words[i] != NULL) {
      Word *temp = words[i];
      words[i] = words[i]->next;
      delete temp;
    }
  }
  delete[] words;
}

int myhash(string s, int table_size)
{
  unsigned int i, h = 0;
  for (i=0; i<s.length(); i++)
    h = (h * 2917 + (unsigned int)s[i]) % table_size;
  return h;
}

bool Web::find(string key)
{
  int h = myhash(key, size);
  Node *n = table[h];
  while (n != NULL) {
    if (n->key == key) return true;
    n = n->next;
  }
  return false;
}

void Web::insertnewpage(string s) {

  assert (!find(s));
  numpages++;
  int h = 0;

  // If the table gets too full, copy it to another table with double the size.
  if (numpages == size) {

    size = size*2;
    Node ** newtable = new Node *[size]; // allocate table, initialize head ptrs all to NULL
    for (int i=0; i<size; i++) {
      newtable[i] = NULL;
    }

  Node * temp = NULL;

    for (int i=0; i<numpages; i++) {
      for (Node * current = table[i]; current != NULL; current = temp) {
        int temph = myhash(current->key, size);
        newtable[temph] = new Node(current->key, newtable[temph]);
        newtable[temph]->urls = current->urls;
        newtable[temph]->t = current->t;
        temp = current->next;
        delete current;
      }
    }
    delete [] table;
    table = newtable;
  }

  h = myhash(s, size);
  table[h] = new Node(s, table[h]);
}

void Web::inserturl(string s, string urlhash) {

  // Use the NEWPAGE url to hash to the proper location in the table
  int h = myhash(urlhash, size);
  // This saves time calculating the page rank later on, as apposed to calling
  // myhash for everyloop, the program only calls it once here
  int y = myhash(s, size);
  Node * current = table[h];
  while (current != NULL) {
    if (current->key == urlhash) {
      current->urls = new Data(s, y, current->urls);
      ++current->t;
      return;
    }
    else {
      current = current->next;
    }
  }
}



void Web::insertword(string s, string urlhash) {

  int h = 0;
  ++totwords;

  // If the table gets too full, copy it to another table with double the size.
  if (totwords == wordsize) {

    wordsize = wordsize * 2;
    Word ** newwords = new Word *[wordsize]; // allocate table, initialize head ptrs all to NULL
    for (int i=0; i<wordsize; i++) {
      newwords[i] = NULL;
    }

    Word * tempword = NULL;
    for (int i=0; i<totwords; i++) {
      for (Word * current = words[i]; current != NULL; current = tempword) {
        int temph = myhash(current->data, wordsize);
        newwords[temph] = new Word(current->data, current->urlparent, newwords[temph]);
        tempword = current->next;
        delete current;
      }
    }
    delete [] words;
    words = newwords;
  }

  h = myhash(s, wordsize);
  words[h] = new Word(s, urlhash, words[h]);
}


void Web::rankpage() {

  double count = 0.0;
  Node * temp = NULL;
  int h = 0;

  // Assigns initial weight
  for (int i=0; i<size; ++i) {
    for (Node * current = table[i]; current != NULL; current = current->next) {
      current->weight = 1.0/numpages;
    }
  }

  for (int i = 0; i < 50; ++i) {

    // Assigns initial new weight
    for (int j = 0; j < size; ++j) {
      for (Node * current = table[j]; current != NULL; current = current->next) {
        current->newweight = 0.1/numpages;
      }
    }

    // Assigns new weight
    for (int j = 0; j < size; ++j) {
      for (Node * current = table[j]; current != NULL; current = current->next) {
        for (Data * templink = current->urls; templink != NULL; templink = templink->next) {
          for (Node * temp = table[templink->cache]; temp != NULL; temp = temp->next) {
            if (temp->key == templink->data) {
              temp->newweight += (0.9 * current->weight)/current->t;
              break;
            }
          }
        }
      }
    }

    // Updates weight
    for (int j = 0; j < size; ++j) {
      for (Node * current = table[j]; current != NULL; current = current->next) {
        current->weight = current->newweight;
      }
    }
  }
}

void Web::reverseindex() {

  string input = " ";
  int h = 0;
  while (input != "q") {

    cout << endl << "Please enter the word you want to search for (press q to quit)" << endl;
    cin >> input;
    cout << endl;
    if (input != "q") {
      for (int i = 0; i < wordsize; i++) {
        for (Word * current = words[i]; current != NULL; current = current->next) {
          if (input == current->data) {
            h = myhash(current->urlparent, size);
            for (Node * temp = table[h]; temp != NULL; temp = temp->next) {
              if (temp->key == current->urlparent) {
                cout << (int)(100 * (temp->weight) * numpages) << " " << current->urlparent << endl;
              }
            }
          }
        }
      }
    }
  }
}
