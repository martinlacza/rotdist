#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <cassert>
#include <deque>
#include <algorithm>
#include <thread>
//#include <unordered_set>
#include "config.hpp"
#include "algo.hpp"
using namespace std;
using namespace tree;





void test_algorithms ( size_t runs ) {
  size_t i;
  Random gen( 0 );// (unsigned long) &i );


  for (i = 0; i < runs; ++i ) {
    ztree<N> a( gen );
    ztree<N> b( gen );
    ptree<int> aa( a );//, aaa( a );
    ptree<int> bb( b );//, bbb( b );

    size_t visited;
    int toptimal, tdistance;//, toldistance;
    toptimal = distance(a, b, visited );
    tdistance = newalgo( aa, bb );
    //toldistance = aaa.oldistance( bbb );

    assert( aa == bb );
    //assert( aaa == bbb );
    assert( toptimal <= tdistance );

    if ( toptimal < tdistance )
    cout << a << "\t" << b << "\t" << toptimal << "\t" << tdistance << "\t" /*<< toldistance << "\t"*/ << visited << endl;
  }
}


int main ( int argv, char** argc ) {
  /*ztree<7> a( "110101100110000" );
  ztree<7> b( "111101001001000" );
  ptree<int> aa( a );//, aaa( a );
  ptree<int> bb( b );//, bbb( b );
  print<int>( aa, bb, cout );

  cout << newalgo( aa, bb ) << endl;
  return 0;*/

  if ( argv != 3 || atoi( argc[1] ) <= 0 || atoi( argc[2] ) <= 0 ) return 1;
  size_t threads = atoi( argc[1] );
  size_t runs = atoi( argc[2] );

  cout << "first\tsecond\topt\tdist\tvisited\n";
  vector<thread> t;

  //Launch a group of threads
  for (int i = threads; i > 0; --i) {
    size_t part = runs / i;
    t.push_back( thread( test_algorithms, part ) );
    runs -= part;
  }

  //Join the threads with the main thread
  for( auto& i : t ) i.join();

    return 0;
}


