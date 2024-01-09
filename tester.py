import socket

IP = '127.0.0.1'
PORT = 8200

REQUEST = 'GET http://gnu.org/ HTTP/1.1 \
Host: gnu.org \
Connection: keep-alive \
Upgrade-Insecure-Requests: 1 \
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36 OPR/105.0.0.0 \
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7 \
Accept-Encoding: gzip, deflate \
Accept-Language: en-US,en;q=0.9 \
'

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect((IP, PORT))
        sock.send(REQUEST)
        print(sock.revc(1024))


if __name__ == '__main__':
    main()