import xmlrpc.client

with xmlrpc.client.ServerProxy("http://127.0.0.1:10002/RPC2") as proxy:
    while 1:
        dane = input()
        proxy.server(dane)