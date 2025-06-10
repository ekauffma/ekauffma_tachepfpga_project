#ifndef _MAKECLUSTERS_H_
#define _MAKECLUSTERS_H_

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <sstream>
#include <ap_fixed.h>
#include <ap_int.h>

#define iEtaBins 34
#define iPhiBins 72
#define nGenClusters 5
#define max3x3Clusters 6
#define max5x5Clusters 6
#define max7x7Clusters 6
#define clusterThreshold 3 // energy at which to include cluster
#define clusterStopThreshold 0.95 // ratio at which to stop expanding cluster (smaller threshold energy / larger threshold energy)

typedef ap_fixed<16, 6> fixed_t;
typedef ap_fixed<22, 12> sum_t;
typedef ap_uint<6> coord_t;
typedef ap_uint<3> cluster_size_t;
typedef ap_uint<3> cluster_count_t;

struct Cluster {
    coord_t iEta;
    coord_t iPhi;
    cluster_size_t size;
    sum_t energy;
};

void makeClusters(
    fixed_t caloGrid[iEtaBins][iPhiBins],
    Cluster clusters3x3[max3x3Clusters],
    Cluster clusters5x5[max5x5Clusters],
    Cluster clusters7x7[max7x7Clusters],
    cluster_count_t &n_3x3Clusters,
    cluster_count_t &n_5x5Clusters,
    cluster_count_t &n_7x7Clusters
);

#endif
