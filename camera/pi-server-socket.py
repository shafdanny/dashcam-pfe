import socket
import sys

while True:

    try:
        # Create a TCP/IP socket
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server_address = ('0.0.0.0', 10000)
        print >>sys.stderr, 'starting up on %s port %s' % server_address
        sock.bind(server_address)
        # Listen for incoming connections
        sock.listen(1)
        # Wait for a connection
        print >>sys.stderr, 'waiting for a connection'
        connection, client_address = sock.accept()
        print >>sys.stderr, 'connection from', client_address

        # Receive the data in small chunks and retransmit it
        error = False
        while True and not error:
            print >>sys.stderr, 'Sending data...'
            for line in sys.stdin:
                try:
                    connection.send(line)
                    print >>sys.stderr, 'Success send data'
                except socket.error, e:
                    print >> sys.stderr, 'Socket error! Maybe a client closed a connection?'
                    error = True
                    break
    finally:
        # Clean up the connection
        print ("Closing connection")
        connection.close()
