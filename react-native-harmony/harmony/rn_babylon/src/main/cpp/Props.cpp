//
// Created on 2025/8/13.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#include "Props.h"
#include <react/renderer/core/PropsParserContext.h>
#include <react/renderer/core/propsConversions.h>

namespace facebook {
namespace react {


EngineViewProps::EngineViewProps(const PropsParserContext &context, const EngineViewProps &sourceProps,
                                 const RawProps &rawProps)
    : ViewProps(context, sourceProps, rawProps),

      isTransparent(convertRawProp(context, rawProps, "isTransparent", sourceProps.isTransparent, {false})),
      antiAliasing(convertRawProp(context, rawProps, "antiAliasing", sourceProps.antiAliasing, {0})),
      androidView(convertRawProp(context, rawProps, "androidView", sourceProps.androidView, {""})) {}

} // namespace react
} // namespace facebook