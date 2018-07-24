//
//  main.cpp
//  ekf_bug
//
//  Created by Shrinand  Thakkar on 7/23/18.
//  Copyright © 2018 Shrinand  Thakkar. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>

std::vector<cv::Mat> getData(int stateSize){
    double dl=0.1f;
    double theta=45;
    double vel=1.67f;
    double dt=1.0f/30.0f;
    
    std::vector<cv::Mat> data;
    int i=0;
    while (i<50){
        dl=dl+vel*dt*cos(theta);
        cv::Mat L = (cv::Mat_<float>(3, 1) << dl,theta,vel);
        data.push_back(L);
        i++;
    }
    return data;
}

std::vector<double> sense(std::vector<cv::Mat> data){
    std::vector<double> sensor_readings;
    for (int i=0;i<data.size();i++){
        sensor_readings.push_back(data[i].at<float>(0)+((double) rand() / (RAND_MAX)));
    }
    return sensor_readings;
}

std::vector<double> ground(std::vector<cv::Mat> data){
    std::vector<double> sensor_readings;
    for (int i=0;i<data.size();i++){
        sensor_readings.push_back(data[i].at<float>(0));
    }
    return sensor_readings;
}

int main(int argc, const char * argv[]) {
//     insert code here...
//    std::cout << "Hello, World!\n";
    int stateSize=3;
    int measSize=1;
    int contSize=0;
    double vel=1.67f;
    double dt=1.0f/30.0f;
    double theta=45;
    
    cv::KalmanFilter KF(stateSize,measSize,contSize,CV_32F);
    
    cv::Mat state(stateSize,1,CV_32F);
    cv::Mat meas(measSize,1,CV_32F);
    
    cv::Mat A(stateSize,stateSize,CV_32F);
    A.at<float>(2) = dt*cos(theta);
    cv::Mat H(measSize,measSize,CV_32F);
    
    cv::setIdentity(KF.transitionMatrix);
    KF.measurementMatrix=cv::Mat::zeros(measSize, stateSize, CV_32F);
    KF.measurementMatrix.at<float>(0)=1.0f;

    KF.processNoiseCov.at<float>(0)=0.000333f;
    KF.processNoiseCov.at<float>(1)=0.001015f;
    KF.processNoiseCov.at<float>(2)=0.008333f;
    
    cv::setIdentity(KF.measurementNoiseCov, cv::Scalar(0.05f));
    
    std::vector<cv::Mat> data = getData(stateSize);
    
//    std::cout<<data[4];
    
    std::vector<double> sensor_readings = sense(data);
    std::vector<double> ground_values = ground(data);
    std::vector<double> predicted_values;
//    std::cout<<sensor_readings[4];
 //    KF.transitionMatrix.at<float>(2) = dT;
    int i=1;
    state.at<float>(0)=sensor_readings[0];
    KF.statePost=state;
    while(i<50){
        cv::Mat temp=KF.statePost;
        KF.transitionMatrix.at<float>(2) = dt*cos(theta);
        KF.transitionMatrix.at<float>(1) = -1*vel*dt*sin(theta);
        state=KF.predict();
        KF.statePre=temp;
        KF.statePre.at<float>(0) = temp.at<float>(0)+vel*dt*cos(theta);
        meas.at<float>(0)=sensor_readings[i];
        KF.correct(meas);
        KF.temp5.at<float>(0)= meas.at<float>(0)-KF.statePre.at<float>(0);
        KF.statePost = KF.statePre+KF.gain*KF.temp5;
        predicted_values.push_back(KF.statePost.at<float>(0));
        i++;
    }
    std::cout<<"done";
    for (int i=0;i<predicted_values.size();i++){
        std::cout<<predicted_values[i]<<std::endl;
    }
//    std::ofstream myfilestream("text.txt");
//    std::copy(ground_values.begin(), ground_values.end(), std::ostream_iterator<double>(myfilestream, "\n"));
    return 0;
}
