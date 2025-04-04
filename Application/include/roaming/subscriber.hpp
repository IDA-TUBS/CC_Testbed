#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <w2rp/entities/reader.hpp>
#include <w2rp/config/readerConfig.hpp>
#include <w2rp/helper/safe_queue.hpp>

#include <roaming/img_config.hpp>
#include <roaming/misc.hpp>

#include <atomic>

class Subscriber
{
public:

    Subscriber():
        gui_active(false),
        logging_active(false),
        stopFlag(false),
        receiver{},
        logger{}
    {
        logInfo("[APP] App started")
    }

    Subscriber(bool gui, bool img_log):
        gui_active(gui),
        logging_active(img_log),
        stopFlag(false),
        receiver{},
        logger{}
    {
        logInfo("[APP] App started")
        if(img_log)
        {
            createLogDirectory();
        }
    }

    ~Subscriber()
    {}

    bool init(uint16_t participant_id ,std::string cfg, std::string setup, image_config img)
    {
        w2rp::config::readerCfg r_config("READER_01", cfg, setup);
        image = img;

        reader = new w2rp::Reader(participant_id, r_config);
        return true;
    }

    void run()
    {    
        receiver = std::thread(&Subscriber::receive_sample, this);
        if(logging_active)
        {
            logger = std::thread(&Subscriber::save_sample, this);
        }
    }

    cv::Mat get_frame()
    {
        if(!visualizer_queue.empty())
        {
            return visualizer_queue.dequeue();
        }
        else
        {
            return cv::Mat{};
        }
    }

    void stop()
    {
        stopFlag.store(true);
        join();
    }

private:

    w2rp::Reader* reader;
    bool gui_active;
    bool logging_active;
    
    std::atomic<bool> stopFlag;

    image_config image;

    SafeQueue<cv::Mat> visualizer_queue;
    SafeQueue<std::pair<std::string, cv::Mat>> logger_queue;

    std::thread receiver;
    std::thread logger;

    void receive_sample()
    {    
        logInfo("[APP] Subscriber running.")

        w2rp::Reader::sample sample;
        uint64_t sequence_num = 0;
        w2rp::SerializedPayload payload;
        int imageCounter = 0;

        while(!stopFlag.load())
        {
            reader->retrieveSample(sample);
            // logInfo("\n----------------------------------------------------------------------------------------\n[APP] Received sample\n---------------------------------------------------------\n");

            sequence_num = sample.first;
            payload = sample.second;

            unsigned char* imgData = new unsigned char[payload.length];
            memcpy(imgData, payload.data, payload.length);

            // Create a cv::Mat from the unsigned char array
            cv::Mat img(image.rows, image.columns, CV_8UC1, imgData);

            if(gui_active)
            {
                visualizer_queue.enqueue(img);
            }
            if(logging_active)
            {
                std::string filename = "sample_" + std::to_string(sequence_num) + ".png";
                logger_queue.enqueue(std::make_pair(filename, img));
            }
        }   
    }

    void save_sample()
    {
        std::pair<std::string, cv::Mat> item;
        while(!stopFlag.load())
        {
            item = logger_queue.dequeue();
                
            // Save the image to disk
            std::vector<int> compression_params;
            compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
            compression_params.push_back(3);  // Adjust compression level (0-9) for performance

            std::string file_path = home_dir + '/' + log_dir + item.first;
            cv::imwrite(file_path, item.second, compression_params);
        }
    }

    void join()
    {
        receiver.join();
        if(logging_active)
        {
            logger.join();
        }
    }

};

#endif //SUBSCRIBER_H