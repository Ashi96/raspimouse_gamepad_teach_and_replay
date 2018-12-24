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
	Observation(double left_f,double left_s, double right_s, double right_f);

	void setValues(double left_f,double left_s, double right_s, double right_f);

	int lf;
	int ls;
	int rs;
	int rf;
/*	double log_lf;
	double log_ls;
	double log_rs;
	double log_rf; */

	vector<double> ct_theta;	//[lf_theta, ls_theta, rs_theta, rf_theta]
	vector<double> ct_linear_x;
	vector<double> ct_linear_y;
	double centroid_x;
	double centroid_y;
};

#endif
