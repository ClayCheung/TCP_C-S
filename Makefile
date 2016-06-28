# this is make file
s.c = src/time_tcp_server.c
c.c = src/time_tcp_client.c
s = bin/time_tcp_server
c = bin/time_tcp_client
all:$(s) $(c)
$(s):$(s.c)
		gcc -o $(s) $(s.c)
$(c):$(c.c)
		gcc -o $(c) $(c.c) 


