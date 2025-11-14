/**
 * MIT License
 *
 * Copyright (C) 2025 Huawei Device Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <vector>
#include <string>
#include <multimedia/image_framework/image/pixelmap_native.h>
#include <multimedia/image_framework/image/image_packer_native.h>
#include "BaseNativeEngineViewComponentInstance.h"

namespace rnoh {
    BaseNativeEngineViewComponentInstance::BaseNativeEngineViewComponentInstance(Context context)
        : CppComponentInstance(std::move(context)) {}

    BaseNativeEngineViewComponentInstance::~BaseNativeEngineViewComponentInstance() {}

    void BaseNativeEngineViewComponentInstance::onChildInserted(ComponentInstance::Shared const &childComponentInstance,
                                                            std::size_t index) {
        CppComponentInstance::onChildInserted(childComponentInstance, index);
    }

    void BaseNativeEngineViewComponentInstance::onChildRemoved(ComponentInstance::Shared const &childComponentInstance) {
        CppComponentInstance::onChildRemoved(childComponentInstance);
    };

    RNCEngineNode &BaseNativeEngineViewComponentInstance::getLocalRootArkUINode() {
        return m_EngineNode;
    }

    void BaseNativeEngineViewComponentInstance::onPropsChanged(SharedConcreteProps const &props) {
        CppComponentInstance::onPropsChanged(props);
        if (props == nullptr) {
            return;
        }

        DLOG(INFO) << "onPropsChanged antiAliasing:" << props->antiAliasing;
        DLOG(INFO) << "onPropsChanged isTransparent:" << props->isTransparent;
        DLOG(INFO) << "onPropsChanged androidView:" << props->androidView;

        if (props->antiAliasing) {
            m_EngineNode.setAntiAliasing(props->antiAliasing);
        }
        if (props->isTransparent) {
            m_EngineNode.setIsTransparent(props->isTransparent);
        }
        m_EngineNode.setAndroidView(props->androidView);
    }

    void BaseNativeEngineViewComponentInstance::onSnapshotDataReturned(std::string data) {
        if (m_eventEmitter) {
            DLOG(INFO) << "onSnapshotDataReturned send data";
            m_eventEmitter->onSnapshotDataReturned({data});
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

    void BaseNativeEngineViewComponentInstance::onSnapshoting() {
        OH_PixelmapNative *pixelmap = nullptr;
        OH_Pixelmap_ImageInfo *imageInfo = nullptr;
        OH_ImagePackerNative *imagePacker = nullptr;
        std::string base64Result = "";

        try {
            // 获取节点快照
            int32_t code = OH_ArkUI_GetNodeSnapshot(m_EngineNode.getArkUINodeHandle(), nullptr, &pixelmap);
            if (code != ARKUI_ERROR_CODE_NO_ERROR) {
                DLOG(ERROR) << "OH_ArkUI_GetNodeSnapshot failfail, errCode: " << code;
                throw std::runtime_error("OH_ArkUI_GetNodeSnapshot fail");
            }

            // 创建图像信息对象
            Image_ErrorCode errCode = OH_PixelmapImageInfo_Create(&imageInfo);
            if (errCode != IMAGE_SUCCESS) {
                DLOG(ERROR) << "OH_PixelmapImageInfo_Create fail, errCode: " << errCode;
                throw std::runtime_error("OH_PixelmapImageInfo_Create fail");
            }

            // 获取图像信息
            errCode = OH_PixelmapNative_GetImageInfo(pixelmap, imageInfo);
            if (errCode != IMAGE_SUCCESS) {
                DLOG(ERROR) << "OH_PixelmapNative_GetImageInfo fail, errCode: " << errCode;
                throw std::runtime_error("OH_PixelmapNative_GetImageInfo fail");
            }

            // 获取宽度
            uint32_t width;
            errCode = OH_PixelmapImageInfo_GetWidth(imageInfo, &width);
            if (errCode != IMAGE_SUCCESS) {
                DLOG(ERROR) << "OH_PixelmapImageInfo_GetWidth fail, errCode: " << errCode;
                throw std::runtime_error("OH_PixelmapImageInfo_GetWidth fail");
            }

            // 获取高度
            uint32_t height;
            errCode = OH_PixelmapImageInfo_GetHeight(imageInfo, &height);
            if (errCode != IMAGE_SUCCESS) {
                DLOG(ERROR) << "OH_PixelmapImageInfo_GetHeight fail, errCode: " << errCode;
                throw std::runtime_error("OH_PixelmapImageInfo_GetHeight fail");
            }

            // 获取像素格式
            int32_t pixelFormat;
            errCode = OH_PixelmapImageInfo_GetPixelFormat(imageInfo, &pixelFormat);
            if (errCode != IMAGE_SUCCESS) {
                DLOG(ERROR) << "OH_PixelmapImageInfo_GetPixelFormat fail, errCode: " << errCode;
                throw std::runtime_error("OH_PixelmapImageInfo_GetPixelFormat fail");
            }

            // 检查图像尺寸是否有效
            if (width == 0 || height == 0) {
                DLOG(ERROR) << "Invalid image size: " << width << "x" << height;
                throw std::runtime_error("Invalid image size");
            }

            // 分配像素数据缓冲区（RGBA格式，每像素4字节）
            size_t bufferSize = width * height * 4;
            std::vector<uint8_t> destination(bufferSize);
            
            // 读取像素数据
            errCode = OH_PixelmapNative_ReadPixels(pixelmap, destination.data(), &bufferSize);
            if (errCode != IMAGE_SUCCESS) {
                DLOG(ERROR) << "OH_PixelmapNative_ReadPixels fail, errCode: " << errCode;
                throw std::runtime_error("OH_PixelmapNative_ReadPixels fail");
            }

            // 创建ImagePacker实例
            errCode = OH_ImagePackerNative_Create(&imagePacker);
            if (errCode != IMAGE_SUCCESS) {
                DLOG(ERROR) << "OH_ImagePackerNative_Create fail, errCode: " << errCode;
                throw std::runtime_error("OH_ImagePackerNative_Create fail");
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
                DLOG(ERROR) << "OH_ImagePackerNative_PackToDataFromPixelmap fail, errCode: " << errCode;
                throw std::runtime_error("OH_ImagePackerNative_PackToDataFromPixelmap fail");
            }
            
            // 转换为Base64编码
            base64Result = base64Encode(image.data(), imageSize);
            if (base64Result.empty()) {
                DLOG(ERROR) << "base64Encode fail";
                throw std::runtime_error("base64Encode fail");
            }
            
            DLOG(INFO) << "Snapshort success, width: " << width << ", height: " << height << ", size: " << base64Result.length();
        } catch (const std::exception& e) {
            DLOG(ERROR) << "Snapshort exception: " << e.what();
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
    };

    void BaseNativeEngineViewComponentInstance::handleCommand(std::string const &commandName, folly::dynamic const &args) {
        DLOG(INFO) << "handleCommand:" << commandName;
        if (commandName == "takeSnapshot") {
        this->onSnapshoting();
        }
    }
} // namespace rnoh