#include "shape_demo.hpp"
#include "../render/render.hpp"
#include "../render/render_batch.hpp"
#include <memory>
#include "glm/gtc/quaternion.hpp"
#include "render/sprite.hpp"

void ShapeDemo::init(){
    viewWidth_ = appContext->viewWidth_;
    viewHeight_ = appContext->viewHeight_;

    renderEngine_ = appContext->getRender();
    appContext->addEventActionCallback(this);
    appContext->isShowFps = true;

    //todo init
    
    if(isInited){
        return;
    }
    isInited = true;

    image_ = BuildImageByAsset("lan.jpg");
    

    customTextureShader_ = 
        ShaderManager::getInstance()->loadAssetShader("custom_texture_shader2",
                    "shader/sprite_batch_vertex.glsl", 
                    "shader/custom_texture_shader.glsl");

    std::cout << "video prepare" << std::endl;
    videoDemux_ = std::make_shared<VideoDemux>("e:/assets/mv/yangzi_ai.mp4");
    if(videoDemux_->prepare() < 0){
        Logi("video" , "video prepare file failed!");
        return;
    }

    auto info = videoDemux_->getVideoInfo();
    videoTextureY_ = BuildEmptyImage(info.videoFrameWidth , info.videoFrameHeight, 
        GL_RED);
    videoTextureU_ = BuildEmptyImage(info.videoFrameWidth / 2 , info.videoFrameHeight / 2, 
        GL_RED);
    videoTextureV_ = BuildEmptyImage(info.videoFrameWidth / 2 , info.videoFrameHeight / 2, 
        GL_RED);
}

void ShapeDemo::buildViews(){
}

void ShapeDemo::tick(){
    // testRenderShapeRect();
    // testRenderLines();
    // testRenderLines2();
    // testRenderLinesStar();
    // testRenderArc();
    // testRenderArc2();
    // testRenderArc3();
    // testCustomTextureShader();

    // testRenderShapeRect2();

    renderVideo();
}



void ShapeDemo::testCustomTextureShader(){
    Rect rect;
    rect.left = 0.0f;
    rect.top = viewHeight_;
    rect.width = viewHeight_;
    rect.height = viewHeight_;

    renderEngine_->renderTextureShader(customTextureShader_ , rect , 
        image_->getTextureId() , nullptr);
}

void ShapeDemo::testRenderArc3() {
    static float time = 0.0f;
    float stokenWidth = 50.0f * glm::sin(time) + 50.0f;
    time += 0.01f;

    Paint paint;
    paint.color = COLOR_SKY_BLUE;
    paint.fillStyle = Stroken; 
    paint.stokenWidth = stokenWidth;

    // arc1
    float radius = viewHeight_ / 4.0f;
    float centerX = viewWidth_  / 2.0f;
    float centerY = viewHeight_ / 2.0f;

    glm::vec2 leftArcCenter = glm::vec2(centerX - radius - radius , centerY);
    glm::vec2 rightArcCenter = glm::vec2(centerX + radius + radius , centerY);

    renderEngine_->renderArc(leftArcCenter[0] , leftArcCenter[1] 
        , radius , 90.0f , 270.0f ,false, paint);

    std::vector<float> line1(2*2);
    line1[0] = leftArcCenter[0] + paint.stokenWidth / 2.0f;
    line1[1] = leftArcCenter[1] + radius;

    line1[2] = rightArcCenter[0] - paint.stokenWidth / 2.0f;
    line1[3] = rightArcCenter[1] + radius;
    renderEngine_->renderLines(line1 , paint);

    renderEngine_->renderArc(rightArcCenter[0] , rightArcCenter[1] 
        , radius , 90.0f , 270.0f ,true, paint);

    std::vector<float> line2(2*2);
    line2[0] = leftArcCenter[0] + paint.stokenWidth / 2.0f;
    line2[1] = leftArcCenter[1] - radius;

    line2[2] = rightArcCenter[0]- paint.stokenWidth / 2.0f;
    line2[3] = rightArcCenter[1] - radius;
    renderEngine_->renderLines(line2 , paint);
}

