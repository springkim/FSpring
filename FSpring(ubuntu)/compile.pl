use strict;
use warnings;

system "g++ -Wl,-rpath,. -O2 main.cpp -o \"FSpring(linux64)/FSpring\" -std=c++11 -lnana -lX11 -lstdc++fs -lpthread -lfontconfig -lXft -L3rdparty/staticlib -lopencv_world -ldl -I3rdparty/include/ -lavcodec -lavformat -lavutil-ffmpeg -llibwebp -lz -lpng -lIlmImf -lipp_iw -lippicv -littnotify -llibprotobuf -lswscale -ljpeg -ltiff -ljasper ";
