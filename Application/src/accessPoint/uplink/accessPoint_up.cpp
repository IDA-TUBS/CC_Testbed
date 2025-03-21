#include <cc_linkmonitor/mobility_management/connectionManager.hpp>
#include <cc_linkmonitor/mobility_management/linkMonitor.hpp>
#include <cc_linkmonitor/mobility_management/mobilityServer.hpp>
#include <cc_linkmonitor/abstraction/socket_endpoint.hpp>
#include <w2rp/log.hpp>

#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <mutex>
#include <condition_variable>

#include <opencv2/opencv.hpp>

#include <roaming/subscriber.hpp>
#include <roaming/misc.hpp>

/**
 * @brief Basic access point for the roaming demonstrator
 * 
 * @param argc 
 * @param argv [1] node ID suffix [2] heartbeat period [us] [3] image rows [4] image columns
 * @return int 
 */
int main(int argc, char* argv[])
{
    w2rp::init_console_log();
    
    // Default configuration
    int node_suffix = 1;
    std::chrono::microseconds hb_period(3000);
    image_config img;

    // Check supplied arguments
    if(argc > 1)
    {
        node_suffix = atoi(argv[1]);
    }
    if(argc > 2)
    {
        hb_period = std::chrono::microseconds(atoi(argv[2]));
    }
    if(argc > 4)
    {
        img = image_config(atoi(argv[3]), atoi(argv[4]));
    }

    logInfo("Image Config: " << img.rows << ":" << img.columns)


    /* -------------------------------- Define ID ------------------------------- */
    // 00.00.00.00.00.00.00.00.00.00.00.00.00.00.FF.xx
    unsigned char guid[16] = {0x00};
    
    guid[14] = static_cast<unsigned char>(0xff);
    guid[15] = static_cast<unsigned char>(node_suffix);
    
    cc_linkmonitor::UUID_t node_id(guid);

    // create ip map for connection manager
    cc_linkmonitor::ConnectionManager::ip_map map;
    map.insert(std::make_pair("192.168.2.102", "192.168.20.6"));
    map.insert(std::make_pair("192.168.2.103", "192.168.30.6"));
    
    
    cc_linkmonitor::linkReaderConfig_t link_conf(
        /*HB Socket interface*/ "192.168.2.100",
        /*HB Socket Port */     50000,
        /*HB Period*/           hb_period,
        /*HB Slack*/            std::chrono::microseconds(2000),
        /*Max HB loss*/         2  
    );
    
    cc_linkmonitor::linkReader link_reader(
        node_id,
        link_conf.socket,
        link_conf.period,
        link_conf.slack,
        link_conf.loss
    );

    /* ------------------------- create mobility server ------------------------- */
    cc_linkmonitor::socket_endpoint mobility_server_endpoint(
        std::string("0.0.0.0"),
        40000
    );
    cc_linkmonitor::mobilityServer server(
        node_id,
        mobility_server_endpoint,
        map,
        link_reader.link_status()
    );

    std::function<void(std::string, int)> writer_callback;
    writer_callback = server.get_callback();

    cc_linkmonitor::ConnectionManager manager(
        map,
        writer_callback,
        link_reader.link_change(),
        link_reader.link_status()
    );

    cc_linkmonitor::linkMessage temp;
    udp::endpoint link = link_reader.init_heartbeat(temp);
    manager.init(link.address().to_string());
    server.init(link.address().to_string(), link.port());

    /* ---------------------------- start subscriber ---------------------------- */
    uint16_t p_id = 0x5340;
    
    std::string cfg_path = home_dir + w2rp_cfg_up;
    std::string setup_path = home_dir + setup_defines_path;

    Subscriber mySub(true, true);
    if (mySub.init(p_id, cfg_path, setup_path, img))
    {
        mySub.run();
    }

    while(true)
    {

    }

    mySub.stop();
    link_reader.stop();

    return 0;
}