void ShapeDemo::testRenderArc2() {
    Paint paint;
    paint.color = glm::vec4(1.0f , 0.0f , 0.0f , 1.0f);
    paint.fillStyle = Stroken; 
    paint.stokenWidth = 20.0f;

    float cx = 200.0f;
    float cy = viewHeight_ / 2.0f;

    float radius = 200.0f;
    static float angle = 0.0f;
    angle += 1.0f;
    renderEngine_->renderArc(cx , cy , radius , angle , 360.0f ,true, paint);
    renderEngine_->renderArc(cx + radius + radius , cy , radius , angle , 360.0f ,false, paint);
}

void ShapeDemo::testRenderArc(){
    Paint paint;
    paint.color = COLOR_SKY_BLUE;

    float cx = 200.0f;
    float cy = viewHeight_ / 2.0f;

    float radius = 200.0f;
    static float angle = 0.0f;
    angle += 1.0f;
    renderEngine_->renderArc(cx , cy , radius , angle , 270.0f , true, paint);
}

void ShapeDemo::testRenderLinesStar(){
    glm::vec2 center(viewWidth_ / 2.0f , viewHeight_ / 2.0f);

    const uint8_t iter = 5;
    glm::vec2 p[iter];
    float deltaAngleRad = 2*PI / iter;
    float radius = viewHeight_ / 3.0f;
    float angle = deltaAngleRad / 4.0f;
    for(int i = 0 ; i< iter ;i++){
        p[i][0] = center[0] + radius * glm::cos(angle);
        p[i][1] = center[1] + radius * glm::sin(angle);

        angle += deltaAngleRad;
    }//end for i

    std::vector<float> points(2*(iter + 1));
    points[0] = p[0][0];
    points[1] = p[0][1];

    points[2] = p[2][0];
    points[3] = p[2][1];

    points[4] = p[4][0];
    points[5] = p[4][1];

    points[6] = p[1][0];
    points[7] = p[1][1];

    points[8] = p[3][0];
    points[9] = p[3][1];

    points[10] = p[0][0];
    points[11] = p[0][1];

    Paint paint;
    paint.stokenWidth = 16.0f;
    paint.color = glm::vec4(1.0f , 0.0f , 0.0f , 1.0f);
    renderEngine_->renderLines(points , paint);
}

void ShapeDemo::testRenderLines2(){
    Paint paint;
    paint.stokenWidth = 20.0f;
    paint.color = glm::vec4(1.0f , 1.0f , 0.0f , 1.0f);

    std::vector<float> points(2 * 3);
    points[0] = 0.0f;
    points[1] = 0.0f;

    points[2] = viewWidth_ / 2.0f;
    points[3] = viewHeight_ / 2.0f;

    points[4] = viewWidth_;
    points[5] = 0.0f;

    renderEngine_->renderLines(points , paint);
}

void ShapeDemo::testRenderLines(){
    Paint paint;
    paint.stokenWidth = 8.0f;
    paint.color = glm::vec4(1.0f , 1.0f , 1.0f , 1.0f);

    std::vector<float> points(2 * 3);
    points[0] = 30.0f;
    points[1] = 30.0f;

    points[2] = viewWidth_ / 2.0f;
    points[3] = viewHeight_ / 2.0f;

    points[4] = viewWidth_;
    points[5] = 0.0f;

    renderEngine_->renderLines(points , paint);
}

void ShapeDemo::testRenderShapeRect(){
    Rect rect;

    rect.width = viewHeight_ / 4.0f;
    rect.height = viewHeight_ / 2.0f;
    rect.left = 0.0f;
    rect.top = rect.height;

    Paint paint;
    paint.color = COLOR_SKY_BLUE;
    paint.fillStyle = Filled;
    
    static float angle = 0.0f;
    glm::mat4 mat(1.0f);
    auto center = rect.center();
    mat = glm::translate(mat , glm::vec3(center.x , center.y , 0.0f));
    mat = glm::rotate(mat , glm::radians(angle) , glm::vec3(0.0, 0.0 , 1.0f));
    // mat = glm::scale(mat , glm::vec3(1.0f , 1.0f , 1.0f));
    mat = glm::translate(mat , glm::vec3(-center.x , -center.y , 0.0f));
    // mat = glm::translate(mat , glm::vec3(rect.left , 0.0f , 0.0f));

    renderEngine_->renderRect(rect , mat , paint);
    angle++;
}

