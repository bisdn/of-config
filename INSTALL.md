Requirements
============

* libnetconf (0.9.x)
* netopeer
* xdpd (with xmp config plugin)

Installation
============

    ./autogen.sh
    ./configure
    make install
    
    # add module to netopeer
    netopeer-manager add --name of-config1.1.1 --model ./of-config1.1.1.yin --transapi /PATH/TO/of-config1.1.1.so --datastore /var/lib/libnetconf/ofc_datastore.xml
    netopeer-manager add --name of-config1.1.1 --augment ./xdpd-mgmt.yin
    
    # delete module from server
    netopeer-manager rm --name of-config1.1.1
