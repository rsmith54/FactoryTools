#ifndef RJIGSAW_CALCULATOR_H
#define RJIGSAW_CALCULATOR_H
//author : Russell Smith
//date   : December 2015

#include "EventLoop/StatusCode.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODMissingET/MissingET.h"

#include <unordered_map>

class RJigsawCalculator {

public :
  virtual ~RJigsawCalculator() = 0;


public :
  EL::StatusCode initialize(){return doInitialize();};
  //to be used per event
  EL::StatusCode clearEvent(){return doClearEvent();};
  EL::StatusCode calculate(std::unordered_map<std::string, double>& RJVars,
			   xAOD::IParticleContainer& particles,
			   xAOD::MissingET & met
			   ){return doCalculate( RJVars,
						 particles,
						 met);};

private :
  //todo probably clean this up
  virtual EL::StatusCode doInitialize(){assert(0&&"you called the base calculator function! Exiting");return EL::StatusCode::FAILURE;};
  virtual EL::StatusCode doClearEvent(){assert(0&&"you called the base calculator function! Exiting");return EL::StatusCode::FAILURE;};
  virtual EL::StatusCode doCalculate(std::unordered_map<std::string, double>& RJVars,
                                     xAOD::IParticleContainer& particles,
                                     xAOD::MissingET& met
                                     ){assert(0&&"you called the base calculator function! Exiting");return EL::StatusCode::FAILURE;};

public :
  // this is needed to distribute the algorithm to the workers
  ClassDef(RJigsawCalculator, 1);

};

inline RJigsawCalculator::~RJigsawCalculator() { }

#endif //RJIGSAW_CALCULATOR_H
