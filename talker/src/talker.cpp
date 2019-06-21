#include <ros/ros.h>
#include <std_msgs/Int32MultiArray.h>
#include <vector>
#include <queue>
#include <ctime>
class echopub{
    private://for check taken time 
        double start;
        double end;
        double time;//taken time
        int count;
        std::vector<double>time_arr; //save taken time
        std::vector<double>time_start;
        //std::queue<double>time_start;
    private:
        ros::NodeHandle nh;
        ros::Publisher publisher;
        ros::Subscriber subscriber;
        std_msgs::Int32MultiArray msg;
    public:
        echopub(int size){
            for(int i=0; i<size/4; i++)
                msg.data.push_back(1); //resize msg for various data size
            msg.layout.data_offset=0;
            publisher=nh.advertise<std_msgs::Int32MultiArray>("pub_to_sub",1);
            subscriber=nh.subscribe("sub_to_pub",1,&echopub::echocallback,this);
        }
        void pub(){ 
            //ROS_INFO("msg size : %ldbyte  pub_to_sub : %d",
            //4*msg.data.size()+8,count,msg.pub_to_sub);;
            msg.layout.data_offset++;
            time_start.push_back(ros::Time::now().toSec());
            publisher.publish(msg);
        }
        void echocallback(const std_msgs::Int32MultiArrayConstPtr& ptr){         
            end=ros::Time::now().toSec();
            time=end-time_start[ptr->layout.data_offset];
            time_arr.push_back(time);
            ROS_INFO("CB msg size : %ldbyte  time : %f count : %ld",4*msg.data.size(),time, time_arr.size());
        }
        void time_result(){
            double sum,avg_time=0;
            double max,min=time_arr.front();
            int count=time_arr.size();
            for(std::vector<double>::iterator it=time_arr.begin(); it!=time_arr.end(); ++it){
                if(*it>max) max=*it;
                else if(*it<min) min=*it;
                sum+=*it;
            }
            avg_time = sum/count;
            ROS_INFO("msg count : %d",count);
            ROS_INFO("avg time : %f",avg_time);
            ROS_INFO("max time : %f",max);
            ROS_INFO("min time : %f",min);
        }
};


int main(int argc, char* argv[]){
    if(argc!=3){
        ROS_ERROR("input msg_size test_time");
    }
    msg_size=atoi(argv[1]);
    test_time=atoi(argv[2]);   
    ros::init(argc,argv,"talker");
    echopub epub(msg_size);
    ros::Rate loop_rate(10);

    for(int i=0; i<test_time; i++)    {
        epub.pub();
        ros::spinOnce();        
        loop_rate.sleep();
    }
    epub.time_result();
    return 0;
}



