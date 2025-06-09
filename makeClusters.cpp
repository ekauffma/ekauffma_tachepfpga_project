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
        clusters7x7[i].size = 7;
        clusters7x7[i].energy = 0;
    }

    n_3x3Clusters = 0;
    n_5x5Clusters = 0;
    n_7x7Clusters = 0;

    for (int iEta = 0; iEta < iEtaBins; ++iEta) {
        for (int iPhi = 0; iPhi < iPhiBins; ++iPhi) {
            fixed_t currentEnergy = caloGrid[iEta][iPhi];
            bool isLocalMaximum = true;

            // energy must be larger than threshold to continue
            if (currentEnergy<clusterThreshold) isLocalMaximum = false;

            // check if 4 neighbors are larger
            if (iEta>0) {
                if (caloGrid[iEta-1][iPhi] > currentEnergy) isLocalMaximum = false; // check upper neighbor
            }
            if (iEta<iEtaBins-1) {
                if (caloGrid[iEta+1][iPhi] > currentEnergy) isLocalMaximum = false; // check lower neighbor
            }
            if (caloGrid[iEta][(iPhi-1) % iPhiBins] > currentEnergy) isLocalMaximum = false; // check left neighbor
            if (caloGrid[iEta][(iPhi+1) % iPhiBins] > currentEnergy) isLocalMaximum = false; // check right neighbor

            // cluster if isLocalMaximum
            if (isLocalMaximum) {
                fixed_t energy3x3 = 0.0;
                fixed_t energy5x5 = 0.0;
                fixed_t energy7x7 = 0.0;
                std::cout << "Local Maximum found at (iEta=" << iEta << ", iPhi=" << iPhi << ") with energy=" << (float)currentEnergy << std::endl;

                // compute 3x3 energy
                energy3x3 += currentEnergy;
                energy3x3 += caloGrid[iEta][(iPhi-1) % iPhiBins];
                energy3x3 += caloGrid[iEta][(iPhi+1) % iPhiBins];
                if (iEta>0) {
                    energy3x3 += caloGrid[iEta-1][iPhi];
                    energy3x3 += caloGrid[iEta-1][(iPhi-1) % iPhiBins];
                    energy3x3 += caloGrid[iEta-1][(iPhi+1) % iPhiBins];
                }
                if (iEta<iEtaBins-1) {
                    energy3x3 += caloGrid[iEta+1][iPhi];
                    energy3x3 += caloGrid[iEta+1][(iPhi-1) % iPhiBins];
                    energy3x3 += caloGrid[iEta+1][(iPhi+1) % iPhiBins];
                }

                // compute 5x5 energy
                energy5x5 += energy3x3;
                energy5x5 += caloGrid[iEta][(iPhi-2) % iPhiBins];
                energy5x5 += caloGrid[iEta][(iPhi+2) % iPhiBins];
                if (iEta>0) {
                    energy5x5 += caloGrid[iEta-1][(iPhi-2) % iPhiBins];
                    energy5x5 += caloGrid[iEta-1][(iPhi+2) % iPhiBins];
                }
                if (iEta>1) {
                    energy5x5 += caloGrid[iEta-2][(iPhi-2) % iPhiBins];
                    energy5x5 += caloGrid[iEta-2][(iPhi+2) % iPhiBins];
                    energy5x5 += caloGrid[iEta-2][(iPhi-1) % iPhiBins];
                    energy5x5 += caloGrid[iEta-2][(iPhi+1) % iPhiBins];
                    energy5x5 += caloGrid[iEta-2][iPhi];
                }
                if (iEta<iEtaBins-1) {
                    energy5x5 += caloGrid[iEta+1][(iPhi-2) % iPhiBins];
                    energy5x5 += caloGrid[iEta+1][(iPhi+2) % iPhiBins];
                }
                if (iEta<iEtaBins-2) {
                    energy5x5 += caloGrid[iEta+2][(iPhi-2) % iPhiBins];
                    energy5x5 += caloGrid[iEta+2][(iPhi+2) % iPhiBins];
                    energy5x5 += caloGrid[iEta+2][(iPhi-1) % iPhiBins];
                    energy5x5 += caloGrid[iEta+2][(iPhi+1) % iPhiBins];
                    energy5x5 += caloGrid[iEta+2][iPhi];
                }

                // decide whether to stop at 3x3
                if ((energy3x3 / energy5x5) < clusterStopThreshold) {
                    if (n_3x3Clusters < max3x3Clusters) {
                        clusters3x3[n_3x3Clusters].iEta = iEta;
                        clusters3x3[n_3x3Clusters].iPhi = iPhi;
                        clusters3x3[n_3x3Clusters].energy = energy3x3;
                        n_3x3Clusters++;
                    }
                }
                else {
                    // compute 7x7 energy
                    energy7x7 += energy5x5;
                    energy7x7 += caloGrid[iEta][(iPhi-3) % iPhiBins];
                    energy7x7 += caloGrid[iEta][(iPhi+3) % iPhiBins];
                    if (iEta>0) {
                        energy7x7 += caloGrid[iEta-1][(iPhi-3) % iPhiBins];
                        energy7x7 += caloGrid[iEta-1][(iPhi+3) % iPhiBins];
                    }
                    if (iEta>1) {
                        energy7x7 += caloGrid[iEta-2][(iPhi-3) % iPhiBins];
                        energy7x7 += caloGrid[iEta-2][(iPhi+3) % iPhiBins];
                    }
                    if (iEta>2) {
                        energy7x7 += caloGrid[iEta-3][(iPhi-3) % iPhiBins];
                        energy7x7 += caloGrid[iEta-3][(iPhi+3) % iPhiBins];
                        energy7x7 += caloGrid[iEta-3][(iPhi-2) % iPhiBins];
                        energy7x7 += caloGrid[iEta-3][(iPhi+2) % iPhiBins];
                        energy7x7 += caloGrid[iEta-3][(iPhi-1) % iPhiBins];
                        energy7x7 += caloGrid[iEta-3][(iPhi+1) % iPhiBins];
                        energy7x7 += caloGrid[iEta-3][iPhi];
                    }
                    if (iEta<iEtaBins-1) {
                        energy7x7 += caloGrid[iEta+1][(iPhi-3) % iPhiBins];
                        energy7x7 += caloGrid[iEta+1][(iPhi+3) % iPhiBins];
                    }
                    if (iEta<iEtaBins-2) {
                        energy7x7 += caloGrid[iEta+2][(iPhi-3) % iPhiBins];
                        energy7x7 += caloGrid[iEta+2][(iPhi+3) % iPhiBins];
                    }
                    if (iEta>iEtaBins-3) {
                        energy7x7 += caloGrid[iEta+3][(iPhi-3) % iPhiBins];
                        energy7x7 += caloGrid[iEta+3][(iPhi+3) % iPhiBins];
                        energy7x7 += caloGrid[iEta+3][(iPhi-2) % iPhiBins];
                        energy7x7 += caloGrid[iEta+3][(iPhi+2) % iPhiBins];
                        energy7x7 += caloGrid[iEta+3][(iPhi-1) % iPhiBins];
                        energy7x7 += caloGrid[iEta+3][(iPhi+1) % iPhiBins];
                        energy7x7 += caloGrid[iEta+3][iPhi];
                    }

                    if ((energy7x7 / energy5x5) < clusterStopThreshold) {
                        if (n_5x5Clusters < max5x5Clusters) {
                            clusters5x5[n_5x5Clusters].iEta = iEta;
                            clusters5x5[n_5x5Clusters].iPhi = iPhi;
                            clusters5x5[n_5x5Clusters].energy = energy5x5;
                            n_5x5Clusters++;
                        }
                    }
                    else {
                        if (n_7x7Clusters < max7x7Clusters) {
                            clusters7x7[n_7x7Clusters].iEta = iEta;
                            clusters7x7[n_7x7Clusters].iPhi = iPhi;
                            clusters7x7[n_7x7Clusters].energy = energy7x7;
                            n_7x7Clusters++;
                        }
                    }
                }
            }
        }
    }
}
