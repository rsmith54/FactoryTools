#ifndef REGION_VARIABLE_CALCULATOR_H
#define REGION_VARIABLE_CALCULATOR_H
//author : Russell Smith
//date   : January 2015

#include "EventLoop/StatusCode.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODMissingET/MissingET.h"

#include <map>
#include <iostream>

namespace EL{
  class Worker;
}

class RegionVarCalculator {

public :
  virtual ~RegionVarCalculator() = 0;


public :
  EL::StatusCode initialize(EL::Worker * worker){return doInitialize( worker );}
  //to be used per event
  EL::StatusCode calculate(std::map<std::string, double>              & RegionVars,
			   std::map<std::string, std::vector<double> >& VecRegionVars
			   //This is somewhat for simplicity, to be able to write out vectors of i.e. jet pts
			   ){return doCalculate( RegionVars, VecRegionVars);}


private :
  //todo probably clean this up
  virtual EL::StatusCode doInitialize(EL::Worker * worker){
    std::cout << "you called the base calculator function! Exiting" << std::endl;
    return EL::StatusCode::FAILURE;};
  virtual EL::StatusCode doCalculate(std::map<std::string, double>              & RegionVars,
                                     std::map<std::string, std::vector<double> >& VecRegionVars)
{std::cout << "you called the base calculator function! Exiting" << std::endl;return EL::StatusCode::FAILURE;};

protected :
  RegionVarCalculator() : m_worker(nullptr) {};//this is to set this to nullptr in the constructor for the derived class
  EL::Worker * m_worker; //we need to give the Worker to the calculator for flexibility.
  //This should be pointed to the EventLoop Worker in your alg which fills your map

  EL::StatusCode doGeneralCalculations(std::map<std::string, double>              & RegionVars,
				       std::map<std::string, std::vector<double> >& VecRegionVars);
public :
  // this is needed to distribute the algorithm to the workers
  ClassDef(RegionVarCalculator, 1);

};

inline RegionVarCalculator::~RegionVarCalculator() { }

#endif //REGION_VARIABLE_CALCULATOR_H

//  LocalWords:  ifndef
