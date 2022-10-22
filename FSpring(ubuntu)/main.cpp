/*
*  main.cpp
*  FSpring
*
*  Created by kimbom on 2017. 9. 23...
*  Copyright 2017 kimbom. All rights reserved.
*
*/
#include<iostream>
#include<vector>
#include<set>
#include<atomic>
#include<thread>

#include<unistd.h>
#include<sys/stat.h>

#include<opencv2/opencv.hpp>

#include<nana/gui.hpp>
#include<nana/gui/wvl.hpp>
#include<nana/gui/widgets/label.hpp>
#include<nana/gui/widgets/listbox.hpp>
#include<nana/gui/widgets/menu.hpp>
#include<nana/gui/widgets/button.hpp>
#include<nana/gui/filebox.hpp>
#include<nana/gui/widgets/menubar.hpp>
#include<nana/gui/widgets/progress.hpp>

extern "C" {
#include <libavformat/avformat.h>
#include<libavcodec/avcodec.h>
}

#include<omp.h>
//https://stackoverflow.com/questions/33543674/cannot-use-nana-library
void GetVideoInfo(std::string video_file, int64_t* pframe_count = nullptr, int64_t* pduration_msec = nullptr, double* pfps = nullptr) {
    bool ret = true;
    av_register_all();
    AVFormatContext* pFormatCtx = nullptr;
    avformat_open_input(&pFormatCtx, video_file.c_str(), NULL, NULL);
    avformat_find_stream_info(pFormatCtx, NULL);
    int videoStream = -1;   //Find video Stream
    for (int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            break;
        }
    }
    int64_t frame_count = pFormatCtx->streams[videoStream]->nb_frames;

    if (frame_count<=0 || frame_count>INT_MAX) {
        //frame_count is incorrect
        frame_count = 0;
        AVPacket packet;
        while (av_read_frame(pFormatCtx, &packet) >= 0) {
            if (packet.stream_index == videoStream) {
                frame_count++;
            }
            av_packet_unref(&packet);
        }
    }
    int64_t duration_msec = pFormatCtx->duration*1000.0 / AV_TIME_BASE;
    double fps = frame_count * 1000.0 / duration_msec;
    if (frame_count) {
        *pframe_count = frame_count;
    }
    if (pduration_msec) {
        *pduration_msec = duration_msec;
    }
    if (pfps) {
        *pfps = fps;
    }
    avformat_close_input(&pFormatCtx);
    avformat_free_context(pFormatCtx);
}
class MainForm{
protected:
    int W=1200;
    int H=600;
    int H_text=30;
    int H_menu=0;
    nana::form* m_form;
    nana::menubar* m_menu;
    nana::listbox* m_listbox;
    nana::label* m_text;
    std::set<std::string> m_files;
public:
    nana::color m_color_bk=nana::color(253,246,227);
    MainForm(){
        //Create Form
        m_form=new nana::form(nana::API::make_center(W,H));
        m_form->caption("FSpring(ubuntu)1.0");
        m_form->bgcolor(m_color_bk);
        //Create Menu
        m_menu=new nana::menubar(*m_form);
        m_menu->bgcolor(m_color_bk);
        m_menu->push_back("File(&F)");
        m_menu->push_back("Extract(&E)");
        H_menu=m_menu->size().height;
        //Create Listbox
        m_listbox=new nana::listbox(*m_form,nana::rectangle(0,H_menu,W,H-H_menu-H_text));
        m_listbox->append_header("Header", W-10);
        m_listbox->show_header(false);
        m_listbox->bgcolor(m_color_bk);
        //Create label
        m_text=new nana::label(*m_form,nana::rectangle(0,H-H_text,W,H_text));
        m_text->caption("waiting");
        AddEvents();
    }
    void Run(){
        m_form->show();
    }
    ~MainForm(){
        delete m_form;
        delete m_menu;
        delete m_listbox;
        delete m_text;
    }
protected:
    void AddEvents(){
        //Auto resize
        m_form->events().resized([&]()->void{
            W=m_form->size().width;
            H=m_form->size().height;
            m_listbox->move(nana::rectangle(0,H_menu,W,H-H_menu-H_text));
            m_text->move(nana::rectangle(0,H-H_text,W,H_text));
        });
        m_menu->at(0).append("Add video", [&](nana::menu::item_proxy& ip){
            std::set<std::string> extension={"avi","mp4","mkv","mov"};
            char buf[10000];
            FILE *fp = popen("zenity --file-selection --multiple", "r");
            char* p=fgets(buf, 10000, fp);
            std::string str=buf;
            if(str.length()==0)return;
            str.pop_back(); //last char is '\n'

            std::vector<std::string> files;
            std::string token = "|";
            std::string::size_type offset = 0;
            while (offset<str.length()){
                std::string word = str.substr(offset, str.find(token, offset) - offset);
                offset += word.length() + 1;
                size_t pos=word.find_last_of('.');
                if(pos!=std::string::npos) {
                    std::string ext = word.substr(pos+1, word.length() - pos - 1);
                    if (extension.find(ext) != extension.end()) {
                        files.push_back(word);
                    }
                }
            }
            //remove overlap files
            for(auto& e:files){
                m_files.insert(e);

            }
            m_listbox->erase();
            for(auto& e:m_files){
                m_listbox->at(0).append({e});
            }
        });
        m_menu->at(0).append("Clear video", [&](nana::menu::item_proxy& ip){
            m_listbox->erase();
        });
        m_menu->at(0).append("Exit", [&](nana::menu::item_proxy& ip){
            nana::API::exit_all();
        });
        m_menu->at(1).append("Save as JPG", [&](nana::menu::item_proxy& ip){
            m_menu->enabled(false);
            ExtractVideos(".jpg");
            m_menu->enabled(true);
        });
        m_menu->at(1).append("Save as PNG", [&](nana::menu::item_proxy& ip){
            m_menu->enabled(false);
            ExtractVideos(".png");
            m_menu->enabled(true);
        });
    }
    void ExtractVideos(std::string ext){
        std::atomic<int64_t> total_frame;
        std::atomic<int64_t> process_frame;
        auto Extract=[&]()->void{
            std::vector<std::string> files;
            for(auto& file:m_files){
                files.push_back(file);
            }
#pragma omp parallel for schedule(dynamic)
            for(int i=0;i<files.size();i++){
                auto& file=files[i];
                cv::VideoCapture vc;
                vc.open(file);
                std::string dir=file+".frames";
                mkdir(dir.c_str(),0777);
                cv::Mat frame;
                int c=0;
                while(vc.read(frame)==true){
                    std::ostringstream oss;
                    oss.width(8);
                    oss.fill('0');
                    oss << c++;
                    process_frame++;
                    cv::imwrite(dir+"/"+oss.str()+ext,frame);
                }
            }
        };
        process_frame=0;
        total_frame=0;
        //for calculate total frame count
        for(auto& file:m_files){
            int64_t fc;
            GetVideoInfo(file,&fc);
            total_frame+=fc;
        }
        std::thread thread1(Extract);
        int ratio=0;
        int bratio=-1;
        while(process_frame < total_frame){
            int ratio=((float)process_frame/total_frame)*100;
            if(ratio!=bratio) {
                bratio=ratio;
                m_text->caption(std::to_string(ratio)+"%");
            }
        }
        thread1.join();
        m_text->caption("Complete!");
    }
};



int main(){
    MainForm form;

    form.Run();
    nana::exec();
}
