# Generated automatically from Makefile.in by configure.
# The program name
PROGNAME = flash

MANPAGE = flash.1
GPL = COPYING
HELP = Manual
INITIALMENU = system.menu

# Directory to install everything to (change with configure --prefix=blah)
EXEC_PREFIX = /opt/lash
BINDIR = $(EXEC_PREFIX)/sbin
# Careful changing the LIBDIR the code expects LIBDIR to be
# what LIBPREFIX is defined as by configure...                
LIBDIR = $(EXEC_PREFIX)/lib


MANDIR = $(EXEC_PREFIX)/man/man1

INSTALL = /opt/imake/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644

MKDIR = mkdir -p
BINFLAGS = -m 755 -s
MANFLAGS = -m 444

RM = rm -f

TAR = tar
TAROPTIONS = zcvf
RELNAME = $(PROGNAME).tar.gz

# C Compiler
CC = gcc 
DEBUG  =
CPPFLAGS = -DHAVE_CONFIG_H -DPREFIX=\"$(EXEC_PREFIX)\" -DDONT_HIGHLIGHT_WITH_REVERSE 
CFLAGS = -O2 $(CPPFLAGS) $(DEBUG) -DHPUX_SOURCE -I/opt/ncurses/include

#
# For Solaris users please use the next 2 lines instead.
#CPPFLAGS = -DHAVE_CONFIG_H -DPREFIX=\"$(EXEC_PREFIX)\" -DDONT_HIGHLIGHT_WITH_REVERSE -DSOLARIS
#CFLAGS = -O2 $(CPPFLAGS) $(DEBUG)
#
# Comment out for Solaris
#LDFLAGS = -rdynamic
#

LIBRARIES =-L/opt/ncurses/lib -lncurses
# For Solaris:
#LIBRARIES = -lcurses -ldl

SCREEN_SOURCES = screens/nc_init.c screens/nc_lock.c screens/nc_about.c screens/nc_menus.c screens/nc_misc.c screens/nc_job.c screens/nc_module.c

# Add any modules that you want to include here 
MODULE_PROGS = modules/alarms modules/countdown modules/background    

SOURCES = main.c parse.c string.c set.c misc.c menu.c exec.c rc.c tree.c parseline.c event.c sha.c module.c debug.c $(SCREEN_SOURCES)
OBJECTS = $(SOURCES:.c=.o)

all: $(PROGNAME)   

.c.o:
		$(CC) $(CFLAGS) -c -o $@ $<

$(PROGNAME):	$(OBJECTS) $(MODULE_PROGS)
		$(RM) $@
		$(CC) -o $@ $(OBJECTS) $(LDFLAGS) $(LIBRARIES)

# since make -C dosent work on Solaris make, we just cd in and make..
$(MODULE_PROGS):
		cd modules; $(MAKE)

shacrypt:	
		$(CC) $(CFLAGS) -DO_WRAP -o $@ sha.c

#
#CONFIG=\
#sample-config/system.flashlogin   sample-config/system.module\
#sample-config/system.menu         sample-config/system.rc
#

# Easier to just use cp, mkdir etc, rather than (g)install... 
# something else that needs to be fixed in configure...
install:
		$(MKDIR) $(BINDIR)
		$(INSTALL) $(BINFLAGS) $(PROGNAME) $(BINDIR)/$(PROGNAME)
		$(MKDIR) $(LIBDIR)
		$(INSTALL) $(MANFLAGS) $(GPL) $(LIBDIR)/GPL
		$(INSTALL) $(MANFLAGS) $(HELP) $(LIBDIR)/Manual
		$(MKDIR) $(MANDIR)
		$(INSTALL) $(MANFLAGS) $(MANPAGE) $(MANDIR)/$(MANPAGE)
		for i in $(MODULE_PROGS) ; do \
			bsdinst -c  -m 0775 $$i $(LIBDIR) ; \
		done
		$(MKDIR) -p $(LIBDIR)/flash	
		bsdinst -c  -m 0644 sample-config/system.menu $(LIBDIR)/flash
		bsdinst -c  -m 0644 sample-config/system.rc $(LIBDIR)/flash



old:
		#$(MKDIR) $(BINDIR)	
		#cp $(PROGNAME) $(BINDIR)/$(PROGNAME)
		#chmod 755 $(BINDIR)/$(PROGNAME)
		#$(MKDIR) $(LIBDIR)
		#cp $(GPL) $(LIBDIR)/GPL
		#cp $(HELP) $(LIBDIR)/Manual
		#cp sample-config/* $(LIBDIR)
		#$(MKDIR) $(MANDIR)
		#cp $(MANPAGE) $(MANDIR)/$(MANPAGE)
		#cp $(MODULE_PROGS) $(LIBDIR)
		#chmod 644 $(LIBDIR)/*
		#cp $(MANPAGE) $(MANDIR)/$(MANPAGE)

clean:		
		$(RM) $(PROGNAME) $(OBJECTS) shacrypt  modules/background modules/alarms modules/test modules/countdown \
		config.cache

veryclean:		
		$(RM) $(PROGNAME) $(OBJECTS) shacrypt *~ screens/*~ orcasetup/*~ orcasetup/background orcasetup/alarms orcasetup/test orcasetup/countdown modules/*~ modules/background modules/alarms modules/test modules/countdown

mrproper:	clean
		$(RM) config.cache config.status config.log config.h Makefile .depend

$(RELNAME):
		$(TAR) $(TAROPTIONS) $(RELNAME) .

release: 	$(RELNAME)
		echo "Tarfile created"


#
#Vanilla make on HPUX cant handle this ...
#depend:		dep
#
#dep:		
#		$(CC) $(CPPFLAGS) -M $(SOURCES) > .depend
#
##Comment the following three lines out on Solaris if you have to
#ifeq (.depend,$(wildcard .depend))
#include .depend
#endif
#
