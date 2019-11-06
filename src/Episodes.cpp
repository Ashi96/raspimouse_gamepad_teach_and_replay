#include "Episodes.h"
#include <iostream>
#include <cmath>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/SVD>
using namespace std;

Episodes::Episodes()
{
	reset();
}

void Episodes::append(Event e)
{
	e.event_id = current_event_id;
	//e.episode_id = current_episode_id;
	data.push_back(e);

	current_event_id++;
}

void Episodes::coordinatetransformation(void)
{
	//double ct_x[4], ct_y[4], ct_theta[4];
	double ct_x[20], ct_y[20], ct_theta[20];
	/*constexpr double slf = sin(-3 * 3.141592 / 180.0);
  constexpr double sls = sin(-45 * 3.141592 / 180.0);
  constexpr double srs = sin(45 * 3.141592 / 180.0);
  constexpr double srf = sin(3 * 3.141592 / 180.0);
  constexpr double clf = cos(-3 * 3.141592 / 180.0);
  constexpr double cls = cos(-45 * 3.141592 / 180.0);
  constexpr double crs = cos(45 * 3.141592 / 180.0);
  constexpr double crf = cos(3 * 3.141592 / 180.0);*/

  //constexpr double urg_deg[] = {3, 15, 30, 45, 60};
  //constexpr cr[5], sr[5], cl[5], sl[5];

  constexpr double cr1 = cos(3 * 3.141592 / 180.0);
  constexpr double cr2 = cos(15 * 3.141592 / 180.0);
  constexpr double cr3 = cos(20 * 3.141592 / 180.0);
  constexpr double cr4 = cos(30 * 3.141592 / 180.0);
  constexpr double cr5 = cos(40 * 3.141592 / 180.0);
  constexpr double cr6 = cos(45 * 3.141592 / 180.0);
  constexpr double cr7 = cos(60 * 3.141592 / 180.0);
  constexpr double cr8 = cos(75 * 3.141592 / 180.0);
  constexpr double cr9 = cos(90 * 3.141592 / 180.0);
  constexpr double cr10 = cos(120 * 3.141592 / 180.0);
  constexpr double sr1 = sin(3 * 3.141592 / 180.0);
  constexpr double sr2 = sin(15 * 3.141592 / 180.0);
  constexpr double sr3 = sin(20 * 3.141592 / 180.0);
  constexpr double sr4 = sin(30 * 3.141592 / 180.0);
  constexpr double sr5 = sin(40 * 3.141592 / 180.0);
  constexpr double sr6 = sin(45 * 3.141592 / 180.0);
  constexpr double sr7 = sin(60 * 3.141592 / 180.0);
  constexpr double sr8 = sin(75 * 3.141592 / 180.0);
  constexpr double sr9 = sin(90 * 3.141592 / 180.0);
  constexpr double sr10 = sin(120 * 3.141592 / 180.0);
  constexpr double cl1 = cos(-3 * 3.141592 / 180.0);
  constexpr double cl2 = cos(-15 * 3.141592 / 180.0);
  constexpr double cl3 = cos(-20 * 3.141592 / 180.0);
  constexpr double cl4 = cos(-30 * 3.141592 / 180.0);
  constexpr double cl5 = cos(-40 * 3.141592 / 180.0);
  constexpr double cl6 = cos(-45 * 3.141592 / 180.0);
  constexpr double cl7 = cos(-60 * 3.141592 / 180.0);
  constexpr double cl8 = cos(-75 * 3.141592 / 180.0);
  constexpr double cl9 = cos(-90 * 3.141592 / 180.0);
  constexpr double cl10 = cos(-120 * 3.141592 / 180.0);
  constexpr double sl1 = sin(-3 * 3.141592 / 180.0);
  constexpr double sl2 = sin(-15 * 3.141592 / 180.0);
  constexpr double sl3 = sin(-20 * 3.141592 / 180.0);
  constexpr double sl4 = sin(-30 * 3.141592 / 180.0);
  constexpr double sl5 = sin(-40 * 3.141592 / 180.0);
  constexpr double sl6 = sin(-45 * 3.141592 / 180.0);
  constexpr double sl7 = sin(-60 * 3.141592 / 180.0);
  constexpr double sl8 = sin(-75 * 3.141592 / 180.0);
  constexpr double sl9 = sin(-90 * 3.141592 / 180.0);
  constexpr double sl10 = sin(-120 * 3.141592 / 180.0);

	for (auto &e : data)
	{
	
		//e.observation.ct_theta = {-3, -45, 45, 3}; //[lf,ls,rs,rf]
		/*ct_x[0] = e.observation.lf * slf;
		ct_y[0] = e.observation.lf * clf;
		ct_theta[0] = -3.0;
		ct_x[1] = e.observation.ls * sls;
		ct_y[1] = e.observation.ls * cls;
		ct_theta[1] = -45.0;
		ct_x[2] = e.observation.rs * srs;
		ct_y[2] = e.observation.rs * crs;
		ct_theta[2] = 45.0;
		ct_x[3] = e.observation.rf * srf;
		ct_y[3] = e.observation.rf * crf;
		ct_theta[3] = 3.0;*/

/*		ct_x[0] = e.observation.l1 * sl1;
		ct_y[0] = e.observation.l1 * cl1;
		ct_x[1] = e.observation.l2 * sl2;
		ct_y[1] = e.observation.l2 * cl2;
		ct_x[2] = e.observation.l3 * sl3;
		ct_y[2] = e.observation.l3 * cl3;
		ct_x[3] = e.observation.l4 * sl4;
		ct_y[3] = e.observation.l4 * cl4;
		ct_x[4] = e.observation.l5 * sl5;
		ct_y[4] = e.observation.l5 * cl5;
		ct_x[5] = e.observation.r1 * sr1;
		ct_y[5] = e.observation.r1 * cr1;
		ct_x[6] = e.observation.r2 * sr2;
		ct_y[6] = e.observation.r2 * cr2;
		ct_x[7] = e.observation.r3 * sr3;
		ct_y[7] = e.observation.r3 * cr3;
		ct_x[8] = e.observation.r4 * sr4;
		ct_y[8] = e.observation.r4 * cr4;
		ct_x[9] = e.observation.r5 * sr5;
		ct_y[9] = e.observation.r5 * cr5;*/
		
		ct_x[0] = e.observation.log_l1 * sl1;
		ct_y[0] = e.observation.log_l1 * cl1;
		ct_x[1] = e.observation.log_l2 * sl2;
		ct_y[1] = e.observation.log_l2 * cl2;
		ct_x[2] = e.observation.log_l3 * sl3;
		ct_y[2] = e.observation.log_l3 * cl3;
		ct_x[3] = e.observation.log_l4 * sl4;
		ct_y[3] = e.observation.log_l4 * cl4;
		ct_x[4] = e.observation.log_l5 * sl5;
		ct_y[4] = e.observation.log_l5 * cl5;
		ct_x[5] = e.observation.log_l6 * sl6;
		ct_y[5] = e.observation.log_l6 * cl6;
		ct_x[6] = e.observation.log_l7 * sl7;
		ct_y[6] = e.observation.log_l7 * cl7;
		ct_x[7] = e.observation.log_l8 * sl8;
		ct_y[7] = e.observation.log_l8 * cl8;
		ct_x[8] = e.observation.log_l9 * sl9;
		ct_y[8] = e.observation.log_l9 * cl9;
		ct_x[9] = e.observation.log_l10 * sl10;
		ct_y[9] = e.observation.log_l10 * cl10;
		ct_x[10] = e.observation.log_r1 * sr1;
		ct_y[10] = e.observation.log_r1 * cr1;
		ct_x[11] = e.observation.log_r2 * sr2;
		ct_y[11] = e.observation.log_r2 * cr2;
		ct_x[12] = e.observation.log_r3 * sr3;
		ct_y[12] = e.observation.log_r3 * cr3;
		ct_x[13] = e.observation.log_r4 * sr4;
		ct_y[13] = e.observation.log_r4 * cr4;
		ct_x[14] = e.observation.log_r5 * sr5;
		ct_y[14] = e.observation.log_r5 * cr5;
		ct_x[15] = e.observation.log_r6 * sr6;
		ct_y[15] = e.observation.log_r6 * cr6;
		ct_x[16] = e.observation.log_r7 * sr7;
		ct_y[16] = e.observation.log_r7 * cr7;
		ct_x[17] = e.observation.log_r8 * sr8;
		ct_y[17] = e.observation.log_r8 * cr8;
		ct_x[18] = e.observation.log_r9 * sr9;
		ct_y[18] = e.observation.log_r9 * cr9;
		ct_x[19] = e.observation.log_r10 * sr10;
		ct_y[19] = e.observation.log_r10 * cr10;

		e.observation.centroid_x = 0;
		e.observation.centroid_y = 0;
		/*e.observation.centroid_x = (ct_x[0] + ct_x[1] + ct_x[2] + ct_x[3]) / 4.0; //4.0 <- number of sensors
		e.observation.centroid_y = (ct_y[0] + ct_y[1] + ct_y[2] + ct_y[3]) / 4.0; //4.0 <- number of sensors */
		//e.observation.centroid_x = (ct_x[0] + ct_x[1] + ct_x[2] + ct_x[3] + ct_x[4] + ct_x[5] + ct_x[6] + ct_x[7] + ct_x[8] + ct_x[9]) / 10.0; //10.0 <- number of sensors
		//e.observation.centroid_y = (ct_y[0] + ct_y[1] + ct_y[2] + ct_y[3] + ct_y[4] + ct_y[5] + ct_y[6] + ct_y[7] + ct_y[8] + ct_y[9]) / 10.0; //10.0 <- number of sensors
		double ct_x_sum = 0;
		double ct_y_sum = 0;
		// 20 <- number of sensors
		for (int i = 0; i < 20; i++)
		{
			ct_x_sum += ct_x[i];
			ct_y_sum += ct_y[i];
		}
		e.observation.centroid_x = ct_x_sum / 20.0; //20.0 <- number of sensors
		e.observation.centroid_y = ct_y_sum / 20.0; //20.0 <- number of sensors

/*		for (int i = 0; i < 4; i++)
		{
			e.observation.ct_linear_x.push_back(ct_x[i] - e.observation.centroid_x);
			e.observation.ct_linear_y.push_back(ct_y[i] - e.observation.centroid_y);
			e.observation.ct_theta.push_back(ct_theta[i]);
		}*/
		// 20 <- number of sensors
		for (int i = 0; i < 20; i++)
		{
			e.observation.ct_linear_x.push_back(ct_x[i] - e.observation.centroid_x);
			e.observation.ct_linear_y.push_back(ct_y[i] - e.observation.centroid_y);
		}
	}
}

void Episodes::reset(void)
{
	data.clear();
	current_event_id = 0;
}

/*
void Episodes::renewEpisode(void)
{
	current_event_id = 0;
	current_episode_id++;
}
*/

void Episodes::print(string filename)
{
	ofstream ofs;
	ostream *io;
	if (filename == "")
		io = &cout;
	else
	{
		ofs.open(filename);
		io = &ofs;
	}

	for (auto e : data)
	{
		*io << e.str() << endl;
	}
}

Event *Episodes::At(int pos)
{
	if (pos < 0 || pos >= data.size())
		return NULL;
	return &(data[pos]);
}

Observation *Episodes::obsAt(int pos)
{
	if (pos < 0 || pos >= data.size())
		return NULL;
	return &(data[pos].observation);
}

Action *Episodes::actionAt(int pos)
{
	if (pos < 0 || pos >= data.size())
	{
		cerr << "OUT OF INDEX at actionAt" << endl;
		exit(1);
		return NULL;
	}
	return &(data[pos].action);
}