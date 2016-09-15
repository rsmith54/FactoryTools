#export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
#alias setupATLAS='source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh'
setupATLAS || true
lsetup fax pyami root dq2 'rcSetup Base,2.4.X,rel_0'
#rcSetup Base,2.4.18
#voms-proxy-init -voms atlas
