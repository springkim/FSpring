# FSpring
### video Frame extractor & Video maker
##### Windows-version 1.3
##### Ubuntu-version 1.0 (not support video maker)

#### Demo

[FSpring(win64).zip](https://github.com/springkim/FSpring/releases/download/bin/FSpring.win64.zip) or `FSpring/FSpring(windows)/FSpring(win64)/`

[FSpring(linux64).tar.gz](https://github.com/springkim/FSpring/releases/download/bin/FSpring.linux64.7z) or `FSpring/FSpring(ubuntu)/FSpring(linux64)/`

### Requirement
* **Windows**
	* [Visual studio 2015](https://imagine.microsoft.com/ko-KR/Catalog/Product/101)
	* [FFmpeg 3.3.3](https://ffmpeg.zeranoe.com/builds/)
	* [Opencv 3.3.0](https://github.com/opencv/opencv/releases/tag/3.3.0)
	* [MSpring](https://github.com/springkim/MSpring)
* **Ubuntu**
	* [CLion](https://www.jetbrains.com/clion/)
	* [Opencv 3.3.0](https://github.com/opencv/opencv/releases/tag/3.3.0)
	* [nana](http://nanapro.org/en-us/)
	```bash
    apt-get install gcc g++ python3 vim build-essential cmake pkg-config libjpeg-dev libtiff5-dev libjasper-dev libpng12-dev libavcodec-dev libavformat-dev libswscale-dev libxvidcore-dev libx264-dev libxine2-dev libv4l-dev v4l-utils libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libqt4-dev mesa-utils libgl1-mesa-dri libqt4-opengl-dev libatlas-base-dev gfortran libeigen3-dev python2.7-dev python3-dev python-numpy python3-numpy -y
	```

### How to use
<img src="https://i.imgur.com/ElCyyzT.png" width="48">
<img src="https://i.imgur.com/GrbZWbK.gif" width="800">
<img src="https://i.imgur.com/WUmFQlx.png" width="48">
<img src="https://i.imgur.com/226PpTN.png" width="800">

##### 1. Run **FSpring.exe**
<img src="https://i.imgur.com/9MUuQnk.png" width="600">

### Video Frame Extractor
#####  `File(&F) -> Add video` click. You can select multiple video files.
<img src="https://i.imgur.com/IyuPJLI.png" height="100"><img src="https://i.imgur.com/n1mrotm.png" height="100">

#####  Select image extension in `Extract(&M)` menu.
<img src="https://i.imgur.com/SodNUwL.png" height="100"><img src="https://i.imgur.com/Hy4rRMp.png" height="100">

##### Write FPS that you want.(windows only)
<img src="https://i.imgur.com/Bx9FuTF.png" height="150">
Ubuntu version doesn't have this option. It will generate whole frames.


<img src="https://i.imgur.com/ElCyyzT.png" width="48">
<img src="https://i.imgur.com/WsHB7jf.png" width="800">
<img src="https://i.imgur.com/WUmFQlx.png" width="48">
<img src="https://i.imgur.com/ijEOkZa.png" width="800">

### How to use Video Maker
#####  `File(&F) -> Add images` click. You can select multiple image files.
<img src="https://i.imgur.com/UpWa3Pb.png" height="100">
#####  Select video extension in `Make(&M)` menu.
<img src="https://i.imgur.com/d3pCo3a.png" height="100">

##### Write video path,name and FPS in Dialog.

<img src="https://i.imgur.com/aNSHbdi.png" width="800">
<img src="https://i.imgur.com/85PfPus.png" width="800">

#### Version Info

* Windows
    * 0.1.0
        * test.
    * 1.0.0
        * Support multi-core.
        * Fix minor bug.
    * 1.1
        * Fix openmp error.
    * 1.2
    	* Video Maker.
    * 1.3
    	* Progress bug fixed.
    	* Extraction according to FPS.
    	* Adding prefix for each frame.
* Linux
	* 1.0.0
		* first release.
