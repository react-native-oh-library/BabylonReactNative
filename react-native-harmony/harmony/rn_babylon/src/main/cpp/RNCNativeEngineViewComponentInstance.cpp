//
// Created on 2025/8/13.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#include <vector>
#include <string>
#include <multimedia/image_framework/image/pixelmap_native.h>
#include <multimedia/image_framework/image/image_packer_native.h>
#include "RNCNativeEngineViewComponentInstance.h"

namespace rnoh {
RNCNativeEngineViewComponentInstance::RNCNativeEngineViewComponentInstance(Context context)
    : CppComponentInstance(std::move(context)) {}

RNCNativeEngineViewComponentInstance::~RNCNativeEngineViewComponentInstance() {}

void RNCNativeEngineViewComponentInstance::onChildInserted(ComponentInstance::Shared const &childComponentInstance,
                                                           std::size_t index) {
    CppComponentInstance::onChildInserted(childComponentInstance, index);
    auto mWidth = childComponentInstance->getLayoutMetrics().frame.size.width;
}

void RNCNativeEngineViewComponentInstance::onChildRemoved(ComponentInstance::Shared const &childComponentInstance) {
    CppComponentInstance::onChildRemoved(childComponentInstance);
};

RNCEngineNode &RNCNativeEngineViewComponentInstance::getLocalRootArkUINode() { return m_EngineNode; }

void RNCNativeEngineViewComponentInstance::onPropsChanged(SharedConcreteProps const &props) {
    CppComponentInstance::onPropsChanged(props);
    if (props == nullptr) {
        return;
    }
    DLOG(INFO) << "sssss-------->onPropsChanged antiAliasing:" << props->antiAliasing;
    DLOG(INFO) << "sssss-------->onPropsChanged isTransparent:" << props->isTransparent;
    DLOG(INFO) << "sssss-------->onPropsChanged androidView:" << props->androidView;
    if (props->antiAliasing) {
        m_EngineNode.setAntiAliasing(props->antiAliasing);
    }
    if (props->isTransparent) {
        m_EngineNode.setIsTransparent(props->isTransparent);
    }
    m_EngineNode.setAndroidView(props->androidView);
}

void RNCNativeEngineViewComponentInstance::onSnapshotDataReturned(std::string data) {
    DLOG(INFO) << "onSnapshotDataReturned 准备发送";
    if (m_eventEmitter) {
        DLOG(INFO) << "onSnapshotDataReturned 准备发送 1";
        m_eventEmitter->onSnapshotDataReturned({data});
        DLOG(INFO) << "onSnapshotDataReturned 已发送";
    }
};

static const std::string base64Chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

std::string base64Encode(const uint8_t* data, size_t size) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char charArray3[3];
    unsigned char charArray4[4];

    while (size--) {
        charArray3[i++] = *(data++);
        if (i == 3) {
            charArray4[0] = (charArray3[0] & 0xfc) >> 2;
            charArray4[1] = ((charArray3[0] & 0x03) << 4) + ((charArray3[1] & 0xf0) >> 4);
            charArray4[2] = ((charArray3[1] & 0x0f) << 2) + ((charArray3[2] & 0xc0) >> 6);
            charArray4[3] = charArray3[2] & 0x3f;

            for(i = 0; i < 4 ; i++) {
                ret += base64Chars[charArray4[i]];
            }
            
            i = 0;
        }
    }

    if (i) {
        for(j = i; j < 3; j++) {
            charArray3[j] = '\0';
        }

        charArray4[0] = (charArray3[0] & 0xfc) >> 2;
        charArray4[1] = ((charArray3[0] & 0x03) << 4) + ((charArray3[1] & 0xf0) >> 4);
        charArray4[2] = ((charArray3[1] & 0x0f) << 2) + ((charArray3[2] & 0xc0) >> 6);
        charArray4[3] = charArray3[2] & 0x3f;

        for (j = 0; j < i + 1; j++) {
            ret += base64Chars[charArray4[j]];
        }

        while (i++ < 3) {
            ret += '=';
        }
    }

    return ret;
}

