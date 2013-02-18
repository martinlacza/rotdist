#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <cassert>
#include <deque>
#include <fstream>
#include <algorithm>
#include <thread>
#include <mutex>
#include "config.hpp"
#include "algo.hpp"
using namespace std;
using namespace tree;

mutex output_mutex;



void test_algorithms ( size_t runs, int index ) {
  size_t i;
  Random gen( index );// (unsigned long) &i );


  for (i = 0; i < runs; ++i ) {
    ztree<N> a( gen );
    ztree<N> b( gen );
    ptree<int> aa( a ), aaa( a ), aaaa( a );
    ptree<int> bb( b ), bbb( b ), bbbb( b );

    if ( has_equivalent(aa,bb) ) { --i; continue; } // only very bad trees

    size_t visited;
    int toptimal, tdistance, tcentral, tnewbetter;
    toptimal = distance( a, b, visited );
    tdistance = newalgo( aa, bb );
    tcentral = central( aaa, bbb );
    tnewbetter = mix( aaaa, bbbb );

    assert( aa == bb );
    assert( aaa == bbb );
    assert( aaaa == bbbb );

    output_mutex.lock();
    cout << a << "\t" << b << "\t" << toptimal << "\t" << tdistance << "\t" << tnewbetter << "\t" << tcentral << "\t" << visited << endl;
    output_mutex.unlock();
  }
}


int main ( int argv, char** argc ) {
  /*// n = 13 very bad mix
  string a = "111010011000111001100110000";
  string b = "110101100110110010010011000";
  ztree<13> aa( a );
  ztree<13> bb( b );

  ptree<int> aaa( aa );
  ptree<int> bbb( bb );
  size_t visited = 0;
  cout << distance( aa, bb, visited ) << endl;
  cout << newalgo( aaa, bbb ) << endl;
  return 0;*/


  if ( argv != 3 || atoi( argc[1] ) <= 0 || atoi( argc[2] ) <= 0 ) return 1;
  size_t threads = atoi( argc[1] );
  size_t runs = atoi( argc[2] );

  cout << "first\tsecond\topt\tdist\tcentral\tvisited\n";
  vector<thread> t;

  //Launch a group of threads
  for (int i = threads; i > 0; --i) {
    size_t part = runs / i;
    int j = i;
    t.push_back( thread( test_algorithms, part, j ) );
    runs -= part;
  }

  //Join the threads with the main thread
  for( auto& i : t ) i.join();

    return 0;
}


