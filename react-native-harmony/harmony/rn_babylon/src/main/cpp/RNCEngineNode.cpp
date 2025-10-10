//
// Created on 2025/8/13.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#include "RNCEngineNode.h"
#include "RNBabylonNativeBridge.h"
#include "RNOH/arkui/NativeNodeApi.h"
#include <ace/xcomponent/native_interface_xcomponent.h>
namespace rnoh {


void RNCEngineNode::OnSurfaceCreated(OH_NativeXComponent *component, void *window) {
    DLOG(INFO) << "RNBabylon OnSurfaceCreated";
    if (window && component) {
        uint64_t width = 0, height = 0;
        int status = OH_NativeXComponent_GetXComponentSize(component, window, &width, &height);
        if (status == OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
            auto node = getNode(component);
            if (node) {
                static_cast<RNCEngineNode *>(node)->UpdateView(window, width, height);
                static_cast<RNCEngineNode *>(node)->startRenderLoop();
            }
        }
    }
}
void RNCEngineNode::OnSurfaceChanged(OH_NativeXComponent *component, void *window) {
    DLOG(INFO) << "RNBabylon OnSurfaceChanged";
    if (window && component) {
        uint64_t width = 0, height = 0;
        int status = OH_NativeXComponent_GetXComponentSize(component, window, &width, &height);
        if (status == OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
            auto node = getNode(component);
            if (node) {
                static_cast<RNCEngineNode *>(node)->UpdateView(window, width, height);
            }
        }
    }
}
void RNCEngineNode::OnSurfaceDestroyed(OH_NativeXComponent *component, void *window) {
    DLOG(INFO) << "RNBabylon OnSurfaceDestroyed";
    auto node = getNode(component);
    if (node) {
        static_cast<RNCEngineNode *>(node)->stopRenderLoop();
    }
}
void RNCEngineNode::DispatchTouchEvent(OH_NativeXComponent *component, void *window) {
    DLOG(INFO) << "RNBabylon DispatchTouchEvent";
    if (component && window) {
        auto node = getNode(component);
        if (node) {
            static_cast<RNCEngineNode *>(node)->reportMotionEvent(component, window);
        }
    }
}


void RNCEngineNode::OnXRSurfaceCreated(OH_NativeXComponent *component, void *window) {
    DLOG(INFO) << "RNBabylon OnXRSurfaceCreated";
    if (component && window) {
        auto node = getNode(component);
        if (node) {
            static_cast<RNCEngineNode *>(node)->UpdateXRView(window);
        }
    }
}
void RNCEngineNode::OnXRSurfaceChanged(OH_NativeXComponent *component, void *window) {
    DLOG(INFO) << "RNBabylon OnXRSurfaceChanged";
    if (component && window) {
        auto node = getNode(component);
        if (node) {
            static_cast<RNCEngineNode *>(node)->UpdateXRView(window);
        }
    }
}
void RNCEngineNode::OnXRSurfaceDestroyed(OH_NativeXComponent *component, void *window) {
    DLOG(INFO) << "RNBabylon OnXRSurfaceDestroyed";
    auto node = getNode(component);
    if (node) {
        static_cast<RNCEngineNode *>(node)->UpdateXRView(nullptr);
    }
}


void RNCEngineNode::onFrameCallback(OH_NativeXComponent *component, uint64_t timestamp, uint64_t targetTimestamp) {
    auto *instance = static_cast<RNCEngineNode *>(getNode(component));
    if (!instance) {
        return;
    }
    // 切换XR
    bool isXR = instance->isXRActive();
    ArkUI_NumberValue visible[] = {{.u32 = isXR ? ARKUI_VISIBILITY_VISIBLE : ARKUI_VISIBILITY_NONE}};
    ArkUI_AttributeItem visibleAttr = {visible, 1};
    NativeNodeApi::getInstance()->setAttribute(instance->m_xrNodeHandle, NODE_VISIBILITY, &visibleAttr);

    instance->renderView();

    // 继续注册下一帧回调？（保持循环）
    if (instance->m_IsRendering) { // TODO
        OH_NativeXComponent_RegisterOnFrameCallback(component, onFrameCallback);
    }
}


RNCEngineNode::RNCEngineNode() : ArkUINode(NativeNodeApi::getInstance()->createNode(ArkUI_NodeType::ARKUI_NODE_STACK)) {

    createGeneralXComponent();
    createXRXComponent();
    // 添加
    NativeNodeApi::getInstance()->insertChildAt(m_nodeHandle, m_General_NodeHandle, 0);
    NativeNodeApi::getInstance()->insertChildAt(m_nodeHandle, m_xrNodeHandle, 1); // XR保持在最上层

    if (m_nativeXComponent) {
        std::lock_guard<std::mutex> lock(s_Mutex);
        s_componentMap[m_nativeXComponent] = this;
        s_componentMap[m_xr_nativeXComponent] = this;
    }
}

RNCEngineNode::~RNCEngineNode() {
    stopRenderLoop();
    if (m_General_NodeHandle) {
        NativeNodeApi::getInstance()->removeChild(m_nodeHandle, m_General_NodeHandle);
        NativeNodeApi::getInstance()->disposeNode(m_General_NodeHandle);
        m_General_NodeHandle = nullptr;
    }
    if (m_xrNodeHandle) {
        NativeNodeApi::getInstance()->removeChild(m_nodeHandle, m_xrNodeHandle);
        NativeNodeApi::getInstance()->disposeNode(m_xrNodeHandle);
        m_xrNodeHandle = nullptr;
    }
    if (m_nativeXComponent != nullptr || m_xr_nativeXComponent != nullptr) {
        std::lock_guard<std::mutex> lock(s_Mutex);
        s_componentMap.erase(m_nativeXComponent);
        s_componentMap.erase(m_xr_nativeXComponent);
        m_nativeXComponent = nullptr;
        m_xr_nativeXComponent = nullptr;
    }
}

void RNCEngineNode::createGeneralXComponent() {
    m_General_NodeHandle = NativeNodeApi::getInstance()->createNode(ArkUI_NodeType::ARKUI_NODE_XCOMPONENT);

    m_xComponentCallback_.OnSurfaceCreated = OnSurfaceCreated;
    m_xComponentCallback_.OnSurfaceChanged = OnSurfaceChanged;
    m_xComponentCallback_.OnSurfaceDestroyed = OnSurfaceDestroyed;
    m_xComponentCallback_.DispatchTouchEvent = DispatchTouchEvent;

    ArkUI_NumberValue surfaceTypeValue[] = {{.u32 = ARKUI_XCOMPONENT_TYPE_SURFACE}};
    ArkUI_AttributeItem surfaceType = {surfaceTypeValue, sizeof(surfaceTypeValue) / sizeof(ArkUI_NumberValue)};
    NativeNodeApi::getInstance()->setAttribute(m_General_NodeHandle, NODE_XCOMPONENT_TYPE, &surfaceType);

    m_nativeXComponent = OH_NativeXComponent_GetNativeXComponent(m_General_NodeHandle);
    OH_NativeXComponent_RegisterCallback(m_nativeXComponent, &m_xComponentCallback_);
}

void RNCEngineNode::createXRXComponent() {
    // XR
    m_xrNodeHandle = NativeNodeApi::getInstance()->createNode(ArkUI_NodeType::ARKUI_NODE_XCOMPONENT);
    m_xrXComponentCallback.OnSurfaceCreated = OnXRSurfaceCreated;
    m_xrXComponentCallback.OnSurfaceChanged = OnXRSurfaceChanged;
    m_xrXComponentCallback.OnSurfaceDestroyed = OnXRSurfaceDestroyed;
    m_xrXComponentCallback.DispatchTouchEvent = [](OH_NativeXComponent *, void *) {};

    ArkUI_NumberValue surfaceTypeValue[] = {{.u32 = ARKUI_XCOMPONENT_TYPE_SURFACE}};
    ArkUI_AttributeItem surfaceType = {surfaceTypeValue, sizeof(surfaceTypeValue) / sizeof(ArkUI_NumberValue)};
    NativeNodeApi::getInstance()->setAttribute(m_xrNodeHandle, NODE_XCOMPONENT_TYPE, &surfaceType);

    m_xr_nativeXComponent = OH_NativeXComponent_GetNativeXComponent(m_xrNodeHandle);
    OH_NativeXComponent_RegisterCallback(m_xr_nativeXComponent, &m_xrXComponentCallback);

    // 先隐藏XR
    ArkUI_NumberValue visible[] = {{.u32 = ARKUI_VISIBILITY_NONE}};
    ArkUI_AttributeItem visibleAttr = {visible, 1};
    NativeNodeApi::getInstance()->setAttribute(m_xrNodeHandle, NODE_VISIBILITY, &visibleAttr);
}

void RNCEngineNode::setAntiAliasing(int32_t value) { UpdateMSAA(value); }
void RNCEngineNode::setAndroidView(std::string androidView) {
    setIsTransparentAndAndroidView(m_IsTransparent, androidView);
}
void RNCEngineNode::setIsTransparent(bool isTransparent) { setIsTransparentAndAndroidView(isTransparent, m_View); }


RNCEngineNode *RNCEngineNode::getNode(OH_NativeXComponent *component) {
    if (component == nullptr) {
        return nullptr;
    }
    std::lock_guard<std::mutex> lock(s_Mutex);
    auto it = s_componentMap.find(component);
    if (it != s_componentMap.end()) {
        return it->second;
    }
    return nullptr;
}

void RNCEngineNode::startRenderLoop() {
    if (m_IsRendering) {
        return;
    }
    m_IsRendering = true;
    auto *nativeXComponent = OH_NativeXComponent_GetNativeXComponent(m_General_NodeHandle);
    OH_NativeXComponent_RegisterOnFrameCallback(nativeXComponent, onFrameCallback);
}

void RNCEngineNode::stopRenderLoop() {
    if (!m_IsRendering) {
        return;
    }
    m_IsRendering = false;
    auto *nativeXComponent = OH_NativeXComponent_GetNativeXComponent(m_General_NodeHandle);
    if (nativeXComponent) {
        OH_NativeXComponent_UnregisterOnFrameCallback(nativeXComponent);
    }
}

void RNCEngineNode::setIsTransparentAndAndroidView(bool transparent, std::string view) {
    if (m_IsTransparent == transparent && m_View == view) {
        return;
    }
    m_IsTransparent = transparent;
    m_View = view;

    if (transparent) {
        // 设置XComponent背景透明 0x00000000
        ArkUI_NumberValue preparedColorValue[] = {{.u32 = 0x00000000}};
        ArkUI_AttributeItem colorItem = {preparedColorValue, sizeof(preparedColorValue) / sizeof(ArkUI_NumberValue)};
        NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_BACKGROUND_COLOR, &colorItem);
    }

