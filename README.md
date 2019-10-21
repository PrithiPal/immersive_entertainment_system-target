# gaming-target


 1. Go to [192.168.1.254](http://192.168.1.254/)

 2. Take note of that IP address that the beaglebone appears on under the list of devices

 3. Ensure that the folder `$(HOME)/cmpt433/public/myApps` is accessible from your beaglebone.

 4. Run following commands
```shell
//from host
git clone https://csil-git1.cs.surrey.sfu.ca/cmpt433brownboyz/gaming-target.git
git clone https://csil-git1.cs.surrey.sfu.ca/cmpt433brownboyz/gaming-libraries.git
cd gaming-target/
export INTERFACE_ADDR="<ip_of_beaglebone>"
make

//from beaglebone
cd /mnt/remote/myApps
./gaming-beaglebone
```

5. Now follow the instructons on [Gaming Host](https://csil-git1.cs.surrey.sfu.ca/cmpt433brownboyz/gaming-host)
