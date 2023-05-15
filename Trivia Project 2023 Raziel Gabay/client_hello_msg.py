import socket
SERVER_IP = "localhost"
SERVER_PORT = 8826

def main():
    choise = 0
    try:
        with socket.socket() as sock:
            server_address = (SERVER_IP, SERVER_PORT)
            sock.connect(server_address)
            server_msg = sock.recv(1024)
            server_msg = server_msg.decode()
            print("server: " + server_msg)
            client_msg = input("Enter 'hello': ")
            sock.sendall(client_msg.encode())
    except Exception as e:
       print("The connection has lost\nerror: ", e)
if __name__ == "__main__":
    main()
