//
// Created on 2025/8/13.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#ifndef HARMONY_RNCNATIVEENGINEVIEWCOMPONENTINSTANCE_H
#define HARMONY_RNCNATIVEENGINEVIEWCOMPONENTINSTANCE_H
#include "RNCEngineNode.h"
#include "RNOH/CppComponentInstance.h"
#include "ShadowNodes.h"

namespace rnoh {
class RNCNativeEngineViewComponentInstance : public rnoh::CppComponentInstance<facebook::react::EngineViewShadowNode> {
public:
    RNCNativeEngineViewComponentInstance(Context context);
    ~RNCNativeEngineViewComponentInstance();
    void onChildInserted(ComponentInstance::Shared const &childComponentInstance, std::size_t index) override;

    void onChildRemoved(ComponentInstance::Shared const &childComponentInstance) override;
    void onSnapshoting();

    void onPropsChanged(SharedConcreteProps const &props) override;
    void handleCommand(std::string const &commandName, folly::dynamic const &args) override;
    RNCEngineNode &getLocalRootArkUINode() override;
    void onSnapshotDataReturned(std::string data) ;
    
private:
    RNCEngineNode m_EngineNode;
};

} // namespace rnoh

#endif // HARMONY_RNCNATIVEENGINEVIEWCOMPONENTINSTANCE_H
