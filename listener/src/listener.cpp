#include <ros/ros.h>
#include <std_msgs/Int32MultiArray.h>
class echosub {
    private:
        ros::NodeHandle nh;
        ros::Subscriber subscriber;
        ros::Publisher publisher;
        std_msgs::Int32MultiArray msg;
    public:
        echosub(){
            subscriber=nh.subscribe("pub_to_sub",1,&echosub::Callback,this);
            publisher=nh.advertise<std_msgs::Int32MultiArray>("sub_to_pub",1);
        }
        void Callback(const std_msgs::Int32MultiArray::ConstPtr& ptr){
            msg.data=ptr->data;
            msg.layout.data_offset=ptr->layout.data_offset;
            ROS_INFO("msg arrived!");
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

