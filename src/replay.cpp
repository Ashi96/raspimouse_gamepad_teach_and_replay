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
#include <vector>
#include "std_srvs/Trigger.h"
#include "geometry_msgs/Twist.h"
#include "raspimouse_ros_2/LightSensorValues.h"
#include "raspimouse_ros_2/TimedMotion.h"
#include "raspimouse_ros_2/ButtonValues.h"
#include "sensor_msgs/LaserScan.h"
#include "raspimouse_gamepad_teach_and_replay/Event.h"
#include "ParticleFilter.h"
#include "raspimouse_gamepad_teach_and_replay/PFoEOutput.h"
using namespace ros;

Episodes ep;
ParticleFilter pf(1000, &ep);

Observation sensor_values;
std::vector<float> left_sensor_value = {3, 15, 20, 30, 40, 45, 60, 75, 90, 120};
std::vector<float> right_sensor_value = {3, 15, 20, 30, 40, 45, 60, 75, 90, 120};

NodeHandle *np;
int sum_forward = 0;

bool on = false;
bool bag_read = false;

const double pi = 3.141592;

void buttonCallback(const raspimouse_ros_2::ButtonValues::ConstPtr &msg)
{
	on = msg->mid_toggle;
}

void sensorCallback(const sensor_msgs::LaserScan::ConstPtr &msg)
{
	/*int lf = (pi * 3.0 / 180 - msg->angle_min) / msg->angle_increment;
	int rf = (-pi * 3.0 / 180 - msg->angle_min) / msg->angle_increment;
	int ls = (pi * 45.0 / 180 - msg->angle_min) / msg->angle_increment;
	int rs = (-pi * 45.0 / 180 - msg->angle_min) / msg->angle_increment;

	double lfv = isnan(msg->ranges[lf]) ? 500.0 : msg->ranges[lf] * 1000;
	double rfv = isnan(msg->ranges[rf]) ? 500.0 : msg->ranges[rf] * 1000;
	double rsv = isnan(msg->ranges[rs]) ? 500.0 : msg->ranges[rs] * 1000;
	double lsv = isnan(msg->ranges[ls]) ? 500.0 : msg->ranges[ls] * 1000;

	sensor_values.setValues(lfv, lsv, rsv, rfv);*/
	/*int l1 = (pi * 3.0 / 180 - msg->angle_min) / msg->angle_increment;
	int l2 = (pi * 15.0 / 180 - msg->angle_min) / msg->angle_increment;
	int l3 = (pi * 30.0 / 180 - msg->angle_min) / msg->angle_increment;
	int l4 = (pi * 45.0 / 180 - msg->angle_min) / msg->angle_increment;
	int l5 = (pi * 60.0 / 180 - msg->angle_min) / msg->angle_increment;
	int r1 = (-pi * 3.0 / 180 - msg->angle_min) / msg->angle_increment;
	int r2 = (-pi * 15.0 / 180 - msg->angle_min) / msg->angle_increment;
	int r3 = (-pi * 30.0 / 180 - msg->angle_min) / msg->angle_increment;
	int r4 = (-pi * 45.0 / 180 - msg->angle_min) / msg->angle_increment;
	int r5 = (-pi * 60.0 / 180 - msg->angle_min) / msg->angle_increment;

	double l1v = isnan(msg->ranges[l1]) ? 500.0 : msg->ranges[l1] * 1000;
	double l2v = isnan(msg->ranges[l2]) ? 500.0 : msg->ranges[l2] * 1000;
	double l3v = isnan(msg->ranges[l3]) ? 500.0 : msg->ranges[l3] * 1000;
	double l4v = isnan(msg->ranges[l4]) ? 500.0 : msg->ranges[l4] * 1000;
	double l5v = isnan(msg->ranges[l5]) ? 500.0 : msg->ranges[l5] * 1000;
	double r1v = isnan(msg->ranges[r1]) ? 500.0 : msg->ranges[r1] * 1000;
	double r2v = isnan(msg->ranges[r2]) ? 500.0 : msg->ranges[r2] * 1000;
	double r3v = isnan(msg->ranges[r3]) ? 500.0 : msg->ranges[r3] * 1000;
	double r4v = isnan(msg->ranges[r4]) ? 500.0 : msg->ranges[r4] * 1000;
	double r5v = isnan(msg->ranges[r5]) ? 500.0 : msg->ranges[r5] * 1000;*/

	/*int l1 = (pi * 3.0 / 180 - msg->angle_min) / msg->angle_increment;
	int l2 = (pi * 15.0 / 180 - msg->angle_min) / msg->angle_increment;
	int l3 = (pi * 20.0 / 180 - msg->angle_min) / msg->angle_increment;
	int l4 = (pi * 30.0 / 180 - msg->angle_min) / msg->angle_increment;
	int l5 = (pi * 40.0 / 180 - msg->angle_min) / msg->angle_increment;
	int l6 = (pi * 45.0 / 180 - msg->angle_min) / msg->angle_increment;
	int l7 = (pi * 60.0 / 180 - msg->angle_min) / msg->angle_increment;
	int l8 = (pi * 75.0 / 180 - msg->angle_min) / msg->angle_increment;
	int l9 = (pi * 90.0 / 180 - msg->angle_min) / msg->angle_increment;
	int l10 = (pi * 120.0 / 180 - msg->angle_min) / msg->angle_increment;
	int r1 = (-pi * 3.0 / 180 - msg->angle_min) / msg->angle_increment;
	int r2 = (-pi * 15.0 / 180 - msg->angle_min) / msg->angle_increment;
	int r3 = (-pi * 20.0 / 180 - msg->angle_min) / msg->angle_increment;
	int r4 = (-pi * 30.0 / 180 - msg->angle_min) / msg->angle_increment;
	int r5 = (-pi * 40.0 / 180 - msg->angle_min) / msg->angle_increment;
	int r6 = (-pi * 45.0 / 180 - msg->angle_min) / msg->angle_increment;
	int r7 = (-pi * 60.0 / 180 - msg->angle_min) / msg->angle_increment;
	int r8 = (-pi * 75.0 / 180 - msg->angle_min) / msg->angle_increment;
	int r9 = (-pi * 90.0 / 180 - msg->angle_min) / msg->angle_increment;
	int r10 = (-pi * 120.0 / 180 - msg->angle_min) / msg->angle_increment;

	double l1v = isnan(msg->ranges[l1]) ? 4100.0 : msg->ranges[l1] * 1000;
	double l2v = isnan(msg->ranges[l2]) ? 4100.0 : msg->ranges[l2] * 1000;
	double l3v = isnan(msg->ranges[l3]) ? 4100.0 : msg->ranges[l3] * 1000;
	double l4v = isnan(msg->ranges[l4]) ? 4100.0 : msg->ranges[l4] * 1000;
	double l5v = isnan(msg->ranges[l5]) ? 4100.0 : msg->ranges[l5] * 1000;
	double l6v = isnan(msg->ranges[l6]) ? 4100.0 : msg->ranges[l6] * 1000;
	double l7v = isnan(msg->ranges[l7]) ? 4100.0 : msg->ranges[l7] * 1000;
	double l8v = isnan(msg->ranges[l8]) ? 4100.0 : msg->ranges[l8] * 1000;
	double l9v = isnan(msg->ranges[l9]) ? 4100.0 : msg->ranges[l9] * 1000;
	double l10v = isnan(msg->ranges[l10]) ? 4100.0 : msg->ranges[l10] * 1000;
	double r1v = isnan(msg->ranges[r1]) ? 4100.0 : msg->ranges[r1] * 1000;
	double r2v = isnan(msg->ranges[r2]) ? 4100.0 : msg->ranges[r2] * 1000;
	double r3v = isnan(msg->ranges[r3]) ? 4100.0 : msg->ranges[r3] * 1000;
	double r4v = isnan(msg->ranges[r4]) ? 4100.0 : msg->ranges[r4] * 1000;
	double r5v = isnan(msg->ranges[r5]) ? 4100.0 : msg->ranges[r5] * 1000;
	double r6v = isnan(msg->ranges[r6]) ? 4100.0 : msg->ranges[r6] * 1000;
	double r7v = isnan(msg->ranges[r7]) ? 4100.0 : msg->ranges[r7] * 1000;
	double r8v = isnan(msg->ranges[r8]) ? 4100.0 : msg->ranges[r8] * 1000;
	double r9v = isnan(msg->ranges[r9]) ? 4100.0 : msg->ranges[r9] * 1000;
	double r10v = isnan(msg->ranges[r10]) ? 4100.0 : msg->ranges[r10] * 1000;*/

	std::vector<int> urg_sensor_left;
	std::vector<int> urg_sensor_right;
	for (unsigned int i = 0; i < left_sensor_value.size(); i++)
	{
		int l = (pi * left_sensor_value[i] / 180 - msg->angle_min) / msg->angle_increment;
		double lv = isnan(msg->ranges[l]) ? 4100.0 : msg->ranges[l] * 1000;
		urg_sensor_left.push_back(lv);
	}
	for (unsigned int i = 0; i < right_sensor_value.size(); i++)
	{
		int r = (pi * right_sensor_value[i] / 180 - msg->angle_min) / msg->angle_increment;
		double rv = isnan(msg->ranges[r]) ? 4100.0 : msg->ranges[r] * 1000;
		urg_sensor_right.push_back(rv);
	}
	//sensor_values.setValues(l1v, l2v, l3v, l4v, l5v, r1v, r2v, r3v, r4v, r5v);
	//sensor_values.setValues(l1v, l2v, l3v, l4v, l5v, l6v, l7v, l8v, l9v, l10v, r1v, r2v, r3v, r4v, r5v, r6v, r7v, r8v, r9v, r10v);
	sensor_values.setValues(urg_sensor_left, urg_sensor_right);
}

