# TAC-HEP FPGA Project

## Setup

First I wrote a script to generate dummy calorimeter grids `generateCalorimeterGrid.cpp`. 
This creates 3 jets per event as Gaussian deposits at randomized locations, wrapping around `iPhi` cyclically.
In addition, Gaussian-distributed random noise is deposited throughout the grid.
Grid inputs must be a minimum of `1e-2` or else the cell is set to 0.
Here are some examples of the dummy events, plotted using the ROOT macro `plotCalorimeterGrid.C`:

![Example 1](./images/caloGrid_1.png)
![Example 2](./images/caloGrid_2.png)
![Example 3](./images/caloGrid_3.png)
![Example 4](./images/caloGrid_4.png)
![Example 5](./images/caloGrid_5.png)
![Example 6](./images/caloGrid_6.png)
![Example 7](./images/caloGrid_7.png)
![Example 8](./images/caloGrid_8.png)

