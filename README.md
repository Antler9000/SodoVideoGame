# SodoVideoGame
- **장르**  
아직 구현하지 않음

- **특징**  
아직 구현하지 않음

- **기술**  
D3D12, Win32

- **현재**  
윈도우 3D 게임 형태를 갖추기 위한 탐색 구현 중

<br>




<!----------------------------------------------------------------------------------------------------------------------------------------------->
## 목차
- [1. 데모 영상](#1-데모-영상)
- [2. 실행 방법](#2-실행-방법)
- [3. 제작 동기](#3-제작-동기)
- [4. 주요 기능](#4-주요-기능)
- [5. 구현 구조](#5-구현-구조)
- [6. 성능 측정](#6-성능-측정)
- [7. 외부 항목](#7-외부-항목)
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
## 3. 제작 동기
D3D12의 기본적인 사용 기법을 이용하여 게임을 제작해보고자 본 프로젝트를 시작하였습니다.

다양한 기법들을 직교적으로 조합해가며 가능성을 탐색하되, 하나의 핵심 가치를 기준으로 이들을 선별하여 일관된 방향성을 구축하고자 합니다.

본 프로젝트에서 추구하는 핵심 가치는 '상식이나 승리에 얽매이지 않은, 더 나은 제3의 선택'입니다.

<br>




<!----------------------------------------------------------------------------------------------------------------------------------------------->
## 4. 주요 기능
### 4.1. 조명 기능
아직 구현하지 않음

<br>




### 4.2. 소리 기능
아직 구현하지 않음

<br>




### 4.3. 초목 기능
아직 구현하지 않음

<br>




### 4.4 건물 기능
아직 구현하지 않음

<br>




### 4.5. 인물 기능
아직 구현하지 않음

<br>




### 4.6. 플레이 기능
아직 구현하지 않음

<br>




<!----------------------------------------------------------------------------------------------------------------------------------------------->
## 5. 구현 구조
### 5.1. 폴더 구성
용도별로 다음과 같은 하위 폴더들을 두었습니다.

<br>

- `Code` (=`추가 포함 디렉토리`)  
  : 헤더 파일과 소스 파일이 위치합니다.

<br>

- `External`  
  : 리포지토리 내부에 직접 포함된 외부 항목이 위치합니다.

<br>

- `IntDir` ( =`중간 디렉토리`)  
  : 컴파일 결과물이 위치합니다.

<br>

- `Model`  
  : 오브젝트, 애니메이션 파일들이 위치합니다.

<br>

- `OutDir` (=`출력 디렉토리`, `작업 디렉토리`)  
  : 링크 결과물과, 런타임들이 위치합니다.

<br>

- `Shader`  
  : HLSL 셰이더 파일들이 위치합니다.

<br>

- `Texture`  
  : DDS 텍스처 파일들이 위치합니다.

<br>




### 5.2. 빌드 구성
일단 별도의 빌드 시스템 없이 Visual Studio에서 직접 빌드를 관리하고 있습니다.

임시 코드를 `main.cpp`에 삽입하여 확인한 결과, 본 리포지토리 환경에서 빌드와 실행에 사용되는 DirectX 관련 파일들의 경로는 다음과 같습니다.

각 경로는 관리 주체와 목적이 다릅니다.
| 경로 | 관리 주체 | 목적 |
|---|---|---|
| `C:\Windows\System32` | Windows | 응용 프로그램 구동에 필요한 런타임 보유 |
| `C:\Program Files (x86)\Windows Kits` | Visual Studio Installer | 개발 빌드에 필요한 헤더, 라이브러리 보유 | 
| `SodoVideoGame` 리포지토리 내부 | NuGet | 최신 헤더, 라이브러리, 런타임 복원 |

<br>

- 컴파일 입력 (헤더 파일)
  - 확인 방법
    1. `프로젝트 속성` > `C/C++` > `포함 표시` > `예(/showIncludes)` 설정
    2. 상단의 `빌드` > `솔루션 빌드` > 하단의 출력창의 `출력 보기 선택`을 `빌드`로 선택

  - 확인된 목록
    - 변화가 적은 dxgi와 DirectXMath는 NuGet의 관리 대상이 아니기에  
      이들에 관련된 헤더 파일들은 Visual Studio Installer가 관리하는 Window Kits에 위치합니다.  
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

- 링크 입력 (라이브러리)
  - 확인 방법
    1. `프로젝트 속성` > `링커` > `명령줄` > `추가 옵션` > `/VERBOSE:LIB` 입력
    2. 상단의 `빌드` > `솔루션 빌드` > 하단의 출력창의 `출력 보기 선택`을 `빌드`로 선택

  - 확인된 목록
    - 변화가 적은 하위 로직을 담당하는 dxgi.lib와 gdi32.lib은 Windows Kits에 위치합니다.  
      다만 DirectXMath는 헤더파일(.h or .inl)으로만 구현되므로 라이브러리가 존재하지 않습니다.
      - `C:\Program Files (x86)\Windows Kits\10\lib\10.0.26100.0\um\x64\gdi32.lib`
    
      - `C:\Program Files (x86)\Windows Kits\10\lib\10.0.26100.0\um\x64\dxgi.lib`

    - D3D12.lib은 dll로 가기 위한 임포트 라이브러리이기에 굳이 최신 버전으로 관리될 필요가 없어  
      Visual Studio Installer가 관리하는 Windows Kits 아래에 위치합니다.  
      - `C:\Program Files (x86)\Windows Kits\10\lib\10.0.26100.0\um\x64\D3D12.lib`

    - DirectXTK12는 NuGet에 의해 관리되므로, 리포지토리 내부에 위치합니다.
      - `SodoVideoGame\packages\directxtk12_desktop_win10.2026.5.8.1\build\native\..\..\native\lib\x64\Debug\DirectXTK12.lib`

    - DXC는 NuGet에 의해 관리되므로, dxcompiler.lib은 리포지토리 내에 위치하며 관리됩니다.  
      다만, 레거시인 d3dcompiler.lib은 파악하지 못한 간접적인 경로로 링킹된 것으로 추측합니다.  
      - `SodoVideoGame\packages\Microsoft.Direct3D.DXC.1.9.2602.24\build\native\lib\x64\dxcompiler.lib`

      - `C:\Program Files (x86)\Windows Kits\10\lib\10.0.26100.0\um\x64\d3dcompiler.lib`

<br>

- 실행 의존 (런타임)
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




### 5.3. 입력 방식
일반적인 윈도우 메시지 처리 방식을 사용하고 있습니다.

<br>




<!----------------------------------------------------------------------------------------------------------------------------------------------->
## 6. 성능 측정
### 6.1. 측정 환경
아직 구현하지 않음

<br>




### 6.2. 측정 방식
아직 구현하지 않음

<br>




### 6.3. 측정 결과
아직 구현하지 않음

<br>




<!----------------------------------------------------------------------------------------------------------------------------------------------->
## 7. 외부 항목
개발 및 빌드를 위해 다음 SDK, 툴킷, 라이브러리, 도구를 사용합니다.

<br>




### 7.1. DirectX 12 Agility SDK
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




### 7.2. DirectX Tool Kit (DirectXTK12)
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




### 7.3. DirectX Shader Compiler (DXC)
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




### 7.4 Dear ImGui
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
## 8. 참고 자료
[1] "DirectX 12를 이용한 3D 게임 프로그래밍 입문" (Frank D. Luna 지음 / 류광 옮김 / 한빛미디어 출판 / 2017년 발행)

[2] "Introduction To 3D Game Programming With DirectX 12 Second Edition" (Frank D. Luna 지음 / Mercury Learning And Information 출판 / 2025년 발행)

[3] "Get Started with Win32 and C++" (Microsoft Learn / 2026년 6월 재방문)
`https://learn.microsoft.com/en-us/windows/win32/learnwin32/learn-to-program-for-windows`