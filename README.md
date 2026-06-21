# SodoVideoGame
- **장르**  
아직 구현하지 않음

- **특징**  
아직 구현하지 않음

- **기술**  
D3D12, Win32

- **배경**  
D3D12를 이용하여 게임을 제작해보기 위함

- **현재**  
윈도우 3D 게임 형태를 갖추기 위한 탐색 구현 중

<br>




<!----------------------------------------------------------------------------------------------------------------------------------------------->
## 목차
- [1. 데모 영상](#1-데모-영상)
- [2. 실행 방법](#2-실행-방법)
- [3. 주요 기능](#3-주요-기능)
- [4. 구현 구조](#4-구현-구조)
- [5. 성능 측정](#5-성능-측정)
- [6. 외부 항목](#6-외부-항목)
- [7. 출처 고지](#7-출처-고지)
- [8. 참고 자료](#8-참고-자료)

<br>




<!----------------------------------------------------------------------------------------------------------------------------------------------->
## 1. 데모 영상
### 1.1. 데모 영상
아직 구현하지 않음

<br>




### 1.2. 데모 스크린샷
아직 구현하지 않음

<br>




<!----------------------------------------------------------------------------------------------------------------------------------------------->
## 2. 실행 방법
### 2.1. 실행 환경
아직 구현하지 않음

<br>




### 2.2. 빌드 방법
아직 구현하지 않음

<br> 




### 2.3. 설정 방법
아직 구현하지 않음

<br>




### 2.4. 조작 방법
아직 구현하지 않음

<br>




<!----------------------------------------------------------------------------------------------------------------------------------------------->
## 3. 주요 기능
### 3.1. 조명 기능
아직 구현하지 않음

<br>




### 3.2. 소리 기능
아직 구현하지 않음

<br>




### 3.3. 초목 기능
아직 구현하지 않음

<br>




### 3.4 건물 기능
아직 구현하지 않음

<br>




### 3.5. 인물 기능
아직 구현하지 않음

<br>




### 3.6. 플레이 기능
아직 구현하지 않음

<br>




<!----------------------------------------------------------------------------------------------------------------------------------------------->
## 4. 구현 구조
### 4.1. 폴더 구성
용도별로 다음과 같은 하위 폴더들을 두었습니다.

<br>

- `Code`  
  : 헤더 폴더(=`추가 포함 디렉토리`)와 소스 폴더가 위치합니다.

- `External`  
  : 리포지토리 내부에 직접 포함된 외부 항목이 위치합니다.

- `IntDir` ( =`중간 디렉토리`)  
  : 컴파일 결과물이 위치합니다.

- `Model`  
  : 오브젝트, 애니메이션 파일들이 위치합니다.

- `OutDir` (=`출력 디렉토리`, `작업 디렉토리`)  
  : 링크 결과물과, 런타임들이 위치합니다.

- `Shader`  
  : HLSL 셰이더 파일들이 위치합니다.

- `Texture`  
  : DDS 텍스처 파일들이 위치합니다.

<br>




### 4.2. 빌드 구성
일단 별도의 빌드 시스템 없이 Visual Studio에서 직접 빌드를 관리하고 있습니다.

일반적으로 Visual Studio에서 응용 프로그램의 빌드와 실행은 다음과 같이 이뤄집니다[3].
| 단계 | 직접 입력 | 직접 입력 대상 | 간접 입력 | 간접 입력 대상 명시 | 간접 입력 대상 경로 명시 | 출력 | 출력 경로 |
|---|---|---|---|---|---|---|---|
| 전처리 + 컴파일 | `.cpp` | 프로젝트에 포함된 각 `.cpp` 파일들 | `.h`, `.inl` | `#include <xxx.h>`, `#include "xxx.h"`| (`프로젝트 속성` > `C/C++` > `일반` > `추가 포함 디렉토리` > `편집`) | `.obj` | `중간 디렉토리` |
| 링크 | `.obj` | 컴파일된 `.obj` 파일들 | `.lib` | `#pragma comment(lib, "xxx.lib")` <br> or <br> (`프로젝트 속성` > `링커` > `입력` > `추가 종속성` > `편집`) | (`프로젝트 속성` > `링커` > `일반` > `추가 라이브러리 디렉토리` > `편집`) | `.exe` |  `출력 디렉토리` |
| 실행 | `.exe` | 링크된 `.exe` |  `.dll` | 링크 시점에 임포트 라이브러리를 링크 <br> (로드-타임 동적 링킹) <br> or <br> 실행 중 `LoadLibraryA(..)` 함수 사용 <br> (런-타임 동적 링킹) | 해당 없음(`.dll`을 `.exe`의 디렉토리에 복사해주는 것이 일반적) | 프로세스 | 해당 없음 |

<br>

D3D12에 관련된 간접 입력 대상들을 가장 간단히 세팅한다면 이용하는 대상은 주로 다음과 같습니다[4][5].
| 대상 | 관리 주체 | 내용물 | 목적 | 경로 | 세팅 방법 |
|---|---|---|---|---|---|
| Windows SDK | Visual Studio Installer | `.h`, `.inl`, `.lib` | 윈도우 앱 빌드에 필요한 헤더, 라이브러리 사용 |`C:\Program Files (x86)\Windows Kits` | 경로는 기본 설정되어 있음, <br> 무엇을 사용할지만 다음처럼 명시하면 됨 <br> `#include <d3d12.h>` <br> `#include <dxgi1_6.h>` <br> `#pragma comment(lib, "dxgi.lib")` <br> `#pragma comment(lib, "D3D12.lib")` <br> ... |
| DirectX Header | Microsoft Github Repository | `.h` | `d3dx12*.h` 헬퍼 구조체, 헬퍼 함수 사용 | https://github.com/microsoft/DirectX-Headers | 수동으로 다운로드 받아 프로젝트에 포함 |
| D3D12 Runtime | Windows | `.dll` | 응용 프로그램 구동에 필요한 런타임 사용 | `C:\Windows\System32` | 경로는 기본 설정되어 있음, <br> 임포트 라이브러리인 `D3D12.lib`을 통해 로드-타임 동적 링킹이 일어나므로 별도의 런-타임 동적 링킹 또한 필요 없음 |

<br>

본 리포지토리에서는 위 방법에 더하여, 최신의 D3D12 관련 기능을 배포된 리포지토리 내부에서 복원하여 사용할 수 있도록 NuGet을 이용하였습니다.
| 대상 | 목적 | 파일 |
|---|---|---|
| DirectX 12 Agility SDK | 최신 D3D12 기능 사용 | `.h`, `.dll` |
| DirectXTK12 | 최신 DirectX12 툴 사용 | `.h`, `.inl`, `.dll` |
| DirectX Shader Compiler (DXC) | HLSL 셰이더 파일 컴파일 | `.h`, `.lib`, `.dll` |

<br>

임시 코드를 `main.cpp`에 삽입하여 확인한 결과, 본 리포지토리 환경에서 빌드에 사용되는 DirectX 관련 파일들의 경로는 다음과 같습니다.

- 전처리 및 컴파일 입력
  - 확인 방법
    1. `프로젝트 속성` > `C/C++` > `포함 표시` > `예(/showIncludes)` 설정
    2. 상단의 `빌드` > `솔루션 빌드` > 하단의 출력창의 `출력 보기 선택`을 `빌드`로 선택

  - 확인된 목록
    - 변화가 적은 dxgi와 DirectXMath는 NuGet의 관리 대상이 아니기에  
      이들에 관련된 헤더 파일들은 Visual Studio Installer가 관리하는 Windows Kits에 위치합니다.  
      - `C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\um\DirectXMath.h`

      - `C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\um\DirectXMathConvert.inl`

      - `C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\um\DirectXMathVector.inl`

      - `C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\um\DirectXMathMatrix.inl`

      - `C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\um\DirectXMathMisc.inl`

      - `C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\um\DirectXPackedVector.h`

      - `C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\um\DirectXPackedVector.inl`

      - `C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\shared\dxgi.h`

      - `C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\shared\dxgicommon.h`

      - `C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\shared\dxgi1_2.h`

      - `C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\shared\dxgi1_3.h`

      - `C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\shared\dxgi1_4.h`

      - `C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\shared\dxgi1_5.h`

      - `C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\shared\dxgi1_6.h`

      - `C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\shared\dxgitype.h`

    - 특이하게 dxgiformat.h는 두 경로의 파일 모두가 사용되었습니다.  
      이는, 다른 경로에 속한 헤더파일로부터의 각각 간접적으로 include되었기 때문으로 추측합니다.  
      - `C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\shared\dxgiformat.h`

      - `SodoVideoGame\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\dxgiformat.h`

    - DirectX Agility SDK, DirectXTK12, DXC는 NuGet에 의해 관리되기에  
      d3d12, d3dx12, SimpleMath, dxcapi 관련 헤더파일들은 본 리포지토리의 아래에 복원됩니다.  
      - `SodoVideoGame\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3d12.h`

      - `SodoVideoGame\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dcommon.h`

      - `SodoVideoGame\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3d12sdklayers.h`

      - `SodoVideoGame\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dx12\d3dx12.h`

      - `SodoVideoGame\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dx12\d3dx12_barriers.h`

      - `SodoVideoGame\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dx12\d3dx12_core.h`

      - `SodoVideoGame\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dx12\d3dx12_default.h`

      - `SodoVideoGame\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dx12\d3dx12_pipeline_state_stream.h`

      - `SodoVideoGame\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dx12\d3dx12_render_pass.h`

      - `SodoVideoGame\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dx12\d3dx12_resource_helpers.h`

      - `SodoVideoGame\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dx12\d3dx12_property_format_table.h`

      - `SodoVideoGame\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dx12\d3dx12_root_signature.h`

      - `SodoVideoGame\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dx12\d3dx12_state_object.h`

      - `SodoVideoGame\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dx12\d3dx12_check_feature_support.h`

      - `SodoVideoGame\packages\directxtk12_desktop_win10.2026.5.8.1\include\SimpleMath.h`

      - `SodoVideoGame\packages\directxtk12_desktop_win10.2026.5.8.1\include\SimpleMath.inl`

      - `SodoVideoGame\packages\directxtk12_desktop_win10.2026.5.8.1\include\GraphicsMemory.h`

      - `SodoVideoGame\packages\Microsoft.Direct3D.DXC.1.9.2602.24\build\native\include\dxcapi.h`

<br>

- 링크 입력
  - 확인 방법
    1. `프로젝트 속성` > `링커` > `명령줄` > `추가 옵션` > `/VERBOSE:LIB` 입력
    2. 상단의 `빌드` > `솔루션 빌드` > 하단의 출력창의 `출력 보기 선택`을 `빌드`로 선택

  - 확인된 목록
    - 변화가 적은 하위 로직을 담당하는 dxgi.lib와 gdi32.lib은 Windows Kits에 위치합니다.  
      다만 DirectXMath는 헤더파일(.h or .inl)으로만 구현되므로 라이브러리 파일이 존재하지 않습니다.
      - `C:\Program Files (x86)\Windows Kits\10\lib\10.0.26100.0\um\x64\gdi32.lib`
    
      - `C:\Program Files (x86)\Windows Kits\10\lib\10.0.26100.0\um\x64\dxgi.lib`

    - D3D12.lib은 dll을 로드하기 위한 임포트 라이브러리이기에 변화가 적어  
      Visual Studio Installer가 관리하는 Windows Kits 아래에 위치합니다.  
      - `C:\Program Files (x86)\Windows Kits\10\lib\10.0.26100.0\um\x64\D3D12.lib`

    - DirectXTK12는 NuGet에 의해 관리되므로, 리포지토리 내부에 위치합니다.
      - `SodoVideoGame\packages\directxtk12_desktop_win10.2026.5.8.1\build\native\..\..\native\lib\x64\Debug\DirectXTK12.lib`

    - DXC는 NuGet에 의해 관리되므로, dxcompiler.lib은 리포지토리 내에 위치하며 관리됩니다.  
      다만, 레거시인 d3dcompiler.lib은 파악하지 못한 간접적인 경로로 링킹된 것으로 추측합니다.  
      - `SodoVideoGame\packages\Microsoft.Direct3D.DXC.1.9.2602.24\build\native\lib\x64\dxcompiler.lib`

      - `C:\Program Files (x86)\Windows Kits\10\lib\10.0.26100.0\um\x64\d3dcompiler.lib`

<br>

- 실행 의존
  - 확인 방법
    1. 상단의 `디버그` > `디버깅 시작`
    2. 상단의 `디버그` > `창` > `모듈`

  - 확인된 목록
    - 변화가 적은 gdi32와 dxgi는 NuGet이 관리하지 않습니다.  
      그렇기에 윈도우가 관리하는 System32에 런타임이 위치합니다.  
      - `C:\Windows\System32\gdi32.dll`

      - `C:\Windows\System32\gdi32full.dll`

      - `C:\Windows\System32\dxgi.dll`

      - `C:\Windows\System32\DXGIDebug.dll`

      - `C:\Windows\System32\DXCore.dll`

      - `C:\Windows\System32\directxdatabasehelper.dll`

    - D3D12.dll은 핵심 로직을 수행하지 않고, 얇은 로더 역할을 수행하므로 NuGet에 의해 관리되지 않습니다.  
      그러나 핵심 로직이 담긴 D3D12Core.dll은 NuGet에 의해 관리되기에 리포지토리 내부에 복원됩니다.  
      - `C:\Windows\System32\D3D12.dll`

      - `SodoVideoGame\OutDir\D3D12\D3D12Core.dll`

      - `SodoVideoGame\OutDir\D3D12\D3D12SDKLayers.dll`

    - DXC는 NuGet에 의해 관리되므로 dxcompiler.dll은 리포지토리 내에 위치하고  
      레거시인 D3DCompiler_47.dll는 NuGet에 의해 관리되지 않기에 System32 아래에 위치합니다.  
      - `SodoVideoGame\OutDir\dxcompiler.dll`

      - `C:\Windows\System32\D3DCompiler_47.dll`

<br>




### 4.3. 시스템 구성
- 프로젝트에서 D3D12 기능을 사용하는 경우 주로 다음과 같은 경로를 따라가게 됩니다[5].
1. `d3d12.h`를 통해 D3D12 기능을 호출
2. `D3D12.lib`을 통해 임포트된...TODO

- 이때 `D3D12Core.dll`은 다음과 같은 시스템 요소들과 요청 및 결과를 주고 받으며 작업을 수행합니다[6]. 
  - 유저모드 : D3D12 런타임, DXGI, WDDM UserMode Driver, gdi32.dll
  - 커널모드 : dxgkrnl.sys, dxgmms2.sys, WDDM KernelMode Driver

<br>




### 4.3. 입력 방식
일반적인 윈도우 메시지 처리 방식을 사용하고 있습니다[7].

<br>




<!----------------------------------------------------------------------------------------------------------------------------------------------->
## 5. 성능 측정
### 5.1. 측정 환경
아직 구현하지 않음

<br>




### 5.2. 측정 방식
아직 구현하지 않음

<br>




### 5.3. 측정 결과
아직 구현하지 않음

<br>




<!----------------------------------------------------------------------------------------------------------------------------------------------->
## 6. 외부 항목
개발 및 빌드를 위해 다음 SDK, 툴킷, 라이브러리, 도구를 사용합니다.

<br>




### 6.1. DirectX 12 Agility SDK
- 목적
  - 윈도우 업데이트와 상관 없이 최신 D3D12 헤더, 런타임 사용

- 사용 방식
  - NuGet 패키지 관리 (Microsoft.Direct3D.D3D12)
  
- 복원 위치
  - `SodoVideoGame\packages\Microsoft.Direct3D.D3D12.<버전명>`

- 라이선스 종류
  - MICROSOFT SOFTWARE LICENSE
  - MIT License

- 라이선스 위치
  - `SodoVideoGame\packages\Microsoft.Direct3D.D3D12.<버전명>\LICENSE.txt`
  - `SodoVideoGame\packages\Microsoft.Direct3D.D3D12.<버전명>\LICENSE-CODE.txt`

<br>




### 6.2. DirectX Tool Kit (DirectXTK12)
- 목적
  - Direct3D 12 유틸리티 라이브러리 사용

- 사용 방식
  - NuGet 패키지 관리 (directxtk12_desktop_win10)

- 복원 위치
  - `SodoVideoGame\packages\directxtk12_desktop_win.<버전명>`

- 라이선스 종류
  - MIT License

- 라이선스 위치
  - `SodoVideoGame\packages\directxtk12_desktop_win10.<버전명>\docs\README.md` 내부 간접 링크
  - `https://github.com/microsoft/DirectXTK12/blob/main/LICENSE`

<br>




### 6.3. DirectX Shader Compiler (DXC)
- 목적
  - HLSL 셰이더 컴파일

- 사용 방식
  - NuGet 패키지 관리 (Microsoft.Direct3D.DXC)
  
- 복원 위치
  - `SodoVideoGame\packages\Microsoft.Direct3D.DXC.<버전명>`

- 라이선스 종류
  - MIT License
  - LLVM Release License
  - MICROSOFT SOFTWARE LICENSE

- 라이선스 위치
  - `SodoVideoGame\packages\Microsoft.Direct3D.DXC.<버전명>\LICENCE-MIT.txt`
  - `SodoVideoGame\packages\Microsoft.Direct3D.DXC.<버전명>\LICENSE-LLVM.txt`
  - `SodoVideoGame\packages\Microsoft.Direct3D.DXC.<버전명>\LICENSE-MS.txt`

<br>




### 6.4 Dear ImGui
- 목적
  - 인게임 GUI 제공

- 사용 방식
  - 내부 직접 포함
  - 포함 경로 : `SodoVideoGame\External\imgui`

- 원본 리포지토리
  - ocornut/imgui  
    `https://github.com/ocornut/imgui`

- 라이선스 종류
  - MIT License

- 라이선스 위치
  - `SodoVideoGame\External\imgui\LICENSE.txt`

<br>




<!----------------------------------------------------------------------------------------------------------------------------------------------->
## 7. 출처 고지
Frank D. Luna의 "Introduction To 3D Game Programming With DirectX 12 Second Edition"의 설명과 주제 흐름을 참고했지만,

본 리포지토리는 해당 도서의 본문, 예제 코드, 에셋을 복사하거나 포함하지 않음을 알립니다.

<br>




<!----------------------------------------------------------------------------------------------------------------------------------------------->
## 8. 참고 자료
[1] "DirectX 12를 이용한 3D 게임 프로그래밍 입문" (Frank D. Luna 지음 / 류광 옮김 / 한빛미디어 출판 / 2017년 발행)

[2] "Introduction To 3D Game Programming With DirectX 12 Second Edition" (Frank D. Luna 지음 / Mercury Learning And Information 출판 / 2025년 발행)

[3] "Dynamic-Link Libraries (Dynamic-Link Libraries)" (Microsoft Learn / 2026년 6월 재방문)  
https://learn.microsoft.com/en-us/windows/win32/dlls/dynamic-link-libraries

[4] "Helper structures and functions for Direct3D 12" (Microsoft Learn / 2026년 6월 재방문)  
https://learn.microsoft.com/en-us/windows/win32/direct3d12/helper-structures-and-functions-for-d3d12

[5] "Direct3D 12 programming environment setup" (Microsoft Learn / 2026년 6월 재방문)  
https://learn.microsoft.com/en-us/windows/win32/direct3d12/directx-12-programming-environment-set-up

[6] "WDDM architecture" (Microsoft Learn / 2026년 6월 재방문)  
https://learn.microsoft.com/en-us/windows-hardware/drivers/display/windows-vista-and-later-display-driver-model-architecture

[7] "Get Started with Win32 and C++" (Microsoft Learn / 2026년 6월 재방문)  
https://learn.microsoft.com/en-us/windows/win32/learnwin32/learn-to-program-for-windows