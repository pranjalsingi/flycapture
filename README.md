# wireless-microscope

## Running via Docker

* install docker https://docs.docker.com/engine/installation/
* `cd` to the `flycapture` directory
* run `docker build -t flycapture .`
* run `docker run -p 8081:8081 --privileged --name app flycapture`
* visit the server
* to close the app run `docker stop app && docker rm app`
* to get an interactive shell of the container run `docker run -p 8081:8081 -it --privileged --name app flycapture /bin/bash`

## Running natively

Step 1: Download Flycapture SDK from https://www.ptgrey.com/support/downloads

	Camera : Chameleon 3, Model : CM3-U3-50S5C-CS, OS : Linux Ubuntu 16.04
	Download the FlyCapture 2.11.3.121 SDK - Linux Ubuntu 16.04 (64-bit) from Software
	
Step 2: Update kernel
	
	- sudo apt-get update
	- sudo apt-get upgrade
	- sudo apt-get dist-upgrade

Step 3: Goto https://www.ptgrey.com/tan/10685 and follow the steps

	- Install and configure linux
		* sudo apt-get install libgtkmm-2.4-dev libglademm-2.4-dev libusb-1.0
		* sudo apt-get install libraw1394-11 libgtkmm-2.4-1v5 libglademm-2.4-1v5 libgtkglextmm-x11-1.2-dev libgtkglextmm-x11-1.2 libusb-1.0-0

	- Check Linux version -> If its 3.5.0 or newer then its fine or follow steps from the URL metioned above

	- Configure USBFS -> follow steps from the URL
		
	- For compiling examples
		* sudo apt-get install build-essential
 
Step 4: Uncompress the tar file to install Flycapture SDK:
	
	- goto the folder where the sdk was downloaded
	- tar zxvf flycapture2-2.11.3.121-amd64-pkg.tgz
	- sudo sh install_flycapture.sh
		* If this doesn't install correctly, then the dependencies are not correctly setup
		* remove the flycapture installation by sudo sh remove_flycapture.sh
		* install dependencies correctly by reading document at Step 2
	- the flycapture folder would be generated in /usr/src
	
Step 5: Copy the flycapture folder to a place where you want to run it.

Step 6: Change the permission of the flycapture folder so that one can delete or make changes without root permission

Step 7: Copy all the files in the repo to the flycapture folder

Step 8: Copy the files in cpp folder to src/CustomImageEx

Step 9: Run following command in src/CustomImageEx

	- sudo make BASE_NAME=VideoImageEx
	- sudo make BASE_NAME=BinnedImageEx

Step 10: Running the project
	
	- run npm install
	- run npm start