void RNCNativeEngineViewComponentInstance::onSnapshoting() {
    DLOG(INFO) << "onSnapshoting 准备开始截图";

    // 初始化变量
    OH_PixelmapNative *pixelmap = nullptr;
    OH_Pixelmap_ImageInfo *imageInfo = nullptr;
    OH_ImagePackerNative *imagePacker = nullptr;
    std::string base64Result = "";

    try {
        // 获取节点快照
        int32_t code = OH_ArkUI_GetNodeSnapshot(m_EngineNode.getArkUINodeHandle(), nullptr, &pixelmap);
        if (code != ARKUI_ERROR_CODE_NO_ERROR) {
            DLOG(ERROR) << "OH_ArkUI_GetNodeSnapshot 失败: " << code;
            throw std::runtime_error("获取快照失败");
        }

        // 创建图像信息对象
        Image_ErrorCode errCode = OH_PixelmapImageInfo_Create(&imageInfo);
        if (errCode != IMAGE_SUCCESS) {
            DLOG(ERROR) << "OH_PixelmapImageInfo_Create 失败:" << errCode;
            throw std::runtime_error("创建图像信息失败");
        }

        // 获取图像信息
        errCode = OH_PixelmapNative_GetImageInfo(pixelmap, imageInfo);
        if (errCode != IMAGE_SUCCESS) {
            DLOG(ERROR) << "OH_PixelmapNative_GetImageInfo 失败:" << errCode;
            throw std::runtime_error("获取图像信息失败");
        }

        // 获取宽度
        uint32_t width;
        errCode = OH_PixelmapImageInfo_GetWidth(imageInfo, &width);
        if (errCode != IMAGE_SUCCESS) {
            DLOG(ERROR) << "OH_PixelmapImageInfo_GetWidth 失败:" << errCode;
            throw std::runtime_error("获取宽度失败");
        }

        // 获取高度
        uint32_t height;
        errCode = OH_PixelmapImageInfo_GetHeight(imageInfo, &height);
        if (errCode != IMAGE_SUCCESS) {
            DLOG(ERROR) << "OH_PixelmapImageInfo_GetHeight 失败:" << errCode;
            throw std::runtime_error("获取高度失败");
        }

        // 获取像素格式
        int32_t pixelFormat;
        errCode = OH_PixelmapImageInfo_GetPixelFormat(imageInfo, &pixelFormat);
        if (errCode != IMAGE_SUCCESS) {
            DLOG(ERROR) << "OH_PixelmapImageInfo_GetPixelFormat 失败:" << errCode;
            throw std::runtime_error("获取像素格式失败");
        }

        DLOG(INFO) << "图像信息 - 宽度: " << width << ", 高度: " << height << ", 像素格式: " << pixelFormat;

        // 检查图像尺寸是否有效
        if (width == 0 || height == 0) {
            DLOG(ERROR) << "无效的图像尺寸: " << width << "x" << height;
            throw std::runtime_error("无效的图像尺寸");
        }

        // 分配像素数据缓冲区（RGBA格式，每像素4字节）
        size_t bufferSize = width * height * 4;
        std::vector<uint8_t> destination(bufferSize);
        
        // 读取像素数据
        errCode = OH_PixelmapNative_ReadPixels(pixelmap, destination.data(), &bufferSize);
        if (errCode != IMAGE_SUCCESS) {
            DLOG(ERROR) << "OH_PixelmapNative_ReadPixels 失败:" << errCode;
            throw std::runtime_error("读取像素数据失败");
        }
    
        DLOG(INFO) << "成功读取像素数据，大小: " << bufferSize << " 字节";
    
        // 创建ImagePacker实例
        errCode = OH_ImagePackerNative_Create(&imagePacker);
        if (errCode != IMAGE_SUCCESS) {
            DLOG(ERROR) << "OH_ImagePackerNative_Create failed, errCode: " << errCode;
            throw std::runtime_error("创建ImagePacker实例失败");
        }
    
        // 将PixelMap编码成JPEG格式数据
        OH_PackingOptions *option = nullptr;
        OH_PackingOptions_Create(&option);
        char type[] = "image/jpeg";
        Image_MimeType image_MimeType = {type, strlen(type)};
        OH_PackingOptions_SetMimeType(option, &image_MimeType);
    
        size_t imageSize = 0;
        std::vector<uint8_t> image(bufferSize);
        errCode = OH_ImagePackerNative_PackToDataFromPixelmap(imagePacker, option, pixelmap, image.data(), &imageSize);
        if (errCode != IMAGE_SUCCESS) {
            DLOG(ERROR) << "OH_ImagePackerNative_PackToDataFromPixelmap failed, errCode: " << errCode;
            throw std::runtime_error("将PixelMap编码成JPEG格式数据失败");
        }
        
        // 转换为Base64编码
        base64Result = base64Encode(image.data(), imageSize);
        if (base64Result.empty()) {
            DLOG(ERROR) << "Base64编码失败";
            throw std::runtime_error("Base64编码失败");
        }
        
        DLOG(INFO) << "截图成功完成 - 宽度: " << width << ", 高度: " << height
                   << ", 数据长度: " << base64Result.length() << " 字符";
    } catch (const std::exception& e) {
        DLOG(ERROR) << "截图过程中发生异常: " << e.what();
        base64Result = ""; // 确保返回空字符串表示失败
    }
    
    if (imagePacker != nullptr) {
        OH_ImagePackerNative_Release(imagePacker);
    }
    
    if (imageInfo != nullptr) {
        OH_PixelmapImageInfo_Release(imageInfo);
    }

    if (pixelmap != nullptr) {
        OH_PixelmapNative_Release(pixelmap);
    }

    // 发送结果到React Native层
    this->onSnapshotDataReturned(base64Result);

    DLOG(INFO) << "onSnapshoting 截图流程结束";
};

void RNCNativeEngineViewComponentInstance::handleCommand(std::string const &commandName, folly::dynamic const &args) {
    DLOG(INFO) << "handleCommand:" << commandName;
    if (commandName == "takeSnapshot") {
        this->onSnapshoting();
    }
};

}; // namespace rnoh
