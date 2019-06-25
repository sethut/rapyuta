#include <ros/ros.h>
#include "talker/echomsg.h"
#include <vector>
int pubcount=0;
    
class echopub{
    private://for check taken time 
        double end;
        double time;
    private:
        ros::NodeHandle nh;
        ros::Publisher publisher;
        ros::Subscriber subscriber;
        talker::echomsg msg;
    public: 
        echopub(int size){
            for(int i=0; i<size; i++)
                msg.data.push_back(1); //for various data size
            msg.count=0;
            msg.listenertime=0;
            publisher=nh.advertise<talker::echomsg>("pub_to_sub",100);
        }
        int pub(){
            msg.delay=ros::Time::now().toSec();
            msg.count=pubcount++;
            publisher.publish(msg);
        }
};
int main(int argc, char* argv[]){
    if(argc!=5){
        ROS_ERROR("input msg_size test_time");
        exit(argc);
    }
    int msg_size=atoi(argv[1]);
    int test_time=atoi(argv[2]);   
    ros::init(argc,argv,"talker");
    echopub talker(msg_size);
    ros::Rate loop_rate(10);
    while(pubcount<=test_time){
        loop_rate.sleep(); 
        talker.pub();
    }
    return 0;
}