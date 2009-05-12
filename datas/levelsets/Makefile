include ../../Makefile.config

LEVELSETS = cyco g-star

install :
	install -m 0755 -d $(LEVELSETS_DIR)
	$(INSTALL) -R $(LEVELSETS) $(LEVELSETS_DIR)
	chmod -R 0755 $(LEVELSETS_DIR)
	for i in $(LEVELSETS) ; do chmod 0644 $(LEVELSETS_DIR)/$$i/* ; done

uninstall :
	-rm -fr $(LEVELSETS_DIR)

clean :
	-rm *~ cyco/*~ g-star/*~
#	for dir in $(LEVELSETS); do \
#		rm $$dir/*~; \
#	done
