#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

// 시작 P-Box 상수
const int initialPermutationTable[64] = { 57, 49, 41, 33, 25, 17, 9, 1,
									59, 51, 43, 35, 27, 19, 11, 3,
									61, 53, 45, 37, 29, 21, 13, 5,
									63, 55, 47, 39, 31, 23, 15, 7,
									56, 48, 40, 32, 24, 16, 8, 0,
									58, 50, 42, 34, 26, 18, 10, 2,
									60, 52, 44, 36, 28, 20, 12, 4,
									62, 54, 46, 38, 30, 22, 14, 6 };

// 최종 P-Box 상수
const int finalPermutationTable[64] = { 39, 7, 47, 15, 55, 23, 63, 31,
								38, 6, 46, 14, 54, 22, 62, 30,
								37, 5, 45, 13, 53, 21, 61, 29,
								36, 4, 44, 12, 52, 20, 60, 28,
								35, 3, 43, 11, 51, 19, 59, 27,
								34, 2, 42, 10, 50, 18, 58, 26,
								33, 1, 41, 9, 49, 17, 57, 25,
								32, 0, 40, 8, 48, 16, 56, 24 };

// 확장 P-Box 상수
const int expansionPermutationTable[48] = { 31, 0, 1, 2, 3, 4,
										3, 4, 5, 6, 7, 8,
										7, 8, 9, 10, 11, 12,
										11, 12, 13, 14, 15, 16,
										15, 16, 17, 18, 19, 20,
										19, 20, 21, 22, 23, 24,
										23, 24, 25, 26, 27, 28,
										27, 28, 30, 30, 31, 0 };

// 단순 P-Box 상수
const int straightPermutationTable[32] = { 15, 6, 19, 20, 28, 11, 27, 16,
									0, 14, 22, 25, 4, 17, 30, 9,
									1, 7, 23, 13, 31, 26, 2, 8,
									18, 12, 29, 5, 21, 10, 3, 24 };

// 패리티 비트 제거 Table
const int ParityDropTable[56] = { 56, 48, 40, 32, 24, 16, 8, 0,
								57, 49, 41, 33, 25, 17, 9, 1,
								58, 50, 42, 34, 26, 18, 10, 2,
								59, 51, 43, 35, 62, 54, 46, 38,
								30, 22, 14, 6, 61, 53, 45, 37,
								29, 21, 13, 5, 60, 52, 44, 36,
								28, 20, 12, 4, 27, 19, 11, 3 };

// Shiftleft 연산 시 이동할 비트의 수
const int ShiftTable[16] = { 1, 1, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 1 };

// 키 축소 P-Box 상수
const int KeyCompressionTable[48] = { 13, 16, 10, 23, 0, 4, 2, 27,
									14, 5, 20, 9, 22, 18, 11, 3,
									25, 7, 15, 6, 26, 19, 12, 1,
									40, 51, 30, 36, 46, 54, 29, 39,
									50, 44, 32, 47, 43, 48, 38, 55,
									33, 52, 45, 41, 49, 35, 28, 31 };

