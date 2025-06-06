#include "makeClusters.h"

void readCaloGridFromFile(const std::string& fileName, fixed_t caloGrid[iEtaBins][iPhiBins]) {
    std::ifstream infile(fileName);
    if (!infile) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return;
    }

    std::string line;
    int iEta = 0;

    while (std::getline(infile, line) && iEta < iEtaBins) {
        std::istringstream iss(line);
        for (int iPhi = 0; iPhi < iPhiBins; ++iPhi) {
            double currentVal;
            if (!(iss >> currentVal)) {
                std::cerr << "Error parsing value at (iEta=" << iEta << ", iPhi=" << iPhi << ")" <<std::endl;
                currentVal = 0.0;
            }
            caloGrid[iEta][iPhi] = static_cast<fixed_t>(currentVal);
        }
        ++iEta;
    }

    if (iEta!=iEtaBins) {
        std::cerr << "Warning: expected " << iEtaBins << " rows, but got " << iEta << std::endl;
    }

    infile.close();
}

int main() {
    fixed_t caloGrid[iEtaBins][iPhiBins];
    Cluster cluster3x3_out[max3x3Clusters];
    Cluster cluster5x5_out[max5x5Clusters];
    Cluster cluster7x7_out[max7x7Clusters];
    cluster_count_t n_3x3found;
    cluster_count_t n_5x5found;
    cluster_count_t n_7x7found;

    readCaloGridFromFile("inputs/caloGrid_1.dat", caloGrid);
    std::cout << "caloGrid[17][32] = " << (float)caloGrid[17][32] << std::endl;

    makeClusters(caloGrid, cluster3x3_out, cluster5x5_out, cluster7x7_out, n_3x3found, n_5x5found, n_7x7found);

    return 0;
}
