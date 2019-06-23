#include <ros/ros.h>
#include "talker/echomsg.h"
#include "listener/echomsg.h"
class echosub{
    private:
        ros::NodeHandle nh;
        ros::Subscriber subscriber;
        ros::Publisher publisher;
        listener::echomsg msg;
    public:
        echosub(){
            subscriber=nh.subscribe("pub_to_sub",100,&echosub::Callback,this);
            publisher=nh.advertise<talker::echomsg>("sub_to_sub",100);
        }
        void Callback(const talker::echomsgConstPtr& ptr){
            msg.data=ptr->data;
            msg.delay=ptr->delay;
            msg.count =ptr->count;
            ROS_INFO("msg arrived!");
            publisher.publish(msg);
        }
};
int main(int argc, char* argv[]){
    ros::init(argc,argv,"listener");
    echosub listener;
    while(ros::ok())
        ros::spinOnce();
    return 0;
}
