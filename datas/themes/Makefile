include ../../Makefile.config

THEMES = cyco001  cyco002  HP48  inversed

install :
	install -m 0755 -d $(THEMES_DIR)
	$(INSTALL) -R $(THEMES) $(THEMES_DIR)
	chmod -R 0755 $(THEMES_DIR)
	for i in $(THEMES) ; do chmod 0644 $(THEMES_DIR)/$$i/* ; done

uninstall :
	-rm -fr $(THEMES_DIR)

clean :
	-rm *~ cyco001/*~  cyco002/*~  HP48/*~  inversed/*~
