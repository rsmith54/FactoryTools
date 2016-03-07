#ifndef REGION_VARIABLE_CALCULATOR_TLS_H
#define REGION_VARIABLE_CALCULATOR_TLS_H
//author : Russell Smith
//date   : January 2015

#include "EventLoop/StatusCode.h"
#include "RJigsawTools/RegionVarCalculator.h"

#include <map>
#include <iostream>

class RegionVarCalculator_tls : public RegionVarCalculator {

public :

private :
  //todo probably clean this up
  virtual EL::StatusCode doInitialize(EL::Worker * worker);
  virtual EL::StatusCode doCalculate (std::map<std::string, double>               & RegionVars,
				      std::map<std::string, std::vector<double> > & VecRegionVars);

  EL::StatusCode doAllCalculations   (std::map<std::string, double>               & RegionVars,
				      std::map<std::string, std::vector<double> > & VecRegionVars);

  EL::StatusCode doSRCalculations    (std::map<std::string, double>               & RegionVars,
				      std::map<std::string, std::vector<double> > & VecRegionVars);

  EL::StatusCode doCR1LCalculations  (std::map<std::string, double>               & RegionVars,
				      std::map<std::string, std::vector<double> > & VecRegionVars);

  EL::StatusCode doCR0LCalculations  (std::map<std::string, double>               & RegionVars,
				      std::map<std::string, std::vector<double> > & VecRegionVars);


public :
  // this is needed to distribute the algorithm to the workers
  ClassDef(RegionVarCalculator_tls, 1);

};

#endif //REGION_VARIABLE_CALCULATOR_TLS_H

//  LocalWords:  ifndef
