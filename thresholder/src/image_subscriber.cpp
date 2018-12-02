#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
    cv::Mat img_bgr, img_hsv; //Variables to store images in different color spaces
    cv::Mat red1, red2, green, blue;
    img_bgr = cv_bridge::toCvShare(msg, "bgr8")->image; //Store original image (in BGR)
    cv::cvtColor(img_bgr, img_hsv, CV_RGB2HSV); //Convert image from BGR to HSV
    cv::imshow("view", img_bgr); //Show BRG image

    //Threshold red colors
    cv::inRange(img_hsv, cv::Scalar(0, 128, 102), cv::Scalar(20, 255, 255), red1);
    cv::inRange(img_hsv, cv::Scalar(155, 128, 102), cv::Scalar(180, 255, 255), red2);
    red1 = red1+red2; //Add the two pictures
    cv::imshow("red", red1); //Show red colors

    //Threshold green colors
    cv::inRange(img_hsv, cv::Scalar(35, 128, 102), cv::Scalar(75, 255, 255), green);
    cv::imshow("green", green); //Show green colors

    //Threshold blue colors
    cv::inRange(img_hsv, cv::Scalar(80, 128, 102), cv::Scalar(130, 255, 255), blue);
    cv::imshow("blue", blue); //Show blue colors
    cv::waitKey(30);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "image_listener");
  ros::NodeHandle nh;
  cv::namedWindow("view"); cv::namedWindow("red"); cv::namedWindow("green"); cv::namedWindow("blue");
  cv::startWindowThread();
  image_transport::ImageTransport it(nh);
  //image_transport::Subscriber sub = it.subscribe("camera/image", 1, imageCallback);
  image_transport::Subscriber sub = it.subscribe("/webcam/image_raw", 1, imageCallback); //TOPIC DE LA CAM DEL PORTATIL -> CAMBIAR POR EL DEL TURTLEBOT
  ros::spin();
  cv::destroyWindow("view"); cv::destroyWindow("red"); cv::destroyWindow("green"); cv::destroyWindow("blue"); 
}
