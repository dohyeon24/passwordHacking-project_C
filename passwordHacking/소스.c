#include <windows.h>  // Windows API 사용
#include <stdio.h>    // 표준 입출력
#include <string.h>   // 문자열 관련 함수

const char* file_path = "C:\\Users\\김도현\\Desktop\\passwordHacking.exe";  // 실행 파일 경로

// 1. strings 명령어 실행하여 문자열 추출
void extract_strings(const char* file_path) {
    char command[512];
    snprintf(command, sizeof(command), "strings \"%s\"", file_path);  // strings 명령어 생성

    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };

    HANDLE hStdOutRead, hStdOutWrite;
    if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0)) {
        printf("CreatePipe failed\n");
        return;
    }

    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = hStdOutWrite;

    if (!CreateProcess(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        printf("CreateProcess failed (%d)\n", GetLastError());
        return;
    }

    // 파이프에서 읽기
    char buffer[1024];
    DWORD bytesRead;
    while (ReadFile(hStdOutRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';  // 문자열 종료
        printf("%s", buffer);  // 출력
    }

    CloseHandle(hStdOutRead);
    CloseHandle(hStdOutWrite);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

// 2. objdump 명령어 실행하여 어셈블리 코드 분석
void disassemble_file(const char* file_path) {
    char command[512];
    snprintf(command, sizeof(command), "objdump -d \"%s\"", file_path);  // objdump 명령어 생성

    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };

    HANDLE hStdOutRead, hStdOutWrite;
    if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0)) {
        printf("CreatePipe failed\n");
        return;
    }

    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = hStdOutWrite;

    if (!CreateProcess(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        printf("CreateProcess failed (%d)\n", GetLastError());
        return;
    }

    // "cmp" 명령어가 포함된 라인 출력
    char buffer[1024];
    DWORD bytesRead;
    while (ReadFile(hStdOutRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';  // 문자열 종료
        if (strstr(buffer, "cmp") != NULL) {
            printf("%s", buffer);  // "cmp" 포함된 라인 출력
        }
    }

    CloseHandle(hStdOutRead);
    CloseHandle(hStdOutWrite);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

// 3. 비밀번호 추정 (16진수 -> 10진수 변환)
void print_password() {
    unsigned int password_hex = 0x4E55741;  // 예상 비밀번호 16진수
    unsigned int password_decimal = password_hex;  // 10진수로 변환

    printf("\n[예상 비밀번호는: %u (10진수)]\n", password_decimal);  // 10진수 출력
}

int main() {
    printf("[실행 파일에서 문자열 추출]\n");
    extract_strings(file_path);  // 문자열 추출

    printf("\n[실행 파일 역어셈블]\n");
    disassemble_file(file_path);  // 어셈블리 코드 분석

    print_password();  // 예상 비밀번호 출력

    return 0;
}
