#include "EventLoop/StatusCode.h"
#include "EventLoop/Worker.h"

#include "xAODRootAccess/TEvent.h"
#include "xAODEventInfo/EventInfo.h"

#include "SUSYTools/SUSYObjDef_xAOD.h"

#include "RJigsawTools/RegionVarCalculator.h"
#include "RJigsawTools/strongErrorCheck.h"

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

  RegionVars["runNumber"]   = eventInfo->runNumber();
  RegionVars["lumiBlock"]   = eventInfo->lumiBlock();
  RegionVars["bcid"]        = eventInfo->bcid();
  RegionVars["eventNumber"] = eventInfo->eventNumber();
  RegionVars["mcChannelNumber"] = eventInfo->mcChannelNumber();
  RegionVars["actualInteractionsPerCrossing"] = eventInfo->actualInteractionsPerCrossing();
  RegionVars["averageInteractionsPerCrossing"] = eventInfo->averageInteractionsPerCrossing();

  RegionVars["mcEventWeight"] = eventInfo->auxdecor< float >("mcEventWeight");
  RegionVars["pileupWeight"]  = eventInfo->auxdecor< float >("PileupWeight");


  // Get relevant info from the vertex container //////////////////////
  //

  const xAOD::VertexContainer* vertices = nullptr;
  STRONG_CHECK(event->retrieve( vertices, "PrimaryVertices"));
  RegionVars["NPV"] = HelperFunctions::countPrimaryVertices(vertices, 2);

  //
  /////////////////////////////////////////////////////////////////////


  return EL::StatusCode::SUCCESS;
}

