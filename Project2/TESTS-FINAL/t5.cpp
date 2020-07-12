
#include "GridWorld.h"
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "_test.h"
#include "_util.h"

using std::cout;
using std::cin;
using std::string;



GridWorld * t5setup(int n) {

  GridWorld *gw = new GridWorld(20, 20);
  int i, id;

  for(i=0; i<n; i++) 
    gw->birth(0,0,id);

  for(i=0; i<n-1; i++)
    gw->move(i, 1,1);

  return gw;
}

bool t5(GridWorld *gw, int n) {
  vector<int> ref;
  vector<int> *m;

  ref.push_back(n-1);

  m = gw->members(0, 0);
  if(*m != ref) {
    // std::cout << "FAILURE type-2:  members wrong?\n";
    delete m;
    return false;
  }
  delete m;
  return true;

}



















/*
 *
 * Optional cmd-line arg is "n" which is used to determine
 *   number of operations (births, etc.) performed.
 *
 * This test case focuses on birth, move and whereis
 */
int main(int argc, char *argv[]){
  int n = ___N;
  int ntrials = __NTRIALS;

  if(argc > 1)
    n = atoi(argv[1]);
  if(argc > 2)
    ntrials = atoi(argv[2]);

  set_ntrials(ntrials);


  START("~ TIMED TEST: members after everybody killed but last one");
  {

    GridWorld *gwBig   =   t5setup(n);
    GridWorld *gwSmall = t5setup(1);

    PtsPer = 30.0;

    TIME_RATIO(t5(gwBig, n), t5(gwSmall, 1), 
        "singleton members timed test",
        1, 1.25, PtsPer);

    delete gwBig;
    delete gwSmall;
  }
  END
  return 0;
}