void on_shutdown(int sig)
{
	ros::ServiceClient motor_off = np->serviceClient<std_srvs::Trigger>("motor_off");
	std_srvs::Trigger t;
	motor_off.call(t);

	shutdown();
}

void readEpisodes(string file)
{
	ep.reset();

	rosbag::Bag bag1(file, rosbag::bagmode::Read);

	vector<std::string> topics;
	topics.push_back("/event");

	rosbag::View view(bag1, rosbag::TopicQuery(topics));

	double start = view.getBeginTime().toSec() + 5.0; //discard first 5 sec
	double end = view.getEndTime().toSec() - 5.0;	 //discard last 5 sec
	for (auto i : view)
	{
		auto s = i.instantiate<raspimouse_gamepad_teach_and_replay::Event>();
		//Observation obs(s->left_forward, s->left_side, s->right_side, s->right_forward);
		//Observation obs(s->left_1, s->left_2, s->left_3, s->left_4, s->left_5, s->left_6, s->left_7, s->left_8, s->left_9, s->left_10, s->right_1, s->right_2, s->right_3, s->right_4, s->right_5, s->right_6, s->right_7, s->right_8, s->right_9, s->right_10);
		std::vector<int> left_sensor(10, 0), right_sensor(10, 0);
		//cout << "left_sensor_value.size()" << left_sensor_value.size() << endl;
		//cout << "right_sensor_value.size()" << right_sensor_value.size() << endl;
		// left_sensor.reserve(left_sensor_value.size());
		// right_sensor.reserve(right_sensor_value.size());
		/*
		left_sensor[0] = s->left_1;
		left_sensor[1] = s->left_2;
		left_sensor[2] = s->left_3;
		left_sensor[3] = s->left_4;
		left_sensor[4] = s->left_5;
		left_sensor[5] = s->left_6;
		left_sensor[6] = s->left_7;
		left_sensor[7] = s->left_8;
		left_sensor[8] = s->left_9;
		left_sensor[9] = s->left_10;
		right_sensor[0] = s->right_1;
		right_sensor[1] = s->right_2;
		right_sensor[2] = s->right_3;
		right_sensor[3] = s->right_4;
		right_sensor[4] = s->right_5;
		right_sensor[5] = s->right_6;
		right_sensor[6] = s->right_7;
		right_sensor[7] = s->right_8;
		right_sensor[8] = s->right_9;
		right_sensor[9] = s->right_10;*/
		for (unsigned int i = 0; i < left_sensor.size(); i++)
		{
			left_sensor[i] = s->left[i];
		}
		for (unsigned int i = 0; i < right_sensor.size(); i++)
		{
			right_sensor[i] = s->right[i];
		}

		//std::cout << "left_sensor.size() :" << left_sensor.size() << std::endl;
		Observation obs(left_sensor, right_sensor);
		//cout << "obs.l.size() :" << obs.l.size() << endl;
		//cout << "obs.log_l.size() :" << obs.log_l.size() << endl;

		Action a = {s->linear_x, s->angular_z};
		Event e(obs, a, 0.0);
		e.time = i.getTime();

		if (e.time.toSec() < start)
			continue;

		std::cout << "e.observation.l.size():" << e.observation.l.size() << std::endl;
		ep.append(e);

		if (e.time.toSec() > end)
			break;
	}
}

