# setupATLAS # or equivalent on your machine

ls -l /cvmfs/atlas.cern.ch/repo
pwd
source ~/.bashrc  || echo ignore alrb
lsetup root || echo ignore alrb
rcSetup -r || echo ignore alrb
#rcSetup Base,2.4.X,rel_0 || echo ignore alrb
rcSetup Base,2.4.18 || echo ignore alrb

echo "${servicePass}" | kinit ${serviceUser}@CERN.CH
klist

echo "Host svn.cern.ch
   user atlzerol" >> ~/.ssh/config
