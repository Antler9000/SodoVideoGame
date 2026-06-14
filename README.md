# SteelBallet
- **장르**  
실시간 전술 단편 스토리 게임

- **특징**  
조작 유닛 규모를 소대급으로 제한하여, 조작 부담을 줄이고 각 유닛의 기능과 개성 그리고 스토리에 집중함

- **기술**  
D3D12와 Win32로 제작

- **목표**    
올해 2026년 3분기까지 작고 조잡하더라도 초안 완성하기

- **현재**  
최소한의 윈도우 3D 게임 형태를 갖추기 위한 탐색 구현 단계 중

<br>




## 목차
- [1. 데모 영상](#1-데모-영상)
- [2. 실행 방법](#2-실행-방법)
- [3. 제작 동기](#3-제작-동기)
- [4. 기능 구상](#4-기능-구상)
- [5. 설계 사항](#5-설계-사항)
- [6. 성능 측정](#6-성능-측정)
- [7. 제외 기능](#7-제외-기능)
- [8. 외부 항목](#8-외부-항목)
- [9. 참고 자료](#9-참고-자료)

<br>




## 1. 데모 영상
### 1.1. 데모 영상
아직 구현하지 않음

<br>




### 1.2. 데모 스크린샷
아직 구현하지 않음

<br>




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




## 3. 제작 동기
요소들이 직교적으로 조합되며 다가올 때, 새로운 가능성과 즐거움을 느낀다고 생각합니다.

실시간 전술 게임은 그러한 요소들을 탑다운 뷰, 맵의 관경, 유닛들, 전쟁이라는 배경을 통해 담아냅니다.

컴퍼니 오브 히어로즈는 그러한 게임 중 하나이지만 많은 후반전 유닛 수로 인해 조작이 부담스럽고, 유닛이 표준화되며, 스토리가 게임성과 결합되지 못하는 점이 아쉬웠습니다.

현재까지 학습한 D3D12의 기본적인 렌더링과 지형 생성 그리고 화면 공간 선택 등의 기법을 이용하면 실시간 전술 게임을 제작할 수 있다고 생각했고,

따라서 유닛 수를 줄여 이들의 기능과 개성 그리고 스토리와의 결합에 집중한 작은 실시간 전술 게임을 만들어보고자 본 프로젝트를 시작하게 되었습니다.

<br>




## 4. 기능 구상
### 4.1. 조명 기능
  | 종류 | 계산 방식 |
  |---|---|
  | 반영 조명(Specular Light) | 프레넬 상수 + 표면 거칠기 상수 + 쉐도우 맵 + 법선 맵 |
  | 분산 조명(Diffuse Light) | 분산 맵 + 쉐도우 맵 + 법선 맵 |
  | 주변 조명(Ambient Light) | SSAO |

  일단 학습 경험이 있는 다이렉트X 12 입문 도서에서의 조명 방식을 사용하여 기초적인 시각 경험을 제공할 예정입니다.

  추가적인 렌더링 기법은 학습에 따라 점진적으로 도입할 예정입니다.

  <br>

  **(관련 상호작용)**
  - 카메라 조작
  - 사격 및 피격 효과
  - 그래픽 설정 GUI

<br>




### 4.2. 소리 기능
  | 종류 | 녹음 방식 |
  |---|---|
  | 환경 소리 | 미정 |
  | 차량 소리 | 미정 |
  | 인물 대사 | 자막 처리 |
  | 배경 음악 | 미정 |

  본 리포지토리의 라이선스에 적합한 생성 모델을 결정 중에 있습니다.  

  ElevenCreative의 출력물에 부여된 라이선스를 확인 후 결정지을 예정입니다.

  다만 AI로 생성한 대사는 어색한 느낌이 있어 인물의 대사는 모두 자막 처리하기로 하였습니다.

  <br>

  **(관련 상호작용)**
  - 유닛 선택 대사 자막
  - 유닛 기동, 사격, 피격 효과음
  - GUI 클릭 소리
  - 음량 설정 GUI

<br>




### 4.3. 환경 기능
  | 종류 | 제작 방식 |
  |---|---|
  | 초목 | ChatGPT |
  | 바위 | 위와 동일 |
  | 건물 | 위와 동일 |
  
  일단 출력물의 사용 제약이 적은 ChatGPT로 제작한 기초적인 에셋을 관상용 목적으로 배치할 계획입니다.
  
  <br>

  **(관련 상호작용)**
  - 유닛들의 기동, 시야, 사격을 제한
  - 지형의 트랙 자국 효과
  - 유닛의 사격에 의한 분진 효과
  - 각 환경 요소마다 다른 피탄 효과

<br>




### 4.4. 유닛 기능
  | 종류 | 구성 방식 |
  |---|---|
  | 차량 | 2차 세계대전의 기갑 차량 |
  | 인물 | 각 차량의 차장 총 4인 |

  중후한 외관을 뽐내는 영국과 독일의 2차 세계대전 기갑 장비들을 등장시키려 합니다.

  주인공 외 4명의 등장 인물로 각 유닛을 나타내어 유닛의 대사에 개성을 부여하려 합니다.

  <br>

  **(관련 상호작용)**
  - 마우스을 통한 유닛 선택 및 분대 지정
  - 이동 명령, 공격 명령, 능력 사용 명령
  - 자원 소비 및 관리
  - 위 상호작용의  GUI

<br>




### 4.5. 분위기 기능
  | 종류 | 전달 방식 |
  |---|---|
  | 기능 | 어둠과 안개를 가르는 육중함과 포화|
  | 시대 | 1940년대, 기술 진보 |
  | 전투 | 발레 |
  | 역할 | 관측용 항공기 탑승 지휘 |

  기갑 차량은 장갑-화력-기동을 갖춰 전선을 돌파하는 역할을 수행하기에, 이를 증폭시키는 분위기를 설정하려 합니다.

  그리고 1940년대의 느낌과 함께 당시대 최신예 전차들의 기술 진보적인 느낌을 첨가하려 합니다.

  여러 기갑 차량이 각자의 방향으로 질주하고 차체와 포탑이 회전하는 모습에선 거친 발레와 같은 느낌을 줄 생각입니다.

  그리고 탑다운 뷰로 유닛을 조작하는 게임의 특성에 어울리도록, 정찰기로 선회하며 지휘를 하는듯한 현장감을 부여할 예정입니다.

  <br>

  **(관련 상호작용)**
  - 적 행동 스크립트
  - 유닛 가속력, 감속력
  - 차체와 포탑 명령 분리
  - 유닛 명령 반응 지연
  - 시야 조작 가능 범위를 항공기의 선회 반경 내부로 제한
  - 시야 확대시 조준경 조절 사운드

<br>




### 4.6. 스토리 기능
  | 종류 | 전달 방식 |
  |---|---|
  | 컷씬 | 도입, 엔딩 |
  | 대사 | 미션 중 |
  | 플롯 | 선형적 진행 |
  | 주제 | 없음 |

  스토리 전달을 위한 도입용 컷씬과 엔딩 컷씬을 AI를 이용한 삽화로 제작할 계획입니다.

  게임 중에는 유닛의 대사 자막을 통해 스토리를 진행해 나갈 예정입니다.

  플레이어의 행동에 따라 스토리를 분기하기보다는 하나의 선형적인 스토리를 제시하는 것에 집중하려 합니다.

  게임 전체를 누르는 무거운 주제 의식보다는 플레이 과정의 즐거움에 집중하고자 주제를 설정하지 않았습니다.
  
  <br>

  **(관련 상호작용)**
  - 게임 시작, 일시 중단 GUI

<br>




## 5. 설계 사항
### 5.1. 폴더 구조
용도별로 다음과 같은 하위 폴더들을 두었습니다.
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




### 5.2. 빌드 관리
일단 별도의 빌드 시스템 없이 Visual Studio에서 직접 빌드를 관리하고 있습니다.

임시 코드를 `main.cpp`에 삽입하여 확인한 결과, 본 리포지토리 환경에서 빌드와 실행에 사용되는 DirectX 관련 파일들은 다음과 같습니다.

이들은 경로에 따라 관리 주체와 목적이 다릅니다.
| 경로 | 관리 주체 | 목적 |
|---|---|---|
| `C:\Windows\System32` | Windows | 응용 프로그램 구동에 필요한 런타임 보유 |
| `C:\Program Files (x86)\Windows Kits` | Visual Studio Installer | 개발 빌드에 필요한 헤더, 라이브러리 보유 | 
| `SteelBallet` 리포지토리 내부 | NuGet | 최신 헤더, 라이브러리, 런타임 복원 |

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

      - `SteelBallet\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\dxgiformat.h`

    - DirectX Agility SDK, DirectXTK12, DXC는 NuGet에 의해 관리되기에  
      d3d12, d3dx12, SimpleMath, dxcapi 관련 헤더파일들은 본 리포지토리의 아래에 복원됩니다.  
      - `SteelBallet\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3d12.h`

      - `SteelBallet\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dcommon.h`

      - `SteelBallet\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3d12sdklayers.h`

      - `SteelBallet\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dx12\d3dx12.h`

      - `SteelBallet\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dx12\d3dx12_barriers.h`

      - `SteelBallet\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dx12\d3dx12_core.h`

      - `SteelBallet\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dx12\d3dx12_default.h`

      - `SteelBallet\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dx12\d3dx12_pipeline_state_stream.h`

      - `SteelBallet\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dx12\d3dx12_render_pass.h`

      - `SteelBallet\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dx12\d3dx12_resource_helpers.h`

      - `SteelBallet\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dx12\d3dx12_property_format_table.h`

      - `SteelBallet\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dx12\d3dx12_root_signature.h`

      - `SteelBallet\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dx12\d3dx12_state_object.h`

      - `SteelBallet\packages\Microsoft.Direct3D.D3D12.1.619.3\build\native\include\d3dx12\d3dx12_check_feature_support.h`

      - `SteelBallet\packages\directxtk12_desktop_win10.2026.5.8.1\include\SimpleMath.h`

      - `SteelBallet\packages\directxtk12_desktop_win10.2026.5.8.1\include\SimpleMath.inl`

      - `SteelBallet\packages\directxtk12_desktop_win10.2026.5.8.1\include\GraphicsMemory.h`

      - `SteelBallet\packages\Microsoft.Direct3D.DXC.1.9.2602.24\build\native\include\dxcapi.h`

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
      - `SteelBallet\packages\directxtk12_desktop_win10.2026.5.8.1\build\native\..\..\native\lib\x64\Debug\DirectXTK12.lib`

    - DXC는 NuGet에 의해 관리되므로, dxcompiler.lib은 리포지토리 내에 위치하며 관리됩니다.  
      다만, 레거시인 d3dcompiler.lib은 파악하지 못한 간접적인 경로로 링킹된 것으로 추측합니다.  
      - `SteelBallet\packages\Microsoft.Direct3D.DXC.1.9.2602.24\build\native\lib\x64\dxcompiler.lib`

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

      - `SteelBallet\OutDir\D3D12\D3D12Core.dll`

      - `SteelBallet\OutDir\D3D12\D3D12SDKLayers.dll`

    - DXC는 NuGet에서 관리되므로 dxcompiler.dll은 리포지토리 내에 위치하고  
      레거시인 D3DCompiler_47.dll는 NuGet에 의해 관리되지 않기에 System32 아래에 위치합니다.  
      - `SteelBallet\OutDir\dxcompiler.dll`

      - `C:\Windows\System32\D3DCompiler_47.dll`

<br>




### 5.3. 입력 방식
일반적인 윈도우 메시지 처리 방식을 사용하고 있습니다.

<br>




### 5.4. 소리 방식
아직 설계하지 않음

<br>




### 5.5. 제시 방식
제시(Present) 모드는 마이크로소프트의 최적화 권장을 따라 flip 모드를 사용할 계획입니다.

`SetMaximumFrameLatency(..)`의 인자 값은 큐잉 상태에서 렌더링이 가능한 최솟값인 `2`로 두려고 합니다.

백버퍼 포맷은 아직 결정하지 않았습니다.

<br>




### 5.5. GUI 구성
imgui를 이용하여 단순한 GUI를 제공하려고 합니다.  

<br>




### 5.6. 프레임 자원
아직 설계하지 않음

<br>




### 5.7. 루트 서명
아직 설계하지 않음

<br>




### 5.8. 패스 구성
아직 설계하지 않음

<br>




### 5.9. 멀티 스레딩
아직 설계하지 않음

<br>




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




## 7. 제외 기능
아래 기능은 투자 대비 만족이 크지 않기에 포함하지 않을 예정입니다.

<br>




### 7.1. 조명 기능
  - 아직 제외 대상 없음

<br>




### 7.2. 소리 기능
  - 인물 음성

<br>




### 7.3. 환경 기능
  - 초목, 건물, 차량 잔해의 물리 효과

<br>




### 7.4. 유닛 기능
  - 보병 유닛
  - 키 매핑 설정 기능

<br>




### 7.5. 분위기 기능
  - 적 행동 AI

<br>




### 7.6. 스토리 기능
  - 세이브, 로드 기능
  - 미션 에디터

<br>




### 7.7. 공통 기능
  - 자체 GUI

<br>




## 8. 외부 항목
개발 및 빌드를 위해 다음 SDK, 툴킷, 라이브러리, 도구를 사용합니다.

<br>




### 8.1. DirectX 12 Agility SDK
- 목적
  - 윈도우 업데이트와 상관 없이 최신 D3D12 헤더, 런타임 사용

- 사용 방식
  - NuGet 패키지 관리 (Microsoft.Direct3D.D3D12)
  
- 복원 위치
  - `SteelBallet\packages\Microsoft.Direct3D.D3D12.<버전명>`

- 라이선스 종류
  - MICROSOFT SOFTWARE LICENSE
  - MIT License

- 라이선스 위치
  - `SteelBallet\packages\Microsoft.Direct3D.D3D12.<버전명>\LICENSE.txt`
  - `SteelBallet\packages\Microsoft.Direct3D.D3D12.<버전명>\LICENSE-CODE.txt`

<br>




### 8.2. DirectX Tool Kit (DirectXTK12)
- 목적
  - Direct3D 12 유틸리티 라이브러리 사용

- 사용 방식
  - NuGet 패키지 관리 (directxtk12_desktop_win10)

- 복원 위치
  - `SteelBallet\packages\directxtk12_desktop_win.<버전명>`

- 라이선스 종류
  - MIT License

- 라이선스 위치
  - `SteelBallet\packages\directxtk12_desktop_win10.<버전명>\docs\README.md` 내부 간접 링크
  - `https://github.com/microsoft/DirectXTK12/blob/main/LICENSE`

<br>




### 8.3. DirectX Shader Compiler (DXC)
- 목적
  - HLSL 셰이더 컴파일

- 사용 방식
  - NuGet 패키지 관리 (Microsoft.Direct3D.DXC)
  
- 복원 위치
  - `SteelBallet\packages\Microsoft.Direct3D.DXC.<버전명>`

- 라이선스 종류
  - MIT License
  - LLVM Release License
  - MICROSOFT SOFTWARE LICENSE

- 라이선스 위치
  - `SteelBallet\packages\Microsoft.Direct3D.DXC.<버전명>\LICENCE-MIT.txt`
  - `SteelBallet\packages\Microsoft.Direct3D.DXC.<버전명>\LICENSE-LLVM.txt`
  - `SteelBallet\packages\Microsoft.Direct3D.DXC.<버전명>\LICENSE-MS.txt`

<br>




### 8.4 Dear ImGui
- 목적
  - 인게임 GUI 제공

- 사용 방식
  - 내부 직접 포함
  - 포함 경로 : `SteelBallet\External\imgui`

- 원본 리포지토리
  - ocornut/imgui  
    `https://github.com/ocornut/imgui`

- 라이선스 종류
  - MIT License

- 라이선스 위치
  - `SteelBallet\External\imgui\LICENSE.txt`

<br>




## 9. 참고 자료
[1] "DirectX 12를 이용한 3D 게임 프로그래밍 입문" (Frank D. Luna 지음 / 류광 옮김 / 한빛미디어 출판 / 2017년 발행)

[2] "Introduction To 3D Game Programming With DirectX 12 Second Edition" (Frank D. Luna 지음 / Mercury Learning And Information 출판 / 2025년 발행)

[3] "Get Started with Win32 and C++" (Microsoft Learn / 2026년 6월 재방문)
`https://learn.microsoft.com/en-us/windows/win32/learnwin32/learn-to-program-for-windows`