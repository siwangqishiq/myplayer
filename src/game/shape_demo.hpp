#pragma once

#include "application.hpp"
#include "utils.hpp"
#include <memory>
#include "render/sprite.hpp"

extern "C"
{
#include "libavdevice/avdevice.h"
#include "libavutil/imgutils.h"
// #include "libavutil/samplefmt.h"
// #include "libavutil/timestamp.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
}

struct VideoInfo{
    unsigned int frameCount;

    int videoFrameWidth;
    int videoFrameHeight;

    AVPixelFormat videoPixelFormat;
};

class VideoDemux{
public:
    VideoDemux(std::string path):filepath_(path){
    };

    int prepare();

    bool decodeVideNextFrame();

    VideoInfo getVideoInfo();

    ~VideoDemux();

    uint8_t *videoDstBuf[4];
    int videoBufSize;

    uint8_t *pixelYBuf = nullptr;
    uint8_t *pixelUBuf = nullptr;
    uint8_t *pixelVBuf = nullptr;
private:
    std::string filepath_;

    int decodeVideoRet = -1;
    bool isDecodeFinished = false;

    int videoWidth;
    int videoHeight;
    AVPixelFormat videoFormat;

    int videoStreamIndex = -1;
    long videoDuration;

    int lineSizes[4];

    AVFormatContext *formatContext = nullptr;
    const AVCodec *pCodec = nullptr;
    AVCodecContext *codecCtx = nullptr;

    AVPacket *packet = nullptr;
    AVFrame *frame = nullptr;
};

class ShapeDemo : public IScene,EventActionCallback {
public:
    ShapeDemo(Application *app){
        appContext = app;
    }

    glm::vec4 COLOR_SKY_BLUE = ConvertColor(33,150,243,255);
    glm::vec4 COLOR_WHITE = ConvertColor(255,255,255,255);
    glm::vec4 COLOR_GRAY = ConvertColor(115 , 115 , 115 , 255);

    bool isInited = false;
    
    virtual void init() override;

    virtual void tick() override;

    virtual void dispose() override;

    virtual bool onEventAction(int action , float x , float y) override;

    void buildViews();
private:
    Application *appContext;

    int viewWidth_;
    int viewHeight_;

    std::shared_ptr<VideoDemux> videoDemux_;

    std::shared_ptr<RenderEngine> renderEngine_;

    Shader customTextureShader_;
    std::shared_ptr<TextureImage> image_;
    std::shared_ptr<TextureImage> videoTextureY_;
    std::shared_ptr<TextureImage> videoTextureU_;
    std::shared_ptr<TextureImage> videoTextureV_;

    void testRenderShapeRect();
    void testRenderShapeRect2();
    void testRenderLines();
    void testRenderLines2();
    void testRenderLinesStar();
    
    void testRenderArc();
    void testRenderArc2();
    void testRenderArc3();

    void testCustomTextureShader();

    void renderVideo();
};