// DES Function에서 사용되는 S-Box 상수
const int SubstitutionTables[8][4][16] = { {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
											{0, 15, 7, 4, 14, 2, 13, 10, 3, 6, 12, 11, 9, 5, 3, 8},
											{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
											{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
											{{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
											{3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
											{0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
											{13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
											{{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
											{13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
											{13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
											{1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
											{{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
											{13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
											{10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
											{3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
											{{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
											{14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
											{4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
											{11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
											{{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
											{10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
											{9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
											{4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
											{{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
											{13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
											{1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
											{6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
											{{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
											{1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
											{7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
											{2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}} };


// 모든 배열 매개변수는 배열의 주소를 받아옵니다. (Call by Reference)

// 기본 연산 함수
// copy
void copy(int bit, int* input, int* output) {
	for (int i = 0; i < bit; i++) {             // 받아온 비트의 개수만큼 반복
		output[i] = input[i];                   // output 원소에 input 값 저장
	}
}

// xor
void exclusiveOr(int bit, int* input1, int* input2, int* output) {
	for (int i = 0; i < bit; i++) {                     // 받아온 비트의 개수만큼 반복
		output[i] = (input1[i] == input2[i]) ? 0 : 1;   // 해당 원소들의 값을 비교해 같으면 0, 다르면 1을 output 원소에 저장 (XOR연산)
	}
}

// permutation
void permute(int inputbit, int outputbit, int *input, int *output, const int *PermutationTable) {
	for (int i = 0; i < outputbit; i++) {              // 받아온 output 비트 개수만큼 반복
		output[i] = input[PermutationTable[i]];        // output 원소에 P-Box의 해당 원소의 값을 받아와 input 인덱스에 넣어 나오는 해당 원소를 저장
	}
}

// split two blocks
void split(int inputbit, int outputbit, int* inBlock, int* leftBlock, int* rightBlock) {
	for (int i = 0; i < outputbit; i++) {              // 받아온 output 비트 개수만큼 반복
		leftBlock[i] = inBlock[i];                     // inBlock 원소를 leftblock에 저장
		rightBlock[i] = inBlock[outputbit + i];        // inBlock 원소를 rightblock에 저장
	}
}

// 좌우블럭 분리
void swapper(int leftBlock[32], int rightBlock[32]) {
	int T[48];                                         // 임시 배열
	copy(32, leftBlock, T);                            // leftBlock의 값을 T에 복사
	copy(32, rightBlock, leftBlock);                   // rightBlock의 값을 leftBlock으로 복사
	copy(32, T, rightBlock);                           // T의 값을 rightBlock으로 복사
}


// 결합
void combine(int inputbit, int outputbit, int* leftBlock, int* rightBlock, int *outBlock) {
	for (int i = 0; i < inputbit; i++) {               // 받아온 inputbit 개수만큼 반복
		outBlock[i] = leftBlock[i];                    // leftblock 원소를 outBlock에 저장
		outBlock[inputbit + i] = rightBlock[i];        // rightblock 원소를 outBlock에 저장
	}
}

// 2진수 -> 16진수 변환
void binToHex(int bit, int* bin, char* hex) {
	int n = bit / 4;                                           // 4비트씩 세기위해 들어온 비트수 계산
	for (int i = 0; i < n; i++) {
		int tmp[4];                                            // 4비트씩 임시로 저장할 변수
		int tmp2 = 0;                                          // 4비트를 16진수로 계산해 임시로 저장
		for (int j = i * 4, k = 3; k > -1; j++, k--) {         // 4비트씩 반복
			if (bin[j] == 1) {                                 // 해당 비트가 1인 경우
				tmp[k] = 1;                                    // tmp에 1 저장
			}
			else {                                             // 해당 비트가 0인 경우
				tmp[k] = 0;                                    // tmp에 0 저장
			}
		}
		for (int j = 0; j < 4; j++) {                          // 4번 반복
			if (tmp[j] == 1) {                                 // 해당 원소가 1인 경우
				tmp2 += (int)pow(2, j);                        // 해당 원소에 index 제곱하여 더함
			}
		}
		// 16진수를 char로 저장
		if (tmp2 == 0) { hex[i] = '0'; }
		else if (tmp2 == 1) { hex[i] = '1'; }
		else if (tmp2 == 2) { hex[i] = '2'; }
		else if (tmp2 == 3) { hex[i] = '3'; }
		else if (tmp2 == 4) { hex[i] = '4'; }
		else if (tmp2 == 5) { hex[i] = '5'; }
		else if (tmp2 == 6) { hex[i] = '6'; }
		else if (tmp2 == 7) { hex[i] = '7'; }
		else if (tmp2 == 8) { hex[i] = '8'; }
		else if (tmp2 == 9) { hex[i] = '9'; }
		else if (tmp2 == 10) { hex[i] = 'A'; }
		else if (tmp2 == 11) { hex[i] = 'B'; }
		else if (tmp2 == 12) { hex[i] = 'C'; }
		else if (tmp2 == 13) { hex[i] = 'D'; }
		else if (tmp2 == 14) { hex[i] = 'E'; }
		else if (tmp2 == 15) { hex[i] = 'F'; }
	}
}

// DES 함수에서 S-Box 통과
void substitute(int inBlock[48], int outBlock[32], const int SubstitutionTables[8][4][16])
{
	for (int i = 0; i < 8; i++) {                                                                                        // 8번 수행
		int row = 2 * inBlock[i * 6] + inBlock[i * 6 + 5];                                                               // 첫번째, 여섯번째 비트를 이용해 row값 계산
		int col = 8 * inBlock[i * 6 + 1] + 4 * inBlock[i * 6 + 2] + 2 * inBlock[i * 6 + 3] + inBlock[i * 6 + 4];         // 위 두 비트를 제외한 나머지 비트를 이용해 col값 계산
		

		int value = SubstitutionTables[i][row][col];              // 해당 row, col 값을 이용해 i번째 S-Box 테이블에서 값을 찾아 저장
		
		// value의 값을 10진수에서 2진수로 변환합니다
		outBlock[i * 4] = value / 8; value %= 8; 
		outBlock[i * 4 + 1] = value / 4; value %= 4;
		outBlock[i * 4 + 2] = value / 2; value %= 2;
		outBlock[i * 4 + 3] = value;
	}

}


// DES 함수
void function(int inBlock[32], int RoundKey[48], int outBlock[32]) {
	int tmp1[48];              // 임시 배열 변수1
	int tmp2[48];              // 임시 배열 변수2
	int otmp[32];              // 임시 배열 변수3

	permute(32, 48, inBlock, tmp1, expansionPermutationTable);    // inBlock 배열을 확장 P-Box에 통과시킨 후 tmp1에 저장
	exclusiveOr(48, tmp1, RoundKey, tmp2);                        // tmp1과 RoundKey를 이용해 Xor 연산을 수행하고 tmp2에 저장
	substitute(tmp2, otmp, SubstitutionTables);                   // tmp2를 S-Box에 통과시킨 후 otmp에 저장
	permute(32, 32, otmp, outBlock, straightPermutationTable);    // otmp 배열을 단순 P-Box에 통과시킨 후 outBlock에 저장
}

// 좌우블럭 xor 결합
void mixer(int leftBlock[48], int rightBlock[48], int RoundKey[48]) {
	int T1[48];						      // 임시 배열 변수1
	int T2[48];						      // 임시 배열 변수2
	int T3[48];						      // 임시 배열 변수1
	copy(32, rightBlock, T1);		      // rightBlock의 값을 T1에 복사

	function(T1, RoundKey, T2);           // T1과 RoundKey의 값을 이용해 DES Function을 호출하여 수행한 뒤의 값을 T2에 저장
	
	exclusiveOr(32, leftBlock, T2, T3);   // leftBlock과 T2를 이용해 XOR 연산을 수행한 후 T3에 저장
	copy(32, T3, rightBlock);             // T3의 값을 rightBlock에 복사
	
	// 해당 라운드의 왼쪽 블럭을 출력합니다.
	char left[8];                                     // 16진수 형태로 저장할 변수
	binToHex(32, leftBlock, left);                    // 16진수 변환
	for (int i = 0; i < 8; i++) {
		printf("%c", left[i]);
	}

	printf("  ");

	// 해당 라운드의 오른쪽 블럭을 출력합니다.
	char right[8];                                     // 16진수 형태로 저장할 변수
	binToHex(32, rightBlock, right);                   // 16진수 변환
	for (int i = 0; i < 8; i++) {
		printf("%c", right[i]);
	}

	printf("  ");

	// 해당 라운드에 사용된 키를 출력합니다.
	char key[12];                               // 16진수 형태로 저장할 변수
	binToHex(48, RoundKey, key);                // 16진수 변환
	for (int i = 0; i < 12; i++) {
		printf("%c", key[i]);
	}

}

// DES
void Cipher(int plainBlock[64], int RoundKeys[16][48], int cipherBlock[64]) {
	int inBlock[64];                                                     // 입력받은 평문이 P-Box 통과한 값을 저장할 배열
	int leftBlock[32];                                                   // 왼쪽 블럭을 저장할 배열
	int rightBlock[32];                                                  // 오른쪽 블럭을 저장할 배열
	int outBlock[64];                                                    // 암호화된 값이 최종 P-Box 통과하기 전의 값을 저장할 배열

	permute(64, 64, plainBlock, inBlock, initialPermutationTable);       // 입력받은 평문 plainBlock을 시작 P-Box에 통과시킨 후 inBlock에 저장

	// P-Box 통과한 inBlock 값을 출력합니다.
	printf("After initial permutation : ");
	char init[16];                               // 16진수 형태로 저장할 변수
	binToHex(64, inBlock, init);                 // 16진수 변환
	for (int i = 0; i < 16; i++) {
		printf("%c", init[i]);
	}
	printf("\n");

	split(64, 32, inBlock, leftBlock, rightBlock);                        // inBlock을 왼쪽 블럭과 오른쪽 블럭으로 분리

	// 왼쪽, 오른쪽 분리된 값을 16진수로 출력합니다.
	printf("After splitting : L0 = ");
	char left0[8];                               // 16진수 형태로 저장할 변수
	binToHex(32, leftBlock, left0);              // 16진수 변환
	for (int i = 0; i < 8; i++) {
		printf("%c", left0[i]);
	}
	printf(", R0 = ");
	char right0[8];                               // 16진수 형태로 저장할 변수
	binToHex(32, rightBlock, right0);             // 16진수 변환
	for (int i = 0; i < 8; i++) {
		printf("%c", right0[i]);
	}
	printf("\n=============================================================================\n");
	printf(" Round    Left     Right     Round Key\n");

	// 해당 Round 수행 후 값을 출력합니다.
	for (int i = 0; i < 16; i++) {
		printf("Round %d ", i + 1);                         // 해당 Round 출력
		mixer(leftBlock, rightBlock, RoundKeys[i]);         // leftBlock, rightBlock, 해당 Round의 Key를 매개변수로 한 mixer 함수 호출
		if (i != 15) {                                      // 라운드가 16이 아닌경우 (여기서는 count가 0부터 시작하기 때문에 15로 표시)
			swapper(leftBlock, rightBlock);                 // 왼쪽 블럭과 오른쪽 블럭 교체
			printf("\n");                                   // Round 출력 구분을 위한 줄바꿈
		}
	}

	combine(32, 64, leftBlock, rightBlock, outBlock);       // leftBlock과 rightBlock을 결합해 outBlock에 저장

	// 16라운드 진행 후 왼족, 오른쪽 블럭을 결합한 값을 출력합니다.
	printf("\n=============================================================================\n");
	printf("After combination : ");
	char mix[16];
	binToHex(64, outBlock, mix);
	for (int i = 0; i < 16; i++) {
		printf("%c", mix[i]);
	}
	printf("\n");

	permute(64, 64, outBlock, cipherBlock, finalPermutationTable);   // outBlock을 최종 P-Box에 통과 후 cipherBlock에 저장
}

// ShiftLeft 연산
void shiftLeft(int block[28], int numOfShifts) {
	for (int i = 0; i < numOfShifts; i++) {                          // 입력받은 numOfShifts 값만큼 반복
		int T = block[0];                                            // 첫번째 Block 값을 T에 저장
		for (int j = 1; j < 28; j++) {                               // 두번째부터 끝 원소까지 반복
			block[j - 1] = block[j];                                 // 원소들을 한칸씩 왼쪽으로 이동
		}
		block[27] = T;                                               // 마지막 원소에 T값 저장
	}
}

// 라운드 키 생성
void Key_Generator(int keyWithParities[64], int RoundKeys[16][48], const int ShiftTable[16]) {
	int cipherKey[56];                                                                            // 입력받은 key의 Parity Bit를 제거한 값을 저장할 배열
	int leftKey[28];                                                                              // 왼쪽 Key를 저장할 배열
	int rightKey[28];                                                                             // 오른쪽 Key를 저장할 배열
	int preRoundKey[56];                                                                          // 축소 P-Box를 통과하기 전 값을 저장할 배열
	permute(64, 56, keyWithParities, cipherKey, ParityDropTable);                                 // 입력받은 키 keyWithParities의 Parity Bit를 제거한 후 cipherkey에 저장
	split(56, 28, cipherKey, leftKey, rightKey);                                                  // cipherkey를 왼쪽과 오른쪽으로 분리 후 leftKey, rightKey에 저장
	for (int i = 0; i < 16; i++) {                                                                // 16개의 키 생성
		shiftLeft(leftKey, ShiftTable[i]);                                                        // leftKey를 ShiftTable의 값을 이용해 shiftLeft 연산 수행
		shiftLeft(rightKey, ShiftTable[i]);                                                       // rightKey를 ShiftTable의 값을 이용해 shiftLeft 연산 수행
		combine(28, 56, leftKey, rightKey, preRoundKey);                                          // leftKey와 rightKey를 결합해 preRoundKey에 저장
		permute(56, 48, preRoundKey, RoundKeys[i], KeyCompressionTable);                          // preRoundKey를 축소 P-Box에 통과시킨 후 RoundKeys에 저장
	}
}

// 문자열을 비트로 변경
void strToInt(int strlen, int bit, char* str, int* bits) {
	for (int i = 0; i < strlen; i++) {                                                            // 입력받은 문자열의 개수만큼 반복
		int temp = (int)str[i];                                                                   // 해당 문자열을 ASCII코드로 저장
		int tmp[8];                                                                               // 해당 문자열을 2진수로 변환한 값을 저장할 배열

		// 10진수를 2진수로 변환하는 알고리즘입니다.
		int q, r;
		int c = 0;
		do
		{
			q = temp / 2;
			r = temp - q * 2;
			tmp[c++] = r;
			temp = q;
		} while (q != 0);
		int space = 8 - c;                                                                         // 앞의 0의 개수를 파악하기 위한 변수 (ex. 1001일 경우 8비트 기준으로 앞에 0이 4개 붙음)

		// 변환값 저장
		for (int j = 7; j >= 0; j--) {
			if (j + 1 > 8 - space) bits[(7 - j) + (8 * i)] = 0;
			else bits[(7 - j) + (8 * i)] = tmp[j];
		}
	}
}

int main() {
	puts("19121112-윤창현");

	// 평문 관련 코드
	char plain[9];                                 // 평문을 저장할 배열 (9개의 원소를 저장하는 이유는 마지막에 '\0' 문자 포함)
	printf("plaintext (8 len) : ");
	scanf("%[^\n]s ", plain);                      // 평문 입력

	int plainbit[64];                              // 평문을 bit로 저장할 배열
	strToInt(8, 64, plain, plainbit);              // 입력된 평문을 bit로 변환 후 plainbit에 저장

	char keyText[9];                               // 키를 저장할 배열 (9개의 원소를 저장하는 이유는 마지막에 '\0' 문자 포함)
	printf("key text (8 len) : ");
	scanf(" %[^\n]s", keyText);                    // 키 입력

	int keyWithParities[64];                       // 키를 비트로 저장
	strToInt(8, 64, keyText, keyWithParities);     // 입력된 키를 bit로 변환 후 keyWithParities에 저장

	// bit로 변환된 평문을 16진수로 출력합니다.
	printf("\nPlaintext : ");
	char plainhex[16];                             // 16진수 형태로 저장할 변수
	binToHex(64, plainbit, plainhex);              // 16진수 변환
	for (int i = 0; i < 16; i++) {
		printf("%c", plainhex[i]);
	}
	printf("\n=============================================================================\n");

	int RoundKeys[16][48];                                    // 라운드별 사용될 키를 저장할 배열
	Key_Generator(keyWithParities, RoundKeys, ShiftTable);    // keyWithParities, RoundKeys, ShiftTable을 매개로 해 Key_Generator 함수를 호출하여 키 생성
	
	
	int cipherBlock[64];   // 암호문 비트 저장
	Cipher(plainbit, RoundKeys, cipherBlock);    // 평문 bit, RoundKeys를 이용해 암호화 수행 후 cipherBlock에 저장

	// 최종 암호문 16진수로 출력
	printf("Ciphertext : ");
	char cipherhex[16];                             // 16진수 형태로 저장할 변수
	binToHex(64, cipherBlock, cipherhex);           // 16진수 변환
	for (int i = 0; i < 16; i++) {
		printf("%c", cipherhex[i]);
	}
	printf("\n");
}
