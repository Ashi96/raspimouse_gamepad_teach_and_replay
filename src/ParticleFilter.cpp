#include <vector>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/SVD>
#include <cmath>
#include "Particle.h"
#include "ParticleFilter.h"
#include "Observation.h"
#include "Episodes.h"
using namespace std;
using Eigen::MatrixXd;

ParticleFilter::ParticleFilter(int num, Episodes *ep)
{
	double w = 1.0/num;
	Particle p(w);
	for(int i=0;i<num;i++){
		particles.push_back(p);
	}

	episodes = ep;
}

void ParticleFilter::init(void)
{
	cout << "particles.size : "<< particles.size() << endl;
	cout << episodes->data.size() << endl;
	double w = 1.0/particles.size();
	for(auto &p : particles){
		p.pos = prob.uniformRandInt(0,episodes->data.size()-2);
		p.weight = w;
	}
}

void ParticleFilter::print(void)
{
	int i = 0;
	for(auto &p : particles){
		if(p.pos >= 0){
			auto d = episodes->data[p.pos];
			cout << d.str() << endl;
			i++;
		}
		if(i==10)
			return;
	}
}

Action ParticleFilter::modeParticle(Episodes *ep)
{
	double fw = 0.0;
	double rot = 0.0;
	double max = 0.0;
	cout << "mode particle" << endl;
	for(auto &p : particles){
		auto e = ep->actionAt(p.pos);
		if(max < p.weight){
			max = p.weight;
			fw = e->linear_x;
			rot = e->angular_z;
		}
	}
	Action a;
	a.linear_x = fw;
	a.angular_z = rot;
	return a;
}

Action ParticleFilter::mode(Episodes *ep)
{
	for(auto &p : particles){
		auto e = ep->At(p.pos);
		e->counter = 0;
	}
	for(auto &p : particles){
		auto e = ep->At(p.pos);
		e->counter++;
	}
	int max = 0;
	Action *mode_a;
	for(auto &p : particles){
		auto e = ep->At(p.pos);
		if(e->counter > max){
			max = e->counter;
			mode_a = ep->actionAt(p.pos+1);
		}
		e->counter = 0;
	}

	Action a;
	a.linear_x = mode_a->linear_x;
	a.angular_z = mode_a->angular_z;
	return a;
}

Action ParticleFilter::average(Episodes *ep)
{
	double fw = 0.0;
	double rot = 0.0;
	cout << "avg" << endl;
	for(auto &p : particles){
		auto e = ep->actionAt(p.pos+1);
		fw += p.weight * e->linear_x;
		rot += p.weight * e->angular_z;
	}
	Action a;
	a.linear_x = fw;
	a.angular_z = rot;
	return a;
}

Action ParticleFilter::sensorUpdate(Observation *obs, Action *act, Episodes *ep, raspimouse_gamepad_teach_and_replay::PFoEOutput *out)
{
	out->eta = 0.0;

	lastobscoordinatetransformation(obs);

//	cout << "size: " << obs->ct_linear_x.size() << endl;

	cout << "obs likelihood" << endl;
	for(auto &p : particles){

		double h = icplikelihood(episodes->obsAt(p.pos),obs);
		
		//double h = likelihood(episodes->obsAt(p.pos),obs);
		//double h = likelihood(episodes->obsAt(p.pos),obs, episodes->actionAt(p.pos), act);
		p.weight *= h;
		out->eta += p.weight;
	}
/*
	cout << "mode particle" << endl;
	double max = 0.0;
	Particle *max_p = NULL;
	for(auto &p : particles){
		if(max < p.weight){
			max = p.weight;
			max_p = &p;
		}
	}
	out->particles_pos.push_back(max_p->pos);
	return modeParticle(ep);
*/

	normalize();
	resampling(&particles);
	
	//cout << "OUTPUT " << fw << " " << rot << endl;

	for(auto &p : particles){
		out->particles_pos.push_back(p.pos);
	}


	obs->ct_linear_x.clear();
	obs->ct_linear_y.clear();
	obs->ct_theta.clear();

	cout << "mode" << endl;
	return mode(ep);
//	cout << "avg" << endl;
//	return average(ep);
}

