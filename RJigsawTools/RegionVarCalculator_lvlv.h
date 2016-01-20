#ifndef REGION_VARIABLE_CALCULATOR_LVLV_H
#define REGION_VARIABLE_CALCULATOR_LVLV_H
//author : Russell Smith
//date   : January 2015

#include "EventLoop/StatusCode.h"
#include "RJigsawTools/RegionVarCalculator.h"

#include <unordered_map>
#include <iostream>

class RegionVarCalculator_lvlv : public RegionVarCalculator {

public :

private :
  //todo probably clean this up
  virtual EL::StatusCode doInitialize(xAOD::TStore * store);
  virtual EL::StatusCode doCalculate (std::unordered_map<std::string, double>               & RegionVars,
				      std::unordered_map<std::string, std::vector<double> > & VecRegionVars);

  EL::StatusCode doAllCalculations   (std::unordered_map<std::string, double>               & RegionVars,
				      std::unordered_map<std::string, std::vector<double> > & VecRegionVars);

  EL::StatusCode doSRCalculations    (std::unordered_map<std::string, double>               & RegionVars,
				      std::unordered_map<std::string, std::vector<double> > & VecRegionVars);

  EL::StatusCode doCR1LCalculations  (std::unordered_map<std::string, double>               & RegionVars,
				      std::unordered_map<std::string, std::vector<double> > & VecRegionVars);

  EL::StatusCode doCR0LCalculations  (std::unordered_map<std::string, double>               & RegionVars,
				      std::unordered_map<std::string, std::vector<double> > & VecRegionVars);


public :
  // this is needed to distribute the algorithm to the workers
  ClassDef(RegionVarCalculator_lvlv, 1);

};

#endif //REGION_VARIABLE_CALCULATOR_LVLV_H

//  LocalWords:  ifndef