int main(int argc, char **argv)
{
	//std::cout << "start" << endl;
	init(argc, argv, "go_around");
	NodeHandle n;
	np = &n;

	//Subscriber sub = n.subscribe("lightsensors", 1, sensorCallback);
	Subscriber sub = n.subscribe("/scan", 1, sensorCallback);
	Subscriber sub_b = n.subscribe("buttons", 1, buttonCallback);
	Publisher cmdvel = n.advertise<geometry_msgs::Twist>("cmd_vel", 1);
	Publisher pfoe_out = n.advertise<raspimouse_gamepad_teach_and_replay::PFoEOutput>("pfoe_out", 100);
	ros::ServiceClient motor_on = n.serviceClient<std_srvs::Trigger>("motor_on");
	ros::ServiceClient tm = n.serviceClient<raspimouse_ros_2::TimedMotion>("timed_motion");

	signal(SIGINT, on_shutdown);

	motor_on.waitForExistence();
	std_srvs::Trigger t;
	motor_on.call(t);

	geometry_msgs::Twist msg;
	//	pf.init();
	Rate loop_rate(10);
	Action act = {0.0, 0.0};
	while (ok())
	{
		if (not on)
		{
			cout << "idle" << endl;
			bag_read = false;
			spinOnce();
			loop_rate.sleep();
			continue;
		}
		else if (not bag_read)
		{
			string bagfile;
			n.getParam("/current_bag_file", bagfile);
			readEpisodes(bagfile);
			bag_read = true;
			pf.init();
			bag_read = true;
			spinOnce();

			ep.coordinatetransformation();

			loop_rate.sleep();
			continue;
		}
		raspimouse_gamepad_teach_and_replay::PFoEOutput out;

		act = pf.sensorUpdate(&sensor_values, &act, &ep, &out);
		msg.linear.x = act.linear_x;
		out.linear_x = act.linear_x;
		msg.angular.z = act.angular_z;
		out.angular_z = act.angular_z;

		/*out.left_forward = sensor_values.lf;
		out.left_side = sensor_values.ls;
		out.right_forward = sensor_values.rf;
		out.right_side = sensor_values.rs;*/
		/*out.left_1 = sensor_values.l1;
		out.left_2 = sensor_values.l2;
		out.left_3 = sensor_values.l3;
		out.left_4 = sensor_values.l4;
		out.left_5 = sensor_values.l5;
		out.left_6 = sensor_values.l6;
		out.left_7 = sensor_values.l7;
		out.left_8 = sensor_values.l8;
		out.left_9 = sensor_values.l9;
		out.left_10 = sensor_values.l10;
		out.right_1 = sensor_values.r1;
		out.right_2 = sensor_values.r2;
		out.right_3 = sensor_values.r3;
		out.right_4 = sensor_values.r4;
		out.right_5 = sensor_values.r5;
		out.right_6 = sensor_values.r6;
		out.right_7 = sensor_values.r7;
		out.right_8 = sensor_values.r8;
		out.right_9 = sensor_values.r9;
		out.right_10 = sensor_values.r10;*/
		/*out.left_1 = sensor_values.l[0];
		out.left_2 = sensor_values.l[1];
		out.left_3 = sensor_values.l[2];
		out.left_4 = sensor_values.l[3];
		out.left_5 = sensor_values.l[4];
		out.left_6 = sensor_values.l[5];
		out.left_7 = sensor_values.l[6];
		out.left_8 = sensor_values.l[7];
		out.left_9 = sensor_values.l[8];
		out.left_10 = sensor_values.l[9];
		out.right_1 = sensor_values.r[0];
		out.right_2 = sensor_values.r[1];
		out.right_3 = sensor_values.r[2];
		out.right_4 = sensor_values.r[3];
		out.right_5 = sensor_values.r[4];
		out.right_6 = sensor_values.r[5];
		out.right_7 = sensor_values.r[6];
		out.right_8 = sensor_values.r[7];
		out.right_9 = sensor_values.r[8];
		out.right_10 = sensor_values.r[9];*/
		for (unsigned int i = 0; i < sensor_values.l.size(); i++)
		{
			out.left.push_back(sensor_values.l[i]);
		}
		for (unsigned int i = 0; i < sensor_values.r.size(); i++)
		{
			out.right.push_back(sensor_values.r[i]);
		}

		cmdvel.publish(msg);
		pfoe_out.publish(out);
		pf.motionUpdate(&ep);

		spinOnce();
		loop_rate.sleep();
	}
	return 0;
}
