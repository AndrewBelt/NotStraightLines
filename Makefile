RACK_DIR ?= ../..
SLUG = NotStraightLines
VERSION = 0.6.0

SOURCES += $(wildcard *.cpp)

DISTRIBUTABLES += $(wildcard LICENSE*) $(wildcard *.svg)

include $(RACK_DIR)/plugin.mk
