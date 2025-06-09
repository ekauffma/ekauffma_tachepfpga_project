#include <hls_stream.h>
#include "makeClusters.h"

void makeClusters(
    fixed_t caloGrid[iEtaBins][iPhiBins],
    Cluster clusters3x3[max3x3Clusters],
    Cluster clusters5x5[max5x5Clusters],
    Cluster clusters7x7[max7x7Clusters],
    cluster_count_t &n_3x3Clusters,
    cluster_count_t &n_5x5Clusters,
    cluster_count_t &n_7x7Clusters
) {

    std::cout << "caloGrid inside makeClusters function = " << std::endl;
    for (int i = 0; i<5; ++i) {
        std::cout << "    ";
        for (int j = 46; j<51; ++j) {
            std::cout << (float)caloGrid[i][j] << "  ";
        }
        std::cout << std::endl;
    }

    // initialize cluster arrays and counters to 0
    for (int i = 0; i < max3x3Clusters; ++i) {
        clusters3x3[i].iEta = 0;
        clusters3x3[i].iPhi = 0;
        clusters3x3[i].size = 3;
        clusters3x3[i].energy = 0;
    }

    for (int i = 0; i < max5x5Clusters; ++i) {
        clusters5x5[i].iEta = 0;
        clusters5x5[i].iPhi = 0;
        clusters5x5[i].size = 5;
        clusters5x5[i].energy = 0;
    }

    for (int i = 0; i < max7x7Clusters; ++i) {
        clusters7x7[i].iEta = 0;
        clusters7x7[i].iPhi = 0;
        clusters7x7[i].size = 3;
        clusters7x7[i].energy = 0;
    }

    n_3x3Clusters = 0;
    n_5x5Clusters = 0;
    n_7x7Clusters = 0;

    coord_t iEtaNeighbor;
    coord_t iPhiNeighbor;

    for (int iEta = 0; iEta < iEtaBins; ++iEta) {
        for (int iPhi = 0; iPhi < iPhiBins; ++iPhi) {
            fixed_t currentEnergy = caloGrid[iEta][iPhi];
            bool isLocalMaximum = true;

            if (currentEnergy<clusterThreshold) isLocalMaximum = false;

            // check upper neighbor
            if (iEta>0) {
                iEtaNeighbor = iEta-1;
                if (caloGrid[iEtaNeighbor][iPhi] > currentEnergy) isLocalMaximum = false;
            }
            // check lower neighbor
            if (iEta<iEtaBins-1) {
                iEtaNeighbor = iEta+1;
                if (caloGrid[iEtaNeighbor][iPhi] > currentEnergy) isLocalMaximum = false;
            }
            // check left neighbor
            iPhiNeighbor = iPhi-1;
            if (iPhiNeighbor < 0) iPhiNeighbor = iPhiBins-1;
            if (caloGrid[iEta][iPhiNeighbor] > currentEnergy) isLocalMaximum = false;
                
            // check right neighbor
            iPhiNeighbor = iPhi+1;
            if (iPhiNeighbor>iPhiBins-1) iPhiNeighbor = 0;
            if (caloGrid[iEta][iPhiNeighbor] > currentEnergy) isLocalMaximum = false;

            if (isLocalMaximum) {
                std::cout << "Local Maximum found at (iEta=" << iEta << ", iPhi=" << iPhi << ") with energy=" << (float)currentEnergy << std::endl;
            }
        }
    }
}
