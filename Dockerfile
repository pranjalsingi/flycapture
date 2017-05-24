FROM ubuntu:16.04

#NOTE: All commands are run as root

ENV DEBIAN_FRONTEND noninteractive


# STEP 2, Update Kernel
RUN apt-get update -y
RUN apt-get upgrade -y
RUN apt-get dist-upgrade -y

# Install Node (and some debugging tools)
RUN apt-get install -y curl vim
RUN curl -sL https://deb.nodesource.com/setup_4.x | bash -
RUN apt-get install -y nodejs
RUN apt-get update && apt-get install -y imagemagick
RUN apt-get install -y zip

# STEPS 3-5 Install flycapture and dependencies
# ported from readme and install_flycapture.sh scripts inside flycap tar
RUN apt-get install -y libraw1394-11 libgtkmm-2.4-dev \
     libglademm-2.4-dev libgtkglextmm-x11-1.2-dev libusb-1.0-0

COPY flycapture.tgz /src/

WORKDIR /src
RUN tar -zxvf flycapture*

RUN dpkg -i /src/flycapture2*/libflycapture-2*
RUN dpkg -i /src/flycapture2*/libflycapturegui-2*
RUN dpkg -i /src/flycapture2*/libflycapture-c-2*
RUN dpkg -i /src/flycapture2*/libflycapturegui-c-2*
RUN dpkg -i /src/flycapture2*/libmultisync-2*
RUN dpkg -i /src/flycapture2*/libmultisync-c-2*
RUN dpkg -i /src/flycapture2*/flycap-2*
RUN dpkg -i /src/flycapture2*/flycapture-doc-2*
RUN dpkg -i /src/flycapture2*/updatorgui*

#STEP 6: Copy repo files into flycapture folder
COPY . /usr/src/flycapture

#STEP 7: Copy files from cpp to src/CustomImageEx
COPY cpp/BinnedImageEx.cpp /usr/src/flycapture/src/CustomImageEx
COPY cpp/VideoImageEx.cpp /usr/src/flycapture/src/CustomImageEx

#STEP 8: Run make command in src/CustomImageEx
WORKDIR /usr/src/flycapture/src/CustomImageEx
RUN make BASE_NAME=VideoImageEx
RUN make BASE_NAME=BinnedImageEx

#STEP 9: Remove the blank.txt from images, generate, thumbnail and movies folder
WORKDIR /usr/src/flycapture
RUN rm generate/blank.txt
RUN rm images/blank.txt 
RUN rm thumbnail/blank.txt
RUN rm movies/blank.txt

#STEP 10: Run the project

EXPOSE 8081 

RUN npm install
CMD ["npm", "start"]
