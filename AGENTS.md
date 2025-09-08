# Repository Guidelines

## Project Structure & Modules
- `Source/Nom3/`: C++ 게임 코드. `Public/`는 헤더, `Private/`는 구현을 둡니다.
- `Content/`: 에셋(메시, 머티리얼, 블루프린트 등).
- `Config/`: 프로젝트/엔진 설정(`Default*.ini`).
- `Intermediate/`, `DerivedDataCache/`, `Saved/`: 빌드·캐시·세이브 산출물.
- `Tests/`(선택): 자동화/기능 테스트. 예: `Source/Nom3/Tests`.

## Build, Test, and Development Commands
- IDE 빌드: `Nom3.sln`을 Rider/Visual Studio로 열고 `Build → Rebuild`.
- 명령행(에디터 타겟 예시):
  `"<UE>\Engine\Build\BatchFiles\Build.bat" Nom3Editor Win64 Development -Project="<PATH>\Nom3.uproject" -WaitMutex`
- 실행: 언리얼 에디터에서 맵을 열고 `Play`.
- 핫리로드: C++ 변경 후 에디터 `Compile` 또는 IDE에서 빌드.

## Coding Style & Naming Conventions
- UE 스타일: `UCLASS/USTRUCT/UFUNCTION` 매크로, 타입·클래스는 PascalCase, 변수는 `camelCase`.
- 메타데이터: `UPROPERTY/UPARAM`의 카테고리, Edit/Blueprint 공개 범위를 명확히.
- 폴더 네임스페이스: `Source/Nom3/<Domain>/...` (예: `Component`, `AI`, `UI`).
- 로깅: 모듈별 `DECLARE_LOG_CATEGORY_EXTERN`/`DEFINE_LOG_CATEGORY`, 사용 시 `UE_LOG(Category, Verbosity, TEXT("..."));`.

## Testing Guidelines
- 프레임워크: `AutomationTest.h` 기반 단위/기능 테스트 권장.
- 파일/테스트명: 파일은 `*Tests.cpp`, 테스트명은 `Project.Feature.Scenario`.
- 실행: 에디터 `Session Frontend → Automation` 또는
  `Nom3Editor.exe -ExecCmds="Automation RunTests Project.*; Quit"`.

## Commit & Pull Request Guidelines
- 브랜치 규칙: `feat/`, `fix/`, `chore/` 접두사.
- 커밋 메시지: `type(scope): summary` (예: `fix(skill): null check in AddSkill`).
- PR 체크리스트: 변경 요약, 필요 시 스크린샷/로그, 관련 이슈 링크, 테스트 결과.

## Security & Configuration Tips
- 민감정보는 에셋/`*.ini`에 직접 저장하지 말고 환경변수/시크릿을 사용.
- `Config/Default*.ini` 변경 시 팀에 영향 범위를 PR 설명에 명시.

