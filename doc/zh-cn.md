> 模板版本：v0.2.2

<p align="center">
  <h1 align="center"> <code>BabylonReactNative</code> </h1>
</p>
<p align="center">
    <a href="https://github.com/BabylonJS/BabylonReactNative">
        <img src="https://img.shields.io/badge/platforms-android%20%7C%20ios%20%7C%20Universal%20Windows%20Platform%20(UWP)%20%7C%20harmony-lightgrey.svg" alt="Supported platforms" />
    </a>
    <a href="https://github.com/BabylonJS/BabylonReactNative/blob/master/LICENSE">
        <img src="https://img.shields.io/badge/license-MIT-green.svg" alt="License" />
    </a>
</p>

> [!TIP] [Github 地址](https://github.com/react-native-oh-library/BabylonReactNative)

该第三方库的仓库Github上，且支持直接从 npm 下载，包名为：@react-native-oh-tpl/babylonjs-react-native和@react-native-oh-tpl/babylonjs-react-native-harmony，具体版本所属关系如下：

| 三方库版本  | 包名                              | 仓库地址 | 发布(Release) | 支持RN版本 |
| ---------- | ------------------------------------------------------------ | ---------- |  ---------- |  ---------- |
| 1.8.7 | @react-native-oh-tpl/babylonjs-react-native和@react-native-oh-tpl/babylonjs-react-native-harmony | [Github](https://github.com/react-native-oh-library/BabylonReactNative)|[Github Releases](https://github.com/react-native-oh-library/BabylonReactNative/releases)|0.72       |

## 安装与使用

请到三方库的 Releases 发布地址查看配套的版本信息：[@react-native-oh-tpl/babylonjs-react-native和@react-native-oh-tpl/babylonjs-react-native-harmony Releases](https://github.com/react-native-oh-library/BabylonReactNative/releases)。

进入到工程目录并输入以下命令：

<!-- tabs:start -->

#### **npm**

```bash
npm install @react-native-oh-tpl/babylonjs-react-native
npm install @react-native-oh-tpl/babylonjs-react-native-harmony
```

#### **yarn**

```bash
yarn add @react-native-oh-tpl/babylonjs-react-native
yarn add @react-native-oh-tpl/babylonjs-react-native-harmony
```

#### **如果在DevEco Studio里使用BabylonReactNative**

```bash
在DevEco Studio的终端里执行: git --version，如果查不到git的版本号，依次执行以下步骤:
1.安装git（git已安装，跳过该步骤）
2.在系统环境变量Path里加入git可执行文件路径（Path里已存在git可执行文件路径，跳过该步骤）
3.复位电脑，让环境变量在DevEco Studio里生效
```

<!-- tabs:end -->

下面的代码展示了这个库的基本使用场景：

> [!WARNING] 使用时 import 的库名不变。

```js
import React, { useState, FunctionComponent, useEffect, useCallback } from 'react';
import { StatusBar, Button, View, Text, ViewProps, Image } from 'react-native';
import { SafeAreaView } from '@react-native-oh-tpl/react-native-safe-area-context';

import { EngineView, useEngine, EngineViewCallbacks } from '@babylonjs/react-native';
 import { Scene, Vector3, ArcRotateCamera, Camera, WebXRSessionManager, SceneLoader, TransformNode, DeviceSourceManager, DeviceType, PointerInput, WebXRTrackingState, IMouseEvent } from '@babylonjs/core';
import '@babylonjs/loaders';
import Slider from '@react-native-community/slider';

const EngineScreen: FunctionComponent<ViewProps> = (props: ViewProps) => {
  const defaultScale = 1;
  const enableSnapshots = false;
  // const enableSnapshots = true;

  const engine = useEngine();
  const [toggleView, setToggleView] = useState(false);
  const [camera, setCamera] = useState<Camera>();
  const [rootNode, setRootNode] = useState<TransformNode>();
  const [scene, setScene] = useState<Scene>();
  const [xrSession, setXrSession] = useState<WebXRSessionManager>();
  const [scale, setScale] = useState<number>(defaultScale);
  const [snapshotData, setSnapshotData] = useState<string>();
  const [engineViewCallbacks, setEngineViewCallbacks] = useState<EngineViewCallbacks>();
  const [trackingState, setTrackingState] = useState<WebXRTrackingState>();

  useEffect(() => {
    if (engine) {
      const scene = new Scene(engine);
      setScene(scene);
      scene.createDefaultCamera(true);
      (scene.activeCamera as ArcRotateCamera).beta -= Math.PI / 8;
      setCamera(scene.activeCamera!);
      scene.createDefaultLight(true);
      const rootNode = new TransformNode('Root Container', scene);
      setRootNode(rootNode);

      const deviceSourceManager = new DeviceSourceManager(engine);
       const handlePointerInput = (event: IMouseEvent) => {
         if (event.inputIndex === PointerInput.Move && event.movementX) {
           rootNode.rotate(Vector3.Down(), event.movementX * 0.005);
        };
      };

      deviceSourceManager.onDeviceConnectedObservable.add(device => {
        if (device.deviceType === DeviceType.Touch) {
           const touch = deviceSourceManager.getDeviceSource(device.deviceType, device.deviceSlot)!;
          touch.onInputChangedObservable.add(touchEvent => {
             handlePointerInput(touchEvent);
          });
        } else if (device.deviceType === DeviceType.Mouse) {
           const mouse = deviceSourceManager.getDeviceSource(device.deviceType, device.deviceSlot)!;
          mouse.onInputChangedObservable.add(mouseEvent => {
            if (mouse.getInput(PointerInput.LeftClick)) {
               handlePointerInput(mouseEvent);
            }
          });
        }
      });

      const transformContainer = new TransformNode('Transform Container', scene);
      transformContainer.parent = rootNode;
      transformContainer.scaling.scaleInPlace(0.2);
      transformContainer.position.y -= .2;

      scene.beforeRender = function () {
        transformContainer.rotate(Vector3.Up(), 0.005 * scene.getAnimationRatio());
      };

      SceneLoader.ImportMeshAsync('', 'https://raw.githubusercontent.com/KhronosGroup/glTF-Sample-Models/master/2.0/BoxAnimated/glTF-Binary/BoxAnimated.glb').then(result => {
        const mesh = result.meshes[0];
        mesh.parent = transformContainer;
      });
    }
  }, [engine]);

  useEffect(() => {
    if (rootNode) {
      rootNode.scaling = new Vector3(scale, scale, scale);
    }
  }, [rootNode, scale]);

  const trackingStateToString = (trackingState: WebXRTrackingState | undefined) : string => {
    return trackingState === undefined ? '' : WebXRTrackingState[trackingState];
  };

  const onToggleXr = useCallback(() => {
    (async () => {
      if (xrSession) {
        await xrSession.exitXRAsync();
      } else {
        if (rootNode !== undefined && scene !== undefined) {
          const xr = await scene.createDefaultXRExperienceAsync({ disableDefaultUI: true, disableTeleportation: true })
          const session = await xr.baseExperience.enterXRAsync('immersive-ar', 'unbounded', xr.renderTarget);
          setXrSession(session);
          session.onXRSessionEnded.add(() => {
            setXrSession(undefined);
            setTrackingState(undefined);
          })

          setTrackingState(xr.baseExperience.camera.trackingState);
          xr.baseExperience.camera.onTrackingStateChanged.add((newTrackingState) => {
            setTrackingState(newTrackingState);
          });

          // TODO: Figure out why getFrontPosition stopped working
          //box.position = (scene.activeCamera as TargetCamera).getFrontPosition(2);
          const cameraRay = scene.activeCamera!.getForwardRay(1);
          rootNode.position = cameraRay.origin.add(cameraRay.direction.scale(cameraRay.length));
          rootNode.rotate(Vector3.Up(), 3.14159);
        }
      }
    })();
  }, [rootNode, scene, xrSession]);

  const onInitialized = useCallback(async(engineViewCallbacks: EngineViewCallbacks) => {
    setEngineViewCallbacks(engineViewCallbacks);
  }, [engine]);

  const onSnapshot = useCallback(async () => {
    if (engineViewCallbacks) {
      setSnapshotData('data:image/jpeg;base64,' + await engineViewCallbacks.takeSnapshot());
    }
  }, [engineViewCallbacks]);

  return (
    <>
      <View style={props.style}>
        <Button title="Toggle EngineView" onPress={() => { setToggleView(!toggleView) }} />
        <Button title={ xrSession ? 'Stop XR' : 'Start XR'} onPress={onToggleXr} />
        { !toggleView &&
          <View style={{flex: 1}}>
            { enableSnapshots && 
              <View style ={{flex: 1}}>
                <Button title={'Take Snapshot'} onPress={onSnapshot}/>
                <Image style={{flex: 1}} source={{uri: snapshotData }} />
              </View>
            }
            <EngineView camera={camera} onInitialized={onInitialized} displayFrameRate={true} antiAliasing={2}/>
            <Slider style={{position: 'absolute', minHeight: 50, margin: 10, left: 0, right: 0, bottom: 0}} minimumValue={0.2} maximumValue={2} step={0.01} value={defaultScale} onValueChange={setScale} />
            <Text style={{color: 'yellow',  position: 'absolute', margin: 3}}>{trackingStateToString(trackingState)}</Text>
          </View>
        }
        { toggleView &&
          <View style={{flex: 1, justifyContent: 'center', alignItems: 'center'}}>
            <Text style={{fontSize: 24}}>EngineView has been removed.</Text>
            <Text style={{fontSize: 12}}>Render loop stopped, but engine is still alive.</Text>
          </View>
        }
      </View>
    </>
  );
};

const App = () => {
  const [toggleScreen, setToggleScreen] = useState(false);

  return (
    <>
      <StatusBar barStyle="dark-content" />
      <SafeAreaView style={{flex: 1, backgroundColor: 'white'}}>
        { !toggleScreen &&
          <EngineScreen style={{flex: 1}} />
        }
        { toggleScreen &&
          <View style={{flex: 1, justifyContent: 'center', alignItems: 'center'}}>
            <Text style={{fontSize: 24}}>EngineScreen has been removed.</Text>
            <Text style={{fontSize: 12}}>Engine has been disposed, and will be recreated.</Text>
          </View>
        }
        <Button title="Toggle EngineScreen" onPress={() => { setToggleScreen(!toggleScreen); }} />
      </SafeAreaView>
    </>
  );
};

export default App;
```

## Link

目前 HarmonyOS 暂不支持 AutoLink，所以 Link 步骤需要手动配置。

首先需要使用 DevEco Studio 打开项目里的 HarmonyOS 工程 `harmony`

### 1.在工程根目录的 `oh-package.json5` 添加 overrides 字段

```json
{
  ...
  "overrides": {
    "@rnoh/react-native-openharmony" : "./react_native_openharmony"
  }
}
```

### 2.引入原生端代码

目前有两种方法：

1. 通过 har 包引入（在 IDE 完善相关功能后该方法会被遗弃，目前首选此方法）；
2. 直接链接源码。

方法一：通过 har 包引入

> [!TIP] har 包位于三方库安装路径的 `harmony` 文件夹下。

打开 `entry/oh-package.json5`，添加以下依赖

```json
"dependencies": {
    "@rnoh/react-native-openharmony": "file:../react_native_openharmony",

    "@react-native-oh-tpl/babylonjs-react-native-harmony": "file:../../node_modules/@react-native-oh-tpl/babylonjs-react-native-harmony/harmony/babylonjs.har"
  }
```

点击右上角的 `sync` 按钮

或者在终端执行：

```bash
cd entry
ohpm install
```

方法二：直接链接源码

> [!TIP] 如需使用直接链接源码，请参考[直接链接源码说明](/zh-cn/link-source-code.md)

### 3.配置 CMakeLists 和引入 BabylonPackage

打开 `entry/src/main/cpp/CMakeLists.txt`，添加：

```diff
```diff
project(rnapp)
cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_SKIP_BUILD_RPATH TRUE)
set(RNOH_APP_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(NODE_MODULES "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../node_modules")
+ set(OH_MODULES "${CMAKE_CURRENT_SOURCE_DIR}/../../../oh_modules")
set(RNOH_CPP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../react-native-harmony/harmony/cpp")
set(LOG_VERBOSITY_LEVEL 1)
set(CMAKE_ASM_FLAGS "-Wno-error=unused-command-line-argument -Qunused-arguments")
set(CMAKE_CXX_FLAGS "-fstack-protector-strong -Wl,-z,relro,-z,now,-z,noexecstack -s -fPIE -pie")
set(WITH_HITRACE_SYSTRACE 1) # for other CMakeLists.txt files to use
add_compile_definitions(WITH_HITRACE_SYSTRACE)

add_subdirectory("${RNOH_CPP_DIR}" ./rn)

# RNOH_BEGIN: manual_package_linking_1
add_subdirectory("../../../../sample_package/src/main/cpp" ./sample-package)
+ add_subdirectory("${OH_MODULES}/@react-native-oh-tpl/babylonjs-react-native-harmony/src/main/cpp" ./babylonjs)
# RNOH_END: manual_package_linking_1

file(GLOB GENERATED_CPP_FILES "./generated/*.cpp")

add_library(rnoh_app SHARED
    ${GENERATED_CPP_FILES}
    "./PackageProvider.cpp"
    "${RNOH_CPP_DIR}/RNOHAppNapiBridge.cpp"
)
target_link_libraries(rnoh_app PUBLIC rnoh)

# RNOH_BEGIN: manual_package_linking_2
target_link_libraries(rnoh_app PUBLIC rnoh_sample_package)
+ target_link_libraries(rnoh_app PUBLIC rnoh_babylon)
# RNOH_END: manual_package_linking_2
```

打开 `entry/src/main/cpp/PackageProvider.cpp`，添加：

```diff
#include "RNOH/PackageProvider.h"
#include "generated/RNOHGeneratedPackage.h"
#include "SamplePackage.h"
+ #include "BabylonPackage.h"

using namespace rnoh;

std::vector<std::shared_ptr<Package>> PackageProvider::getPackages(Package::Context ctx) {
    return {
        std::make_shared<RNOHGeneratedPackage>(ctx),
        std::make_shared<SamplePackage>(ctx),
+       std::make_shared<BabylonPackage>(ctx),
    };
}
```

### 4.在 ArkTs 侧引入 BabylonPackage

打开 `entry/src/main/ets/RNPackagesFactory.ts`，添加：

```diff
...

+ import {BabylonPackage} from '@react-native-oh-tpl/babylonjs-react-native-harmony/ts';

export function createRNPackages(ctx: RNPackageContext): RNPackage[] {
  return [
    new SamplePackage(ctx),
+   new BabylonPackage(ctx)
  ];
}
```

### 5.运行

点击右上角的 `sync` 按钮

或者在终端执行：

```bash
cd entry
ohpm install
```

然后编译、运行即可。

## 约束与限制

## 兼容性

要使用此库，需要使用正确的 React-Native 和 RNOH 版本。另外，还需要使用配套的 DevEco Studio 和 手机 ROM。

请到三方库相应的 Releases 发布地址查看 Release 配套的版本信息：[@react-native-oh-tpl/babylonjs-react-native和@react-native-oh-tpl/babylonjs-react-native-harmony Releases](https://github.com/react-native-oh-library/BabylonReactNative/releases)

## 属性

> [!TIP] "Platform"列表示该属性在原三方库上支持的平台。

> [!TIP] "HarmonyOS Support"列为 yes 表示 HarmonyOS 平台支持该属性；no 则表示不支持；partially 表示部分支持。使用方法跨平台一致，效果对标 iOS 或 Android 的效果。

**组件 EngineView**

`EngineView` 是一个 React Native 组件，用来渲染 Babylon.js 的 3D 场景。

| Name               | Description                                                                                                                                                                 | Type     | Required | Platform     | HarmonyOS Support |
| ------------------ | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | -------- | -------- | ------------ | ----------------- |
| `camera`           | 初始化时默认绑定的摄像机对象                                                                                                                                                   | Camera   | Yes       | iOS Android  | Yes               |
| `displayFrameRate` | 是否显示 FPS 计数器                                                                                                                                                           | boolean  | No       | iOS Android  | Yes               |
| `isTransparent`    | 是否启用透明背景                                                                                                                                                              | boolean  | No       | iOS Android  | Yes               |
| `androidView`      | 用于 native 渲染绑定的SurfaceView ID                                                                                                                                          | string   | No       | iOS Android  | Yes               |
| `antiAliasing`     | 抗锯齿等级，可选值:0、1、2、4、8、16，值越大理论上消除锯齿越明显，但性能代价越高，0和1表示关闭抗锯齿。如果设置的抗锯齿等级超过设备支持的最大抗锯齿等级，以设备实际支持的最大抗锯齿等级生效。 | number   | No       | iOS Android  | Yes               |
| `onInitialized`    | 在原生引擎初始化完成后的回调                                                                                                                                                    | function | No       | iOS Android  | Yes               |

`NativeEngineView` 是 React Native 原生桥接的 Native UI 组件。

| Name                     | Description                                                                                                                                                                  | Type     | Required | Platform     | HarmonyOS Support |
| ------------------------ | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------  | -------- | -------- | ------------ | ----------------- |
| `isTransparent`          | 控制原生渲染背景是否透明                                                                                                                                                       | boolean  | No       | iOS Android  | Yes               |
| `antiAliasing`           | 抗锯齿等级，可选值:0、1、2、4、8、16，值越大理论上消除锯齿越明显，但性能代价越高，0和1表示关闭抗锯齿。如果设置的抗锯齿等级超过设备支持的最大抗锯齿等级，以设备实际支持的最大抗锯齿等级生效。 | number   | No       | iOS Android  | Yes               |
| `androidView`            | 用于 native 渲染绑定的SurfaceView ID                                                                                                                                          | string   | No       | iOS Android  | Yes               |
| `onSnapshotDataReturned` | 原生返回截图数据的回调                                                                                                                                                         | function | No       | iOS Android  | Yes               |

# API

> [!TIP] "Platform"列表示该属性在原三方库上支持的平台。

> [!TIP] "HarmonyOS Support"列为 yes 表示 HarmonyOS 平台支持该属性；no 则表示不支持；partially 表示部分支持。使用方法跨平台一致，效果对标 iOS 或 Android 的效果。

| Name         | Description               | Type     | Required | Platform    | HarmonyOS Support |
| ------------ | ------------------------- | -------- | -------- | ----------- | ----------------- |
| `useEngine`  | 用于在组件中获取当前引擎实例 | function | No       | iOS Android | Yes               |
| `initialize` | 初始化 BabylonNative 引擎  | function | No       | iOS Android | Yes               |
| `resetView`  | 重置当前引擎视图            | function | No       | iOS Android | Yes               |

## 遗留问题

## 其他

## 开源协议

本项目基于 [The MIT License (MIT)](https://github.com/BabylonJS/BabylonReactNative/blob/master/LICENSE) ，请自由地享受和参与开源。
