#include <ros/ros.h>
#include "talker/echomsg.h"
#include <vector>
#include <ctime>
class echopub{
    private://for check taken time 
        double start;
        double end;
        double time;//taken time
        int count;
        std::vector<double>time_arr; //save taken time
    private:
        ros::NodeHandle nh;
        ros::Publisher publisher;
        ros::Subscriber subscriber;
        talker::echomsg msg;
    public:
        echopub(int size){
            for(int i=0; i<size; i++)
                msg.data.push_back(1); //resize msg for various data size
            msg.pub_to_sub=0;
            msg.sub_to_pub=0;
            time_arr.clear();
            publisher=nh.advertise<talker::echomsg>("pub_to_sub",1);
            subscriber=nh.subscribe("sub_to_pub",100,&echopub::echocallback,this);
        }
        void pub(){
            msg.pub_to_sub++;
            //ROS_INFO("msg size : %ldbyte  pub_to_sub : %d",
            //4*msg.data.size()+8,count,msg.pub_to_sub);;
            start=ros::Time::now().toSec();
            publisher.publish(msg);
        }
        void echocallback(const talker::echomsgConstPtr& ptr){         
            end=ros::Time::now().toSec();    
            time=end-start;
            time_arr.push_back(time);
            count = ptr->sub_to_pub; //real counting for sub_to_pub msg
            ROS_INFO("CB msg size : %ldbyte  time : %f sub_to_pub : %d, %d",
            4*msg.data.size()+8,time,count,ptr->pub_to_sub);
        }
        void time_result(){
            double sum,avg_time=0;
            double max,min=time_arr.front();
            for(std::vector<double>::iterator it=time_arr.begin(); it!=time_arr.end(); ++it){
                if(*it>max)
                    max=*it;
                else if(*it<min)
                    min=*it;
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
    if(argc!=3&&argc!=5){
        ROS_ERROR("input msg_size test_time %s %s ",argv[1],argv[2]);
        exit(argc);
    }
    ros::init(argc,argv,"talker");
    int msg_size=atoi(argv[1]);
    int test_time=atoi(argv[2]);
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


