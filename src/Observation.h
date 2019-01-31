#ifndef __OBS_H_ 
#define __OBS_H_

#include <string>
#include <vector>
#include <fstream>
#include "ros/ros.h"
using namespace std;

class Observation{
public:
	Observation();
	//Observation(int left_f,int left_s, int right_s, int right_f);
	Observation(int left_1,int left_2, int left_3, int left_4, int left_5, int right_1, int right_2, int right_3, int right_4, int right_5);

	//void setValues(int left_f,int left_s, int right_s, int right_f);
	void setValues(int left_1,int left_2, int left_3, int left_4, int left_5, int right_1, int right_2, int right_3, int right_4, int right_5);

	/*int lf;
	int ls;
	int rs;
	int rf;
	double log_lf;
	double log_ls;
	double log_rs;
	double log_rf;*/

	int l1,l2,l3,l4,l5;
	int r1,r2,r3,r4,r5;
	double log_l1,log_l2,log_l3,log_l4,log_l5;
	double log_r1,log_r2,log_r3,log_r4,log_r5;

	vector<double> ct_theta;	//[lf_theta, ls_theta, rs_theta, rf_theta]
	vector<double> ct_linear_x;
	vector<double> ct_linear_y;
	double centroid_x;
	double centroid_y;
};

#endif
