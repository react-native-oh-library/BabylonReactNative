//
// Created on 2025/8/13.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#ifndef HARMONY_RNCENGINENODE_H
#define HARMONY_RNCENGINENODE_H
#include "RNOH/arkui/ArkUINode.h"
#include <ace/xcomponent/native_interface_xcomponent.h>
namespace rnoh {
class RNCEngineNode : public ArkUINode {

public:
    RNCEngineNode();
    ~RNCEngineNode() override;
    // 普通
    static void OnSurfaceCreated(OH_NativeXComponent *component, void *window);
    static void OnSurfaceChanged(OH_NativeXComponent *component, void *window);
    static void OnSurfaceDestroyed(OH_NativeXComponent *component, void *window);
    static void DispatchTouchEvent(OH_NativeXComponent *component, void *window);

    // XR
    static void OnXRSurfaceCreated(OH_NativeXComponent *component, void *window);
    static void OnXRSurfaceChanged(OH_NativeXComponent *component, void *window);
    static void OnXRSurfaceDestroyed(OH_NativeXComponent *component, void *window);

    void setAntiAliasing(int32_t value);
    void setAndroidView(std::string androidView);
    void setIsTransparent(bool isTransparent);

private:
    void setIsTransparentAndAndroidView(bool isTransparent, std::string androidView);
    void startRenderLoop();
    void stopRenderLoop();
    static void onFrameCallback(OH_NativeXComponent *component, uint64_t timestamp, uint64_t targetTimestamp);
    static RNCEngineNode *getNode(OH_NativeXComponent *component);

    void UpdateView(void *window, uint64_t width, uint64_t height);
    void UpdateXRView(void *window);
    void UpdateMSAA(int32_t value);
    bool isXRActive();
    void renderView();
    void reportMotionEvent(OH_NativeXComponent *component, void *window);

    void createGeneralXComponent();

    void createXRXComponent();

private:
    OH_NativeXComponent_Callback m_xComponentCallback_;
    OH_NativeXComponent_Callback m_xrXComponentCallback;
    ArkUI_NodeHandle m_General_NodeHandle{nullptr};
    ArkUI_NodeHandle m_xrNodeHandle{nullptr};
    bool m_IsRendering{false};
    bool m_IsTransparent{false};
    std::string m_View{""};
    OH_NativeXComponent *m_nativeXComponent{nullptr};
    OH_NativeXComponent *m_xr_nativeXComponent{nullptr};

    static inline std::unordered_map<OH_NativeXComponent *, RNCEngineNode *> s_componentMap;
    static inline std::mutex s_Mutex;
};
} // namespace rnoh


#endif // HARMONY_RNCENGINENODE_H
