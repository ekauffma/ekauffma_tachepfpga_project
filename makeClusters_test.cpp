#include <fstream>
#include <cmath>
#include <random>

#include "makeClusters.h"

struct SimCluster {
    double iEta_center;
    double iPhi_center;
    double amplitude;
    double sigma_iEta;
    double sigma_iPhi;
};

double get2DGaussian(int iEta, int iPhi, const SimCluster& cluster) {
    double diEta = iEta - cluster.iEta_center;
    double diPhiRaw = iPhi - cluster.iPhi_center;

    double diPhi = std::fmod(std::abs(diPhiRaw), iPhiBins);
    if (diPhi > iPhiBins / 2.0) {
        diPhi = iPhiBins - diPhi;
    }

    double exponent = -(diEta*diEta / (2*cluster.sigma_iEta*cluster.sigma_iEta) + 
                        diPhi*diPhi / (2*cluster.sigma_iPhi*cluster.sigma_iPhi));

    return cluster.amplitude * std::exp(exponent);
}

void generateTestCaloGrid(fixed_t caloGrid[iEtaBins][iPhiBins]) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> center_iEta_dist(0, iEtaBins);
    std::uniform_real_distribution<> center_iPhi_dist(0, iPhiBins);
    std::uniform_real_distribution<> amplitude_dist(2.0, 30.0);
    std::uniform_real_distribution<> sigma_dist(0.3, 2.0);
    std::uniform_real_distribution<> prob_dist(0.0, 1.0);
    std::normal_distribution<> noise_dist(0.0, 1.0);

    SimCluster clusters[nGenClusters];

    // Generate clusters
    for (int i = 0; i < nGenClusters; ++i) {
        double sigma = sigma_dist(gen);
        clusters[i] = {
            center_iEta_dist(gen),
            center_iPhi_dist(gen),
            amplitude_dist(gen),
            sigma,
            sigma
        };
    }

    // Fill grid
    for (int iEta = 0; iEta < iEtaBins; ++iEta) {
        for (int iPhi = 0; iPhi < iPhiBins; ++iPhi) {
            double sum = 0.0;
            for (int c = 0; c < nGenClusters; ++c) {
                sum += get2DGaussian(iEta, iPhi, clusters[c]);
            }

            if (prob_dist(gen) < 0.1) {
                sum += noise_dist(gen);
            }

            caloGrid[iEta][iPhi] = (sum > 1e-3) ? fixed_t(sum) : fixed_t(0.0);
        }
    }

    std::ofstream outfile("caloGrid_input.dat");
    if (!outfile) {
        std::cerr << "Error: could not open caloGrid_input.dat for writing.\n";
        return;
    }

    for (int iEta = 0; iEta < iEtaBins; ++iEta) {
        for (int iPhi = 0; iPhi < iPhiBins; ++iPhi) {
            outfile << float(caloGrid[iEta][iPhi]) << "\t";
        }
        outfile << "\n";
    }

    outfile.close();
}

int main() {
    fixed_t caloGrid[iEtaBins][iPhiBins];
    Cluster cluster3x3_out[max3x3Clusters];
    Cluster cluster5x5_out[max5x5Clusters];
    Cluster cluster7x7_out[max7x7Clusters];
    cluster_count_t n_3x3found;
    cluster_count_t n_5x5found;
    cluster_count_t n_7x7found;

    generateTestCaloGrid(caloGrid);

    std::cout << "caloGrid before makeClusters function = " << std::endl;
    for (int i = 0; i<5; ++i) {
        std::cout << "    ";
        for (int j = 46; j<51; ++j) {
            std::cout << (float)caloGrid[i][j] << "  ";
        }
        std::cout << std::endl;
    }

    makeClusters(caloGrid, cluster3x3_out, cluster5x5_out, cluster7x7_out, n_3x3found, n_5x5found, n_7x7found);

    // print clusters
    std::cout<<"3x3 Clusters: "<<std::endl;
    for (int i = 0; i < n_3x3found; ++i) {
        std::cout << "    Cluster " << i << ": iEta=" << cluster3x3_out[i].iEta << ", iPhi=" << cluster3x3_out[i].iPhi << ", Energy=" << cluster3x3_out[i].energy << std::endl;
    }
    std::cout<<"5x5 Clusters: "<<std::endl;
    for (int i = 0; i < n_5x5found; ++i) {
        std::cout << "    Cluster " << i << ": iEta=" << cluster5x5_out[i].iEta << ", iPhi=" << cluster5x5_out[i].iPhi << ", Energy=" << cluster5x5_out[i].energy << std::endl;
    }
    std::cout<<"7x7 Clusters: "<<std::endl;
    for (int i = 0; i < n_7x7found; ++i) {
        std::cout << "    Cluster " << i << ": iEta=" << cluster7x7_out[i].iEta << ", iPhi=" << cluster7x7_out[i].iPhi << ", Energy=" << cluster7x7_out[i].energy << std::endl;
    }

    return 0;
}
