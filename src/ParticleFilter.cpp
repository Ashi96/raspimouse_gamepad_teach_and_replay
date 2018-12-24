#include <vector>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/SVD>
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

	cout << "size: " << obs->ct_linear_x.size() << endl;

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

double ParticleFilter::likelihood(Observation *past, Observation *last)
{
/*	double diff[4] = {	past->log_lf - last->log_lf,
				past->log_ls - last->log_ls,
				past->log_rs - last->log_rs,
				past->log_rf - last->log_rf };*/
	
	double diff[4] = {	(double)past->lf - (double)last->lf,
				(double)past->ls - (double)last->ls,
				(double)past->rs - (double)last->rs,
				(double)past->rf - (double)last->rf };
	//cout << diff[1] << '\t' << diff[2] << endl;
/*	double diff[4] = {
		(past->lf + past->rf) - (last->lf + last->rf), ((past->lf + past->rf) + (last->lf + last->rf)),
		(past->ls + past->rs) - (last->ls + last->rs), ((past->ls + past->rs) + (last->ls + last->rs)) };
				*/

	/*
	double ans = 1.0;
	double sigma = 300;
	double coef = 1.0 / (sqrt(2*sigma*sigma));
	for(double &d : diff){
		ans *= coef * exp( -(d*d) / (2*sigma*sigma));
	}
	*/
	double ans = 1.0;
	for(double &d : diff){
		ans /= (1 + fabs(d));
	}

	return ans;
}

double ParticleFilter::likelihood(Observation *past, Observation *last, Action *past_a, Action *last_a)
{
/*	double diff[4] = {	past->log_lf - last->log_lf,
				past->log_ls - last->log_ls,
				past->log_rs - last->log_rs,
				past->log_rf - last->log_rf };*/

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
}

double ParticleFilter::icplikelihood(Observation *past, Observation *last)
{
	MatrixXd w;
	//MatrixXd s;
	MatrixXd u, v;
	MatrixXd R, t;
	MatrixXd mpast(3,4);	//4 <- Number of sensors
	MatrixXd mlast(3,4);	//4 <- Number of sensors
	MatrixXd mm(3, 1);
	MatrixXd ms(3, 1);
	//cout << "size: " << past->ct_linear_x.size() << endl;
	//cout << "size: " << last->ct_linear_x.size() << endl;
	for(int i = 0; i < 4; i++){
		mpast(0, i) = past->ct_linear_x[i];
		mpast(1, i) = past->ct_linear_y[i];
		mpast(2, i) = past->ct_theta[i];
	}
	for(int i = 0; i < 4; i++){
		mlast(0, i) = last->ct_linear_x[i];
		mlast(1, i) = last->ct_linear_y[i];
		mlast(2, i) = last->ct_theta[i];
	}
	//4 <- Number of sensors
	cout << "mpast" << mpast << endl;
	cout << "mlast" << mlast << endl;
	w = mpast * mlast.transpose();
	Eigen::JacobiSVD< MatrixXd > svd(w, Eigen::ComputeThinU | Eigen::ComputeThinV);
	//s = svd.singularValues();
	u = svd.matrixU();
	v = svd.matrixV();
	R = u * v;
	mm(0, 0) = past->centroid_x;
	mm(1, 0) = 0.0;
	mm(2, 0) = past->centroid_y;
	ms(0, 0) = last->centroid_x;
	ms(1, 0) = 0.0;
	ms(2, 0) = last->centroid_y;
	t = mm - R * ms;
	double ans;
	ans = (1.0 / (1.0 + fabs(t(0,0)))) * (1.0 / (1.0 + fabs(t(2.0)))) * (1.0 / (1.0 + fabs(acos(R(0,0)))));
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
	cout << "odom" << endl;
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
	double ct_x[4], ct_y[4], ct_theta[4];
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
	}
}

