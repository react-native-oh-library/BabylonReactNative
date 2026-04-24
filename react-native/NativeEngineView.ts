import codegenNativeComponent from 'react-native/Libraries/Utilities/codegenNativeComponent';
import codegenNativeCommands from "react-native/Libraries/Utilities/codegenNativeCommands";
import type { ViewProps, HostComponent } from 'react-native';
import { Int32,DirectEventHandler,WithDefault } from "react-native/Libraries/Types/CodegenTypes";

export type SnapshotEventData = Readonly<{
    data?: string;
  }>

export interface NativeEngineViewProps extends ViewProps {
    isTransparent?: WithDefault<boolean, false>;
    antiAliasing?: Int32;
    androidView?: string;
    onSnapshotDataReturned?: DirectEventHandler<SnapshotEventData>;
}

type NativeType = HostComponent<NativeEngineViewProps>;

interface NativeCommands {
    takeSnapshot: (viewRef: React.ElementRef<NativeType>) => void;
}
  
export const Commands: NativeCommands = codegenNativeCommands<NativeCommands>({
    supportedCommands: ["takeSnapshot"],
});

export default codegenNativeComponent<NativeEngineViewProps>(
    "NativeEngineView"
  ) as HostComponent<NativeEngineViewProps>;
