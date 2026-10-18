# c4: Connect 4 tools for the thesis.
#   make          builds build/c4 (downloads CLI11 the first time)
#   make clean    removes build/
CXXFLAGS ?= -std=c++17 -O3 -DNDEBUG -Wall
CPPFLAGS += -DC4_WIDTH=6 -DC4_HEIGHT=5 -I.. -isystem $(BUILD) -MMD -MP

BUILD := build
OBJS  := $(BUILD)/c4.o $(BUILD)/generator.o

# CLI11: single-header command-line library. Downloaded once, with a pinned version and checksum.
CLI11_VERSION := 2.7.2
CLI11_SHA256  := ffa9a30da295c5858fb5f91f9f45771bab09471d7010a34c7c68c857a330dd76
CLI11         := $(BUILD)/CLI11.hpp

# indicators: single-header progress bars. Downloaded once, with a pinned version and checksum.
INDICATORS_VERSION := 2.3
INDICATORS_SHA256  := 4dd20018c30dcb608a85635d7a1405b3cc2e6de31c6862df489b9baeff79d487
INDICATORS         := $(BUILD)/indicators.hpp

$(BUILD)/c4: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD)/c4.o: $(CLI11) $(INDICATORS)

$(BUILD)/%.o: %.cpp | $(BUILD)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

$(BUILD)/%.o: ../%.cpp | $(BUILD)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

$(CLI11): | $(BUILD)
	curl -fsSL -o $@.tmp https://github.com/CLIUtils/CLI11/releases/download/v$(CLI11_VERSION)/CLI11.hpp
	echo "$(CLI11_SHA256)  $@.tmp" | shasum -a 256 -c -
	mv $@.tmp $@

$(INDICATORS): | $(BUILD)
	curl -fsSL -o $@.tmp https://raw.githubusercontent.com/p-ranav/indicators/v$(INDICATORS_VERSION)/single_include/indicators/indicators.hpp
	echo "$(INDICATORS_SHA256)  $@.tmp" | shasum -a 256 -c -
	mv $@.tmp $@

$(BUILD):
	mkdir -p $@

clean:
	rm -rf $(BUILD)

.PHONY: clean
-include $(OBJS:.o=.d)
