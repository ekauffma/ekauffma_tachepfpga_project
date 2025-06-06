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
}
