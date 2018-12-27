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
	cout << "coord" << endl;
	double ct_x[4], ct_y[4], ct_theta[4];
	constexpr double slf = sin(-3 * 3.141592 / 180.0);
        constexpr double sls = sin(-45 * 3.141592 / 180.0);
        constexpr double srs = sin(45 * 3.141592 / 180.0);
        constexpr double srf = sin(3 * 3.141592 / 180.0);
        constexpr double clf = cos(-3 * 3.141592 / 180.0);
        constexpr double cls = cos(-45 * 3.141592 / 180.0);
        constexpr double crs = cos(45 * 3.141592 / 180.0);
        constexpr double crf = cos(3 * 3.141592 / 180.0);

	for (auto &e : data)
	{
		//e.observation.ct_theta = {-3, -45, 45, 3}; //[lf,ls,rs,rf]
		ct_x[0] = e.observation.lf * slf;
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
		ct_theta[3] = 3.0;
		e.observation.centroid_x = 0;
		e.observation.centroid_y = 0;
		e.observation.centroid_x = (ct_x[0] + ct_x[1] + ct_x[2] + ct_x[3]) / 4.0; //4.0 <- number of sensors
		e.observation.centroid_y = (ct_y[0] + ct_y[1] + ct_y[2] + ct_y[3]) / 4.0; //4.0 <- number of sensors{
		for (int i = 0; i < 4; i++)
		{
			e.observation.ct_linear_x.push_back(ct_x[i] - e.observation.centroid_x);
			e.observation.ct_linear_y.push_back(ct_y[i] - e.observation.centroid_y);
			e.observation.ct_theta.push_back(ct_theta[i]);
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
