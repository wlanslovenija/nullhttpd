#
# Mitar <mitar@tnode.com>
# http://mitar.tnode.com/
# In kind public domain.
#
# $Id$
# $Revision$
# $HeadURL$
#

all: nullhttpd

nullhttpd: main.o api.o ip_acl.o protocol.o version.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -I. -o $@ $<

clean:
	rm -rf *.o nullhttpd
