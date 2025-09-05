ifbeamreader.py:
it can be run using the command below:
in interactive Python:
ipython
import ifbeam_reader
tofs = ifbeam_reader.get_tofs("2025-08-25T11:11:11-05:00","2025-08-25T11:11:18-05:00",60,0) for the specific time ranges you can select what you want to get the TOF.
then just type: tofs to see the values in the screen. you can run in your local computer or in lxplus. 




IFBeam_Analysis.py:
directly can be run by using the command below: 
python3 IFBeam_Analysis.py



third.py 
directly can be run
python3 third.py z,pdune 2025-08-25T11:11:11-05:00 2025-08-25T11:12:15-05:00 clock.csv



TOF_CKOV.py
directly can be run by using the command below: 
python3 TOF_CKOV.py
