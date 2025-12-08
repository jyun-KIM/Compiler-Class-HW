// 목적: 여러 변수 비교 및 값 갱신
let a = 30;
let b = 50;
let c = 10;

let max = a; // 일단 a가 크다고 가정

if (b > max) {
    max = b;
}
if (c > max) {
    max = c;
}

print(max); // 정답: 50