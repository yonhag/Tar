import socket
import json

def send_http_request():
    # Create a socket object
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    # Define the server address and port
    server_address = ('localhost', 8202)
    
    # Connect to the server
    s.connect(server_address)
    
    try:
        # Construct a simple HTTP GET request message
        request_header = 'GET / HTTP/1.1\r\nHost: google.com\r\n\r\n'
        
        # Send the HTTP request
        s.send(request_header.encode())
        
        # Receive the response (buffer size 4096)
        response = s.recv(4096)
        print("Response from server:")
        print(response.decode())
    finally:
        # Close the socket
        s.close()

send_http_request()
