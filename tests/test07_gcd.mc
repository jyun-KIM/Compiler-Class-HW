// 목적: 뺄셈을 이용한 GCD 알고리즘 구현 (나머지 연산자 대체)
// 12와 18의 최대공약수 구하기

let a = 12;
let b = 18;
let diff = 1; // loop condition flag

while (diff) {
    if (a == b) {
        diff = 0; // 반복 종료
    }
    if (a > b) {
        a = a - b;
    }
    if (b > a) {
        b = b - a;
    }
}
print(a); // 정답: 6