#include "model.h"
#include <math.h>
#include <time.h>
#include <boost/random.hpp>

Model::Model()
{
}

void Model::init(double dt, int m, int n, int nh, double sigma, double sv0)
{
    this->dt = pow(10, dt);
    this->nh = nh;
    this->n = n;
    this->m = m;
    if (nh % 2 == 0)
        this->N = nh*nh - nh/2;
    else
        this->N = nh*nh - (nh-1)/2;
    this->sigma = sigma;
    this->sv0 = sv0;

    update();

    p.resize(2, N);
    q.resize(2, N);
    f1.resize(2, N);
    f2.resize(2, N);
    f3.resize(2, N);
    f4.resize(2, N);
    v1.resize(2, N);
    v2.resize(2, N);
    v3.resize(2, N);
    v4.resize(2, N);

    typedef boost::mt19937                     ENG;    // Mersenne Twister
    typedef boost::normal_distribution<double> DIST;   // Normal Distribution
    typedef boost::variate_generator<ENG,DIST> GEN;    // Variate generator

    ENG  eng;
    DIST dist(0, sv0);
    GEN  gen(eng, dist);

    gen.engine().seed(time(NULL) + ::getpid());
    gen.distribution().reset();

    Eigen::Vector2d
            qi(sigma - 0.5, sigma - 0.5),
            n1(sigma, 0),
            n2(sigma/2, sigma*sqrt(3)/2);

    for (int i=0; i<N; ++i) {
        p(0,i) = gen();
        p(1,i) = gen();
        q.col(i) = qi;
        qi += n1;
        if (qi[0] > nh*sigma - 0.25)
            qi += n2 - nh*n1;
    }
}

void Model::update()
{
    this->mass = 1.0;
    this->boxW = sigma * (nh + 1) - 1;
    this->boxH = sigma * ((nh - 1) * sqrt(3)/2 + 2) - 1;
    this->packingFractionHCP = (N * M_PI / 4) / (boxW * boxH) / (M_PI / (2*sqrt(3)));
    if (m == 0)
        this->cutoff = pow(1e-10, -1./(n+1.));
    else
        this->cutoff = pow((n/(double)m-1.)*1e-10, -1./(m+1.));
}

void Model::forces(const Eigen::Matrix2Xd &p, const Eigen::Matrix2Xd &q,
                   Eigen::Matrix2Xd &f, Eigen::Matrix2Xd &v)
{
    Eigen::Vector2d rij, fij;
    double r;

    for (int i=0; i<N; ++i)
        f.col(i) << 0, 0;

    for (int i=0; i<N; ++i) {
        fij << n*pow(q(0,i)+0.5, -n-1) - m*pow(q(0,i)+0.5, -m-1) - \
               n*pow(boxW-q(0,i)+0.5, -n-1) + m*pow(boxW-q(0,i)+0.5, -m-1),
               n*pow(q(1,i)+0.5, -n-1) - m*pow(q(1,i)+0.5, -m-1) - \
               n*pow(boxH-q(1,i)+0.5, -n-1) + m*pow(boxH-q(1,i)+0.5, -m-1);
        f.col(i) = f.col(i) + fij;

        for (int j=i+1; j<N; ++j) {
            rij = q.col(j) - q.col(i);
            r = rij.norm();
            if (r > cutoff)
                continue;
            fij = (n*pow(r, -n-1) - m*pow(r, -m-1)) * rij.normalized();
            f.col(j) = f.col(j) + fij;
            f.col(i) = f.col(i) - fij;
        }
    }

    v = p;
}

void Model::rk4step()
{
    forces(p,             q,             f1, v1);
    forces(p + 0.5*dt*f1, q + 0.5*dt*v1, f2, v2);
    forces(p + 0.5*dt*f2, q + 0.5*dt*v2, f3, v3);
    forces(p + dt*f3,     q + dt*v3,     f4, v4);

    p += dt * (f1 + 2*(f2 + f3) + f4) / 6.0;
    q += dt * (v1 + 2*(v2 + v3) + v4) / 6.0;

    t += dt;
}










