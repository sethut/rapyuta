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
            subscriber=nh.subscribe("pub_to_sub",100,&echosub::Callback,this);
            publisher=nh.advertise<std_msgs::Int32MultiArray>("sub_to_pub",100);
        }
        void Callback(const std_msgs::Int32MultiArray::ConstPtr& ptr){
            msg.data=ptr->data;
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
