# setupATLAS # or equivalent on your machine

ls -l /cvmfs/atlas.cern.ch/repo
pwd
source ~/.bashrc  || echo ignore alrb
lsetup root || echo ignore alrb
rcSetup -r || echo ignore alrb
#rcSetup Base,2.4.X,rel_0 || echo ignore alrb
rcSetup Base,2.4.19 || echo ignore alrb

echo "${servicePass}" | kinit ${serviceUser}@CERN.CH
klist

# echo "Host svn.cern.ch
#    user atlzerol" >> ~/.ssh/config

echo "Host svn.cern.ch
   User atlzerol
   GSSAPIAuthentication yes
   GSSAPIDelegateCredentials yes
   Protocol 2
   ForwardX11 no
   IdentityFile ~/.ssh/id_rsa" >> ~/.ssh/config