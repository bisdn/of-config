OF-Config for xDPd
==================

This repository provides the configuration and management module for
[xDPd][1]. It implements the [OF-Config protocol][2] (version 1.1.1) and uses
[netopeer][3] as NETCONF server. Currently only the parts covered by the xDPd
management API are available via OF-Config.

Examples 
--------

    <!-- create logical switch instance -->
    <capable-switch xmlns="urn:onf:of111:config:yang">
      <id>capable-switch-0</id>
      <logical-switches>
        <switch>
          <id>logical-switch-123</id>
          <datapath-id>11:11:11:11:11:11:11:11</datapath-id>
          <controllers>
            <controller>
              <id>controller-0</id>
              <ip-address>10.0.0.10</ip-address>
            </controller>
          </controllers>
        </switch>
      </logical-switches>
    </capable-switch>

    <!-- delete logical switch -->
    <capable-switch xmlns="urn:onf:of111:config:yang" xmlns:xc="urn:ietf:params:xml:ns:netconf:base:1.0">
      <id>capable-switch-0</id>
      <logical-switches>
        <switch xc:operation="delete" />
      </logical-switches>
    </capable-switch>

    <!-- set a new id -->
    <capable-switch xmlns="urn:onf:of111:config:yang" xmlns:xc="urn:ietf:params:xml:ns:netconf:base:1.0">
      <id>capable-switch-0</id>
    </capable-switch>

    <!-- delete the id (will not work later) -->
    <capable-switch xmlns="urn:onf:of111:config:yang" xmlns:xc="urn:ietf:params:xml:ns:netconf:base:1.0">
      <id xc:operation="delete">capableasdf</id>
    </capable-switch>


References
----------

[1]: http://www.xdpd.org/
[2]: https://www.opennetworking.org/technical-communities/areas/specification
[3]: https://code.google.com/p/netopeer/
