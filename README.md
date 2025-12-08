# My Language Interpreter 

C, Flex, Bison을 사용하여 구현한 토이 언어 인터프리터입니다.
변수 선언, 산술 연산, 제어 흐름(`if`, `while`), 입출력 기능뿐만 아니라 파이프 연산자(`|>`)와 AST(추상 구문 트리) 시각화 기능을 지원합니다.

## 목차

1.  주요 기능 (Features)
2.  설치 및 요구사항 (Prerequisites)
3.  빌드 방법 (Build)
4.  실행 방법 (Usage)
5.  문법 예제 (Syntax Examples)
6.  파일 구조 (File Structure)

-----

## 주요 기능 (Features)

  * **자료형**: 정수형(Integer) 지원
  * **변수**: `let` 키워드를 이용한 선언 및 할당
  * **제어문**: `if` 분기문, `while` 반복문, 블록 스코프 `{ ... }`
  * **입출력**: `print()` 출력, `scan()` 입력
  * **연산자**:
      * 산술: `+`, `-`, `*`, `/`
      * 비교: `==`, `<`, `>`
      * **파이프 연산자**: `|>` (예: `x + 1 |> print;`)
  * **AST 시각화**: 실행 후 파싱 트리를 `ast.dot` 파일로 생성 (Graphviz 호환)
  * **내장 게임 모드**: 업다운 게임(Up & Down Game) 로직 내장

-----

## 설치 및 요구사항 (Prerequisites)

이 프로젝트를 빌드하고 실행하기 위해서는 아래 도구들이 설치되어 있어야 합니다.

  * **GCC** (C Compiler)
  * **Flex** (Lexical Analyzer)
  * **Bison** (Parser Generator)
  * **Graphviz** (AST 시각화용, 선택 사항)

### Ubuntu/Debian

```bash
sudo apt-get update
sudo apt-get install gcc flex bison graphviz
```

### macOS (Homebrew)

```bash
brew install flex bison graphviz
# macOS의 기본 bison과 충돌할 경우 경로 설정이 필요할 수 있습니다.
```

-----

## 빌드 방법 (Build)

포함된 `Makefile`을 사용하여 간편하게 빌드할 수 있습니다.

```bash
# 컴파일 (my_interpreter 실행 파일 생성)
make

# 빌드 파일 제거 (Clean)
make clean
```

-----

## 실행 방법 (Usage)

빌드가 완료되면 `./my_interpreter` 명령어로 실행합니다. 실행 시 메뉴가 나타납니다.

```bash
./my_interpreter
```

### 실행 메뉴

1.  **Simple Calculator**: 터미널에서 직접 코드를 입력하여 실행하는 모드입니다. (종료 시 `Ctrl+D`)
2.  **Up & Down Game**: `game.mc` 파일을 로드하여 업다운 게임을 실행합니다.
3.  **Exit**: 프로그램 종료

### AST 시각화 (Graphviz)

코드가 성공적으로 파싱되면 `ast.dot` 파일이 생성됩니다. 별도의 도구 설치 없이 아래 웹사이트를 통해 시각화 결과를 확인할 수 있습니다.

1. 생성된 ast.dot 파일을 텍스트 에디터로 열어 내용을 전체 복사합니다.
2. [Graphviz Online](https://dreampuf.github.io/GraphvizOnline/?engine=dot#digraph%20G%20%7B%0A%0A%20%20subgraph%20cluster_0%20%7B%0A%20%20%20%20style%3Dfilled%3B%0A%20%20%20%20color%3Dlightgrey%3B%0A%20%20%20%20node%20%5Bstyle%3Dfilled%2Ccolor%3Dwhite%5D%3B%0A%20%20%20%20a0%20-%3E%20a1%20-%3E%20a2%20-%3E%20a3%3B%0A%20%20%20%20label%20%3D%20%22process%20%231%22%3B%0A%20%20%7D%0A%0A%20%20subgraph%20cluster_1%20%7B%0A%20%20%20%20node%20%5Bstyle%3Dfilled%5D%3B%0A%20%20%20%20b0%20-%3E%20b1%20-%3E%20b2%20-%3E%20b3%3B%0A%20%20%20%20label%20%3D%20%22process%20%232%22%3B%0A%20%20%20%20color%3Dblue%0A%20%20%7D%0A%20%20start%20-%3E%20a0%3B%0A%20%20start%20-%3E%20b0%3B%0A%20%20a1%20-%3E%20b3%3B%0A%20%20b2%20-%3E%20a3%3B%0A%20%20a3%20-%3E%20a0%3B%0A%20%20a3%20-%3E%20end%3B%0A%20%20b3%20-%3E%20end%3B%0A%0A%20%20start%20%5Bshape%3DMdiamond%5D%3B%0A%20%20end%20%5Bshape%3DMsquare%5D%3B%0A%7D) 사이트에 접속합니다.
3. 좌측 편집창에 복사한 내용을 붙여넣으면, 즉시 우측에서 파싱 트리 이미지를 확인할 수 있습니다.

-----

## 문법 예제 (Syntax Examples)

### 1\. 변수 선언 및 연산

```javascript
let a = 10;
let b = 20;
print(a + b * 2); // 결과: 50
```

### 2\. 제어문 (If / While)

```javascript
let i = 0;
while (i < 5) {
    if (i > 2) {
        print(i);
    }
    i = i + 1;
}
```

### 3\. 파이프 연산자 (Pipe Operator)

앞의 연산 결과를 뒤의 `print` 함수로 전달합니다.

```javascript
let x = 10;
x + 5 |> print; // 15 출력
```

### 4\. 사용자 입력 (Scan)

```javascript
let input = 0;
input = scan(); // 사용자로부터 정수 입력 받기
print(input);
```

-----

## 파일 구조 (File Structure)

  * `ast.c`, `ast.h`: AST 노드 정의, 심볼 테이블 관리, `eval()`(인터프리터 로직), 시각화 함수 구현
  * `parser.y`: Bison 문법 정의 파일 (구문 분석, AST 생성 규칙)
  * `scanner.l`: Flex 토큰 정의 파일 (어휘 분석)
  * `Makefile`: 빌드 스크립트
  * `game.mc`: 인터프리터 테스트를 위한 업다운 게임 예제 코드
  * `tests/`: 다양한 기능 테스트를 위한 소스 코드 모음 (`.mc` 파일들)

-----

202312347 김지현
