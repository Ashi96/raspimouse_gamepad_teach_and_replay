#include "ros/ros.h"
#include "Event.h"
#include "Episodes.h"
#include "ParticleFilter.h"
#include <iostream>
#include <fstream>
#include <signal.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <ros/package.h> 
#include "std_srvs/Trigger.h"
#include "geometry_msgs/Twist.h"
#include "raspimouse_ros_2/LightSensorValues.h"
#include "raspimouse_ros_2/TimedMotion.h"
#include "raspimouse_maze/Decision.h"
#include "ParticleFilter.h"
//#include "raspimouse_gamepad_training_replay/PFoEOutput.h"
using namespace ros;

Episodes ep;
ParticleFilter pf(1000,&ep);

Observation sensor_values;

NodeHandle *np;
int sum_forward = 0;

void sensorCallback(const raspimouse_ros_2::LightSensorValues::ConstPtr& msg)
{
	sensor_values.setValues(msg->left_forward,msg->left_side,msg->right_side,msg->right_forward);
	sum_forward = msg->sum_forward;
}

void on_shutdown(int sig)
{
	ros::ServiceClient motor_off = np->serviceClient<std_srvs::Trigger>("motor_off");
	std_srvs::Trigger t;
	motor_off.call(t);

	shutdown();
}

bool forward(raspimouse_ros_2::TimedMotion *d)
{
	int side_diff = sensor_values.ls - sensor_values.rs;
	int diff_hz = side_diff/10; //差の1/10

	int fw_hz = (2500 - sum_forward)/10;

	d->request.left_hz = fw_hz + diff_hz;
	d->request.right_hz = fw_hz - diff_hz;
	d->request.duration_ms = 100;
	return true;
}

void readEpisodes(string file)
{
	string path = ros::package::getPath("raspimouse_gamepad_training_replay");
	rosbag::Bag bag1(path + "/bags/" + file, rosbag::bagmode::Read);

	vector<std::string> topics;
	topics.push_back("/decision");
	
	rosbag::View view(bag1, rosbag::TopicQuery(topics));

	double end = view.getEndTime().toSec() - 5.0; //discard last 5 sec
	for(auto i : view){
	        auto s = i.instantiate<raspimouse_maze::Decision>();

		Observation obs(s->left_forward,s->left_side,s->right_side,s->right_forward);
		Action a = {s->linear_x,s->angular_z};
		Event e(obs,a,0.0);
		e.time = i.getTime();
		ep.append(e);
		if(e.time.toSec() > end)
			break;
	}
}

int main(int argc, char **argv)
{
	init(argc,argv,"go_around");
	NodeHandle n;
	np = &n;

	Subscriber sub = n.subscribe("lightsensors", 1, sensorCallback);
	Publisher cmdvel = n.advertise<geometry_msgs::Twist>("cmd_vel", 1);
	Publisher pfoe_out = n.advertise<raspimouse_gamepad_training_replay::PFoEOutput>("pfoe_out", 100);
	ros::ServiceClient motor_on = n.serviceClient<std_srvs::Trigger>("motor_on");
	ros::ServiceClient tm = n.serviceClient<raspimouse_ros_2::TimedMotion>("timed_motion");

	signal(SIGINT, on_shutdown);

	motor_on.waitForExistence();
	std_srvs::Trigger t;
	motor_on.call(t);

	//for(int i=1;i<6;i++){
		string f = "layout_1.bag";
		cout << f << endl;
		readEpisodes(f);
//	}
	//ep.renewEpisode();
	//ep.print("episode20170613_am");

	geometry_msgs::Twist msg;
	pf.init();
	Rate loop_rate(10);
	Action act = {0.0,0.0};
	while(ok()){
		raspimouse_gamepad_training_replay::PFoEOutput out;

		//Action act = pf.sensorUpdate(&sensor_values, &ep, &out);
		act = pf.sensorUpdate(&sensor_values, &act, &ep, &out);
		msg.linear.x = act.linear_x;
		out.linear_x = act.linear_x;
		msg.angular.z = act.angular_z;
		out.angular_z = act.angular_z;

		out.left_forward = sensor_values.lf;
		out.left_side = sensor_values.ls;
		out.right_forward = sensor_values.rf;
		out.right_side = sensor_values.rs;

		cmdvel.publish(msg);
		pfoe_out.publish(out);
		pf.motionUpdate(&ep);

		spinOnce();
		loop_rate.sleep();
	}
	return 0;
}
