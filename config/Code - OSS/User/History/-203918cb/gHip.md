## installation guide:
### Dependencies and Packages:
There are a few steps you need to do before you're ready to use the full potential of this project.
1. Create python Virtual Environment `python -m venv venv`
2. install the required python packages. `pip install -r requirements.txt` 
3. Next step is to build UHD from source, follow this guide `https://files.ettus.com/manual/page_build_guide.html`
4. Create Soft link between the built Python into your Venv.`ln -s path/to/uhd/python path/to/venv/lib/python3.13/site-packges`
5. Put this text into .bashrc file in your user root.`export UHD_IMAGES_DIR ='path/to/images'`  
### Code Guide - RF Recorder:
If you finished this steps above, you're almost done. This section explains how to use RF recorder, Very comfortable and Agile application for Visualing signals in the field and saving the baseband data into your Drive.
1. Make sure the file `main.py` is exectuable, if no use that command: `chmod +x main.py`
2. use that command for `./main.py --test True` for testing.
if it doesn't throw any errors, that's **Great**.
3. Now you can you this command `./main.py --help` and you can see all the options available.

### Code Guide - Data Preperation.


