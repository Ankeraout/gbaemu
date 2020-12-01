FASMARM=fasmarm

SOURCES_TESTROMS=$(wildcard testroms/*.asm)
BINARY_TESTROMS=$(SOURCES_TESTROMS:testroms/%.asm=testroms/%.gba)

all: testroms

testroms: $(BINARY_TESTROMS)

testroms/%.gba: testroms/%.asm
	$(FASMARM) $< $@

clean:
	rm -f $(BINARY_TESTROMS)

.PHONY: testroms clean all