/*double ParticleFilter::likelihood(Observation *past, Observation *last)
{
	//double diff[4] = {	past->log_lf - last->log_lf,
	//			past->log_ls - last->log_ls,
	//			past->log_rs - last->log_rs,
	//			past->log_rf - last->log_rf };
	
	double diff[4] = {	(double)past->lf - (double)last->lf,
				(double)past->ls - (double)last->ls,
				(double)past->rs - (double)last->rs,
				(double)past->rf - (double)last->rf };
	//cout << diff[1] << '\t' << diff[2] << endl;
//	double diff[4] = {
//		(past->lf + past->rf) - (last->lf + last->rf), ((past->lf + past->rf) + (last->lf + last->rf)),
//		(past->ls + past->rs) - (last->ls + last->rs), ((past->ls + past->rs) + (last->ls + last->rs)) };
//				

	
//	double ans = 1.0;
//	double sigma = 300;
//	double coef = 1.0 / (sqrt(2*sigma*sigma));
//	for(double &d : diff){
//		ans *= coef * exp( -(d*d) / (2*sigma*sigma));
//	}

	double ans = 1.0;
	for(double &d : diff){
		ans /= (1 + fabs(d));
	}

	return ans;
}*/

/*double ParticleFilter::likelihood(Observation *past, Observation *last, Action *past_a, Action *last_a)
{
//	double diff[4] = {	past->log_lf - last->log_lf,
//				past->log_ls - last->log_ls,
//				past->log_rs - last->log_rs,
//				past->log_rf - last->log_rf };

	double diff[4] = {	(double)past->lf - (double)last->lf,
				(double)past->ls - (double)last->ls,
				(double)past->rs - (double)last->rs,
				(double)past->rf - (double)last->rf };
	double ans = 1.0;
	for(double &d : diff){
		ans /= (1 + fabs(d));
	}
	ans /= (1 + 0.2*fabs(past_a->linear_x - last_a->linear_x));
	ans /= (1 + 0.2*fabs(past_a->angular_z - last_a->angular_z));

	return ans;
}*/

