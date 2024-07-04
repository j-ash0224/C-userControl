#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

// 회원 구조체 선언
struct User
{
    char name[20];
    char id[20];
    char pw[20];
    char birth[20];
};

// 최대 회원 수 - 상수 선언
#define MAX_USERS 100

// 회원 데이터를 담을 구조체 배열 선언
struct User users[MAX_USERS];

int num_users = 0; // 현재 회원 수 - 전역 변수

// 구조체 배열 순회 함수 선언
// 반환값이 포인터 (구조체로 정의된 특정 사용자 정보 전체를 반환)
//포인터를 반환하는 경우 "자료형*" 형식으로 함수 선언
struct User* findUserByID(char id[20]) {

    for (int i = 0; i < num_users; ++i) {
        /*
            strcmp 함수를 사용해 매개변수와 순회하는 중인 구조체 배열의 값을 비교
            (1) str1 < str2 인 경우에는 음수 반환
            (2) str1 > str2 인 경우에는 양수 반환
            (3) str1 == str2 인 경우에는 0을 반환*/
        if (strcmp(users[i].id, id) == 0) {
            return &users[i];
        }
    }
    return NULL; // ID에 해당하는 회원이 없는 경우
}

void sign_in() {
    printf("로그인 페이지.\n\n");
    char id[20], pw[20];

    printf("ID : ");
    scanf("%s", id);

    printf("PW : ");
    scanf("%s", pw);

    // 입력된 ID로 회원 존재여부를 확인
    struct User* user = findUserByID(id);

    if (user != NULL) {
        if (strcmp(user->pw, pw) == 0) {
            printf("%s 님, 환영합니다!\n", user->name);
            menu();
        }
        else {
            printf("로그인 실패. 비밀번호가 올바르지 않습니다.\n");
            main();
        }
    }
    else {
        printf("해당 ID를 가진 회원을 찾을 수 없습니다.\n");
        main();
    }

}

// 비밀번호 조건을 체크하는 함수
int checkPassword(char* pw) {
    int len = strlen(pw);
    int has_digit = 0;
    int has_special = 0;

    if (len < 8) {
        printf("비밀번호는 8자 이상이어야 합니다.\n");
        return 0;
    }

    for (int i = 0; i < len; ++i) {
        if (isdigit(pw[i])) {
            has_digit = 1;
        }
        else if (!isalnum(pw[i])) {
            has_special = 1;
        }
    }

    if (!has_digit) {
        printf("비밀번호는 최소한 하나 이상의 숫자를 포함해야 합니다.\n");
        return 0;
    }

    if (!has_special) {
        printf("비밀번호는 최소한 하나 이상의 특수문자를 포함해야 합니다.\n");
        return 0;
    }

    return 1;
}

void sign_up() {
    printf("회원 가입 페이지.\n\n");

    // 현재 회원 수가 최대 회원 수를 넘지 않는지 체크
    if (num_users >= MAX_USERS) {
        printf("더 이상 회원 가입이 불가능합니다. 최대 회원 수를 초과했습니다.\n");
        return;
    }

    //구조체 변수 선언 후 값 입력
    struct User newUser;

    printf("이름: ");
    scanf("%s", newUser.name);

    char temp_id[20];

    while (1) {
        printf("ID: ");
        scanf("%s", temp_id);

        // 입력된 ID로 이미 존재하는 사용자인지 확인
        struct User* user = findUserByID(temp_id);

        if (user != NULL) {
            printf("이미 존재하는 아이디입니다. 다시 입력해주세요.\n");
        }
        else {
            strcpy(newUser.id, temp_id); // 중복이 아니라면 newUser.id에 복사
            break; // 중복이 아니면 루프 종료
        }
    }

    char temp_pw[20];

    while (1) {
        printf("PW: ");
        scanf("%s", temp_pw);

        if (!checkPassword(temp_pw)) {
            // 비밀번호 조건을 만족하지 않으면 다시 입력받음
            printf("비밀번호 조건을 만족하지 않습니다. 다시 입력해주세요.\n");
        }
        else {
            strcpy(newUser.pw, temp_pw);
            break; // 조건을 만족하면 루프 종료
        }
    }

    printf("생년월일: ");
    scanf("%s", newUser.birth);

    // 구조체 배열에 새로운 회원정보 추가
    users[num_users++] = newUser;

    printf("회원 가입이 완료되었습니다.\n");

    main();
}

int menu() {
    int choice;
    printf("메뉴를 선택해주세요.\n\n");
    printf("1. 로그 아웃\n");
    printf("2. 회원 탈퇴\n");

    while (1) {
        scanf("%d", &choice);

        if (choice == 1) {
            main();
        }
        else if (choice == 2) {
            resign();
        }
        else {
            printf("올바른 메뉴를 선택해주세요.\n");
        }
    }

    return 0;
}

int resign() {
    printf("회원 탈퇴 페이지.\n\n");
    char id[20], pw[20];

    printf("탈퇴할 회원의 ID를 입력하세요: ");
    scanf("%s", id);

    printf("ID를 입력하세요 : ");
    scanf("%s", pw);

    // 입력된 ID로 회원을 찾음
    struct User* user = findUserByID(id);

    if (user != NULL) {
        if (strcmp(user->pw, pw) == 0) {
            // 해당 회원을 배열에서 제거하고 배열을 재정렬
            for (int i = 0; i < num_users; ++i) {
                if (strcmp(users[i].id, id) == 0) {
                    // 해당 위치부터 끝까지 한 칸씩 앞으로 당김
                    for (int j = i; j < num_users - 1; ++j) {
                        users[j] = users[j + 1];
                    }
                    num_users--;
                    printf("%s 님의 회원 탈퇴가 완료되었습니다.\n", user->name);
                    main();
                }
            }
        }
        else {
            printf("회원탈퇴 실패비밀번호가 올바르지 않습니다.\n");
        }
    }
    else {
        printf("해당 ID를 가진 회원을 찾을 수 없습니다.\n");
    }
    menu();
}

int main(void) {
    int choice;

    printf("메뉴를 선택해주세요.\n\n");
    printf("1. 로그인\n");
    printf("2. 회원 가입\n");
    printf("3. 종료\n");
    scanf("%d", &choice);

    while (choice != 3) {
        if (choice == 1) {
            sign_in();
        }
        else if (choice == 2) {
            sign_up();
        }
        else {
            printf("올바른 메뉴를 선택해주세요.\n");
            scanf("%d", &choice);
        }
    }
    exit(0);
}
