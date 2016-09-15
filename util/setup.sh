export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
alias setupATLAS='source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh'
setupATLAS # or equivalent on your machine
localSetupDQ2Client
localSetupFAX
localSetupPyAMI
localSetupROOT
#rcSetup Base,2.4.18
rcSetup Base,2.4.X,rel_0
voms-proxy-init -voms atlas