void ShapeDemo::testRenderShapeRect2(){
    Rect rect;

    rect.width = viewHeight_ / 4.0f;
    rect.height = viewHeight_ / 2.0f;
    rect.left = viewWidth_ / 2.0f - rect.width / 2.0f;
    rect.top = viewHeight_/ 2.0f + rect.height/2.0f;

    Paint paint;
    paint.color = COLOR_SKY_BLUE;
    paint.fillStyle = Stroken;
    paint.stokenWidth = 20.0f;
    
    static float angle = 0.0f;
    glm::mat4 mat(1.0f);
    auto center = rect.center();
    mat = glm::translate(mat , glm::vec3(center.x , center.y , 0.0f));
    mat = glm::rotate(mat , glm::radians(angle) , glm::vec3(0.0, 0.0 , 1.0f));
    mat = glm::translate(mat , glm::vec3(-center.x , -center.y , 0.0f));
    
    renderEngine_->renderRect(rect , mat , paint);
    angle++;
}

void ShapeDemo::dispose(){
    appContext->removeEventActionCallback(this);
}

bool ShapeDemo::onEventAction(int action , float x , float y){
    return false;
}


int VideoDemux::prepare(){
    int ret;
    ret = avformat_open_input(&formatContext , filepath_.c_str() , nullptr , nullptr);
    
    if(ret < 0){
        Logi("video" , "open file %s failed!" , filepath_.c_str());
        return -1;
    }

    Logi("video" , "open file %s success!" , filepath_.c_str());

    ret = avformat_find_stream_info(formatContext , nullptr);
    if(ret < 0){
        std::cout << "find stream info Error!"<< std::endl;
        return -1;
    }

    for(int i = 0 ; i< formatContext->nb_streams ;i++){
        AVMediaType codecType = formatContext->streams[i]->codecpar->codec_type;
        if(codecType == AVMediaType::AVMEDIA_TYPE_VIDEO){
            videoStreamIndex = i;
        }
    }//end for i

    if(videoStreamIndex < 0){
        Logi("video" , "Not found VideoStream"); 
        return -1;
    }

    AVCodecParameters *codecParams = formatContext->streams[videoStreamIndex]->codecpar;
    AVCodecID codecId = codecParams->codec_id;

    pCodec = avcodec_find_decoder(formatContext->streams[videoStreamIndex]->codecpar->codec_id);
    if(pCodec == nullptr){
        Logi("video" , "not found decoder"); 
        return -1;
    }

    videoDuration = formatContext->streams[videoStreamIndex]->duration;

    codecCtx = avcodec_alloc_context3(pCodec);
    avcodec_parameters_to_context(codecCtx , formatContext->streams[videoStreamIndex]->codecpar);

    avcodec_open2(codecCtx , pCodec , nullptr);

    videoWidth = codecCtx->width;
    videoHeight = codecCtx->height;
    videoFormat = codecCtx->pix_fmt;

    ret = av_image_alloc(videoDstBuf, lineSizes , 
            videoWidth  , videoHeight , videoFormat , 1);
    if(ret < 0){
        Logi("video" , "av_image_alloc error"); 
        return -1;
    }
    videoBufSize = ret;

    pixelYBuf = new uint8_t[videoWidth * videoHeight];
    pixelUBuf = new uint8_t[(videoWidth / 2) * (videoHeight / 2)];
    pixelVBuf = new uint8_t[(videoWidth / 2) * (videoHeight / 2)];

    packet = av_packet_alloc();
    frame = av_frame_alloc();

    return 0;
}


VideoInfo VideoDemux::getVideoInfo(){
    VideoInfo info = {
        0 , 
        videoWidth , 
        videoHeight , 
        videoFormat
    };
    return info;
}

