CC:=gcc
LD:=gcc

BINARY_PATH ?= ./bin
OBJECT_PATH ?= ./objs
TARGET ?= $(BINARY_PATH)/test

src_dir=./

SOURCES+=$(foreach DIR,$(src_dir),$(shell find $(DIR) -maxdepth 1 -name "*.c"))

OBJECTS:=$(addsuffix .o, $(addprefix $(OBJECT_PATH)/, $(basename $(notdir $(SOURCES)))))
DEPENDS:=$(addsuffix .d, $(OBJECTS))

inc_dir+=./

CFLAGS += $(foreach dir, $(inc_dir),  -I$(dir))
LDFLAGS += $(foreach dir, $(inc_dir),  -L$(dir))

LDFLAGS+=

.PHONY : all clean prepare

AT=@
all : prepare $(TARGET)

clean :
	$(AT)rm -rf $(BINARY_PATH)
	$(AT)rm -rf $(OBJECT_PATH)

prepare : $(BINARY_PATH) $(OBJECT_PATH)

$(BINARY_PATH) $(OBJECT_PATH):
	$(AT)test -d $@ || mkdir -p $@

$(TARGET) : $(OBJECTS)
	$(AT)echo -e "generating \033[032m[$(CC)]\033[0m": $@
	$(AT)$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

define make-cmd-cc
$2 : $1
	$(AT)echo -e "compiling \033[032m[$(CC)]\033[0m": $$<
	$(AT)$$(CC) $$(CFLAGS) -MMD -MT $$@ -MF $$@.d -c -o $$@ $$<
endef

$(foreach afile, $(SOURCES), $(eval $(call make-cmd-cc, $(afile), $(addsuffix .o, $(addprefix $(OBJECT_PATH)/, $(basename $(notdir $(afile))))))))

-include $(DEPENDS)
