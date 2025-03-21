#ifndef MISC_h
#define MISC_h

#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem>

/**
 * @brief relative path to downlink w2rp config
 * 
 */
std::string w2rp_cfg_down = "/CC_Testbed/Application/cfg/w2rp_config_down.json";

/**
 * @brief relative path to uplink w2rp config
 * 
 */
std::string w2rp_cfg_up = "/CC_Testbed/Application/cfg/w2rp_config_up.json";

/**
 * @brief relative path to setup defines (host ids)
 * 
 */
std::string setup_defines_path = "/CC_Testbed/Application/cfg/setup_defines.json";

/**
 * @brief Path to home directory
 * 
 */
std::string home_dir = std::string(getenv("HOME"));

/**
 * @brief path to log directory
 * 
 */
std::string log_dir = "RoamingDemonstrator/logs/";

/**
 * @brief filename prefix for writer logs
 * 
 */
std::string writer_prefix = "writer_";

/**
 * @brief filename prefix for reader logs
 * 
 */
std::string reader_prefix = "reader_";

/**
 * @brief generate file name suffix based on current ime
 * 
 * @return std::string 
 */
std::string generateTimestamp() {
    // Get the current time
    std::time_t now = std::time(nullptr);
    
    // Convert to a tm structure
    std::tm* localTime = std::localtime(&now);
    
    // Create a string stream to format the date and time
    std::stringstream ss;
    ss << std::put_time(localTime, "%Y%m%d_%H%M%S");

    // Return the formatted string
    return ss.str();
}

/**
 * @brief Create the Log Directory 
 * 
 * @return int 
 */
int createLogDirectory()
{
    std::filesystem::path dir = home_dir + '/' + log_dir;

    if (std::filesystem::create_directory(dir)) {
        logInfo("Log directory created successfully.")
    } else {
        logError("Failed to create log directory.")
    }
    return 0;
}

# endif