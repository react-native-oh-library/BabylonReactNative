> Template version: v0.2.2

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

> [!TIP] [Github address](https://github.com/react-native-oh-library/BabylonReactNative)

This third-party library has been migrated to Gitcode and is now available for direct download from npm, the new package name is: `@react-native-oh-tpl/babylonjs-react-native and @react-native-oh-tpl/babylonjs-react-native-harmony`, The version correspondence details are as follows:

| Version | Package Name                              | Repository | Release | Support RN version |
| ---------- | ------------------------------------------------------------ | ---------- |  ---------- |  ---------- |
| 1.8.7 | @react-native-oh-tpl/babylonjs-react-native和@react-native-oh-tpl/babylonjs-react-native-harmony | [Github](https://github.com/react-native-oh-library/BabylonReactNative)|[Github Releases](https://github.com/react-native-oh-library/BabylonReactNative/releases)|0.72       |

## Installation and Usage

Find the matching version information in the release address of a third-party library: [@react-native-oh-tpl/babylonjs-react-native and @react-native-oh-tpl/babylonjs-react-native-harmony Releases](https://github.com/react-native-oh-library/BabylonReactNative/releases).

Go to the project directory and execute the following instruction:



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

#### **If using BabylonReactNative in DevEco Studio**

```bash
When running git --version in the DevEco Studio terminal, if the Git version cannot be detected, follow these steps:
1.Install Git.(Skip this step if Git is already installed.)
2.Add the Git executable directory to the system Path environment variable.(Skip this step if the Path already contains the Git directory.)
3.Restart your computer to ensure the environment variables take effect in DevEco Studio.
```

<!-- tabs:end -->

The following code shows the basic use scenario of the repository:

> [!WARNING] The name of the imported repository remains unchanged.

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

Currently, HarmonyOS does not support AutoLink. Therefore, you need to manually configure the linking.

Open the `harmony` directory of the HarmonyOS project in DevEco Studio.

### 1. Adding the overrides Field to oh-package.json5 File in the Root Directory of the Project

```json
{
  ...
  "overrides": {
    "@rnoh/react-native-openharmony" : "./react_native_openharmony"
  }
}
```

### 2. Introducing Native Code

Currently, two methods are available:

Method 1 (recommended): Use the HAR file.

> [!TIP] The HAR file is stored in the `harmony` directory in the installation path of the third-party library.

Open `entry/oh-package.json5` file and add the following dependencies:

```json
"dependencies": {
    "@rnoh/react-native-openharmony": "file:../react_native_openharmony",

    "@react-native-oh-tpl/babylonjs-react-native-harmony": "file:../../node_modules/@react-native-oh-tpl/babylonjs-react-native-harmony/harmony/babylonjs.har"
  }
```

Click the `sync` button in the upper right corner.

Alternatively, run the following instruction on the terminal:

```bash
cd entry
ohpm install
```

Method 2: Directly link to the source code.

> [!TIP] For details, see [Directly Linking Source Code](/en/link-source-code.md).

### 3. Configuring CMakeLists and Introducing BabylonPackage

Open `entry/src/main/cpp/CMakeLists.txt ` and add the following code:

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

Open `entry/src/main/cpp/PackageProvider.cpp` and add the following code:

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

### 4. Introducing BabylonPackage to ArkTS 

Open the`entry/src/main/ets/RNPackagesFactory.ts` file and add the following code:

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

### 5. Running

Click the `sync` button in the upper right corner.

Alternatively, run the following instruction on the terminal:

```bash
cd entry
ohpm install
```

Then build and run the code.

## Compatibility

To use this repository, you need to use the correct React-Native and RNOH versions. In addition, you need to use DevEco Studio and the ROM on your phone.

Check the release version information in the release address of the third-party library:[@react-native-oh-tpl/babylonjs-react-native and @react-native-oh-tpl/babylonjs-react-native-harmony Releases](https://github.com/react-native-oh-library/BabylonReactNative/releases)

## Properties

> [!TIP] The **Platform** column indicates the platform where the properties are supported in the original third-party library.

> [!TIP] If the value of **HarmonyOS Support** is **yes**, it means that the HarmonyOS platform supports this property; **no** means the opposite; **partially** means some capabilities of this property are supported. The usage method is the same on different platforms and the effect is the same as that of iOS or Android.

**EngineView**

`EngineView` is a React Native component that renders the Babylon.js 3D scene.

| Name               | Description                                                              | Type     | Required | Platform    | HarmonyOS Support |
| ------------------ | ------------------------------------------------------------------------ | -------- | -------- | ----------- | ----------------- |
| `camera`           | The camera object bound as the default render camera upon initialization | Camera   | Yes      | iOS Android | Yes               |
| `displayFrameRate` | Whether to display the FPS counter                                       | boolean  | No       | iOS Android | Yes               |
| `isTransparent`    | Whether to enable transparent background rendering                       | boolean  | No       | iOS Android | Yes               |
| `androidView`      | The SurfaceView identifier used for native rendering binding on Android  | string   | No       | iOS Android | Yes               |
| `antiAliasing`     | Anti-aliasing level. Available values: 0, 1, 2, 4, 8, 16. Higher values generally provide better anti-aliasing quality at the cost of performance. 0 and 1 indicate that anti-aliasing is disabled. If the specified anti-aliasing level exceeds the maximum level supported by the device, it will automatically apply the highest supported level instead. | number   | No       | iOS Android | Yes               |
| `onInitialized`    | Callback invoked after the native engine has completed initialization    | function | No       | iOS Android | Yes               |

`NativeEngineView` is a native UI component bridged into React Native.

| Name                     | Description                                                             | Type     | Required | Platform     | HarmonyOS Support |
| ------------------------ | ----------------------------------------------------------------------- | -------- | -------- | ------------ | ----------------- |
| `isTransparent`          | Whether to enable transparent background rendering                      | boolean  | No       | iOS Android  | Yes               |
| `antiAliasing`           | Anti-aliasing level. Available values: 0, 1, 2, 4, 8, 16. Higher values generally provide better anti-aliasing quality at the cost of performance. 0 and 1 indicate that anti-aliasing is disabled. If the specified anti-aliasing level exceeds the maximum level supported by the device, it will automatically apply the highest supported level instead. | number   | No       | iOS Android  | Yes               |
| `androidView`            | The SurfaceView identifier used for native rendering binding on Android | string   | No       | iOS Android  | Yes               |
| `onSnapshotDataReturned` | Callback returning screenshot data from the native side                 | function | No       | iOS Android  | Yes               |

# API

> [!TIP] The **Platform** column indicates the platform where the properties are supported in the original third-party library.

> [!TIP] If the value of **HarmonyOS Support** is **yes**, it means that the HarmonyOS platform supports this property; **no** means the opposite; **partially** means some capabilities of this property are supported. The usage method is the same on different platforms and the effect is the same as that of iOS or Android.

| Name         | Description                                                     | Type     | Required | Platform    | HarmonyOS Support |
| ------------ | --------------------------------------------------------------- | -------- | -------- | ----------- | ----------------- |
| `useEngine`  | Used to obtain the current engine instance within the component | function | No       | iOS Android | Yes               |
| `initialize` | Initialize the Babylon Native engine                            | function | No       | iOS Android | Yes               |
| `resetView`  | Reset the current engine view                                   | function | No       | iOS Android | Yes               |

## Known Issues

## Others

## License

This project is licensed under [The MIT License (MIT)](https://github.com/BabylonJS/BabylonReactNative/blob/master/LICENSE).
