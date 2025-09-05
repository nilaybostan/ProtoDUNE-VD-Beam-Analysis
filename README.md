1) ifbeamreader.py

the code only gives the TOF values from ifbeam database, it can be run using the command below:

in interactive Python:

ipython

import ifbeam_reader

tofs = ifbeam_reader.get_tofs("2025-08-25T11:11:11-05:00","2025-08-25T11:11:18-05:00",60,0) 

for the specific time ranges, you can select what you want to get the TOF.

then just type: tofs 

to see the values in the screen. you can run in your local computer or in lxplus. 


2) IFBeam_Analysis.py

the code gives the beam values (TOF, momentum, CKOV) values from ifbeam database. directly can be run by using the command below: 

python3 IFBeam_Analysis.py

3) clock_ifbeam_reader.py

it gives the clock time for three TOF counter variables, directly can be run using the command below:

python3 clock_ifbeam_reader.py z,pdune 2025-08-25T11:11:11-05:00 2025-08-25T11:12:15-05:00 clock.csv

4) TOF_CKOV.py
   
directly can be run by using the command below: 

python3 TOF_CKOV.py
