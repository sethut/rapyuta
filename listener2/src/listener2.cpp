#include <ros/ros.h>
#include "talker/echomsg.h"
#include <vector>
#include <signal.h>

std::vector<double> time_arr;
void callback(const talker::echomsgConstPtr& ptr){
        double end=ros::Time::now().toSec();
        double result_time=end-ptr->delay;
        double talker_to_listener = ptr->listenertime - ptr->delay;
        double listener_to_listener2 = end - ptr->listenertime;
        ROS_INFO("CB msg size : %ld byte time : %f count : %d",
        4*ptr->data.size()+8,result_time,ptr->count);
        ROS_INFO("talker to listener : %f, listener to listener2 : %f",talker_to_listener,listener_to_listener2);
        time_arr.push_back(result_time);
}
void time_result(){
    double sum,avg=0;
    int count=time_arr.size();
    double min= ros::Time::now().toSec();
    double max = time_arr.front();
    for(std::vector<double>::iterator it=time_arr.begin(); it!=time_arr.end(); it++){
        if(*it>max) max=*it;
        else if(*it<min) min=*it;
        sum+=*it;
    }
    avg=sum/count;
    ROS_INFO("msg count : %d",count);
    ROS_INFO("min time : %f",min);
    ROS_INFO("avg time : %f",avg);
    ROS_INFO("max time : %f",max);
}
void SIGINT_callback(int signum){
    time_result();
    exit(signum);
}

int main(int argc,char*argv[]){
    ros::init(argc,argv,"listener2");
    ros::NodeHandle nh;
    ros::Subscriber subscriber = nh.subscribe("sub_to_sub",100,callback);
    signal(SIGINT,SIGINT_callback);
    while(ros::ok())
        ros::spinOnce();
    return 0;
}