double ParticleFilter::icplikelihood(Observation *past, Observation *last)
{
	MatrixXd w;
	//MatrixXd s;
	MatrixXd u, v;
	MatrixXd R1, R, t;
	/*MatrixXd mpast(3, 4);
	MatrixXd mlast(3, 4);
	MatrixXd mm(3,1);
	MatrixXd ms(3,1);*/
	// 20 <- number of sensors
	MatrixXd mpast(2, 20);
	MatrixXd mlast(2, 20);
	MatrixXd mm(2,1);
	MatrixXd ms(2,1);

	// 20 <- number of sensors
	for(int i = 0; i < 20; i++){
		mpast(0, i) = past->ct_linear_x[i];
		mpast(1, i) = past->ct_linear_y[i];
		//mpast(2, i) = past->ct_theta[i];
	}
	// 20 <- number of sensors
	for(int i = 0; i < 20; i++){
		mlast(0, i) = last->ct_linear_x[i];
		mlast(1, i) = last->ct_linear_y[i];
		//mlast(2, i) = last->ct_theta[i];
	}

	w = mpast * mlast.transpose();
	Eigen::JacobiSVD< MatrixXd > svd(w, Eigen::ComputeThinU | Eigen::ComputeThinV);
	//s = svd.singularValues();
	u = svd.matrixU();
	v = svd.matrixV();
	//v = svd.matrixV().transpose();
	R1 = u * v;
	R = R1.transpose();
	/*cout << "--------------------" << endl;
	cout << "R :" << R << endl;
	cout << "--------------------" << endl;*/
	/*mm(0, 0) = past->centroid_x;
	mm(1, 0) = 0.0;
	mm(2, 0) = past->centroid_y;
	ms(0, 0) = last->centroid_x;
	ms(1, 0) = 0.0;
	ms(2, 0) = last->centroid_y;*/

	mm(0, 0) = past->centroid_x;
	mm(1, 0) = past->centroid_y;
	ms(0, 0) = last->centroid_x;
	ms(1, 0) = last->centroid_y;
	t = mm - R * ms;
/*	cout << "--------------------" << endl;
	cout << "t :" << t << endl;
	cout << "--------------------" << endl;*/

/*
	double cf,cfl1, cfl2, cfl3, cfl4, cfl5, cfl6, cfl7, cfl8, cfl9, cfl10, cfr1, cfr2, cfr3, cfr4, cfr5, cfr6, cfr7, cfr8, cfr9, cfr10;
	cfl1 = (past->log_l1 - last->log_l1) * (past->log_l1 - last->log_l1);
	cfl2 = (past->log_l2 - last->log_l2) * (past->log_l2 - last->log_l2);
	cfl3 = (past->log_l3 - last->log_l3) * (past->log_l3 - last->log_l3);
	cfl4 = (past->log_l4 - last->log_l4) * (past->log_l4 - last->log_l4);
	cfl5 = (past->log_l5 - last->log_l5) * (past->log_l5 - last->log_l5);
	cfl6 = (past->log_l6 - last->log_l6) * (past->log_l6 - last->log_l6);
	cfl7 = (past->log_l7 - last->log_l7) * (past->log_l7 - last->log_l7);
	cfl8 = (past->log_l8 - last->log_l8) * (past->log_l8 - last->log_l8);
	cfl9 = (past->log_l9 - last->log_l9) * (past->log_l9 - last->log_l9);
	cfl10 = (past->log_l10 - last->log_l10) * (past->log_l10 - last->log_l10);
	cfr1 = (past->log_r1 - last->log_r1) * (past->log_r1 - last->log_r1);
	cfr2 = (past->log_r2 - last->log_r2) * (past->log_r2 - last->log_r2);
	cfr3 = (past->log_r3 - last->log_r3) * (past->log_r3 - last->log_r3);
	cfr4 = (past->log_r4 - last->log_r4) * (past->log_r4 - last->log_r4);
	cfr5 = (past->log_r5 - last->log_r5) * (past->log_r5 - last->log_r5);
	cfr6 = (past->log_r6 - last->log_r6) * (past->log_r6 - last->log_r6);
	cfr7 = (past->log_r7 - last->log_r7) * (past->log_r7 - last->log_r7);
	cfr8 = (past->log_r8 - last->log_r8) * (past->log_r8 - last->log_r8);
	cfr9 = (past->log_r9 - last->log_r9) * (past->log_r9 - last->log_r9);
	cfr10 = (past->log_r10 - last->log_r10) * (past->log_r10 - last->log_r10);
*/
	double cfl = 0;
	double cfr = 0;
	for (int i = 0; i < past->log_l.size(); i++){
		cfl += (past->log_l[i] - last->log_l[i]) * (past->log_l[i] - last->log_l[i]);
	}
	for (int i = 0; i < past->log_r.size(); i++){
		cfr += (past->log_r[i] - last->log_r[i]) * (past->log_r[i] - last->log_r[i]);
	}
	//20 <- number of sensors
	//cf = (cfl1 + cfl2 + cfl3 + cfl4 + cfl5 + cfl6 + cfl7 + cfl8 + cfl9 + cfl10 + cfr1 + cfr2 + cfr3 + cfr4 + cfr5 + cfr6 + cfr7 + cfr8 + cfr9 + cfr10) / 20;
	double cf;
	cf = (cfl + cfr) / (past->log_l.size() + past->log_r.size());
	
	//double cf = 1;
	/*cout << "past->log_l1:" << past->log_l1 << endl;
	cout << "last->log_l1:" << last->log_l1 << endl;
	cout << "cfl1:" << cfl1 << endl;*/

	double ans;
	//ans = 1.0 / (1.0 + fabs(t(0,0))) * 1.0 / (1.0 + fabs(t(2,0))) * 1.0 / (1.0 + fabs(acos(R(0,0)) * 180 / 3.141592));
	//ans = 1.0 / (1.0 + fabs(t(0,0))) * 1.0 / (1.0 + fabs(t(1,0))) * 1.0 / (1.0 + fabs(acos(R(0,0)) * 180 / 3.141592));
	ans = 1.0 / (1.0 + fabs(t(0,0))) * 1.0 / (1.0 + fabs(t(1,0))) * 1.0 / (1.0 + fabs(acos(R(0,0)) * 180 / 3.141592)) * 1.0 / (1.0 + cf);
	return ans;
}

void ParticleFilter::resampling(vector<Particle> *ps)
{
        vector<Particle> prev;
        std::shuffle(ps->begin(),ps->end(),std::mt19937());

        double suweighteight = 0.0;
        int num = (int)ps->size();
        for(int i = 0;i < num ;i++){
                ps->at(i).weight += suweighteight;
                suweighteight = ps->at(i).weight;
                prev.push_back(ps->at(i));
        }

        double step = suweighteight / num;
        int* choice = new int[num];
        //double accum = prob.uniformRand(0.0,1.0) / num;
        double accum = step * prob.uniformRand(0.0,0.999999999);
        int j = 0;
        for(int i=0;i<num;i++){
                while(prev[j].weight <= accum)
                        j++;

                if(j == num)
                        j--;

                accum += step;
                choice[i] = j;
        }

        for(int i=0;i<num;i++){
                int j = choice[i];
                ps->at(i) = prev[j];
                ps->at(i).weight = 1.0/num;
        }

        delete [] choice;
}

