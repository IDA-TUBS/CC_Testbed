#include <cc_linkmonitor/mobility_management/linkMonitor.hpp>
#include <cc_linkmonitor/mobility_management/connectionManager.hpp>
#include <cc_linkmonitor/mobility_management/mobilityClient.hpp>
#include <cc_linkmonitor/uuid.hpp>
#include <w2rp/log.hpp>

#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include <opencv2/opencv.hpp>

#include <roaming/publisher.hpp>
#include <roaming/misc.hpp>

/**
 * @brief Basic mobiel node for the roaming demonstrator
 * 
 * @param argc 
 * @param argv [1] node ID suffix [2] heartbeat period [us] [3] image rows [4] image columns
 * @return int 
 */
int main(int argc, char* argv[])
{
    w2rp::init_console_log();
    std::string log_path = log_dir + reader_prefix;
    w2rp::init_file_log(log_path, generateTimestamp());
    
    // Default configuration
    int node_suffix = 0;
    std::chrono::microseconds hb_period(3000);
    std::string ip_base = "192.168.20.";
    image_config img;

    // Check supplied arguments
    parseArguments(
        argc, 
        argv, 
        node_suffix, 
        hb_period, 
        img
    );
    
    logInfo("Node Suffix: " << node_suffix)
    logInfo("HB Period: " << hb_period.count())
    logInfo("Image Config: " << img.rows << ":" << img.columns)

    // Define ID
    // 00.00.00.00.00.00.00.00.00.00.00.00.00.00.FF.xx
    unsigned char guid[16] = {0x00};
    guid[14] = static_cast<unsigned char>(0xff);
    guid[15] = static_cast<unsigned char>(node_suffix);
    
    cc_linkmonitor::UUID_t node_id(guid);    

    // ------------------  create publisher --------------------------------------------------------------------------
    uint16_t p_id = 0x8517;
    std::string cfg_path = home_dir + w2rp_cfg_up;
    std::string setup_path = home_dir + setup_defines_path;

    Publisher myPub;
    myPub.init(p_id, cfg_path, setup_path, img);

    std::function<void(std::string, int)> writer_callback;
    writer_callback = myPub.writer_callback();
    // -----------------------------------------------------------------------------------------------------------------

    /* --------------------------- Mobility Client --------------------------- */
    cc_linkmonitor::socket_endpoint mobility_client_endpoint(
        std::string("0.0.0.0"),
        40000
    );
    cc_linkmonitor::mobilityClient client(
        node_id,
        mobility_client_endpoint
    );


    /* --------------------------- Connection Manager --------------------------- */
    // create ip map for connection manager
    cc_linkmonitor::ConnectionManager::ip_map map;
    map.insert(std::make_pair("192.168.2.102", "192.168.20.4"));
    map.insert(std::make_pair("192.168.2.103", "192.168.30.5"));

    cc_linkmonitor::ConnectionManager manager(
        map,
        writer_callback,
        client.link_change(),
        client.link_status()
    );


    // ------- Link Writer 1 ---------
    // Assemble ip
    std::string node_ip = ip_base + std::to_string(node_suffix);
    // Define HB writer config
    cc_linkmonitor::linkWriterConfig_t link_conf(
        /*HB Socket interface*/ node_ip,
        /*Reader Addr */        "192.168.20.4",
        /*HB Socket Port */     50000,
        /*HB Period*/           hb_period
    );

    // ------- Link Writer 2 ---------
    std::string node_ip_2 = "192.168.30." + std::to_string(node_suffix);
    cc_linkmonitor::linkWriterConfig_t link_conf_2 = link_conf;
    link_conf_2.socket.ip_addr = node_ip_2;
    link_conf_2.reader.ip_addr = "192.168.30.5";

    cc_linkmonitor::linkWriter link_writer(
        node_id, 
        link_conf.socket,
        link_conf.reader,
        link_conf.period, 
        false
    );

    cc_linkmonitor::linkWriter link_writer2(
        node_id, 
        link_conf_2.socket,
        link_conf_2.reader,
        link_conf_2.period, 
        false
    );
    
    link_writer.run();
    link_writer2.run();

    std::string link = client.init();
    CC_logDebug("Init link: " << link)
    manager.init(link);

    myPub.run();
    while(true)
    {
        
    }

    link_writer.stop();
    link_writer2.stop();

    return 0;
}