    if (view == "SurfaceViewZTopMost") {              // 指定层级
        ArkUI_NumberValue zOrder[] = {{.i32 = 1000}}; // TODO 1000 与 500
        ArkUI_AttributeItem zAttr = {zOrder, 1};
        NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_Z_INDEX, &zAttr);
    } else if (view == "SurfaceViewZMediaOverlay") {
        ArkUI_NumberValue zOrder[] = {{.i32 = 500}};
        ArkUI_AttributeItem zAttr = {zOrder, 1};
        NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_Z_INDEX, &zAttr);
    }

    if (view == "TextureView") {
        ArkUI_NumberValue type[] = {{.u32 = ARKUI_XCOMPONENT_TYPE_TEXTURE}};
        ArkUI_AttributeItem typeAttr = {type, 1};
        NativeNodeApi::getInstance()->setAttribute(m_General_NodeHandle, NODE_XCOMPONENT_TYPE, &typeAttr);
    } else {
        ArkUI_NumberValue type[] = {{.u32 = ARKUI_XCOMPONENT_TYPE_SURFACE}};
        ArkUI_AttributeItem typeAttr = {type, 1};
        NativeNodeApi::getInstance()->setAttribute(m_General_NodeHandle, NODE_XCOMPONENT_TYPE, &typeAttr);
    }
}


