#include "sstore.h"
#include <iostream>
#include "../helper/helper.h"
#include <vector>
#include <set>
#include <map>

//
// loadss, a simple loader for a sstore_t 
// 

int main(int argc , char ** argv )
{
  
  std::map<std::string,int> lvls;
  for (int i=1;i<argc;i++)
    lvls[ argv[i] ] = -1 ;
    
  // get header line
  
  std::string line;
  std::getline( std::cin , line , '\n' );
  std::vector<std::string> hdr = Helper::parse( line , "\t" );
  const int n = hdr.size();
  int eidx = -1;
  int iidx = -1;
  int chidx = -1;
  int startidx = -1;
  int stopidx = -1;
  bool has_levels = false;

  std::map<std::string,int> vars;

  for (int i=0;i<n;i++)
    {
      if      ( hdr[i] == "E" ) eidx = i;
      else if ( hdr[i] == "START" ) startidx = i;
      else if ( hdr[i] == "STOP" ) stopidx = i;
      else if ( hdr[i] == "ID" ) iidx = i;
      else if ( hdr[i] == "CH" ) chidx = i;
      else if ( hdr[i] == "CHS" ) chidx = i;       // use this as an alternative for COH analyses
      else if ( lvls.find( hdr[i] ) != lvls.end() ) { lvls[ hdr[i] ] = i; has_levels = true; } 
      else vars[ hdr[i] ] = i;
    }

  bool epochs = eidx != -1;
  bool intervals = startidx != -1 & stopidx != -1;
  if ( iidx == -1 ) Helper::halt( "no ID column" );
  if ( epochs && intervals ) Helper::halt( "cannot have both intervals and epochs" );
  
  bool base = ! ( epochs || intervals );

  bool has_channels = chidx != -1;
  
  //
  // read and process each non-header row
  //

  while ( ! std::cin.eof() )
    {
      
      std::string line;
      std::getline( std::cin , line , '\n' );
      if ( std::cin.eof() ) break;
      if ( line == "" ) break;
      
      std::vector<std::string> tok = Helper::parse( line , "\t" );
      
      // level?
      std::string lvl = ".";
      if ( has_levels ) 
	{
	  lvl = "";
	  std::map<std::string,int>::const_iterator ii = lvls.begin();
	  while ( ii != lvls.end() ) 
	    {
	      if ( ii != lvls.begin() ) lvl += ";";
	      lvl += ii->first + "=" + tok[ ii->second ];
	      
	      ++ii;	      
	    }	  
	}
      
      // channels?
      std::string ch = has_channels ? tok[ chidx ] : ".";
      
      std::map<std::string,int>::const_iterator ii = vars.begin();
      while ( ii != vars.end() )
	{
	  
	  double d;
	  bool is_double = Helper::str2dbl( tok[ ii->second ] , &d );
	  
	  std::cout << ii->first << "\t"
		    << lvl << "\t"
		    << ch << "\t" ;
	  
	  if ( epochs ) 
	    {
	      std::cout << tok[ eidx] << "\t";
	    }
	  else if ( intervals )
	    {
	      std::cout << tok[ startidx ] << "\t" 
			<< tok[ stopidx ] << "\t";
	    }
	  
	  std::cout << ( is_double ? 1 : 0 ) << "\t"
		    << tok[ ii->second ] << "\n";
	  
	  ++ii;
	}

    }

  std::exit(0);
}
