#!/bin/bash
# handler 프로세스에게 시그널을 보내는 기능을 하는 스크립트

# 첫 번째 인자로 받은 값(SIGTERM 또는 SIGALRM)을 signal_type 변수에 저장
signal_type=$1

# handler(현재 실행 파일 이름은 handler_cmt)라는 정확한 이름의 프로세스 ID를 찾아서 pid 변수에 저장
pid=$(pgrep -x handler_cmt)

# pid 변수가 비어 있지 않은 경우 즉, 프로세스가 존재하는 경우 아래 조건문을 실행
if [ ! -z "$pid" ]; then
    # signal_type 값이 'SIGTERM'인 경우 아래 조건문을 실행
    if [ "$signal_type" == "SIGTERM" ]; then
        # 해당 pid의 프로세스에 'SIGTERM' 시그널 전송
        kill -SIGTERM $pid
        # 프로세스 ID와 함께 'SIGTERM'이 보내졌음을 공지
        echo "SIGTERM sent to process ID $pid"
    # signal_type 값이 'SIGALRM'인 경우 아래 조건문을 실행
    elif [ "$signal_type" == "SIGALRM" ]; then
        # 두 번째 인자로 받은 값(초 단위)을 seconds 변수에 저장
        seconds=$2
        # seconds로 지정된 시간 동안 대기
        sleep $seconds
        # 대기 후 해당 pid의 프로세스에 'SIGALRM' 시그널을 전송
        kill -SIGALRM $pid
        # 프로세스 ID와 함께 'SIGALRM'이 설정된 시간 후에 보내졌음을 공지
        echo "SIGALRM sent to process ID $pid after $seconds seconds"
    fi
else
    # pid가 비어 있다면, 즉 프로세스가 존재하지 않는다면 아래 메시지를 출력
    echo "No process found"
fi