void RNCEngineNode::UpdateView(void *window, uint64_t width, uint64_t height) {
    BabyUpdateView(static_cast<OHNativeWindow *>(window), width, height);
}

void RNCEngineNode::UpdateXRView(void *window) { BabyUpdateXRView(static_cast<OHNativeWindow *>(window)); }

void RNCEngineNode::UpdateMSAA(int32_t value) { BabyUpdateMSAA(value); }

bool RNCEngineNode::isXRActive() { return BabyIsXRActive(); }

void RNCEngineNode::renderView() { BabyRenderView(); }


void RNCEngineNode::reportMotionEvent(OH_NativeXComponent *component, void *window) {
    OH_NativeXComponent_TouchEvent event{};
    if (OH_NativeXComponent_GetTouchEvent(component, window, &event) == OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        bool isPointerDown = event.type == OH_NATIVEXCOMPONENT_DOWN;
        bool isPointerUp = event.type == OH_NATIVEXCOMPONENT_UP;
        bool isPointerMove = event.type == OH_NATIVEXCOMPONENT_MOVE;
        if (isPointerDown || isPointerUp) {
            int pointerId = event.id;
            int x = event.x;
            int y = event.y;
            BabySetTouchButtonState(pointerId, isPointerDown, x, y);
        } else if (isPointerMove) {
            auto touchPoints = event.touchPoints;
            int length = sizeof(touchPoints) / sizeof(touchPoints[0]);
            for (int pointerIndex = 0; pointerIndex < length; pointerIndex++) {
                int pointerId = touchPoints[pointerIndex].id;
                int x = touchPoints[pointerIndex].x;
                int y = touchPoints[pointerIndex].y;
                BabySetTouchPosition(pointerId, x, y);
            }
        }
    }
}
} // namespace rnoh
