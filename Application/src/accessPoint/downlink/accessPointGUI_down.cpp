#include <cc_linkmonitor/mobility_management/connectionManager.hpp>
#include <cc_linkmonitor/mobility_management/linkMonitor.hpp>
#include <w2rp/log.hpp>

#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <mutex>
#include <condition_variable>

#include <opencv2/opencv.hpp>

#include <qt5/QtWidgets/QApplication>
#include <qt5/QtWidgets/QWidget>
#include <qt5/QtWidgets/QPushButton>
#include <qt5/QtWidgets/QHBoxLayout>
#include <qt5/QtGui/QPainter>
#include <qt5/QtCore/QTimer> // Include QTimer header

#include <roaming/publisher.hpp>
#include <roaming/colorWidget.hpp>
#include <roaming/misc.hpp>
#include <roaming/linkMonitorWidget.hpp>

/**
 * @brief Basic access point for the roaming demonstrator
 * 
 * @param argc 
 * @param argv [1] node ID suffix [2] heartbeat period [us] [3] image rows [4] image columns
 * @return int 
 */
int main(int argc, char* argv[])
{
    // Init Logging
    w2rp::init_console_log();
    std::string log_path = log_dir + writer_prefix;
    w2rp::init_file_log(log_path, generateTimestamp());

    // Default configuration
    int node_suffix = 1;
    std::chrono::microseconds hb_period(3000);
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
    std::string cfg_path = home_dir + w2rp_cfg_down;
    std::string setup_path = home_dir + setup_defines_path;
    
    Publisher myPub;
    myPub.init(p_id, cfg_path, setup_path, img);

    // -----------------------------------------------------------------------------------------------------------------

    // create ip map for connection manager
    cc_linkmonitor::ConnectionManager::ip_map map;
    map.insert(std::make_pair("192.168.2.102", "192.168.20.6"));
    map.insert(std::make_pair("192.168.2.103", "192.168.30.6"));
    
    std::function<void(std::string, int)> writer_callback;
    writer_callback = myPub.writer_callback();

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

    cc_linkmonitor::ConnectionManager manager(
        map,
        writer_callback,
        link_reader.link_change(),
        link_reader.link_status()
    );

    // Init link reader and connection mananger. Note: init_heartbeat is a blocking call, waits for initial hb reception
    cc_linkmonitor::linkMessage temp;
    udp::endpoint link = link_reader.init_heartbeat(temp);
    manager.init(link.address().to_string());

    /* -------------------------------- QT Config ------------------------------- */
    QApplication app(argc, argv);

    LinkMonitorWidget monitor;
    monitor.setLinkReader(&link_reader);
    monitor.setConnectionManager(&manager);
    monitor.show();

    std::thread threadPub([&myPub]() { myPub.run(); });
    app.exec();

    threadPub.join();

    link_reader.stop();

    return 0;
}
