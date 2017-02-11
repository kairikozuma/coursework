#!/bin/bash
./EnergyModel AerialView_DCT.csv AerialView_DCT_new.csv 200
./EnergyModelPlotting AerialView_DCT.csv AerialView_DCT_results.csv
./EnergyModel Airplane_DCT.csv Airplane_DCT_new.csv 200
./EnergyModelPlotting Airplane_DCT.csv Airplane_DCT_results.csv
./EnergyModel ChemFactory_DCT.csv ChemFactory_DCT_new.csv 200
./EnergyModel ChemFactory_DCT.csv ChemFactory_DCT_new_168.csv 168
./EnergyModelPlotting ChemFactory_DCT.csv ChemFactory_DCT_results.csv
./EnergyModel Clock_DCT.csv Clock_DCT_new.csv 200
./EnergyModelPlotting Clock_DCT.csv Clock_DCT_results.csv
./EnergyModel Cronkite_DCT.csv Cronkite_DCT_new.csv 200
./EnergyModelPlotting Cronkite_DCT.csv Cronkite_DCT_results.csv
./EnergyModel Moon_DCT.csv Moon_DCT_new.csv 200
./EnergyModelPlotting Moon_DCT.csv Moon_DCT_results.csv


