# Continuous Connectivity Testbed: Video Streaming Application

An example data transmission is provided through a video streaming application based on W2RP.
The application generates samples using [openCV](https://opencv.org/).

## Dependencies
The Application uses the [CC_LinkMonitor library](https://github.com/IDA-TUBS/CC_LinkMonitor) for link monitoring and path switching.
For data plane transmissions [lightweight W2RP](https://github.com/IDA-TUBS/lwW2RP) is used.

For detailed information on the functionality and usage of the libraries please refere to the respective repositories.

## How to build
From the `Application` directory execute
`mkdir build && cd build`
`cmake .. -DLOG=ON -DFILE=ON -DCONSOLE=ON`
`cmake --build . -j<n_cpu>`

## Configuration
Sample deadlines and shaping times can be configured through lwW2RP (cf. `cfg/w2rp_config*.json`).
The paths to the w2rp config file are defined in `include/roaming/misc.hpp`.
Sample sizes can be freely configured by defining the height and width of the images.
The color depth of the images is set to 8 bit. 
Hence, a 300x200 image results in a sample size of 60 kB.
With a deadline and period of 100 ms, this leads to a load of around 5 Mbit/s.
The application takes the sample sizes as launch arguments, cf **Running the Application**.

## Running the Application
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
*Note: The publishing application waits for the reception of heartbeats prior to publishing samples.*