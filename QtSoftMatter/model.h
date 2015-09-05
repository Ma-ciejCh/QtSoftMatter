#ifndef MODEL_H
#define MODEL_H

#include <eigen3/Eigen/Core>
#include <vector>

class Model
{
public:
    Model();
    int nh, N, n, m;
    double t, dt, mass, sv0, sigma, boxW, boxH, packingFractionHCP, cutoff;
    Eigen::Matrix2Xd p, q;
    void init(double dt, int m, int n, int nh, double sigma, double sv0);
    void update();
    void forces(const Eigen::Matrix2Xd &p, const Eigen::Matrix2Xd &q,
                Eigen::Matrix2Xd &f, Eigen::Matrix2Xd &v);
    void rk4step();

private:
    Eigen::Matrix2Xd f1, f2, f3, f4, v1, v2, v3, v4;

};

#endif // MODEL_H
