FROM ubuntu:16.04

#NOTE: All commands are run as root

ENV DEBIAN_FRONTEND noninteractive

# STEP 2, Update Kernel
RUN apt-get update
RUN apt-get upgrade
RUN apt-get dist-upgrade

# STEPS 3-5 Install flycapture and dependencies
# ported from readme and install_flycapture.sh scripts inside flycap tar
RUN apt-get install libraw1394-11 libgtkmm-2.4-dev \
     libglademm-2.4-dev libgtkglextmm-x11-1.2-dev libusb-1.0-0

COPY flycapture.tgz /src/
RUN tar -zxvf /src/flycapture*

RUN dpkg -i /src/flycapture2*/libflycapture-2*
RUN dpkg -i /src/flycapture2*/libflycapturegui-2*
RUN dpkg -i /src/flycapture2*/libflycapture-c-2*
RUN dpkg -i /src/flycapture2*/libflycapturegui-c-2*
RUN dpkg -i /src/flycapture2*/libmultisync-2*
RUN dpkg -i /src/flycapture2*/libmultisync-c-2*
RUN dpkg -i /src/flycapture2*/flycap-2*
RUN dpkg -i /src/flycapture2*/flycapture-doc-2*
RUN dpkg -i /src/flycapture2*/updatorgui*

#STEP 7: Copy repo files into flycapture folder
COPY . /src/flycapture2*

#STEP 8: Copy files from cpp to src/CustomImageEx
COPY cpp /src/CustomImageEx

#STEP 9: Run make command in src/CustomImageEx
WORKDIR /src/CustomImageEx
RUN make

#STEP 10: Run the project
RUN npm install
RUN npm start
