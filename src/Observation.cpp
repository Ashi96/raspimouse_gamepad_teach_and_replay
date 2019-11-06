#include "Observation.h"
#include <iostream>
#include <string>
#include <cmath>

Observation::Observation()
{
	ct_theta.reserve(20);
	ct_linear_x.reserve(20);
	ct_linear_y.reserve(20);
}

/*Observation::Observation(int left_f,int left_s, int right_s, int right_f)
{
	setValues(left_f,left_s,right_s,right_f);
}*/
Observation::Observation(int left_1,int left_2, int left_3, int left_4, int left_5, int left_6, int left_7, int left_8, int left_9, int left_10, int right_1, int right_2, int right_3, int right_4, int right_5, int right_6, int right_7, int right_8, int right_9, int right_10)
{
	setValues(left_1,left_2,left_3,left_4,left_5,left_6,left_7,left_8,left_9,left_10,right_1,right_2,right_3,right_4,right_5,right_6,right_7,right_8,right_9,right_10);
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
void Observation::setValues(int left_1,int left_2, int left_3, int left_4, int left_5, int left_6, int left_7, int left_8, int left_9, int left_10, int right_1, int right_2, int right_3, int right_4, int right_5, int right_6, int right_7, int right_8, int right_9, int right_10)
{
	l1 = left_1 > 0 ? left_1 : 1;
	l2 = left_2 > 0 ? left_2 : 1;
	l3 = left_3 > 0 ? left_3 : 1;
	l4 = left_4 > 0 ? left_4 : 1;
	l5 = left_5 > 0 ? left_5 : 1;
	l6 = left_6 > 0 ? left_6 : 1;
	l7 = left_7 > 0 ? left_7 : 1;
	l8 = left_8 > 0 ? left_8 : 1;
	l9 = left_9 > 0 ? left_9 : 1;
	l10 = left_10 > 0 ? left_10 : 1;
	r1 = right_1 > 0 ? right_1 : 1;
	r2 = right_2 > 0 ? right_2 : 1;
	r3 = right_3 > 0 ? right_3 : 1;
	r4 = right_4 > 0 ? right_4 : 1;
	r5 = right_5 > 0 ? right_5 : 1;
	r6 = right_6 > 0 ? right_6 : 1;
	r7 = right_7 > 0 ? right_7 : 1;
	r8 = right_8 > 0 ? right_8 : 1;
	r9 = right_9 > 0 ? right_9 : 1;
	r10 = right_10 > 0 ? right_10 : 1;

/*	log_l1 = log10((double)l1);
	log_l2 = log10((double)l2);
	log_l3 = log10((double)l3);
	log_l4 = log10((double)l4);
	log_l5 = log10((double)l5);
	log_r1 = log10((double)r1);
	log_r2 = log10((double)r2);
	log_r3 = log10((double)r3);
	log_r4 = log10((double)r4);
	log_r5 = log10((double)r5);*/
	log_l1 = log((double)l1);
	log_l2 = log((double)l2);
	log_l3 = log((double)l3);
	log_l4 = log((double)l4);
	log_l5 = log((double)l5);
	log_l6 = log((double)l6);
	log_l7 = log((double)l7);
	log_l8 = log((double)l8);
	log_l9 = log((double)l9);
	log_l10 = log((double)l10);
	log_r1 = log((double)r1);
	log_r2 = log((double)r2);
	log_r3 = log((double)r3);
	log_r4 = log((double)r4);
	log_r5 = log((double)r5);
	log_r6 = log((double)r6);
	log_r7 = log((double)r7);
	log_r8 = log((double)r8);
	log_r9 = log((double)r9);
	log_r10 = log((double)r10);
}
