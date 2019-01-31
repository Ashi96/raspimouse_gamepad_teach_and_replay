#include "Observation.h"
#include <iostream>
#include <string>
#include <cmath>

Observation::Observation()
{
	ct_theta.reserve(10);
	ct_linear_x.reserve(10);
	ct_linear_y.reserve(10);
}

/*Observation::Observation(int left_f,int left_s, int right_s, int right_f)
{
	setValues(left_f,left_s,right_s,right_f);
}*/
Observation::Observation(int left_1,int left_2, int left_3, int left_4, int left_5, int right_1, int right_2, int right_3, int right_4, int right_5)
{
	setValues(left_1,left_2,left_3,left_4,left_5,right_1,right_2,right_3,right_4,right_5);
}

/*void Observation::setValues(int left_f,int left_s, int right_s, int right_f)
{
	lf = left_f > 0 ? left_f : 1;
	ls = left_s > 0 ? left_s : 1;
	rs = right_s > 0 ? right_s : 1;
	rf = right_f > 0 ? right_f : 1;

	log_lf = log10((double)lf);
	log_ls = log10((double)ls);
	log_rs = log10((double)rs);
	log_rf = log10((double)rf);

}*/
void Observation::setValues(int left_1,int left_2, int left_3, int left_4, int left_5, int right_1, int right_2, int right_3, int right_4, int right_5)
{
	l1 = left_1 > 0 ? left_1 : 1;
	l2 = left_2 > 0 ? left_2 : 1;
	l3 = left_3 > 0 ? left_3 : 1;
	l4 = left_4 > 0 ? left_4 : 1;
	l5 = left_5 > 0 ? left_5 : 1;
	r1 = right_1 > 0 ? right_1 : 1;
	r2 = right_2 > 0 ? right_2 : 1;
	r3 = right_3 > 0 ? right_3 : 1;
	r4 = right_4 > 0 ? right_4 : 1;
	r5 = right_5 > 0 ? right_5 : 1;

	log_l1 = log10((double)l1);
	log_l2 = log10((double)l2);
	log_l3 = log10((double)l3);
	log_l4 = log10((double)l4);
	log_l5 = log10((double)l5);
	log_r1 = log10((double)r1);
	log_r2 = log10((double)r2);
	log_r3 = log10((double)r3);
	log_r4 = log10((double)r4);
	log_r5 = log10((double)r5);
}
