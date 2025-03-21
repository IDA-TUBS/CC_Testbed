#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <w2rp/entities/writer.hpp>
#include <w2rp/config/writerConfig.hpp>

#include <opencv2/opencv.hpp>

#include <roaming/img_config.hpp>


class Publisher
{
    public:

        Publisher()
        {}

        virtual ~Publisher()
        {}

        bool init(uint16_t participant_id ,std::string cfg, std::string setup, image_config img)
        {
            counterRow = 0;

            w2rp::config::writerCfg w_config("WRITER_01", cfg, setup);
            image = img;

            writer = new w2rp::Writer(participant_id, w_config);
            return true;
        }

        void run()
        {    
            std::thread thread(&Publisher::runThread, this);
            
            thread.join();
        }


        bool publish()
        {
            cv::Mat img(image.rows, image.columns, CV_8UC1 , cv::Scalar(0));       
            cv::Scalar line_Color(255);//Color of the line
            cv::Point starting(0, counterRow);//Starting Point of the line
            cv::Point ending(image.columns, counterRow);//Ending Point of the line
            line(img, starting, ending, line_Color, 3);//using line() function to draw the line

            size_t imgSize = img.total() * img.elemSize();
            // logDebug("[APP] sending image of size " << imgSize)

            // Allocate memory for the unsigned char array
            unsigned char* imgData = new unsigned char[imgSize];
            memcpy(imgData, img.data, imgSize);


            counterRow++;
            if(counterRow == (image.rows - 3))
            {
                counterRow = 0;
            }
            
            w2rp::SerializedPayload *payload = new w2rp::SerializedPayload(imgData, imgSize);
            // if(writer->write(payload))
            if(writer->write(payload))
            {
                // sample transmitted successfully
                // cv::imshow("Sent Image", img);
                // cv::waitKey(1); // Wait for a key press to close the window
                return true;
            }
            else
            {
                // problem occured
                return false;
            }
        }

        std::function<void(std::string, int)> writer_callback()
        {
            return writer->get_callback();
        }


    private:

        w2rp::Writer* writer;

        int64_t period;
        bool stop_send;

        uint32_t counterRow;

        image_config image;

        void runThread()
        {    
            logInfo("\n[APP] Publisher running.")
            uint32_t i = 0;
            while(true)
            {
                // logInfo("\n----------------------------------------------------------------------------------------\n[APP] - Sending sample with index: " << i << "\n----------------------------------------------------------------------------------------\n")
                if (!publish())
                {
                    i++;
                }
                else
                {
                    // sending worked?!
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                // logInfo(std::endl << std::endl)
            }
        }
};

#endif //PUBLISHER_H