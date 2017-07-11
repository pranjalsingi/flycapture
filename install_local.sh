# STEP 2, Update Kernel
#sudo apt-get update -y
#sudo apt-get upgrade -y
#sudo apt-get dist-upgrade -y

# Install Node (and some debugging tools)
#sudo apt-get install -y curl vim
sudo curl -sL https://deb.nodesource.com/setup_4.x | bash -
sudo apt-get install -y nodejs
sudo apt-get update && apt-get install -y imagemagick
sudo apt-get install -y zip

# STEPS 3-5 Install flycapture and dependencies
# ported from readme and install_flycapture.sh scripts inside flycap tar
sudo apt-get install -y libraw1394-11 libgtkmm-2.4-dev \
     libglademm-2.4-dev libgtkglextmm-x11-1.2-dev libusb-1.0-0

cd ~
mkdir bluedragon
sudo cp ~/flycapture/flycapture.tgz ~/bluedragon

cd ~/bluedragon
sudo tar -zxvf flycapture*

sudo dpkg -i flycapture2*/libflycapture-2*
sudo dpkg -i flycapture2*/libflycapturegui-2*
sudo dpkg -i flycapture2*/libflycapture-c-2*
sudo dpkg -i flycapture2*/libflycapturegui-c-2*
sudo dpkg -i flycapture2*/libmultisync-2*
sudo dpkg -i flycapture2*/libmultisync-c-2*
sudo dpkg -i flycapture2*/flycap-2*
sudo dpkg -i flycapture2*/flycapture-doc-2*
sudo dpkg -i flycapture2*/updatorgui*

#STEP 6: Copy repo files into flycapture folder
sudo cp -r ~/flycapture/* /usr/src/flycapture/

#STEP 7: Copy files from cpp to src/CustomImageEx
sudo cp ~/flycapture/cpp/BinnedImageEx.cpp /usr/src/flycapture/src/CustomImageEx
sudo cp ~/flycapture/cpp/VideoImageEx.cpp /usr/src/flycapture/src/CustomImageEx

#STEP 8: Run make command in src/CustomImage
cd /usr/src/flycapture/src/CustomImageEx
sudo make BASE_NAME=VideoImageEx
sudo make BASE_NAME=BinnedImageEx

#STEP 9: Remove the blank.txt from images, generate, thumbnail and movies folder
cd /usr/src/flycapture
sudo rm generate/blank.txt
sudo rm images/blank.txt 
sudo rm thumbnail/blank.txt
sudo rm movies/blank.txt

npm install
npm start
