void plotCalorimeterGrid(const char* filename = "grid_output.dat", const char* output_png = "grid_output.png") {
    const int iEtaBins = 34;
    const int iPhiBins = 72;

    TCanvas* canvas = new TCanvas("canvas", "Calorimeter Grid", 800, 600);

    TH2F* h2 = new TH2F(
        "h2",
        " ; iPhi; iEta",
        iPhiBins, 0, iPhiBins,
        iEtaBins, 0, iEtaBins
    );

    std::ifstream infile(filename);
    if(!infile) {
        std::cerr << "Error opening file: "<<filename<<std::endl;
        return;
    }

    double currentVal;
    for (int iEta=0; iEta<iEtaBins; ++iEta) {
        for (int iPhi=0; iPhi<iPhiBins; ++iPhi) {
            if (!(infile >> currentVal)) {
                std::cerr << "Error reading data at (iEta, iPhi) = ("<<iEta<<", "<<iPhi<<")"<<std::endl;
                return;
            }
            h2->SetBinContent(iPhi+1, iEta+1, currentVal);
        }
    }

    infile.close();

    gStyle->SetOptStat(0);
    gStyle->SetPalette(kViridis);

    h2->Draw("COLZ");
    canvas->SaveAs(output_png);

    std::cout<<"Saved plot to "<<output_png<<std::endl;
}
