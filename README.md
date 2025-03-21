# Continuous Connectivity Testbed

Repository for a model truck based continuous connectivity testbed.
The goal of the testbed is to provide a proof-of-concept for achieving reliable low latency streaming of large data objects in wireless networks [1,2].
This open-source testbed is intended for researchers, engineers, and developers working on industrial wireless communication and real-time networking.

For more detailed information on how the continuous streaming works, as well as performance evaluation (in simulation and using physical demonstrator setups), please refer to the [LOTUS](https://ida-tubs.github.io/lotus/) project website or the papers listed below.

---

## Hardware

### Vehicle Platform
1/14 scale RC truck and trailer model.
Servos and ESC are interfaced by a PCA9685 servo driver, connected to the compute unit via I2C.

#### Compute Unit
Compute Unit based on an Nvidia Jetson Orin Nano dev board.
The orin nano is equipped with two ath9k-based WiFi NICs (AR9462).
The ath9k driver was patched to allow AP and IBSS mode in the 5 Ghz band.
Refer to `./JetsonBuild/README.md` for more information.

#### Remote Control
Xbox Series gamepad

### Backbone
The backbone consists of three x86 hosts (Dell OptiPlex), running Ubuntu 22.04:

- 1x *Edge Server* node, hosting a linkmonitoring reader in addition to a publishing/subscribing application.
- 2x *Access Point* nodes with one wireless interface each, providing connectivity between edge server and mobile node.

Similar to the vehicle platform, each AP is equipped with a ath9k-based WiFi NIC (AR9462).
The aforementioned patch was applied on the APs as well.
Edge server and access points should be connected via a basic ethernt switch.

---

## Software

this approach leverages proactive link establishment and fast loss detection to meet reliability and latency constraints without requiring redundant data transmissions.

### Streaming Application
For the testbed a video stream is generated using [openCV](https://opencv.org/), allowing free configuration of data sizes.
The data transmission is managed by an open-source implementation of the [Wireless Reliable Realtime Protocol (W2RP)](https://github.com/IDA-TUBS/lwW2RP).
The application can be found in `Application/`.
Both uplink (mobile node -> edge server) and downlink (edge server -> mobile node) implementations are available.

### Resource Management
The resource management leverages proactive link establishment and fast loss detection to meet reliability and latency constraints without requiring redundant data transmissions.
A proof of concept implementation for the resource management is provided by the [CC_LinkMonitor Library](https://github.com/IDA-TUBS/CC_LinkMonitor)

### Vehicle Control
The RC control is based on [DonkeyCar](http://docs.donkeycar.com/).
Allows remote control via gamepad or "path follow autopilot" based on predefined traces.
The configuration files and script to control the truck can be found under `./TruckControl/mycar/`.

---

## Configuration

### Jetson
Configure the wireless interface:
`sudo python3 scripts/startAdhoc.py -n 6 -m COHDA`
`sudo python3 scripts/startAdhoc.py -n 6 -m CTRL`

### Edge Server
Configure forwarding rules
`sudo sh scripts/backbone/edge_forwarding.sh`

### Linux-based Access Points
Configure the wireless interface
- AP1(client2 node): `sudo python3 scripts/startAdhoc.py -n 4 -m COHDA`
- AP2(client3 node): `sudo python3 scripts/startAdhoc.py -n 5 -m CTRL`
Configure Heartbeat and Dataplane forwarding:
`sudo sh scripts/backbone/hb_forwarding.sh`
Check connectivity via ping to Jetson
- AP1(client2): `ping 192.168.20.6`
- AP1(client3): `ping 192.168.30.5`
*Note: ping reports no route to host, reboot **all** systems and repeat the configuration progess!*
*Note: pinging the Jetson from the telemetry does not work. The Jetson does not have the necessary routes configured!*

---

## Starting the Demo Application
For downlink mode, the publisher is hosted on the edge server and the subscriber on the mobile node.
For uplink mode, the subscriber is hosted on the edge server and the publisher on the mobile node.

### Downlink
Start the publisher on the edge server. 
`./Application/build/bin/accessPointGUI_down 3 5000 300 200`

Start the subscriber on the mobile node.
`./Application/build/bin/mobileNodeGUI_down 6 5000 300 200`

### Uplink
Start the publisher on the mobile node. 
`./Application/build/bin/mobileNode_up 6 5000 300 200`

Start the subscriber on the edge server.
`./Application/build/bin/accessPoint_up 3 5000 300 200`

These examples will start the video streaming application with 300x200 images.
The heartbeat period for the linkmonitoring is set to 5000 us.

For more information on the demo application refer to `Application/README.md`

---

## References

[1] Daniel Tappe, Alex Bendrick and Rolf Ernst, "**Continuous multi-access communication for high-resolution low-latency V2X sensor streaming**",  In *2024 IEEE Intelligent Vehicles Symposium (IV)*, 2024, June. <https://doi.org/10.24355/dbbs.084-202405020845-0>

[2] Daniel Tappe, Alex Bendrick and Rolf Ernst, "**Continuous Streaming in Roaming Scenarios: AModel Truck-Based Demonstration**", *arXiv preprint arXiv:n.n*, 2025. <https://doi.org/n.n/arXiv.n.n>

[3] Daniel Tappe, Alex Bendrick and Rolf Ernst, "**Enabling Continuous Low Latency Streaming in Industrial Roaming Scenarios**", *arXiv preprint arXiv:n.n*, 2025. <https://doi.org/n.n/arXiv.n.n>

## Contact

Daniel Tappe (tappe@ida.ing.tu-bs.de)

Alex Bendrick (bendrick@ida.ing.tu-bs.de)

When using or referencing the continuous streaming approach in scientific works we kindly ask you reference the papers mentioned above.

---

## Acknowledgement

<table>
  <tr>
    <td>
        <a href="https://www.dfg.de/en">
            <img src="docs/dfg_logo_weiss.png" alt="dfg_logo" width="100"></td>
        </a>
    </td>
    <td>
        This work was supported by the Deutsche Forschungsgemeinschaft (DFG, German Research Foundation) under Grant ER168/35-1.
    </td>
  </tr>
</table>
