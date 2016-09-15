# setupATLAS # or equivalent on your machine

source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh

localSetupDQ2Client
localSetupFAX
localSetupPyAMI
localSetupROOT
#rcSetup Base,2.4.18
rcSetup Base,2.4.X,rel_0
voms-proxy-init -voms atlas