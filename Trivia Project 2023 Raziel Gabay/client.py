import socket
import json
SERVER_IP = "localhost"
SERVER_PORT = 8826
LOGIN_CODE = 100
SIGNUP_CODE = 101

def main():
    try:
        with socket.socket() as sock:
            server_address = (SERVER_IP, SERVER_PORT)
            sock.connect(server_address)
            client_msg = menu()
            sock.sendall(client_msg.encode())
            server_msg = sock.recv(1024)
            server_msg = server_msg.decode()
            print("server: " + server_msg)
    except Exception as e:
       print("The connection has lost\nerror: ", e)

def menu():
    choise = int(input("""Menu - 
    1 - Login
    2 - Signup
    99 - Exit
    
    Please enter your choise: """))
    match choise:
        case 1:
            return loginRequest()
        case 2:
            return signupRequest()
        case 99:
            exit(0)
        case _:
            print("This is not one of the options!")






def loginRequest():
    login_info = {
        "username" : input("Enter your username: "),
        "password" : input("Enter your password: ")
    }
    json_login_info = json.dumps(login_info)
    return serializeRequest(LOGIN_CODE,json_login_info)

def signupRequest():
    signup_info = {
        "username" : input("Enter a username: "),
        "password" : input("Enter a password: "),
        "mail"     : input("Enter a mail")
    }
    json_signup_info = json.dumps(signup_info)
    return serializeRequest(SIGNUP_CODE,json_signup_info)

def serializeRequest(code, json_request):
    return chr(code) + chr(len(json_request)).rjust(4,chr(0)) + json_request




if __name__ == "__main__":
    main()
