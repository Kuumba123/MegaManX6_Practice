ifeq ($(BUILD_ID),1395)
	LDFLAGS += symbol.ld
else
	LDFLAGS += symbol_jp.ld
endif