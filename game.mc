let target = 7;
let guess = 0;
let diff = 1;
let check = 0; // 유효성 검사 플래그 (1이면 정상, 0이면 에러)

print(900); // 게임 제목
print(800); print(target); // 정답 미리보기

while (diff) {
   guess = scan(); // 숫자 입력 받기
   check = 1;  

   // 100 초과 체크
   if (guess > 100) {
       print(99); // 에러 메시지
       check = 0; // 비정상 표시
   }

   // 0 미만 체크
   if (guess < 0) {
       print(99); 
       check = 0; 
   }

   // 정상 범위(check가 1)일 때만 게임 진행
   if (check) {
       if (guess == target) {
           diff = 0;
       }
       if (guess > target) {
           print(2); // DOWN
       }
       if (guess < target) {
           print(1); // UP
       }
   }
}

print(7777); // 승리 메시지