#
cd ..
tar cvfz WinSURE-distr.tgz --exclude-vcs -X SURE/tar-exclude SURE
mv WinSURE-distr.tgz SURE
echo "WinSURE-distr.tgz created"
tar cvfz WinASSIST-distr.tgz --exclude-vcs -X SURE/tar-exclude SURE/assist
mv WinASSIST-distr.tgz SURE
echo "WinASSIST-distr.tgz created"
ls -s SURE/WinSURE*
ls -s SURE/WinASSIST*
