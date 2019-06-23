#include <ros/ros.h>
#include "talker/echomsg.h"
#include <vector>
#include <signal.h>

double end;
double result_time;
std::vector<double> time_arr;

void callback(const talker::echomsgConstPtr& ptr){
        end=ros::Time::now().toSec();
        result_time=end-ptr->delay;
        ROS_INFO("CB msg size : %ld time : %f count : %d",
        4*ptr->data.size()+8,result_time,ptr->count);
        time_arr.push_back(result_time);
}
void time_result(){
    double min,max=time_arr.front();
    double sum,avg=0;
    int count=time_arr.size();
        for(std::vector<double>::iterator it=time_arr.begin(); it!=time_arr.end(); it++){
            if(*it>max) max=*it;
            else if(*it<min) min=*it;
            sum+=*it;
        }
        avg=sum/count;
        ROS_INFO("msg count : %d",count);
        ROS_INFO("avg time : %f",avg);
        ROS_INFO("max time : %f",max);
        ROS_INFO("min time : %f",min);
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

    while(ros::ok()){
        ros::spin();
    }
    return 0;
}