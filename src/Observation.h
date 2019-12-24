#ifndef __OBS_H_
#define __OBS_H_

#include <string>
#include <vector>
#include <fstream>
#include "ros/ros.h"
using namespace std;

class Observation
{
  public:
	Observation();
	//Observation(int left_f,int left_s, int right_s, int right_f);
	//Observation(int left_1,int left_2, int left_3, int left_4, int left_5, int left_6, int left_7, int left_8, int left_9, int left_10, int right_1, int right_2, int right_3, int right_4, int right_5, int right_6, int right_7, int right_8, int right_9, int right_10);
	Observation(vector<int> &left, vector<int> &right);

	//void setValues(int left_f,int left_s, int right_s, int right_f);
	//void setValues(int left_1,int left_2, int left_3, int left_4, int left_5, int left_6, int left_7, int left_8, int left_9, int left_10, int right_1, int right_2, int right_3, int right_4, int right_5, int right_6, int right_7, int right_8, int right_9, int right_10);
	void setValues(vector<int> &left, vector<int> &right);

	/*int lf;
	int ls;
	int rs;
	int rf;
	double log_lf;
	double log_ls;
	double log_rs;
	double log_rf;*/

	/*int l1,l2,l3,l4,l5,l6,l7,l8,l9,l10;
	int r1,r2,r3,r4,r5,r6,r7,r8,r9,r10;
	double log_l1,log_l2,log_l3,log_l4,log_l5,log_l6,log_l7,log_l8,log_l9,log_l10;
	double log_r1,log_r2,log_r3,log_r4,log_r5,log_r6,log_r7,log_r8,log_r9,log_r10;*/

	vector<int> l;
	vector<int> r;
	vector<double> log_l;
	vector<double> log_r;

	vector<double> ct_theta; //[lf_theta, ls_theta, rs_theta, rf_theta]
	vector<double> ct_linear_x;
	vector<double> ct_linear_y;
	double centroid_x;
	double centroid_y;
};

#endif
