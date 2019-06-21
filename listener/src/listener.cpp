#include <ros/ros.h>
#include "listener/echomsg.h"
#include "talker/echomsg.h"
class echosub {
    private:
        ros::NodeHandle nh;
        ros::Subscriber subscriber;
        ros::Publisher publisher;
        listener::echomsg msg;
    public:
        echosub(){
            msg.sub_to_pub=0;
            subscriber=nh.subscribe("pub_to_sub",100,&echosub::Callback,this);
            publisher=nh.advertise<listener::echomsg>("sub_to_pub",100);
        }
        void Callback(const talker::echomsgConstPtr& ptr){
            msg.sub_to_pub++;   
            msg.pub_to_sub=ptr->pub_to_sub;
            msg.data=ptr->data;
            ROS_INFO("msg arrived! pub_to_sub %d",ptr->pub_to_sub);
            publisher.publish(msg);
        }
        /*void pub()
        {
            publisher.publish(msg);
            loop_rate.sleep();
        }*/
};

int main(int argc, char* argv[]){
    ros::init(argc,argv,"listener");
    echosub eh;
    ros::spin();
}
