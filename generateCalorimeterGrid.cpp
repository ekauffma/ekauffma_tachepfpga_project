#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <fstream>

const int iEtaBins = 34;
const int iPhiBins = 72;

struct Cluster {
    double iEta_center;
    double iPhi_center;
    double amplitude;
    double sigma_iEta;
    double sigma_iPhi;
};

double get2DGaussian(int iEta, int iPhi, const Cluster& cluster) {
    double diEta = iEta - cluster.iEta_center;
    double diPhiRaw = iPhi - cluster.iPhi_center;
    double diPhi = std::fmod(std::abs(diPhiRaw), iPhiBins);
    if (diPhi > iPhiBins/2.0) {
        diPhi = iPhiBins - diPhi;
    }
    double exponent = -(diEta*diEta / (2*cluster.sigma_iEta*cluster.sigma_iEta) + 
                        diPhi*diPhi / (2*cluster.sigma_iPhi*cluster.sigma_iPhi));
    return cluster.amplitude * std::exp(exponent);
}

int main() {

    // define random number generators
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> center_iEta_dist(0, iEtaBins);
    std::uniform_real_distribution<> center_iPhi_dist(0, iPhiBins);
    std::uniform_real_distribution<> amplitude_dist(2.0, 40.0);
    std::uniform_real_distribution<> sigma_dist(0.3, 2.0);
    std::uniform_real_distribution<> prob_dist(0.0, 1.0);
    std::normal_distribution<> noise_dist(0.0, 1.0);

    // initialize grid
    std::vector<std::vector<double>> caloGrid(iEtaBins, std::vector<double>(iPhiBins, 0.0));
    
    // create clusters
    int nClusters = 3;
    std::vector<Cluster> clusters;
    for (int i=0; i<nClusters; ++i) {
        double current_sigma = sigma_dist(gen);
        clusters.push_back(
                {
                    center_iEta_dist(gen),
                    center_iPhi_dist(gen),
                    amplitude_dist(gen),
                    current_sigma,
                    current_sigma,
                }
        );
    }
    
    // fill grid with clusters and noise
    for (int iEta=0; iEta<iEtaBins; ++iEta) {
        for (int iPhi=0; iPhi<iPhiBins; ++iPhi) {
            double currentVal = 0.0;
            for (const auto& cluster : clusters) {
                currentVal += get2DGaussian(iEta, iPhi, cluster);
            }
            if (prob_dist(gen) < 0.1) {
                currentVal += noise_dist(gen);
            }
            caloGrid[iEta][iPhi] = (currentVal > 1e-2) ? currentVal : 0.0;
        }
    }

    // save grid
    std::ofstream outfile("caloGrid.dat");
    if (!outfile) {
        std::cerr << "Error: could not open file for writing.\n";
        return 1;
    }

    for (const auto& iEtaVals: caloGrid) {
        for (double val: iEtaVals) {
            outfile << val << "\t";
        }
        outfile << "\n";
    }

    outfile.close();
    std::cout << "Grid written to caloGrid.dat\n";

    return 0;
}