bool VideoDemux::decodeVideNextFrame(){
    std::cout << "decodeVideoNextFrame" << std::endl;

    int readFrameRet = -1;
    while((readFrameRet = av_read_frame(formatContext, packet)) >= 0){
        std::cout << "readFrameRet : " << readFrameRet << std::endl;
        if (packet->stream_index == videoStreamIndex){
             std::cout << "avcodec_send_packet " << std::endl;
            int sendPacketRet = avcodec_send_packet(codecCtx, packet);
            // std::cout << "sendPacketRet = " << sendPacketRet << std::endl;

            int ret;
            do{
                ret = avcodec_receive_frame(codecCtx, frame);
                if(ret != 0){
                    break;
                }

                if (codecCtx->codec->type == AVMEDIA_TYPE_VIDEO){
                    // av_image_copy(videoDstBuf , lineSizes 
                    //     ,  const_cast<const uint8_t **>(frame->data)
                    //     , frame->linesize 
                    //     , AVPixelFormat::AV_PIX_FMT_YUV420P
                    //     , frame->width 
                    //     , frame->height);
                    memcpy(pixelYBuf,frame->data[0], videoWidth*videoHeight);
                    memcpy(pixelUBuf,frame->data[1], (videoWidth / 2)*(videoHeight / 2));
                    memcpy(pixelVBuf,frame->data[2], (videoWidth / 2)*(videoHeight / 2));
                    // for(int i = 0 ; i < videoWidth*videoHeight;i++){
                    //     std::cout<< pixelBuf[i] << std::endl;
                    // }

                    std::cout << "read video frame success!!!" << std::endl;
                    av_frame_unref(frame);
                    return true;
                }
                av_frame_unref(frame);
            }while (ret >= 0);
        }

        av_packet_unref(packet);
    }//end while

    return false;
}

void ShapeDemo::renderVideo(){
    // read frame from file
    bool hasVideoFrame = videoDemux_->decodeVideNextFrame();
    // std::cout << "hasVideoFrame = " << hasVideoFrame << std::endl;
    if(!hasVideoFrame){
        return;
    }

    // std::cout <<"before get texture  ->" << glGetError() << std::endl;
    //update texture data
    // glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D , videoTextureY_->getTextureId());
    glTexSubImage2D(GL_TEXTURE_2D , 
        0 , 0,  0,
        videoTextureY_->getWidth() , 
        videoTextureY_->getHeight(), 
        GL_RED , GL_UNSIGNED_BYTE, videoDemux_->pixelYBuf);

    glBindTexture(GL_TEXTURE_2D , videoTextureU_->getTextureId());
    glTexSubImage2D(GL_TEXTURE_2D , 
        0 , 0,  0,
        videoTextureU_->getWidth() , 
        videoTextureU_->getHeight(), 
        GL_RED , GL_UNSIGNED_BYTE, videoDemux_->pixelUBuf);

    glBindTexture(GL_TEXTURE_2D , videoTextureV_->getTextureId());
    glTexSubImage2D(GL_TEXTURE_2D , 
        0 , 0,  0,
        videoTextureV_->getWidth() , 
        videoTextureV_->getHeight(), 
        GL_RED , GL_UNSIGNED_BYTE, videoDemux_->pixelVBuf);
    glBindTexture(GL_TEXTURE_2D , 0);


    // std::cout<<"after get texture  ->" << glGetError() << std::endl;
    
    auto videoInfo = videoDemux_->getVideoInfo();
    float ratio = static_cast<float>(videoInfo.videoFrameWidth) / static_cast<float>(videoInfo.videoFrameHeight);

    // render 
    Rect rect;
    rect.height = viewHeight_;
    rect.width = ratio * viewHeight_;
   
    rect.left = viewWidth_ / 2.0f - rect.width / 2.0f;
    rect.top = viewHeight_;
    renderEngine_->renderTextureShader(customTextureShader_ , 
        rect , 
        videoTextureY_->getTextureId() , [this](){
            customTextureShader_.setUniformInt("texY" , 0);
            customTextureShader_.setUniformInt("texU" , 1);
            customTextureShader_.setUniformInt("texV" , 2);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D , videoTextureY_->getTextureId());
            
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D , videoTextureU_->getTextureId());

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D , videoTextureV_->getTextureId());
        });
}

VideoDemux::~VideoDemux(){
    Logi("video_demux" , "~ deconstruct");

    if(packet != nullptr){
        av_packet_free(&packet);
        packet = nullptr;
    }

    av_freep(videoDstBuf[0]);

    if(codecCtx != nullptr){
        avcodec_free_context(&codecCtx);
        codecCtx = nullptr;
    }

    // if(pCodec != nullptr){
    //     pCodec = nullptr;
    // }

    if(formatContext != nullptr){
        avformat_free_context(formatContext);
        formatContext = nullptr;
    }

    if(pixelYBuf != nullptr){
        delete[] pixelYBuf;
    }

    if(pixelUBuf != nullptr){
        delete[] pixelUBuf;
    }

    if(pixelVBuf != nullptr){
        delete[] pixelVBuf;
    }
}

