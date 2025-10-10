//
// Created on 2025/8/13.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#ifndef HARMONY_PROPS_H
#define HARMONY_PROPS_H

#include <jsi/jsi.h>
#include <react/renderer/components/view/ViewProps.h>
#include <react/renderer/core/PropsParserContext.h>
#include <react/renderer/core/propsConversions.h>
#include <react/renderer/graphics/Color.h>
#include <string>

namespace facebook {
namespace react {

class JSI_EXPORT EngineViewProps final : public ViewProps {
public:
    EngineViewProps() = default;
    EngineViewProps(const PropsParserContext &context, const EngineViewProps &sourceProps, const RawProps &rawProps);

#pragma mark - Props

    bool isTransparent{true};
    int32_t antiAliasing{0};
    std::string androidView{""};
};

} // namespace react
} // namespace facebook

#endif // HARMONY_PROPS_H
