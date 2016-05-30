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
							  std::map<std::string, std::vector<double> > & VecRegionVars)
{
  xAOD::TEvent * event = m_worker->xaodEvent();
  // Get relevant info from the EventInfo object

  const xAOD::EventInfo* eventInfo = nullptr;
  STRONG_CHECK(event->retrieve( eventInfo, "EventInfo"));

  // Include variables for all samples ///////////////////////////////
  //
  RegionVars["runNumber"]   = eventInfo->runNumber();
  RegionVars["lumiBlock"]   = eventInfo->lumiBlock();
  RegionVars["bcid"]        = eventInfo->bcid();
  RegionVars["eventNumber"] = eventInfo->eventNumber();

  //
  /////////////////////////////////////////////////////////////////////

  // For MC Samples only (Not defined for data) ///////////////////////
  //

  if(eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) ){
    RegionVars["mcChannelNumber"] = eventInfo->mcChannelNumber();
    RegionVars["mcEventWeight"] = eventInfo->auxdecor< float >("mcEventWeight");
  }
  //
  /////////////////////////////////////////////////////////////////////

  // Include variables for all samples ///////////////////////////////
  //
  RegionVars["pileupWeight"]  = eventInfo->auxdecor< float >("PileupWeight");
  RegionVars["actualInteractionsPerCrossing"] = eventInfo->actualInteractionsPerCrossing();
  RegionVars["averageInteractionsPerCrossing"] = eventInfo->averageInteractionsPerCrossing();

  //
  /////////////////////////////////////////////////////////////////////

  // Get relevant info from the vertex container //////////////////////
  //

  const xAOD::VertexContainer* vertices = nullptr;
  STRONG_CHECK(event->retrieve( vertices, "PrimaryVertices"));
  RegionVars["NPV"] = HelperFunctions::countPrimaryVertices(vertices, 2);

  //
  /////////////////////////////////////////////////////////////////////


  return EL::StatusCode::SUCCESS;
}

