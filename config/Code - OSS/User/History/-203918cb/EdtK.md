## installation guide:
### Dependencies and Packages
There are a few steps you need to do before you're ready to use the full potential of this project.
1. Create python Virtual Environment `python -m venv venv`
2. install the required python packages. `pip install -r requirements.txt` 
3. Next step is to build UHD from source, follow this guide `https://files.ettus.com/manual/page_build_guide.html`
4. Create Soft link between the built Python into your Venv.`ln -s path/to/uhd/python path/to/venv/lib/python3.13/site-packges`
5. Put this text into .bashrc file in your user root.`export UHD_IMAGES_DIR ='path/to/images'`  

If you finished this steps you're almost done. Let's move on to the next steps.
1. Make sure


