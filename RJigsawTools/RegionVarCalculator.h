#ifndef REGION_VARIABLE_CALCULATOR_H
#define REGION_VARIABLE_CALCULATOR_H
//author : Russell Smith
//date   : January 2015

#include "EventLoop/StatusCode.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODMissingET/MissingET.h"

#include <unordered_map>
#include <iostream>

namespace xAOD{
  class TStore;
}

class RegionVarCalculator {

public :
  virtual ~RegionVarCalculator() = 0;


public :
  EL::StatusCode initialize(xAOD::TStore * store){return doInitialize( store );}
  //to be used per event
  EL::StatusCode calculate(std::unordered_map<std::string, double>              & RegionVars,
			   std::unordered_map<std::string, std::vector<double> >& VecRegionVars
			   //This is somewhat for simplicity, to be able to right out vectors of i.e. jet pts
			   ){return doCalculate( RegionVars, VecRegionVars);}


private :
  //todo probably clean this up
  virtual EL::StatusCode doInitialize(xAOD::TStore * store){
    std::cout << "you called the base calculator function! Exiting" << std::endl;
    return EL::StatusCode::FAILURE;};
  virtual EL::StatusCode doCalculate(std::unordered_map<std::string, double>              & RegionVars,
                                     std::unordered_map<std::string, std::vector<double> >& VecRegionVars)
{std::cout << "you called the base calculator function! Exiting" << std::endl;return EL::StatusCode::FAILURE;};

protected :
  RegionVarCalculator() : m_store(nullptr) {};//this is to set this to nullptr in the constructor for the derived class
  xAOD::TStore * m_store; //we need to give the store to the calculator for flexibility.
  //This should be pointed to the EventLoop store in your alg which fills your map

public :
  // this is needed to distribute the algorithm to the workers
  ClassDef(RegionVarCalculator, 1);

};

inline RegionVarCalculator::~RegionVarCalculator() { }

#endif //REGION_VARIABLE_CALCULATOR_H

//  LocalWords:  ifndef
