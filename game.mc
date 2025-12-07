let target = 7;
let guess = 0;
let diff = 1;

print(900); // 게임 제목
print(800); print(target); // 정답 미리보기

while (diff) {
   guess = scan(); // 키보드 입력 기다리기 

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

print(7777); // 승리 메시지 