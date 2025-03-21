## installation guide:
### Dependencies and Packages:
There are a few steps you need to do before you're ready to use the full potential of this project.
1. Create python Virtual Environment `python -m venv venv`
2. install the required python packages. `pip install -r requirements.txt` 
3. Next step is to build UHD from source, follow this guide `https://files.ettus.com/manual/page_build_guide.html`
4. Create Soft link between the built Python into your Venv.`ln -s path/to/uhd/python path/to/venv/lib/python3.13/site-packges`
5. Put this text into .bashrc file in your user root.`export UHD_IMAGES_DIR ='path/to/images'`  
### Code Guide - RF Recorder:
The RF Recorder section is Very comfortable and Agile application for Visualing signals in the field and saving the baseband data into your Drive.
#### Steps to use:
1. Make sure the file `main.py` is executable, if no use that command: `chmod +x main.py`
2. use that command for `./main.py --test True` for testing.
if it doesn't throw any errors, that's **Great**.
3. Now you can use this command `./main.py --help` and you can see all the options available.
#### Examples
1. `./main.py --directory "/mnt" --folder "test" --band_2_4 True --gain 33 --time 0.4`
This command for example would enable the reception in band 2.4 GHz, and save the data in this full path `/mnt/test` (it would create the directory "test") 
2. `./main.py --band_2_4 True --band_5_8 True --visual True`
This command will enable visual representation of the signals in the band 2.4 and 5.8 GHz.
### Code Guide - Data Preperation:
This section, takes the recorded signals of interest. and preperate it for AI Model Training. it outputs a json file for each record file (from RF Recorder). each json file contain left indexes and right indexes which represent the start and end of each signal inside record. Usually the AI model need it in learn what signal to classify.

#### Steps to use:
1. make sure the file `data_preperation.py` is executable, if no use that command: `chmod +x data_preperation.py`
2. You can use this command `./data_preperation.py --help` and you can see all the options available.

#### Examples
1. `./data_preperation.py --recording_dir "/mnt/test"`