void ParticleFilter::normalize(void)
{
	double eta = 0.0;
	for(auto &p : particles)
		eta += p.weight;

	cout << "eta: " << eta << endl;
	for(auto &p : particles)
		p.weight /= eta;
}

void ParticleFilter::motionUpdate(Episodes *ep)
{
/*
	cout << "no odom" << endl;
	init();
*/
	//cout << "odom" << endl;
	for(auto &p : particles){
		if(rand() % 10 == 0){
			p.pos = prob.uniformRandInt(0,episodes->data.size()-2);
			continue;
		}

		int r = rand() % 3;
		if(r==0)
			p.pos++;
		else if(r==1)
			p.pos += 2;

		if(p.pos >= ep->data.size()-1)
			p.pos = prob.uniformRandInt(0,episodes->data.size()-2);

	}
}

void ParticleFilter::lastobscoordinatetransformation(Observation *last){
/*	double ct_x[4], ct_y[4], ct_theta[4];
	constexpr double slf = sin(-3 * 3.141592 / 180.0);
	constexpr double sls = sin(-45 * 3.141592 / 180.0);
	constexpr double srs = sin(45 * 3.141592 / 180.0);
	constexpr double srf = sin(3 * 3.141592 / 180.0);
	constexpr double clf = cos(-3 * 3.141592 / 180.0);
	constexpr double cls = cos(-45 * 3.141592 / 180.0);
	constexpr double crs = cos(45 * 3.141592 / 180.0);
	constexpr double crf = cos(3 * 3.141592 / 180.0);

	ct_x[0] = last->lf * slf;
	ct_y[0] = last->lf * clf;
	ct_theta[0] = -3.0;
	ct_x[1] = last->ls * sls;
	ct_y[1] = last->ls * cls;
	ct_theta[1] = -45.0;
	ct_x[2] = last->rs * srs;
	ct_y[2] = last->rs * crs;
	ct_theta[2] = 45.0;
	ct_x[3] = last->rf * srf;
	ct_y[3] = last->rf * crf;
	ct_theta[3] = 3.0;
	last->centroid_x = (ct_x[0] + ct_x[1] + ct_x[2] + ct_x[3]) / 4.0;
	last->centroid_y = (ct_y[0] + ct_y[1] + ct_y[2] + ct_y[3]) / 4.0;
	for(int i = 0; i < 4; i++){
		last->ct_linear_x.push_back(ct_x[i] - last->centroid_x);
		last->ct_linear_y.push_back(ct_y[i] - last->centroid_y);
		last->ct_theta.push_back(ct_theta[i]);
	}*/
std::vector<float> left_sensor_dig = {3, 15, 20, 30, 40, 45, 60, 75, 90, 120};
std::vector<float> right_sensor_dig = {3, 15, 20, 30, 40, 45, 60, 75, 90, 120};
int sum_dig_num = left_sensor_dig.size() + right_sensor_dig.size();
double ct_x[sum_dig_num], ct_y[sum_dig_num], ct_theta[sum_dig_num];
std::vector<float> cr, sr;
std::vector<float> cl, sl;
cr.reserve(right_sensor_dig.size());
sr.reserve(right_sensor_dig.size());
cl.reserve(left_sensor_dig.size());
sl.reserve(left_sensor_dig.size());
/*constexpr double cr1 = cos(3 * 3.141592 / 180.0);
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
  constexpr double sl10 = sin(-120 * 3.141592 / 180.0);*/
  for (int i = 0; i < right_sensor_dig.size(); i++){
	  cr[i] = cos(right_sensor_dig[i] * 3.141592 / 180.0);
	  sr[i] = sin(right_sensor_dig[i] * 3.141592 / 180.0);
  }
  for (int i = 0; i < left_sensor_dig.size(); i++){
	  cl[i] = cos(-left_sensor_dig[i] * 3.141592 / 180.0);
	  sl[i] = sin(-left_sensor_dig[i] * 3.141592 / 180.0);
  }
  for (int i = 0; i < left_sensor_dig.size(); i++){
	  ct_x[i] = last->log_l[i] * sl[i];
	  ct_y[i] = last->log_l[i] * cl[i];
  }
  for (int i = left_sensor_dig.size(), j = 0; i < sum_dig_num; i++,j++){
	  ct_x[i] = last->log_r[j] * sr[j];
	  ct_y[i] = last->log_r[j] * cr[j];
  }

/*	ct_x[0] = last->l1 * sl1;
	ct_y[0] = last->l1 * cl1;
	ct_x[1] = last->l2 * sl2;
	ct_y[1] = last->l2 * cl2;
	ct_x[2] = last->l3 * sl3;
	ct_y[2] = last->l3 * cl3;
	ct_x[3] = last->l4 * sl4;
	ct_y[3] = last->l4 * cl4;
	ct_x[4] = last->l5 * sl5;
	ct_y[4] = last->l5 * cl5;
	ct_x[5] = last->r1 * sr1;
	ct_y[5] = last->r1 * cr1;
	ct_x[6] = last->r2 * sr2;
	ct_y[6] = last->r2 * cr2;
	ct_x[7] = last->r3 * sr3;
	ct_y[7] = last->r3 * cr3;
	ct_x[8] = last->r4 * sr4;
	ct_y[8] = last->r4 * cr4;
	ct_x[9] = last->r5 * sr5;
	ct_y[9] = last->r5 * cr5;*/

/*	ct_x[0] = last->log_l1 * sl1;
	ct_y[0] = last->log_l1 * cl1;
	ct_x[1] = last->log_l2 * sl2;
	ct_y[1] = last->log_l2 * cl2;
	ct_x[2] = last->log_l3 * sl3;
	ct_y[2] = last->log_l3 * cl3;
	ct_x[3] = last->log_l4 * sl4;
	ct_y[3] = last->log_l4 * cl4;
	ct_x[4] = last->log_l5 * sl5;
	ct_y[4] = last->log_l5 * cl5;
	ct_x[5] = last->log_l6 * sl6;
	ct_y[5] = last->log_l6 * cl6;
	ct_x[6] = last->log_l7 * sl7;
	ct_y[6] = last->log_l7 * cl7;
	ct_x[7] = last->log_l8 * sl8;
	ct_y[7] = last->log_l8 * cl8;
	ct_x[8] = last->log_l9 * sl9;
	ct_y[8] = last->log_l9 * cl9;
	ct_x[9] = last->log_l10 * sl10;
	ct_y[9] = last->log_l10 * cl10;
	ct_x[10] = last->log_r1 * sr1;
	ct_y[10] = last->log_r1 * cr1;
	ct_x[11] = last->log_r2 * sr2;
	ct_y[11] = last->log_r2 * cr2;
	ct_x[12] = last->log_r3 * sr3;
	ct_y[12] = last->log_r3 * cr3;
	ct_x[13] = last->log_r4 * sr4;
	ct_y[13] = last->log_r4 * cr4;
	ct_x[14] = last->log_r5 * sr5;
	ct_y[14] = last->log_r5 * cr5;
	ct_x[15] = last->log_r6 * sr6;
	ct_y[15] = last->log_r6 * cr6;
	ct_x[16] = last->log_r7 * sr7;
	ct_y[16] = last->log_r7 * cr7;
	ct_x[17] = last->log_r8 * sr8;
	ct_y[17] = last->log_r8 * cr8;
	ct_x[18] = last->log_r9 * sr9;
	ct_y[18] = last->log_r9 * cr9;
	ct_x[19] = last->log_r10 * sr10;
	ct_y[19] = last->log_r10 * cr10;*/

	last->centroid_x = 0;
	last->centroid_y = 0;
//	last->centroid_x = (ct_x[0] + ct_x[1] + ct_x[2] + ct_x[3] + ct_x[4] + ct_x[5] + ct_x[6] + ct_x[7] + ct_x[8] + ct_x[9]) / 10.0;
//	last->centroid_y = (ct_y[0] + ct_y[1] + ct_y[2] + ct_y[3] + ct_y[4] + ct_y[5] + ct_y[6] + ct_y[7] + ct_y[8] + ct_y[9]) / 10.0;
	double ct_x_sum = 0;
	double ct_y_sum = 0;
	for(int i = 0; i < sum_dig_num; i++){
		ct_x_sum += ct_x[i];
		ct_y_sum += ct_y[i];
	}
	last->centroid_x = (ct_x_sum) / sum_dig_num;
	last->centroid_y = (ct_y_sum) / sum_dig_num;
	// 20 <- number of sensors
	for(int i = 0; i < sum_dig_num; i++){
		last->ct_linear_x.push_back(ct_x[i] - last->centroid_x);
		last->ct_linear_y.push_back(ct_y[i] - last->centroid_y);
	}
}

