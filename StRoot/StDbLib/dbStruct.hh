#ifndef DBSTRUCT_HH
#define DBSTRUCT_HH

#include <vector>

class basic {

 public:

 char* startKey;
 char* endKey;
 int istart;
 int iend;


};

class stsize : public basic {

public:
 int isize;
   stsize(): isize(0) { startKey="<length>";
   endKey="</length>" ;};


};

class datav : public basic {

public:
 char* data;
   datav() { startKey="<value>";
   endKey="</value>" ;};


};

class elem : public basic {

 public:

  datav val;
  char* name;
  char* type;
  stsize size;

   elem() { startKey="<db";
            endKey="</db" ;};

};


#ifdef ST_NO_TEMPLATE_DEF_ARGS
typedef vector<elem*, allocator<elem*> > elemVec;
#else
typedef vector<elem*> elemVec;
#endif

class accessor : public basic {

 public:

 elemVec e;
 int nelems;

   accessor() { startKey="<StDbAccessor>";
                 endKey="</StDbAccessor>";};

 
};


class dbTable : public accessor {
  
 public: 

  accessor a;
  //  elemVec e;
  //  int nelems;
  char* name;

   dbTable() { startKey="<StDbClass>";
               endKey="</StDbClass>";};

};


#endif
