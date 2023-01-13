# STM32 보드를 활용한 디지털 도어락 개발

**개요**

Nucleo-F103RB 보드로 비밀번호와 NFC 보안 기능을 제공하는 도어 개폐장치를 개발하였다. 1.5V 알카라인 건전지 AA 4개로 작동하며 작동 전압은 6V이다. 비상 전원으로 9V 전지를 사용할 수 있다. 터치 키패드로 비밀번호를 입력받거나 NFC 카드 태그로 문을 열 수 있다. 또한, 버튼을 눌러 문을 개폐할 수 있다. 모든 입력은 LED의 ON/OFF나 동작에 따라 구분되는 부저 소리로 확인할 수 있다.

전체 블록 다이어그램 :

![block](https://user-images.githubusercontent.com/120588410/212366719-e91aa5e6-c40f-4c73-aabd-ad1afe06e934.jpg)

배선도 :

![digital_doorlock_circuit](https://user-images.githubusercontent.com/120588410/212331884-fcc2624e-209c-426b-9dbe-fbef368c1d5e.png)

![20230111_175444](https://user-images.githubusercontent.com/120588410/212367350-db160eb3-b668-4d5b-b73f-4d1bcc6d6d55.jpg)
