cd ..
cp conf/proj /etc/system.conf.d
mkdir /home/proj
cp -vr images/ /home/proj
cd code
chmod 777 compile.sh
chmod 777 run.sh
