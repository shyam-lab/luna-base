
//    --------------------------------------------------------------------
//
//    This file is part of Luna.
//
//    LUNA is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Luna is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Luna. If not, see <http://www.gnu.org/licenses/>.
//
//    Please see LICENSE.txt for more details.
//
//    --------------------------------------------------------------------


#ifndef __HILBERT_H__
#define __HILBERT_H__

struct edf_t;
struct param_t;

#include <vector>
#include "miscmath/miscmath.h"

struct emp_t { 

  void set_observed( double x ) { obs = x; } 
  
  void add_permuted( double x ) { perm.push_back( x ) ; }   

  double calc_stats() 
  {
    mean = MiscMath::mean( perm );
    sd = MiscMath::sdev( perm );
    p = pvalue() ; 
  }

  double pvalue() const {
    int r = 0;
    int n = perm.size();
    for ( int i = 0 ; i < n ; i++) if ( perm[i] >= obs ) ++r;
    return ( r + 1 ) / (double)( n + 1 );
  }

  double obs;
  std::vector<double> perm;
  double mean;
  double sd;
  double p;

};

struct itpc_t {   

  itpc_t( const int ne , const int nbins = 20 ); 

  // for permutation-based results
  int nrep;
  emp_t ninc;
  emp_t itpc;
  emp_t pv; // Rayleigh's Z / p-value
  emp_t sig; // whether pv is significant or not (1/0) at p < 0.05
  emp_t angle; // empirical p-value does make sense here, but store distribution of permuted values nonetheless
  std::vector<emp_t> phasebin; // counts of event phase in (say) 20-deg bins

  // for each passed-in event, set phase, and record whether or not it was in a valid region (i.e. in SO)
  std::vector<double> phase;
  std::vector<bool> event_included;
}; 


struct hilbert_t
{
  
  hilbert_t() { } 
  
  // Hilbert transform
  hilbert_t( const std::vector<double> & d );
  
  // filter-Hilbert
  hilbert_t( const std::vector<double> & d , const int sr , double lwr , double upr , double ripple, double tw );
  
  
  // extract instantaneous phase, magnitude
  const std::vector<double> * phase() const;
  //  const std::vector<double> * angle() const;
  const std::vector<double> * magnitude() const;
  const std::vector<double> * signal() const;
  std::vector<double> instantaneous_frequency(double) const;

  itpc_t phase_events( const std::vector<int> & , const std::vector<bool> * m = NULL , 
		       const int nreps = 0 , 
		       const int sr = 0 , 
		       const double epoch_sec = 0 
		       ) const;
  
private:

  void proc();
  void unwrap(std::vector<double> * ) const;
  void bin( double ph , int bs , std::vector<int> * acc ) const;
  std::vector<double> input;
  std::vector<double> ph;
  std::vector<double> mag;
  
};


#endif
