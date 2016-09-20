#include "EventLoop/StatusCode.h"
#include "EventLoop/Worker.h"

#include "xAODRootAccess/TEvent.h"
#include "xAODEventInfo/EventInfo.h"

#include "SUSYTools/SUSYObjDef_xAOD.h"

#include "FactoryTools/RegionVarCalculator.h"
#include "FactoryTools/strongErrorCheck.h"

#include <xAODAnaHelpers/HelperFunctions.h>


// this is needed to distribute the algorithm to the workers
ClassImp(RegionVarCalculator)

EL::StatusCode RegionVarCalculator::doGeneralCalculations(std::map<std::string, double>& RegionVars,
							  std::map<std::string, std::vector<float> > & VecRegionVars)
{
  xAOD::TEvent * event = m_worker->xaodEvent();
  // Get relevant info from the EventInfo object

  const xAOD::EventInfo* eventInfo = nullptr;
  STRONG_CHECK(event->retrieve( eventInfo, "EventInfo"));

  // Include variables for all samples ///////////////////////////////
  //
  RegionVars["runNumber:int"]   = eventInfo->runNumber();
  RegionVars["lumiBlock:int"]   = eventInfo->lumiBlock();
  RegionVars["bcid:int"]        = eventInfo->bcid();
  RegionVars["eventNumber:longlong"] = eventInfo->eventNumber();

  //
  /////////////////////////////////////////////////////////////////////

  // For MC Samples only (Not defined for data) ///////////////////////
  //

  if(eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) ){
    RegionVars["mcChannelNumber:int"] = eventInfo->mcChannelNumber();
    RegionVars["mcEventWeight:int"] = eventInfo->auxdecor< float >("mcEventWeight");
  }
  //
  /////////////////////////////////////////////////////////////////////

  // Include variables for all samples ///////////////////////////////
  //
  RegionVars["pileupWeight:float"]  = eventInfo->auxdecor< float >("PileupWeight");
  RegionVars["actualInteractionsPerCrossing:float"] = eventInfo->actualInteractionsPerCrossing();
  RegionVars["averageInteractionsPerCrossing:float"] = eventInfo->averageInteractionsPerCrossing();

  //
  /////////////////////////////////////////////////////////////////////

  // Get relevant info from the vertex container //////////////////////
  //

  const xAOD::VertexContainer* vertices = nullptr;
  STRONG_CHECK(event->retrieve( vertices, "PrimaryVertices"));
  RegionVars["NPV:int"] = HelperFunctions::countPrimaryVertices(vertices, 2);

  //
  /////////////////////////////////////////////////////////////////////


  return EL::StatusCode::SUCCESS;
}

